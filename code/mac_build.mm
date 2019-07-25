#include <Cocoa/Cocoa.h>

#if !defined(BUILD_TRAVIS)
# include <iostream>
# include <base/files/file_path.h>
# include <client/crash_report_database.h>
# include <client/settings.h>
# include <client/crashpad_client.h>
#endif // #if !defined(BUILD_TRAVIS)

#include "build.cpp"

#include <mach/mach_time.h>

#include <sys/stat.h>
#include <unistd.h>
#include <libproc.h>
#include <stdio.h> // TODO(yuval): Temporary

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

// TODO(yuval): Use a hash map for this
struct compiler_info
{
    build_compiler_type Type;
    const char* Name;
    const char* Path;
};

#if 0
struct compiler_message_queue
{
    string CompiledFileName;
    
    string Messages[512];
    yd_umm ReadIndex;
    yd_umm WriteIndex;
};
#endif // #if 0

global_variable mach_timebase_info_data_t GlobalTimebaseInfo;
global_variable compiler_info GlobalCompilers[BuildCompiler_Count - 1];
//global_variable process_message_queue GlobalProcessMessageQueue = {};

#if !defined(BUILD_TRAVIS)
internal void
SetupCrashpad()
{
    using namespace crashpad;
    
    std::map<std::string, std::string> Annotations;
    std::vector<std::string> Arguments;
    CrashpadClient Client;
    
    Annotations["format"] = "minidump";
    Annotations["token"] = "b82a6196b7e80dab321f0f414edfe1084b70e5a6cb53b93206c0c8732692e705";
    
    Arguments.push_back("--no-rate-limit");
    Client.StartHandler(base::FilePath{"../crashpad/crashpad/out/Default/crashpad_handler"},
                        base::FilePath{"../crashpad/meta_crashpad_db"},
                        base::FilePath{"../crashpad/meta_crashpad_db"},
                        "https://submit.backtrace.io/yuvaldolev/b82a6196b7e80dab321f0f414edfe1084b70e5a6cb53b93206c0c8732692e705/minidump",
                        Annotations,
                        Arguments,
                        true,
                        true);
}
#endif // #if !defined(BUILD_TRAVIS)

internal f32
MacGetSecondsElapsed(u64 Start, u64 End)
{
    // NOTE(yuval): Elapsed nanoseconds calculation
    f32 Result = ((f32)(End - Start) *
                  ((f32)GlobalTimebaseInfo.numer) /
                  ((f32)GlobalTimebaseInfo.denom));
    
    // NOTE(yuval): Conversion to seconds
    Result *= (f32)1.0E-9;
    
    return Result;
}

internal s32
ExecProcessAndWait(const char* Path, char** Args, memory_arena* Arena)
{
    s32 ExitCode = -1;
    
    s32 PipeEnds[2];
    if (pipe(PipeEnds) != -1)
    {
        int ChildPID = fork();
        
        switch (ChildPID)
        {
            // NOTE(yuval): Fork Failed
            case -1:
            {
                printf("Fork Failed!\n");
            } break;
            
            // NOTE(yuval): Child Process
            case 0:
            {
                // NOTE(yuval): Routing the standard output to the pipe
                while ((dup2(PipeEnds[1], STDOUT_FILENO) == -1) && (errno == EINTR));
                while ((dup2(PipeEnds[1], STDERR_FILENO) == -1) && (errno == EINTR));
                close(PipeEnds[0]);
                close(PipeEnds[1]);
                
                execv(Path, Args);
            } break;
            
            // NOTE(yuval): Parent Process
            default:
            {
                close(PipeEnds[1]);
                
                int Status;
                
                do
                {
                    // NOTE(yuval): Pushing the message string because later it
                    // will be added to a messsage queue
                    string Message = PushString(Arena, 4096);
                    Message.Count = read(PipeEnds[0], Message.Data, Message.MemorySize);
                    
                    if (Message.Count != 0)
                    {
                        printf("%.*s", PrintableString(Message));
                    }
                } while (waitpid(ChildPID, &Status, WNOHANG) == 0);
                
                close(PipeEnds[0]);
                
                ExitCode = WEXITSTATUS(Status);
            } break;
        }
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
    return ExitCode;
}

internal b32
BuildWorkspace(build_workspace* Workspace, memory_arena* Arena)
{
    u64 BuildStartCounter = mach_absolute_time();
    
    build_options* BuildOptions = &Workspace->Options;
    
    compiler_info* CompilerInfo = 0;
    For (GlobalCompilers)
    {
        if (((It.Type == BuildOptions->Compiler) ||
             BuildOptions->Compiler == BuildCompiler_Auto) &&
            It.Path)
        {
            CompilerInfo = &It;
            Break;
        }
    }
    
    if (CompilerInfo)
    {
        pid_t PID = getpid();
        char BuildAppPathZ[PATH_MAX] = {};
        s32 BuildAppPathCount = proc_pidpath(PID, BuildAppPathZ, sizeof(BuildAppPathZ));
        
        if (BuildAppPathCount > 0)
        {
            string BuildAppPath = MakeString(BuildAppPathZ,
                                             BuildAppPathCount,
                                             sizeof(BuildAppPathZ));
            SetLastFolder(&BuildAppPath, "code", '/');
            TerminateWithNull(&BuildAppPath);
            
            const char* CompilerArgs[512] = {};
            CompilerArgs[0] = CompilerInfo->Name;
            CompilerArgs[1] = "-I";
            CompilerArgs[2] = BuildAppPath.Data;
            CompilerArgs[4] = "-c";
            
            const char* LinkerArgs[512] = {};
            LinkerArgs[0] = CompilerInfo->Name;
            LinkerArgs[Workspace->Files.Count + 1] = "-o";
            LinkerArgs[Workspace->Files.Count + 2] = "test"; // TODO(yuval): Use the workspace's output name & path
            
            temporary_memory TempMem = BeginTemporaryMemory(Arena);
            
            for (umm Index = 0;
                 Index < Workspace->Files.Count;
                 ++Index)
            {
                // TODO(yuval): Do I have to push this string????
                CompilerArgs[3] = PushCopyZ(Arena, Workspace->Files.Paths[Index]);
                
                // TODO(yuval): Use PushCopyZ and add functions to append and set extensions for
                // null-terminated strings
                string ObjectFilePath = PushCopyString(Arena, Workspace->Files.Paths[Index]);
                if (SetExtension(&ObjectFilePath, ".o"))
                {
                    TerminateWithNull(&ObjectFilePath);
                    LinkerArgs[Index + 1] = ObjectFilePath.Data;
                }
                else
                {
                    // TODO(yuval): Diagnostic
                }
                
                printf("Compiling: ");
                for (const char** Arg = CompilerArgs; *Arg; ++Arg)
                {
                    printf("%s ", *Arg);
                }
                printf("\n\n");
                
                ExecProcessAndWait(CompilerInfo->Path, (char**)CompilerArgs, Arena);
            }
            
            
            printf("Linking: ");
            for (const char** Arg = LinkerArgs; *Arg; ++Arg)
            {
                printf("%s ", *Arg);
            }
            printf("\n\n");
            
            ExecProcessAndWait(CompilerInfo->Path, (char**)LinkerArgs, Arena);
            
            EndTemporaryMemory(TempMem);
        }
        else
        {
            // TODO(yuval): Diagnostic
        }
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
    u64 BuildEndCounter = mach_absolute_time();
    printf("Build Time: %f\n", MacGetSecondsElapsed(BuildStartCounter, BuildEndCounter));
    
    return true;
}

internal char*
GetCompilerPath(const char* CompilerName,  string EnvPath, memory_arena* Arena)
{
    char* Result = 0;
    
    // TODO(yuval): Make this platform independent (use the max path define for the corrent platform)
    char CompilerPath[PATH_MAX] = {};
    char* PathAt = CompilerPath;
    
    for (size_t Index = 0; Index <= EnvPath.Count; ++Index)
    {
        if ((Index == EnvPath.Count) || (EnvPath.Data[Index] == ':'))
        {
            *PathAt++ = '/';
            
            string CompilerPathTail = MakeString(PathAt, 0,
                                                 sizeof(CompilerPath) - (PathAt - CompilerPath));
            Append(&CompilerPathTail, CompilerName);
            *(PathAt + CompilerPathTail.Count) = 0;
            
            struct stat CompilerStat;
            if ((stat(CompilerPath, &CompilerStat) != -1) &&
                S_ISREG(CompilerStat.st_mode))
            {
                umm CompilerPathCount = PathAt + CompilerPathTail.Count - CompilerPath;
                Result = PushCopyZ(Arena, MakeString(CompilerPath, CompilerPathCount));
                
                break;
            }
            
            PathAt = CompilerPath;
        }
        else
        {
            *PathAt++ = EnvPath.Data[Index];
        }
    }
    
    return Result;
}

int
main(int ArgCount, const char* Args[])
{
#if !defined(BUILD_TRAVIS)
    SetupCrashpad();
#endif // #if !defined(BUILD_TRAVIS)
    
    @autoreleasepool
    {
        // NOTE(yuval): Getting the timebase info
        mach_timebase_info(&GlobalTimebaseInfo);
        
        string Test = MakeLitString("test.cpp");
        printf("Before: %.*s\n", PrintableString(Test));
        RemoveExtension(&Test);
        printf("After: %.*s\n", PrintableString(Test));
        
        if (ArgCount > 1)
        {
            memory_arena Arena = {};
            
            // NOTE(yuval): Compiler Paths Discovery
            string EnvPath = MakeStringSlowly(getenv("PATH"));
            
            compiler_info* Compiler = GlobalCompilers;
            Compiler->Type = BuildCompiler_Clang;
            Compiler->Name = "clang";
            Compiler->Path = GetCompilerPath(Compiler->Name, EnvPath, &Arena);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_GPP;
            Compiler->Name = "g++";
            Compiler->Path = GetCompilerPath(Compiler->Name, EnvPath, &Arena);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_GCC;
            Compiler->Name = "gcc";
            Compiler->Path = GetCompilerPath(Compiler->Name, EnvPath, &Arena);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_MSVC;
            Compiler->Name = "cl";
            Compiler->Path = GetCompilerPath(Compiler->Name, EnvPath, &Arena);
            
            // NOTE(yuval): Build File Workspace Setup
            // TODO(yuval): Maybe name the workspace with the name of the build file?
            build_workspace BuildFileWorkspace = {};
            BuildFileWorkspace.Name = MakeLitString("Build File");
            
            build_options* Options = &BuildFileWorkspace.Options;
            Options->OptimizationLevel = 0; // TODO(yuval): Use max optimization level
            Options->OutputType = BuildOutput_Executable;
            Options->OutputName = MakeLitString("build_file");
            Options->OutputPath = MakeLitString(""); // TODO(yuval): Add an output path;
            Options->Compiler = BuildCompiler_Auto;
            
            BuildAddFile(&BuildFileWorkspace, MakeStringSlowly(Args[1]));
            
            // NOTE(yuval): Build File Workspace Building
            BuildWorkspace(&BuildFileWorkspace, &Arena);
            
#if 0
            
            if (CompilerInstalled(Path, MakeLitString("clang")))
            {
                
            }
            else if (CompilerInstalled(Path, MakeLitString("g++")))
            {
            }
            else if (CompilerInstalled(Path, MakeLitString("cl")))
            {
            }
            else
            {
            }
#endif // #if 0
        }
        else
        {
            printf("A Build File Was Not Specified\n");
        }
    }
    
    return 0;
}

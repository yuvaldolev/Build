#include <Cocoa/Cocoa.h>

#if !defined(BUILD_TRAVIS)
# include <iostream>
# include <base/files/file_path.h>
# include <client/crash_report_database.h>
# include <client/settings.h>
# include <client/crashpad_client.h>
#endif // #if !defined(BUILD_TRAVIS)

#include "build.cpp"

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

global_variable compiler_info GlobalCompilers[BuildCompiler_Count - 1];

#if !defined(BUILD_TRAVIS)
static void
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

internal char*
GetCompilerPath(memory_arena* Arena, string EnvPath, const char* CompilerName)
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

internal b32
BuildWorkspace(memory_arena* Arena, build_workspace* Workspace)
{
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
            CompilerArgs[Workspace->Files.Count + 3] = "-o";
            CompilerArgs[Workspace->Files.Count + 4] = "test"; // TODO(yuval): Use the workspace's output name & path
            
            temporary_memory TempMem = BeginTemporaryMemory(Arena);
            
            // TODO(yuval): Maybe build each file to an object file and like after all files are built?
            for (umm Index = 0;
                 Index < Workspace->Files.Count;
                 ++Index)
            {
                // printf("Building: %.*s\n", PrintableString(Workspace->Files.Paths[Index]));
                CompilerArgs[Index + 3] = PushCopyZ(Arena, Workspace->Files.Paths[Index]);
            }
            
            printf("Running Compiler: ");
            for (const char** Arg = CompilerArgs; *Arg; ++Arg)
            {
                printf("%s ", *Arg);
            }
            printf("\n\n");
            
            int PipeEnds[2];
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
                        
                        execv(CompilerInfo->Path, (char**)CompilerArgs);
                    } break;
                    
                    // NOTE(yuval): Parent Process
                    default:
                    {
                        close(PipeEnds[1]);
                        
                        b32 Flag = true;
                        char Buffer[4096] = {};
                        int Status;
                        
                        size_t Count = 0;
                        
                        do
                        {
                            Count = read(PipeEnds[0], Buffer, sizeof(Buffer));
                            
                            if (Count != 0)
                            {
                                printf("%.*s", (s32)Count, Buffer);
                            }
                        }
                        while (waitpid(ChildPID, &Status, WNOHANG) == 0);
                        
                        close(PipeEnds[0]);
                        
                        int CompilerExitCode =  WEXITSTATUS(Status);
                        printf("\nThe Compiler Has Exited With Exit Code: %d\n", CompilerExitCode);
                    } break;
                }
            }
            else
            {
                // TODO(yuval): Diagnostic
            }
            
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
    
    return true;
}

int
main(int ArgCount, const char* Args[])
{
#if !defined(BUILD_TRAVIS)
    SetupCrashpad();
#endif // #if !defined(BUILD_TRAVIS)
    
    @autoreleasepool
    {
        if (ArgCount > 1)
        {
            memory_arena Arena = {};
            
            // NOTE(yuval): Compiler Paths Discovery
            string EnvPath = MakeStringSlowly(getenv("PATH"));
            
            compiler_info* Compiler = GlobalCompilers;
            Compiler->Type = BuildCompiler_Clang;
            Compiler->Name = "clang";
            Compiler->Path = GetCompilerPath(&Arena, EnvPath, Compiler->Name);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_GPP;
            Compiler->Name = "g++";
            Compiler->Path = GetCompilerPath(&Arena, EnvPath, Compiler->Name);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_GCC;
            Compiler->Name = "gcc";
            Compiler->Path = GetCompilerPath(&Arena, EnvPath, Compiler->Name);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_MSVC;
            Compiler->Name = "cl";
            Compiler->Path = GetCompilerPath(&Arena, EnvPath, Compiler->Name);
            
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
            BuildWorkspace(&Arena, &BuildFileWorkspace);
            
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

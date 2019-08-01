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
#include <libproc.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h> // TODO(yuval): Temporary

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

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

internal PLATFORM_GET_OUTPUT_EXTENSION(MacGetOutputExtension)
{
    const char* Extension;
    
    switch (OutputType)
    {
        case BuildOutput_Executable:
        {
            Extension = "";
        } break;
        
        case BuildOutput_StaticLibrary:
        {
            Extension = ".a";
        } break;
        
        case BuildOutput_SharedLibrary:
        {
            Extension = ".dylib";
        } break;
    }
    
    return Extension;
}

internal PLATFORM_EXEC_PROCESS_AND_WAIT(MacExecProcessAndWait)
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

internal PLATFORM_GET_WALL_CLOCK(MacGetWallClock)
{
    u64 Result = mach_absolute_time();
    return Result;
}

internal PLATFORM_GET_SECONDS_ELAPSED(MacGetSecondsElapsed)
{
    // NOTE(yuval): Elapsed nanoseconds calculation
    f32 Result = ((f32)(End - Start) *
                  ((f32)GlobalTimebaseInfo.numer) /
                  ((f32)GlobalTimebaseInfo.denom));
    
    // NOTE(yuval): Conversion to seconds
    Result *= (f32)1.0E-9;
    
    return Result;
}

internal void
MacFreeFileMemory(void* Memory)
{
    // TODO(yuval): Stop using crt for this
    if (Memory)
    {
        free(Memory);
    }
}

struct read_file_result
{
    void* Contents;
    umm ContentsSize;
};

internal read_file_result
MacReadEntireFile(const char* FileName)
{
    read_file_result Result = {};
    
    s32 FileHandle = open(FileName, O_RDONLY);
    if (FileHandle != -1)
    {
        struct stat FileStat;
        if (fstat(FileHandle, &FileStat) == 0)
        {
            umm FileSize = FileStat.st_size;
            
            // TODO(yuval): Stop using crt for this
            Result.Contents = malloc(FileSize);
            if (Result.Contents)
            {
                umm BytesRead = read(FileHandle, Result.Contents, FileSize);
                
                if (BytesRead == FileSize)
                {
                    Result.ContentsSize = FileSize;
                }
                else
                {
                    MacFreeFileMemory(Result.Contents);
                    Result.Contents = 0;
                }
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
        
        close(FileHandle);
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
    return Result;
}

internal b32
MacWriteEntireFile(const char* FileName, void* Memory, umm MemorySize)
{
    b32 Result = false;
    
    s32 FileHandle = open(FileName, O_WRONLY | O_CREAT, 0644);
    if (FileHandle != -1)
    {
        umm BytesWritten = write(FileHandle, Memory, MemorySize);
        Result = (BytesWritten == MemorySize);
        
        close(FileHandle);
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
    return Result;
}

// TODO(yuval): Maybe factor this function to be non platform specific?
internal char*
MacGetCompilerPath(const char* CompilerName, string EnvPath, memory_arena* Arena)
{
    char* Result = 0;
    
    // TODO(yuval): Make this platform independent (use the max path define for the current platform)
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

#define GENERATED_BUILD_FILE_NAME "build_file.buildgen.cpp"

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
        
        pid_t PID = getpid();
        char BuildAppPath[PATH_MAX];
        s32 BuildAppPathCount = proc_pidpath(PID, BuildAppPath, sizeof(BuildAppPath));
        if (BuildAppPathCount > 0)
        {
            platform_api PlatformAPI;
            
            yd_umm BuildAppPathLastSlashIndex = RFind(BuildAppPath, BuildAppPathCount, '/');
            ConcatStrings(PlatformAPI.BuildRunTreeCodePath, sizeof(PlatformAPI.BuildRunTreeCodePath),
                          BuildAppPath, BuildAppPathLastSlashIndex + 1,
                          Literal("code/"));
            
            PlatformAPI.GetOutputExtension = MacGetOutputExtension;
            PlatformAPI.ExecProcessAndWait = MacExecProcessAndWait;
            
            PlatformAPI.GetWallClock = MacGetWallClock;
            PlatformAPI.GetSecondsElapsed = MacGetSecondsElapsed;
            
            BuildInitialize(PlatformAPI);
            
            if (ArgCount > 1)
            {
                read_file_result BuildFile = MacReadEntireFile(Args[1]);
                string BuildFileContents = MakeString(BuildFile.Contents, BuildFile.ContentsSize);
                
                // TODO(yuval): Better parsing for finding the build function name:
                // Parse the document to tokens, ignore whitespaces, newlines, and so on....
                char BuildFunctionName[64];
                umm BuildFunctionNameCount = 0;
                
                string BuildFileWord = GetFirstWord(BuildFileContents);
                while (!IsNullString(BuildFileWord))
                {
                    if (StringsMatch(BuildFileWord, "#build"))
                    {
                        BuildFunctionNameCount = Copy(BuildFunctionName,
                                                      GetNextWord(BuildFileContents, BuildFileWord));
                        BuildFile.ContentsSize = (umm)(BuildFileWord.Data - BuildFileContents.Data);
                        MacWriteEntireFile(GENERATED_BUILD_FILE_NAME,
                                           BuildFile.Contents,
                                           BuildFile.ContentsSize);
                        break;
                    }
                    
                    BuildFileWord = GetNextWord(BuildFileContents, BuildFileWord);
                }
                
                MacFreeFileMemory(BuildFile.Contents);
                
                if (BuildFunctionNameCount != 0)
                {
                    printf("Build Function: %s\n\n", BuildFunctionName);
                    
                    memory_arena Arena = {};
                    
                    // NOTE(yuval): Compiler Paths Discovery
                    string EnvPath = MakeStringSlowly(getenv("PATH"));
                    
                    compiler_info* Compiler = Platform.Compilers;
                    Compiler->Type = BuildCompiler_Clang;
                    Compiler->Name = "clang";
                    Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &Arena);
                    ++Compiler;
                    
                    Compiler->Type = BuildCompiler_GPP;
                    Compiler->Name = "g++";
                    Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &Arena);
                    ++Compiler;
                    
                    Compiler->Type = BuildCompiler_GCC;
                    Compiler->Name = "gcc";
                    Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &Arena);
                    ++Compiler;
                    
                    Compiler->Type = BuildCompiler_MSVC;
                    Compiler->Name = "cl";
                    Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &Arena);
                    
                    // NOTE(yuval): Build File Workspace Setup
                    build_workspace BuildFileWorkspace = {};
                    BuildFileWorkspace.Name = MakeLitString("Build File");
                    
                    build_options* Options = &BuildFileWorkspace.Options;
                    Options->OptimizationLevel = 0; // TODO(yuval): Use max optimization level
                    Options->OutputType = BuildOutput_SharedLibrary;
                    Options->OutputName = MakeLitString("build_file");
                    Options->OutputPath = MakeLitString(""); // TODO(yuval): Add an output path;
                    Options->Compiler = BuildCompiler_Auto;
                    
                    BuildAddFile(&BuildFileWorkspace, MakeLitString(GENERATED_BUILD_FILE_NAME));
                    
                    // NOTE(yuval): Build File Workspace Building
                    time_events_queue BuildFileTimeEventsQueue;
                    if (BuildWorkspace(&BuildFileWorkspace, &Arena, &BuildFileTimeEventsQueue, true))
                    {
                        void* BuildFileDLL = dlopen("build_file.dylib", RTLD_LAZY | RTLD_GLOBAL);
                        
                        if (BuildFileDLL)
                        {
                            build_function* BuildFunction =
                                (build_function*)dlsym(BuildFileDLL, BuildFunctionName);
                            
                            if (BuildFunction)
                            {
                                BuildFunction(&GlobalApp);
                            }
                            else
                            {
                                // TODO(yuval): Report invalid build function
                            }
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
                }
                else
                {
                    // TODO(yuval): Report no build function!
                }
            }
            else
            {
                printf("A Build File Was Not Specified\n");
            }
        }
        else
        {
            // TODO(yuval): Diagnostic
        }
    }
    
    return 0;
}

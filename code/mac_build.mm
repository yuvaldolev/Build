#include <Cocoa/Cocoa.h>

#include "build.cpp"

#include <mach/mach_time.h>
#include <libproc.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h> // TODO(yuval): Temporary

#include "mac_build.h"

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

global_variable mach_timebase_info_data_t GlobalTimebaseInfo;

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


internal void*
MacThreadProc(void* Parameter)
{
    mac_thread_startup* Startup = (mac_thread_startup*)Parameter;
    platform_work_queue* Queue = Startup->Queue;
    
    for (;;)
    {
        u32 OriginalNextEntryToRead = Queue->NextEntryToRead;
        u32 NewNextEntryToRead = (OriginalNextEntryToRead + 1) % ArrayCount(Queue->Entries);
        
        if (OriginalNextEntryToRead != Queue->NextEntryToWrite)
        {
            u32 Index = AtomicCompareExchangeU32((volatile u32*)&Queue->NextEntryToRead,
                                                 NewNextEntryToRead,
                                                 OriginalNextEntryToRead);
            
            if (Index == OriginalNextEntryToRead)
            {
                platform_work_queue_entry Entry = Queue->Entries[Index];
                Entry.Callback(Queue, Entry.Data);
            }
        }
        else
        {
            sem_wait(&Queue->SemaphoreHandle);
        }
    }
    
    return 0;
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

int
main(int ArgCount, const char* Args[])
{
    // NOTE(yuval): Getting the timebase info
    mach_timebase_info(&GlobalTimebaseInfo);
    
    if (ArgCount > 1)
    {
        pid_t PID = getpid();
        char BuildAppPath[PATH_MAX];
        s32 BuildAppPathCount = proc_pidpath(PID, BuildAppPath, sizeof(BuildAppPath));
        if (BuildAppPathCount > 0)
        {
            build_application* App = BootstrapPushStruct(build_application, AppArena);
            
            yd_umm BuildAppPathLastSlashIndex = RFind(BuildAppPath, BuildAppPathCount, '/');
            ConcatStrings(App->PlatformAPI.BuildRunTreeCodePath, sizeof(Platform.BuildRunTreeCodePath),
                          BuildAppPath, BuildAppPathLastSlashIndex + 1,
                          Literal("code/"));
            
            App->PlatformAPI.GetOutputExtension = MacGetOutputExtension;
            App->PlatformAPI.ExecProcessAndWait = MacExecProcessAndWait;
            
            App->PlatformAPI.GetWallClock = MacGetWallClock;
            App->PlatformAPI.GetSecondsElapsed = MacGetSecondsElapsed;
            
            // NOTE(yuval): Compiler Paths Discovery
            string EnvPath = MakeStringSlowly(getenv("PATH"));
            
            compiler_info* Compiler = App->PlatformAPI.Compilers;
            Compiler->Type = BuildCompiler_Clang;
            Compiler->Name = "clang";
            Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &App->AppArena);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_GPP;
            Compiler->Name = "g++";
            Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &App->AppArena);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_GCC;
            Compiler->Name = "gcc";
            Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &App->AppArena);
            ++Compiler;
            
            Compiler->Type = BuildCompiler_MSVC;
            Compiler->Name = "cl";
            Compiler->Path = MacGetCompilerPath(Compiler->Name, EnvPath, &App->AppArena);
            
            read_file_result BuildFile = MacReadEntireFile(Args[1]);
            string BuildFileContents = MakeString(BuildFile.Contents,
                                                  BuildFile.ContentsSize);
            
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
                    
                    MacWriteEntireFile(GENERATED_BUILD_FILE_NAME,
                                       BuildFile.Contents,
                                       (umm)(BuildFileWord.Data - BuildFileContents.Data));
                    break;
                }
                
                BuildFileWord = GetNextWord(BuildFileContents, BuildFileWord);
            }
            
            MacFreeFileMemory(BuildFile.Contents);
            
            if (BuildFunctionNameCount != 0)
            {
                printf("Build Function: %s\n\n", BuildFunctionName);
                
                if (BuildStartup(App))
                {
                    void* BuildFileDLL = dlopen("build_file.dylib", RTLD_LAZY | RTLD_GLOBAL);
                    
                    if (BuildFileDLL)
                    {
                        build_function* BuildFunction =
                            (build_function*)dlsym(BuildFileDLL, BuildFunctionName);
                        
                        if (BuildFunction)
                        {
                            // NOTE(yuval): Work Queue Creation
                            platform_work_queue WorkQueue = {};
                            mac_thread_info ThreadStartups[8];
                            
                            sem_init(&WorkQueue->SemaphoreHandle, 0, 0);
                            
                            for (u32 ThreadIndex = 0;
                                 ThreadIndex < ArrayCount(ThreadInfos);
                                 ++ThreadIndex)
                            {
                                mac_thread_startup* Startup = &ThreadStartups[ThreadIndex];
                                Startup->Queue = &WorkQueue;
                                
                                pthread_t ThreadHandle;
                                pthread_create(&ThreadHandle, 0, MacThreadProc, Startup);
                                pthread_detach(ThreadHandle);
                            }
                            
                            // NOTE(yuval): Calling the build function
                            BuildFunction(&App->AppLinks);
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
    
    return 0;
}

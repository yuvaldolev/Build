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

#if 0
#define BEGIN_TIMING(Name) u64 Name##StartCounter = mach_absolute_time();
#define END_TIMING(Name) u64 Name##EndCounter = mach_absolute_time();
#endif // #if 0

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
global_variable char GlobalBuildAppPath[PATH_MAX];
global_variable char GlobalBuildRunTreeCodePath[PATH_MAX];
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

enum time_event_type
{
    TimeEvent_None,
    
    TimeEvent_BeginBlock,
    TimeEvent_EndBlock
};

struct time_event
{
    time_event_type Type;
    const char* Name;
    // TODO(yuval): Maybe add a name hash so that we would be able to quickly search an event by its name
    
    u64 Clock;
    u64 CycleCount;
    
    // TODO(yuval): Change this to a union as more time_event types are added
    time_event* Parent; // NOTE(yuval): Used to TimeEvent_BeginBlock
    
};

struct time_events_queue
{
    time_event Events[1024];
    umm ReadIndex;
    umm WriteIndex;
};

global_variable time_events_queue GlobalTimeEventsQueue;

// TODO(yuval): Maybe force sending CycleCount (get rid of the default value for the parameter)
inline b32
RecordTimeEvent(time_events_queue* Queue, time_event_type Type,
                const char* Name, u64 Clock, u64 CycleCount = 0)
{
    umm NextWriteIndex = (Queue->WriteIndex + 1) % ArrayCount(Queue->Events);
    b32 CanRecordTimeEvent = (NextWriteIndex != Queue->ReadIndex);
    
    // TODO(yuval): Make this write thread-safe
    if (CanRecordTimeEvent)
    {
        time_event* Event = &Queue->Events[Queue->WriteIndex];
        Queue->WriteIndex = NextWriteIndex;
        
        Event->Type = Type;
        Event->Name = Name;
        Event->Clock = Clock;
        Event->CycleCount = CycleCount;
    }
    
    return CanRecordTimeEvent;
}

inline time_event*
ReadTimeEvent(time_events_queue* Queue)
{
    // TODO(yuval): Make this read thread-safe
    time_event* Event = 0;
    
    if (Queue->ReadIndex != Queue->WriteIndex)
    {
        Event = &Queue->Events[Queue->ReadIndex++];
        Queue->ReadIndex %= ArrayCount(Queue->Events);
    }
    
    return Event;
}

// TODO(yuval): Maybe take a queue parameter?
#define BeginTimedBlock(Name) { RecordTimeEvent(&GlobalTimeEventsQueue, TimeEvent_BeginBlock, \
    Name, mach_absolute_time()); }
#define EndTimedBlock() { RecordTimeEvent(&GlobalTimeEventsQueue, TimeEvent_EndBlock, \
    "EndBlock_", mach_absolute_time()); }

struct timed_block
{
    timed_block(const char* Name)
    {
        BeginTimedBlock(Name);
    }
    
    ~timed_block()
    {
        EndTimedBlock();
    }
};

#define TimedBlock(Name) timed_block Join2(TimedBlock_, __COUNTER__)(Name)
#define TimedFunction TimedBlock(__FUNCTION__)

// TODO(yuval): Move this to another file
#define COMPILATION_TIMED_BLOCK_NAME "Compilation"
#define LINKAGE_TIMED_BLOCK_NAME "Linkage"

internal void
PrintWorkspaceBuildStats(time_events_queue* Queue)
{
    f32 TotalBuildTime = -1.0f;
    f32 CompilationTime = -1.0f;
    f32 LinkageTime = -1.0f;
    
    time_event* LastTimedBlock = 0;
    
    time_event* Event = ReadTimeEvent(Queue);
    while (Event)
    {
        switch (Event->Type)
        {
            case TimeEvent_BeginBlock:
            {
                // TODO(yuval): The parent event and the OpenBlock event can change when new events
                // are inserted to the queue, FIX THIS!!!
                Event->Parent = LastTimedBlock;
                LastTimedBlock = Event;
            } break;
            
            case TimeEvent_EndBlock:
            {
                if (LastTimedBlock)
                {
                    f32* Time = 0;
                    
                    if (StringsMatch(LastTimedBlock->Name, "BuildWorkspace"))
                    {
                        Time = &TotalBuildTime;
                    }
                    
                    else if (StringsMatch(LastTimedBlock->Name,
                                          COMPILATION_TIMED_BLOCK_NAME))
                    {
                        Time = &CompilationTime;
                    }
                    else if (StringsMatch(LastTimedBlock->Name,
                                          LINKAGE_TIMED_BLOCK_NAME))
                    {
                        Time = &LinkageTime;
                    }
                    
                    if (Time)
                    {
                        *Time = MacGetSecondsElapsed(LastTimedBlock->Clock, Event->Clock);
                    }
                    
                    LastTimedBlock = LastTimedBlock->Parent;
                }
                else
                {
                    // TODO(yuval): Diagnostic
                }
            } break;
        }
        
        Event = ReadTimeEvent(Queue);
    }
    
    if ((TotalBuildTime != -1.0f) && (CompilationTime != -1.0f) && (LinkageTime != -1.0f))
    {
        f32 BackendTime = CompilationTime + LinkageTime;
        f32 FrontendTime = TotalBuildTime - BackendTime;
        
        printf("Front-end Time: %f seconds\n", FrontendTime);
        printf("    Compilation Time: %f seconds\n", CompilationTime);
        printf("    Linkage Time: %f seconds\n", LinkageTime);
        printf("Back-end Time: %f seconds\n", BackendTime);
        printf("Total Build Time: %f seconds\n", TotalBuildTime);
    }
}

internal b32
BuildWorkspace(build_workspace* Workspace, memory_arena* Arena, b32 IsSilentBuild = false)
{
    TimedFunction;
    
    yd_b32 SuccessfulBuild = true;
    
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
        const char* CompilerArgs[512] = {};
        CompilerArgs[0] = CompilerInfo->Name;
        CompilerArgs[1] = "-I";
        CompilerArgs[2] = GlobalBuildRunTreeCodePath;
        CompilerArgs[4] = "-c";
        
        char FullOutputPath[PATH_MAX];
        ConcatStrings(FullOutputPath, sizeof(FullOutputPath),
                      BuildOptions->OutputPath, BuildOptions->OutputName);
        
        const char* LinkerArgs[512] = {};
        LinkerArgs[0] = CompilerInfo->Name;
        
        // TODO(yuval): Add workspace options for compiling as shared/static library
        // in addition to compiling as an executable
        LinkerArgs[Workspace->Files.Count + 1] = "-o";
        LinkerArgs[Workspace->Files.Count + 2] = FullOutputPath; // TODO(yuval): Use the workspace's output name & path
        
        temporary_memory TempMem = BeginTemporaryMemory(Arena);
        
        {
            TimedBlock(COMPILATION_TIMED_BLOCK_NAME);
            
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
                
                if (!IsSilentBuild)
                {
                    printf("Compiling File: ");
                    for (const char** Arg = CompilerArgs; *Arg; ++Arg)
                    {
                        printf("%s ", *Arg);
                    }
                    printf("\n");
                }
                
                int ExitCode = ExecProcessAndWait(CompilerInfo->Path, (char**)CompilerArgs, Arena);
                SuccessfulBuild &= (ExitCode == 0);
            }
            
            if (!IsSilentBuild)
            {
                printf("\n");
            }
        }
        
        if (SuccessfulBuild)
        {
            TimedBlock(LINKAGE_TIMED_BLOCK_NAME);
            
            if (!IsSilentBuild)
            {
                printf("Linking: ");
                for (const char** Arg = LinkerArgs; *Arg; ++Arg)
                {
                    printf("%s ", *Arg);
                }
                printf("\n\n");
            }
            int ExitCode = ExecProcessAndWait(CompilerInfo->Path, (char**)LinkerArgs, Arena);
            SuccessfulBuild &= (ExitCode == 0);
        }
        
        EndTemporaryMemory(TempMem);
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
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
        
        build_app App = {};
        App.CreateWorkspace_ = AppCreateWorkspace;
        App.StartBuild_ = AppStartBuild;
        App.WaitForMessage_ = AppWaitForMessage;
        
        pid_t PID = getpid();
        s32 BuildAppPathCount = proc_pidpath(PID, GlobalBuildAppPath, sizeof(GlobalBuildAppPath));
        if (BuildAppPathCount > 0)
        {
            yd_umm BuildAppFilePathLastSlashIndex = RFind(GlobalBuildAppPath, BuildAppPathCount, '/');
            ConcatStrings(GlobalBuildRunTreeCodePath, sizeof(GlobalBuildRunTreeCodePath),
                          GlobalBuildAppPath, BuildAppFilePathLastSlashIndex + 1,
                          Literal("code/"));
            
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
                if (BuildWorkspace(&BuildFileWorkspace, &Arena))
                {
                    PrintWorkspaceBuildStats(&GlobalTimeEventsQueue);
                    
                    // TODO(yuval): Should we fork and exec the build file?
                    // TODO(yuval): Execv need the full build file path!!!!!!!!
                    const char* BuildFileArgs[2] = {};
                    BuildFileArgs[0] = "build_file";
                    //ExecProcessAndWait("build_file", (char**)BuildFileArgs, &Arena);
                    execv("build_file", (char**)BuildFileArgs);
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

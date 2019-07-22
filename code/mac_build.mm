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

#include <stdio.h> // TODO(yuval): Temporary

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

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

internal b32
GetCompilerPath(memory_arena* Arena, string EnvPath, string CompilerName, string* OutCompilerPath)
{
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
                *OutCompilerPath =
                    PushCopyString(Arena, MakeString(CompilerPath, CompilerPathCount));
                
                return true;
            }
            
            PathAt = CompilerPath;
        }
        else
        {
            *PathAt++ = EnvPath.Data[Index];
        }
    }
    
    *OutCompilerPath = NULL_STRING;
    return false;
}

internal b32
BuildWorkspace()
{
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
            
            string EnvPath = MakeStringSlowly(getenv("PATH"));
            string CompilerPath;
            
            if (GetCompilerPath(&Arena, EnvPath, Lit("clang"), &CompilerPath))
            {
                printf("Clang Path: %.*s\n", PrintableString(CompilerPath));
            }
            else if (GetCompilerPath(&Arena, EnvPath, Lit("g++"), &CompilerPath))
            {
                
            }
            else if (GetCompilerPath(&Arena, EnvPath, Lit("cl"), &CompilerPath))
            {
            }
            else
            {
                // TODO(yuval): Diagnostic
            }
#if 0
            
            if (CompilerInstalled(Path, Lit("clang")))
            {
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
                            
                            const char* ClangArgs[5] = {"clang", "-c", Args[1], "-I", "../code"};
                            
                            execv("/usr/bin/clang", (char**)ClangArgs);
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
                                    printf("Parent Process Read From Child Process: %.*s\n", (s32)Count, Buffer);
                                }
                            }
                            while (waitpid(ChildPID, &Status, WNOHANG) == 0);
                            
                            close(PipeEnds[0]);
                            
                            printf("Parent Process: The Clang Process Has Exited With: %s\n",
                                   (WIFEXITED(Status) ? "Success" : "Failure"));
                        } break;
                    }
                }
                else
                {
                }
            }
            else if (CompilerInstalled(Path, Lit("g++")))
            {
            }
            else if (CompilerInstalled(Path, Lit("cl")))
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
}

#include <Cocoa/Cocoa.h>

#include <iostream>
#include <base/files/file_path.h>
#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>

#include "build.h"

#include <sys/stat.h>

#include <stdio.h> // TODO(yuval): Temporary

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

static void
SetupCrashpad()
{
    using namespace crashpad;
    
    std::map<std::string, std::string> Annotations;
    std::vector<std::string> Arguments;
    CrashpadClient Client;
    
    Annotations["format"] = "minidump";
    Annotations["token"] = "93cec153dd81ff726c0100beae8ea68c2da34d88a9f21b35d0179cfda4275df4";
    
    Arguments.push_back("--no-rate-limit");
    Client.StartHandler(base::FilePath{"../crashpad/crashpad/out/Default/crashpad_handler"},
                        base::FilePath{"../crashpad/meta_crashpad_db"},
                        base::FilePath{"../crashpad/meta_crashpad_db"},
                        "https://submit.backtrace.io/yuvaldolev/93cec153dd81ff726c0100beae8ea68c2da34d88a9f21b35d0179cfda4275df4/minidump",
                        Annotations,
                        Arguments,
                        true,
                        true);
}

internal b32
BuildWorkspace()
{
    return true;
}

internal b32
GetCompilerPath(string EnvPath, string CompilerName, char* OutPath)
{
    b32 Result = false;
    char CurrPath[512] = {}; // TODO(yuval): Use Max Path Length
    char* CurrPathAt = CurrPath;
    
    for (size_t Index = 0; Index <= EnvPath.Count; ++Index)
    {
        if ((Index == EnvPath.Count) ||
            (EnvPath.Data[Index] == ':'))
        {
            *CurrPathAt++ = '/';
            
            string CurrPathTail = MakeString(CurrPathAt, 0,
                                             ArrayCount(CurrPath) - (CurrPathAt - CurrPath));
            Append(&CurrPathTail, CompilerName);
            *(CurrPathAt + CurrPathTail.Count) = 0;
            
            struct stat CompilerStat;
            if ((stat(CurrPath, &CompilerStat) != -1) &&
                S_ISREG(CompilerStat.st_mode))
            {
                Result = true;
                break;
            }
            
            CurrPathAt = CurrPath;
        }
        else
        {
            *CurrPathAt++ = EnvPath.Data[Index];
        }
    }
    
    return Result;
}

int
main(int ArgCount, const char* Args[])
{
    SetupCrashpad();
    
    @autoreleasepool
    {
        if (ArgCount > 1)
        {
            string Path = MakeStringSlowly(getenv("PATH"));
            
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
#endif
        }
        else
        {
            printf("A Build File Was Not Specified\n");
        }
    }
}
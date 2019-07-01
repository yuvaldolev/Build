#include <Cocoa/Cocoa.h>

#include "meta_tool.cpp"

#include <iostream>
#include <base/files/file_path.h>
#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

using namespace crashpad;

int
main(int Argc, const char* Argv[])
{
    // Crashpad Setup
    std::map<std::string, std::string> annotations;
    std::vector<std::string> arguments;
    CrashpadClient client;
    
    annotations["format"] = "minidump";
    annotations["token"] = "90bfc40bf5e4993ccee114f0515fd0d9f65f322034029bd8b215af5b519b6fe2";
    
    arguments.push_back("--no-rate-limit");
    client.StartHandler(base::FilePath{"../crashpad/crashpad/out/Default/crashpad_handler"},
                        base::FilePath{"../crashpad/meta_crashpad_db"},
                        base::FilePath{"../crashpad/meta_crashpad_db"},
                        "https://submit.backtrace.io/yuvaldolev/90bfc40bf5e4993ccee114f0515fd0d9f65f322034029bd8b215af5b519b6fe2/minidump",
                        annotations,
                        arguments,
                        true,
                        true);
    
    @autoreleasepool
    {
        char FileName[] = "test.c";
        MetaToolProcessFile(BundleZ("test.cpp"));
    }
}
#include <Cocoa/Cocoa.h>

#include "meta_tool.cpp"

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

int
main(int Argc, const char* Argv[])
{
    @autoreleasepool
    {
        char FileName[] = "test.c";
        MetaToolProcessFile(BundleZ("test.cpp"));
    }
}
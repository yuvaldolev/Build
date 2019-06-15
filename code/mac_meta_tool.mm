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
        //printf("Test1: {A:%d  B:%d  C:%d  D:%d}", Test1.A, Test1.B, Test1.C, Test1.D);
        char FileName[] = "test.c";
        MetaToolProcessFile(BundleZ("test.cpp"));
    }
}
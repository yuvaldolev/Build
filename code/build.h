#if !defined(BUILD_H)

#define DO_JOIN2(Arg1, Arg2) Arg1 ## Arg2
#define JOIN2(Arg1, Arg2) DO_JOIN2(Arg1, Arg2)
#define JOIN3(Arg1, Arg2, Arg3) JOIN2(JOIN2(Arg1, Arg2), Arg3)
#define JOIN4(Arg1, Arg2, Arg3, Arg4) JOIN2(JOIN3(Arg1, Arg2, Arg3), Arg4)

#include "build_platform.h"
#include "build_for.h"
#include "build_profile.h"

struct Code_File {
    String name;
    String contents;
};

#include "build_lexer.h"
#include "build_parser.h"
#include "build_diagnostics.h"

struct Compilation_Work
{
    Build_Application* app;
    
    char* filename;
    Compiler_Info* compiler_info;
    b32 is_verbose_build;
};

#define BUILD_H
#endif // #if !defined(BUILD_H)
#include "build.h"

build_internal void
Build(build_workspace* Workspace)
{
    for (;;)
    {
        build_message Message = BuildWaitForMessage();
        
        switch (Message.Type)
        {
            case BuildMessage_Started:
            {
                printf("Build Started!\n");
            } break;
            
            case BuildMessage_File:
            {
                printf("Building: %.*s\n",
                       (s32)Message.Data.Count, Message.Data.Data);
            }
            
            case BuildMessage_Completed:
            {
                printf("Finished Building With %s\n",
                       (Message.Success ? "Success" : "Failure"));
            }
        }
    }
}

build_internal void
SetupDebug(build_workspace* Workspace)
{
    build_options* Options = &Workspace->Options;
    
    // NOTE(yuval): Optimization Options
    BuildSetOptimizationLevel(Options, 0);
    
    // NOTE(yuval): Executable Options
    Options->OutputExecutableName = Lit("test_debug");
    Options->OutputPath = Lit("../build");
    
    // NOTE(yuval): Compiler Options
    Options->Compiler = BuildCompiler_Clang;
    
    BuildSetWorkspace(Workspace);
}

build_internal void
SetupRelease(build_workspace* Workspace)
{
    build_options* Options = &Workspace->Options;
    
    // NOTE(yuval): Optimization Options
    BuildSetOptimizationLevel(Options, 3);
    
    // NOTE(yuval): Executable Options
    Options->OutputExecutableName = Lit("test_release");
    Options->OutputPath = Lit("../build");
    
    // NOTE(yuval): Compiler Options
    Options->Compiler = Compiler_Clang;
    
    BuildSetWorkspace(Workspace);
}

build_internal workspace
StartWorkspace(string Name)
{
    build_workspace Workspace = BuildCreateWorkspace(Name);
    
    // TODO(yuval): Flags to indicate whether or not to usemetaprogramming
    
    BuildAddFile(Lit("test.cpp"));
    
    return Workspace;
}

BUILD_FUNC void
BuildDebug()
{
    build_workspace Workspace = StartWorkspace(Lit("Debug"));
    SetupDebug(&Workspace);
    Build();
}

BUILD_FUNC void
BuildRelease()
{
    build_workspace Workspace = StartWorkspace(Lit("Release"));
    SetupRelease(&Workspace);
    Build();
}

// TODO(yuval): #build BuildDebug
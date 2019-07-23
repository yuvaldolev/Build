#include "build_api.h"

#include <stdio.h>

build_internal void
Build()
{
    build_b32 Building = true;
    
    while (Building)
    {
        build_message Message = BuildWaitForMessage();
        
        switch (Message.Type)
        {
            case BuildMessage_Started:
            {
                printf("Build Started!\n");
            } break;
            
            case BuildMessage_Workspace:
            {
                printf("Building Workspace: %.*s\n", PrintableString(Message.Data.String));
            } break;
            
            case BuildMessage_File:
            {
                printf("Building: %.*s\n", PrintableString(Message.Data.String));
            } break;
            
            case BuildMessage_Completed:
            {
                printf("Finished Building With %s\n",
                       (Message.Data.Bool ? "Success" : "Failure"));
                
                Building = false;
            } break;
            
            default:
            {
            } break;
        }
    }
}

build_internal void
SetupDebug(build_workspace* Workspace)
{
    build_options* Options = &Workspace->Options;
    
    // NOTE(yuval): Optimization Options
    Options->OptimizationLevel = 0;
    
    // NOTE(yuval): Executable Options
    Options->OutputName = Lit("test_debug");
    Options->OutputPath = Lit("../build");
    
    // NOTE(yuval): Compiler Options
    Options->Compiler = BuildCompiler_Clang;
}

build_internal void
SetupRelease(build_workspace* Workspace)
{
    build_options* Options = &Workspace->Options;
    
    // NOTE(yuval): Optimization Options
    Options->OptimizationLevel = 3;
    
    // NOTE(yuval): Executable Options
    Options->OutputExecutableName = Lit("test_release");
    Options->OutputPath = Lit("../build");
    
    // NOTE(yuval): Compiler Options
    Options->Compiler = BuildCompiler_Clang;
}

build_internal build_workspace*
StartWorkspace(string Name)
{
    build_workspace* Workspace = BuildCreateWorkspace(Name);
    
    // TODO(yuval): Flags to indicate whether or not to use metaprogramming
    
    BuildAddFile(Workspace, Lit("test.cpp"));
    
    return Workspace;
}

BUILD_FUNC void
BuildDebug()
{
    build_workspace* Workspace = StartWorkspace(Lit("Debug"));
    SetupDebug(Workspace);
    
    build_workspace* Workspace2 = StartWorkspace(Lit("Debug2"));
    SetupDebug(Workspace2);
    
    Build();
}

BUILD_FUNC void
BuildRelease()
{
    build_workspace* Workspace = StartWorkspace(Lit("Release"));
    SetupRelease(Workspace);
    Build();
}

int main()
{
    BuildDebug();
    return 0;
}

// TODO(yuval): #build BuildDebug
#include "build_api.h"

#include <stdio.h>

build_internal void
Build(build_app* App)
{
    build_b32 Building = true;
    
    StartBuild(App);
    
    while (Building)
    {
        build_message Message = BuildWaitForMessage(App);
        
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
    Options->OutputName = MakeLitString("test_debug");
    Options->OutputPath = MakeLitString("../build");
    
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
    Options->OutputName = MakeLitString("test_release");
    Options->OutputPath = MakeLitString("../build");
    
    // NOTE(yuval): Compiler Options
    Options->Compiler = BuildCompiler_Clang;
}

build_internal build_workspace*
StartWorkspace(build_app* App, string Name)
{
    build_workspace* Workspace = BuildCreateWorkspace(App, Name);
    
    // TODO(yuval): Flags to indicate whether or not to use metaprogramming
    
    BuildAddFile(Workspace, MakeLitString("test.cpp"));
    
    return Workspace;
}

extern "C" BUILD_FUNCTION(BuildDebug)
{
    printf("Building In Debug Mode!\n");
    
    build_workspace* Workspace = StartWorkspace(App, MakeLitString("Debug"));
    SetupDebug(Workspace);
    Build(App);
}

extern "C" BUILD_FUNCTION(BuildRelease)
{
    printf("Building In Release Mode!\n");
    
    build_workspace* Workspace = StartWorkspace(App, MakeLitString("Release"));
    SetupRelease(Workspace);
    Build(App);
}

#build BuildDebug
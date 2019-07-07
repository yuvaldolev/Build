#if !defined(BUILD_H)

#define BuildDoJoin2(Arg1, Arg2) Arg1 ## Arg2
#define BuildJoin2(Arg1, Arg2) BuildDoJoin2(Arg1, Arg2)
#define BuildJoin3(Arg1, Arg2, Arg3) BuildJoin2(BuildJoin2(Arg1, Arg2), Arg3)
#define BuildJoin4(Arg1, Arg2, Arg3, Arg4) BuildJoin2(BuildJoin3(Arg1, Arg2, Arg3), Arg4)

#include "build_types.h"
#include "build_for.h"

#define YD_STRING_IMPLEMENTATION
#include "yd_string.h"

#if 0
#include "build_assert.h"
#include "build_shared.h"
#include "build_memory.h"
#include "build_intrinsics.h"
#include "build_tokenizer.h"
#include "build_parser.h"
#include "build_errors.h"
#endif

#include <stdio.h> // TODO(yuval): Temporary

#define BUILD_FUNC

struct build_file_array
{
    string Paths[4096];
    size_t Count;
};

enum build_compiler_type
{
    BuildCompiler_Clang,
    BuildCompiler_GCC,
    BuildCompiler_MSVC
};

struct build_options
{
    build_u32 OptimizationLevel;
    
    string OutputExecutableName;
    string OutputPath;
    
    build_compiler_type Compiler;
};

struct build_workspace
{
    string Name;
    build_file_array Files;
    build_options Options;
};

struct build_workspace_array
{
    build_workspace Workspaces[8];
    size_t Count;
};

enum build_message_type
{
    BuildMessage_Invalid,
    
    BuildMessage_Started,
    BuildMessage_Completed,
    
    BuildMessage_Workspace,
    BuildMessage_File,
};

struct build_message
{
    build_message_type Type;
    
    union
    {
        string String;
        build_b32 Bool;
    } Data;
};

build_global_variable build_workspace_array GlobalWorkspaces = {};

build_internal build_workspace*
BuildCreateWorkspace(string Name)
{
    build_workspace* Result = &GlobalWorkspaces.Workspaces[GlobalWorkspaces.Count++];
    
    Result->Name = Name;
    // TODO(yuval): Default workspace options
    
    return Result;
}

build_internal void
BuildAddFile(build_workspace* Workspace, string FileName)
{
    build_file_array* Files = &Workspace->Files;
    
    if (Files->Count < BuildArrayCount(Files->Paths))
    {
        Files->Paths[Files->Count++] = FileName;
    }
}

build_internal build_message
BuildWaitForMessage()
{
    // TODO(yuval): Create a proper messaging system and dequeue messages here.
    build_local_persist build_b32 Started = false;
    build_local_persist build_b32 WorkspacesBuildShouldStart = false;
    build_local_persist size_t WorkspaceIndex = 0;
    build_local_persist size_t FileIndex = 0;
    
#if 0
    printf("Waiting For Message: (");
    printf("Started: %d, ", Started);
    printf("Workspace Index: %lu/%lu, ", WorkspaceIndex, GlobalWorkspaces.Count - 1);
    printf("File Index: %lu)\n", FileIndex);
#endif
    
    build_message Message = {};
    
    if (!Started)
    {
        Message.Type = BuildMessage_Started;
        Started = true;
        WorkspacesBuildShouldStart = true;
    }
    else if (WorkspaceIndex >= GlobalWorkspaces.Count - 1)
    {
        Message.Type = BuildMessage_Completed;
        Message.Data.Bool = true;
    }
    else
    {
        build_workspace* CurrWorkspace = &GlobalWorkspaces.Workspaces[WorkspaceIndex];
        
        if (WorkspacesBuildShouldStart ||
            (FileIndex >= CurrWorkspace->Files.Count))
        {
            if (WorkspacesBuildShouldStart)
            {
                WorkspacesBuildShouldStart = false;
            }
            else
            {
                CurrWorkspace = &GlobalWorkspaces.Workspaces[++WorkspaceIndex];
            }
            
            Message.Type = BuildMessage_Workspace;
            Message.Data.String = CurrWorkspace->Name;
            
            FileIndex = 0;
        }
        else
        {
            Message.Type = BuildMessage_File;
            Message.Data.String = CurrWorkspace->Files.Paths[FileIndex++];
        }
    }
    
    return Message;
}

#define BUILD_H
#endif
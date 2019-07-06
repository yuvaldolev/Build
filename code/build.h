#if !defined(BUILD_H)

#define BuildDoJoin2(Arg1, Arg2) Arg1 ## Arg2
#define BuildJoin2(Arg1, Arg2) BuildDoJoin2(Arg1, Arg2)
#define BuildJoin3(Arg1, Arg2, Arg3) BuildJoin2(BuildJoin2(Arg1, Arg2), Arg3)
#define BuildJoin4(Arg1, Arg2, Arg3, Arg4) BuildJoin2(BuildJoin3(Arg1, Arg2, Arg3), Arg4)

#include "build_types.h"
#include "build_for.h"

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

struct file_array
{
    string Paths[4096];
    size_t Count;
}

struct build_workspace
{
    string Name;
    file_array Files;
    build_options Options;
};

struct build_workspace_array
{
    build_workspace Workspaces[8];
    size_t Count;
}

build_global_variable build_workspace_array GlobalWorkspaces = {};

build_internal build_workspace*
BuildCreateWorkspace(string Name)
{
    build_workspace* Result = GlobalWorkspaces.Workspaces[GlobalWorkspaces.Count++];
    
    Result->Name = Name;
    // TODO(yuval): Default workspace options
    
    return Result;
}

build_internal void
BuildAddFile(build_workspace* Workspace, string FileName)
{
    file_array* Files = &Workspace->Files;
    
    if (Files->Count < BuildArrayCount(Files->Paths))
    {
        Files->Paths[Files->Count++] = FileName;
    }
}

#define BUILD_H
#endif
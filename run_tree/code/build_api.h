#if !defined(BUILD_API_H)

#if !defined(BUILD_API_TYPES)
#include <stdint.h>
#include <stddef.h>

#define build_internal static
#define build_global static
#define build_local_persist static

// NOTE(yuval): To be used only in the same function
// where the array was defined or with arrays that are defined
// within structs
#define BuildArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

typedef int8_t build_s8;
typedef int16_t build_s16;
typedef int32_t build_s32;
typedef int64_t build_s64;
typedef build_s32 build_b32;

typedef uint8_t build_u8;
typedef uint16_t build_u16;
typedef uint32_t build_u32;
typedef uint64_t build_u64;

typedef float build_f32;
typedef double build_f64;

typedef intptr_t build_smm;
typedef uintptr_t build_umm;
#define BUILD_API_TYPES
#endif // #if !defined(BUILD_API_TYPES)

#include "yd/yd_memory.h"
#include "yd/yd_string.h"

// TODO(yuval): Make this export functions
#define BUILD_FUNCTION(Name) void Name(struct build_application_links* App)
typedef BUILD_FUNCTION(build_function);

struct build_file_array
{
    string Paths[4096];
    build_umm Count;
};

enum build_output_type
{
    BuildOutput_Executable,
    BuildOutput_StaticLibrary,
    BuildOutput_SharedLibrary
};

enum build_compiler_type
{
    // NOTE(yuval): Used for automatic compiler detection (this is the default compiler type)
    BuildCompiler_Auto,
    
    BuildCompiler_Clang,
    BuildCompiler_GPP,
    BuildCompiler_GCC,
    BuildCompiler_MSVC,
    
    BuildCompiler_Count
};

struct build_command_line_args
{
    const char* Args[512];
    build_umm Count;
};

struct build_options
{
    build_u32 OptimizationLevel;
    
    build_output_type OutputType;
    string OutputName;
    string OutputPath;
    
    build_command_line_args CompilerArgs;
    build_command_line_args LinkerArgs;
    
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
    build_umm Count;
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

#define BUILD_CREATE_WORKSPACE(FnName) build_workspace* FnName(struct build_application_links* App, \
string Name)
typedef BUILD_CREATE_WORKSPACE(build_create_workspace);

#define START_BUILD(Name) void Name(struct build_application_links* App)
typedef START_BUILD(start_build);

#define BUILD_WAIT_FOR_MESSAGE(Name) build_message Name(struct build_application_links* App)
typedef BUILD_WAIT_FOR_MESSAGE(build_wait_for_message);

struct build_application_links
{
    build_workspace_array Workspaces;
    
    build_create_workspace* CreateWorkspace_;
    
    start_build* StartBuild_;
    build_wait_for_message* WaitForMessage_;
};

build_internal inline build_workspace*
BuildCreateWorkspace(build_application_links* App, string Name)
{
    return App->CreateWorkspace_(App, Name);
}

build_internal inline void
StartBuild(build_application_links* App)
{
    App->StartBuild_(App);
}

build_internal inline build_message
BuildWaitForMessage(build_application_links* App)
{
    return App->WaitForMessage_(App);
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

#define BUILD_API_H
#endif // #if !defined(BUILD_API_H)
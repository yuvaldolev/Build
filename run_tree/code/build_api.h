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
#define BUILD_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

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
#define BUILD_FUNCTION(name) void name(struct Build_Application_Links* app)
typedef BUILD_FUNCTION(Build_Function);

struct Build_File_Array {
    String paths[4096];
    build_umm count;
};

enum Build_Output_Type {
    BUILD_OUTPUT_EXECUTABLE,
    BUILD_OUTPUT_STATIC_LIBRARY,
    BUILD_OUTPUT_SHARED_LIBRARY
};

enum Build_Compiler_Type {
    // NOTE(yuval): Used for automatic compiler detection (this is the default compiler type)
    BUILD_COMPILER_AUTO,
    
    BUILD_COMPILER_CLANG,
    BUILD_COMPILER_GPP,
    BUILD_COMPILER_GCC,
    BUILD_COMPILER_MSVC,
    
    BUILD_COMPILER_COUNT
};

struct Build_Command_Line_Args {
    const char* args[512];
    build_umm count;
};

struct Build_Options {
    build_u32 optimization_level;
    
    Build_Output_Type output_type;
    String output_name;
    String output_path;
    
    Build_Command_Line_Args compiler_args;
    Build_Command_Line_Args linker_args;
    
    Build_Compiler_Type compiler;
};

struct Build_Workspace {
    String name;
    Build_File_Array files;
    Build_Options options;
};

struct Build_Workspace_Array {
    Build_Workspace workspaces[8];
    build_umm count;
};

enum Build_Message_Type {
    BUILD_MESSAGE_INVALID,
    
    BUILD_MESSAGE_STARTED,
    BUILD_MESSAGE_COMPLETED,
    
    BUILD_MESSAGE_WORKSPACE,
    BUILD_MESSAGE_FILE,
};

struct Build_Message {
    Build_Message_Type type;
    
    union {
        String data_string;
        build_b32 data_b32;
    };
};

#define BUILD_CREATE_WORKSPACE(fn_name) Build_Workspace* \
fn_name(struct Build_Application_Links* app, String name)

typedef BUILD_CREATE_WORKSPACE(Build_Create_Workspace);

#define START_BUILD(name) void name(struct Build_Application_Links* app)
typedef START_BUILD(Start_Build);

#define BUILD_WAIT_FOR_MESSAGE(name) Build_Message name(struct Build_Application_Links* app)
typedef BUILD_WAIT_FOR_MESSAGE(Build_Wait_For_Message);

struct Build_Application_Links {
    Build_Workspace_Array workspaces;
    
    Build_Create_Workspace* create_workspace_;
    
    Start_Build* start_build_;
    Build_Wait_For_Message* wait_for_message_;
};

build_internal inline Build_Workspace*
build_create_workspace(Build_Application_Links* app, String name) {
    return app->create_workspace_(app, name);
}

build_internal inline void
start_build(Build_Application_Links* app) {
    app->start_build_(app);
}

build_internal inline build_message
build_wait_for_message(Build_Application_Links* app) {
    return app->wait_for_message_(app);
}

build_internal void
build_add_file(Build_Workspace* workspace, String filename) {
    Build_File_Array* files = &workspace->files;
    
    if (files->count < BUILD_ARRAY_COUNT(files->paths)) {
        files->paths[files->count++] = filename;
    }
}

#define BUILD_API_H
#endif // #if !defined(BUILD_API_H)
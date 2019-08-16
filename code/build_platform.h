#if !defined(BUILD_PLATFORM_H)

#include "build_api.h"
#include "build_types.h"

// TODO(yuval): Use a hash map for this
struct compiler_info
{
    build_compiler_type Type;
    const char* Name;
    const char* Path;
};

struct platform_work_queue;

#define PLATFORM_WORK_QUEUE_CALLBACK(Name) void Name(platform_work_queue* Queue, void* Data);
typedef PLATFORM_WORK_QUEUE_CALLBACK(platform_work_queue_callback);

#define PLATFORM_ADD_WORK_QUEUE_ENTRY(Name) void Name(platform_work_queue* Queue, \
platform_work_queue_callback* Callback, \
void* Data)
typedef PLATFORM_ADD_WORK_QUEUE_ENTRY(platform_add_work_queue_entry);

#define PLATFORM_GET_OUTPUT_EXTENSION(Name) const char* Name(build_output_type OutputType)
typedef PLATFORM_GET_OUTPUT_EXTENSION(platform_get_output_extension);

#define PLATFORM_EXEC_PROCESS_AND_WAIT(Name) s32 Name(const char* Path, char** Args, memory_arena* Arena)
typedef PLATFORM_EXEC_PROCESS_AND_WAIT(platform_exec_process_and_wait);

#define PLATFORM_GET_WALL_CLOCK(Name) u64 Name()
typedef PLATFORM_GET_WALL_CLOCK(platform_get_wall_clock);

#define PLATFORM_GET_SECONDS_ELAPSED(Name) f32 Name(u64 Start, u64 End)
typedef PLATFORM_GET_SECONDS_ELAPSED(platform_get_seconds_elapsed);

typedef struct platform_api
{
    char BuildRunTreeCodePath[PATH_MAX];
    compiler_info Compilers[BuildCompiler_Count - 1];
    
    platform_add_work_queue_entry* AddWorkQueueEntry;
    
    platform_get_output_extension* GetOutputExtension;
    platform_exec_process_and_wait* ExecProcessAndWait;
    
    platform_get_wall_clock* GetWallClock;
    platform_get_seconds_elapsed* GetSecondsElapsed;
} platform_api;

extern platform_api Platform;

#include "build_assert.h"
#include "build_intrinsics.h"

struct build_application
{
    build_application_links AppLinks;
    
    memory_arena AppArena;
    platform_api PlatformAPI;
};


#define BUILD_PLATFORM_H
#endif // #if !defined(BUILD_PLATFORM_H)
#if !defined(BUILD_PLATFORM_H)

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus
    
#include "build_types.h"
    
#define PLATFORM_EXEC_PROCESS_AND_WAIT(Name) s32 Name(const char* Path, char** Args, memory_arena* Arena)
    typedef PLATFORM_EXEC_PROCESS_AND_WAIT(platform_exec_process_and_wait);
    
#define PLATFORM_GET_WALL_CLOCK(Name) u64 Name()
    typedef PLATFORM_GET_WALL_CLOCK(platform_get_wall_clock);
    
#define PLATFORM_GET_SECONDS_ELAPSED(Name) f32 Name(u64 Start, u64 End)
    typedef PLATFORM_GET_SECONDS_ELAPSED(platform_get_seconds_elapsed);
    
    // TODO(yuval): Use a hash map for this
    struct compiler_info
    {
        build_compiler_type Type;
        const char* Name;
        const char* Path;
    };
    
    
    typedef struct platform_api
    {
        char BuildRunTreeCodePath[PATH_MAX];
        compiler_info Compilers[BuildCompiler_Count - 1];
        
        platform_exec_process_and_wait* ExecProcessAndWait;
        
        platform_get_wall_clock* GetWallClock;
        platform_get_seconds_elapsed* GetSecondsElapsed;
    } platform_api;
    
    extern platform_api Platform;
    
#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#define BUILD_PLATFORM_H
#endif // #if !defined(BUILD_PLATFORM_H)
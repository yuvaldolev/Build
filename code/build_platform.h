#if !defined(BUILD_PLATFORM_H)

#include "build_api.h"
#include "build_types.h"

// TODO(yuval): Use a hash map for this
struct Compiler_Info {
    Build_Compiler_Type type;
    const char* name;
    const char* path;
};

struct Platform_Work_Queue;

#define PLATFORM_WORK_QUEUE_CALLBACK(name) void name(Platform_Work_Queue* queue, void* data)
typedef PLATFORM_WORK_QUEUE_CALLBACK(Platform_Work_Queue_Callback);

#define PLATFORM_ADD_WORK_QUEUE_ENTRY(name) void name(Platform_Work_Queue* queue, \
Platform_Work_Queue_Callback* callback, \
void* data)
typedef PLATFORM_ADD_WORK_QUEUE_ENTRY(Platform_Add_Work_Queue_Entry);

#define PLATFORM_COMPLETE_ALL_WORK_QUEUE_WORK(name) void name(Platform_Work_Queue* queue)
typedef PLATFORM_COMPLETE_ALL_WORK_QUEUE_WORK(Platform_Complete_All_Work_Queue_Work);

#define PLATFORM_FREE_FILE_MEMORY(name) void name(void* memory)
typedef PLATFORM_FREE_FILE_MEMORY(Platform_Free_File_Memory);

struct Read_File_Result {
    void* contents;
    umm contents_size;
};

#define PLATFORM_READ_ENTIRE_FILE(name) Read_File_Result name(const char* filename)
typedef PLATFORM_READ_ENTIRE_FILE(Platform_Read_Entire_File);

#define PLATFORM_WRITE_ENTIRE_FILE(name) b32 name(const char* filename, void* memory, \
umm memory_size)
typedef PLATFORM_WRITE_ENTIRE_FILE(Platform_Write_Entire_File);

#define PLATFORM_GET_OUTPUT_EXTENSION(name) const char* name(Build_Output_Type output_type)
typedef PLATFORM_GET_OUTPUT_EXTENSION(Platform_Get_Output_Extension);

#define PLATFORM_EXEC_PROCESS_AND_WAIT(name) s32 name(const char* path, char** args, Memory_Arena* arena)
typedef PLATFORM_EXEC_PROCESS_AND_WAIT(Platform_Exec_Process_And_Wait);

#define PLATFORM_GET_WALL_CLOCK(name) u64 name()
typedef PLATFORM_GET_WALL_CLOCK(Platform_Get_Wall_Clock);

#define PLATFORM_GET_SECONDS_ELAPSED(name) f32 name(u64 start, u64 end)
typedef PLATFORM_GET_SECONDS_ELAPSED(Platform_Get_Seconds_Elapsed);

struct Platform_API {
    char build_run_tree_code_path[PATH_MAX];
    Compiler_Info compilers[BUILD_COMPILER_COUNT - 1];
    
    Platform_Work_Queue* work_queue;
    
    Platform_Add_Work_Queue_Entry* add_work_queue_entry;
    Platform_Complete_All_Work_Queue_Work* complete_all_work_queue_work;
    
    Platform_Free_File_Memory* free_file_memory;
    Platform_Read_Entire_File* read_entire_file;
    Platform_Write_Entire_File* write_entire_file;
    
    Platform_Get_Output_Extension* get_output_extension;
    Platform_Exec_Process_And_Wait* exec_process_and_wait;
    
    Platform_Get_Wall_Clock* get_wall_clock;
    Platform_Get_Seconds_Elapsed* get_seconds_elapsed;
};

extern Platform_API platform;

#include "build_assert.h"
#include "build_intrinsics.h"

struct Build_Application {
    Build_Application_Links app_links;
    
    Memory_Arena arena;
    Platform_API platform_api;
    
    struct Parser* parser;
    
    b32 is_verbose_build;
};


#define BUILD_PLATFORM_H
#endif // #if !defined(BUILD_PLATFORM_H)
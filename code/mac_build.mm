#include <Cocoa/Cocoa.h>

#include "build.cpp"

#include <mach/mach_time.h>
#include <libproc.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h> // TODO(yuval): Temporary

#include "mac_build.h"

/*
 TODO(yuval): What is left to be done in the platform layer:
  - Proper Args parsing.
 - Platform specific file I/O.
 - Platform specific message boxes.
*/

global mach_timebase_info_data_t global_timebase_info;

internal PLATFORM_GET_OUTPUT_EXTENSION(mac_get_output_extension) {
    const char* extension;
    
    switch (output_type) {
        case BUILD_OUTPUT_EXECUTABLE: {
            extension = "";
        } break;
        
        case BUILD_OUTPUT_STATIC_LIBRARY: {
            extension = ".a";
        } break;
        
        case BUILD_OUTPUT_SHARED_LIBRARY: {
            extension = ".dylib";
        } break;
    }
    
    return extension;
}

internal PLATFORM_EXEC_PROCESS_AND_WAIT(mac_exec_process_and_wait) {
    s32 exit_code = -1;
    
    s32 pipe_ends[2];
    if (pipe(pipe_ends) != -1) {
        int child_pid = fork();
        
        switch (child_pid) {
            // NOTE(yuval): Fork Failed
            case -1: {
                printf("Fork Failed!\n");
            } break;
            
            // NOTE(yuval): Child Process
            case 0: {
                // NOTE(yuval): Routing the standard output to the pipe
                while ((dup2(pipe_ends[1], STDOUT_FILENO) == -1) && (errno == EINTR));
                while ((dup2(pipe_ends[1], STDERR_FILENO) == -1) && (errno == EINTR));
                close(pipe_ends[0]);
                close(pipe_ends[1]);
                
                execv(path, args);
            } break;
            
            // NOTE(yuval): Parent Process
            default: {
                close(pipe_ends[1]);
                
                int status;
                
                do {
                    // NOTE(yuval): Pushing the message string because later it
                    // will be added to a messsage queue
                    String message = push_string(arena, 4096);
                    message.count = read(pipe_ends[0], message.data, message.capacity);
                    
                    if (message.count != 0) {
                        printf("%.*s", PRINTABLE_STRING(message));
                    }
                } while (waitpid(child_pid, &status, WNOHANG) == 0);
                
                close(pipe_ends[0]);
                
                exit_code = WEXITSTATUS(status);
            } break;
        }
    } else {
        // TODO(yuval): Diagnostic
    }
    
    return exit_code;
}

internal PLATFORM_GET_WALL_CLOCK(mac_get_wall_clock) {
    u64 result = mach_absolute_time();
    return result;
}

internal PLATFORM_GET_SECONDS_ELAPSED(mac_get_seconds_elapsed) {
    // NOTE(yuval): Elapsed nanoseconds calculation
    f32 result = ((f32)(end - start) *
                  ((f32)global_timebase_info.numer) /
                  ((f32)global_timebase_info.denom));
    
    // NOTE(yuval): Conversion to seconds
    result *= (f32)1.0E-9;
    
    return result;
}

internal void
mac_free_file_memory(void* memory) {
    // TODO(yuval): Stop using crt for this
    if (memory) {
        free(memory);
    }
}

struct Read_File_Result {
    void* contents;
    umm contents_size;
};

internal Read_File_Result
mac_read_entire_file(const char* filename) {
    Read_File_Result result = {};
    
    s32 file_handle = open(filename, O_RDONLY);
    if (file_handle != -1) {
        struct stat file_stat;
        if (fstat(file_handle, &file_stat) == 0) {
            umm file_size = file_stat.st_size;
            
            // TODO(yuval): Stop using crt for this
            result.contents = malloc(file_size);
            if (result.contents) {
                umm bytes_read = read(file_handle, result.contents, file_size);
                
                if (bytes_read == file_size)
                {
                    result.contents_size = file_size;
                } else {
                    mac_free_file_memory(result.contents);
                    result.contents = 0;
                }
            } else {
                // TODO(yuval): Diagnostic
            }
        } else {
            // TODO(yuval): Diagnostic
        }
        
        close(file_handle);
    } else {
        // TODO(yuval): Diagnostic
    }
    
    return result;
}

internal b32
mac_write_entire_file(const char* filename, void* memory, umm memory_size) {
    b32 result = false;
    
    s32 file_handle = open(filename, O_WRONLY | O_CREAT, 0644);
    if (file_handle != -1) {
        umm bytes_written = write(file_handle, memory, memory_size);
        result = (bytes_written == memory_size);
        
        close(file_handle);
    } else {
        // TODO(yuval): Diagnostic
    }
    
    return result;
}

internal PLATFORM_ADD_WORK_QUEUE_ENTRY(mac_add_work_queue_entry) {
    u32 new_next_entry_to_write = (queue->next_entry_to_write + 1) % ARRAY_COUNT(queue->entries);
    ASSERT(new_next_entry_to_write != queue->next_entry_to_read);
    
    Platform_Work_Queue_Entry* entry = &queue->entries[queue->next_entry_to_write];
    entry->callback = callback;
    entry->data = data;
    
    ++queue->completion_goal;
    
    COMPLETE_PREVIOUS_WRITES_BEFORE_FUTURE_WRITES();
    
    queue->next_entry_to_write = new_next_entry_to_write;
    sem_post(&queue->semaphore_handle);
}

internal b32
mac_do_next_work_queue_entry(Platform_Work_Queue* queue) {
    b32 done_work = true;
    
    u32 original_next_entry_to_read = queue->next_entry_to_read;
    u32 new_next_entry_to_read = (original_next_entry_to_read + 1) % ARRAY_COUNT(queue->entries);
    
    if (original_next_entry_to_read != queue->next_entry_to_write) {
        u32 index = atomic_compare_exchange_u32((volatile u32*)&queue->next_entry_to_read,
                                                new_next_entry_to_read,
                                                original_next_entry_to_read);
        
        if (index == original_next_entry_to_read) {
            Platform_Work_Queue_Entry entry = queue->entries[index];
            entry.callback(queue, entry.data);
            atomic_add_u64((volatile u64*)&queue->completion_count, 1);
        }
    } else {
        done_work = false;
    }
    
    return done_work;
}

internal PLATFORM_COMPLETE_ALL_WORK_QUEUE_WORK(mac_complete_all_work_queue_work) {
    while (queue->completion_count != queue->completion_goal) {
        mac_do_next_work_queue_entry(queue);
    }
    
    queue->completion_count = 0;
    queue->completion_goal = 0;
}

internal void*
mac_thread_proc(void* parameter) {
    Mac_Thread_Startup* startup = (Mac_Thread_Startup*)parameter;
    Platform_Work_Queue* queue = startup->queue;
    
    for (;;) {
        if (!mac_do_next_work_queue_entry(queue)) {
            sem_wait(&queue->semaphore_handle);
        }
    }
}

// TODO(yuval): Maybe factor this function to be non platform specific?
internal char*
mac_get_compiler_path(const char* compiler_name, String env_path, Memory_Arena* arena) {
    char* result = 0;
    
    // TODO(yuval): Make this platform independent (use the max path define for the current platform)
    char compiler_path[PATH_MAX] = {};
    char* path_at = compiler_path;
    
    for (size_t index = 0; index <= env_path.count; ++index) {
        if ((index == env_path.count) || (env_path.data[index] == ':')) {
            *path_at++ = '/';
            
            String compiler_path_tail = make_string(path_at, 0,
                                                    sizeof(compiler_path) - (path_at - compiler_path));
            append(&compiler_path_tail, compiler_name);
            *(path_at + compiler_path_tail.count) = 0;
            
            struct stat compiler_stat;
            if ((stat(compiler_path, &compiler_stat) != -1) && S_ISREG(compiler_stat.st_mode)) {
                umm compiler_path_count = path_at + compiler_path_tail.count - compiler_path;
                result = push_copy_z(arena, make_string(compiler_path, compiler_path_count));
                
                break;
            }
            
            path_at = compiler_path;
        } else {
            *path_at++ = env_path.data[index];
        }
    }
    
    return result;
}

int
main(int arg_count, const char* args[]) {
    // NOTE(yuval): Getting the timebase info
    mach_timebase_info(&global_timebase_info);
    
    if (arg_count > 1) {
        pid_t pid = getpid();
        char build_app_path[PATH_MAX];
        s32 build_app_path_count = proc_pidpath(pid, build_app_path, sizeof(build_app_path));
        
        if (build_app_path_count > 0) {
            Build_Application* app = BOOTSTRAP_PUSH_STRUCT(Build_Application, app_arena);
            
            yd_umm build_app_path_last_slash_index = rfind(build_app_path, build_app_path_count, '/');
            concat(app->platform_api.build_run_tree_code_path,
                   sizeof(platform.build_run_tree_code_path),
                   build_app_path, build_app_path_last_slash_index + 1,
                   LITERAL("code/"));
            
            // NOTE(yuval): Compiler Paths Discovery
            String env_path = make_string_slowly(getenv("PATH"));
            
            Compiler_Info* compiler = app->platform_api.compilers;
            compiler->type = BUILD_COMPILER_CLANG;
            compiler->name = "clang";
            compiler->path = mac_get_compiler_path(compiler->name, env_path, &app->app_arena);
            ++compiler;
            
            compiler->type = BUILD_COMPILER_GPP;
            compiler->name = "g++";
            compiler->path = mac_get_compiler_path(compiler->name, env_path, &app->app_arena);
            ++compiler;
            
            compiler->type = BUILD_COMPILER_GCC;
            compiler->name = "gcc";
            compiler->path = mac_get_compiler_path(compiler->name, env_path, &app->app_arena);
            ++compiler;
            
            compiler->type = BUILD_COMPILER_MSVC;
            compiler->name = "cl";
            compiler->path = mac_get_compiler_path(compiler->name, env_path, &app->app_arena);
            
            // NOTE(yuval): Work Queue Creation
            Platform_Work_Queue work_queue = {};
            Mac_Thread_Startup thread_startups[8];
            
            sem_init(&work_queue.semaphore_handle, 0, 0);
            
            // TODO(yuval): Use array_foreach instead of regular for
            for (u32 thread_index = 0;
                 thread_index < ARRAY_COUNT(thread_startups);
                 ++thread_index) {
                Mac_Thread_Startup* startup = &thread_startups[thread_index];
                startup->queue = &work_queue;
                
                pthread_t thread_handle;
                pthread_create(&thread_handle, 0, mac_thread_proc, startup);
                pthread_detach(thread_handle);
            }
            
            app->platform_api.work_queue = &work_queue;
            
            // NOTE(yuval): PlatformAPI Functions Initilization
            app->platform_api.add_work_queue_entry = mac_add_work_queue_entry;
            app->platform_api.complete_all_work_queue_work = mac_complete_all_work_queue_work;
            
            app->platform_api.get_output_extension = mac_get_output_extension;
            app->platform_api.exec_process_and_wait = mac_exec_process_and_wait;
            
            app->platform_api.get_wall_clock = mac_get_wall_clock;
            app->platform_api.get_seconds_elapsed = mac_get_seconds_elapsed;
            
            // NOTE(yuval): Build File Processing
            Read_File_Result build_file = mac_read_entire_file(args[1]);
            String build_file_contents = make_string(build_file.contents,
                                                     build_file.contents_size);
            
            // TODO(yuval): Better parsing for finding the build function name:
            // Parse the document to tokens, ignore whitespaces, newlines, and so on....
            char build_function_name[64];
            umm build_function_name_count = 0;
            
            String build_file_word = get_first_word(build_file_contents);
            while (!is_null_string(build_file_word)) {
                if (strings_match(build_file_word, "#build")) {
                    build_function_name_count = copy(build_function_name,
                                                     get_next_word(build_file_contents,
                                                                   build_file_word));
                    
                    mac_write_entire_file(GENERATED_BUILD_FILE_NAME,
                                          build_file.contents,
                                          (umm)(build_file_word.data - build_file_contents.data));
                    break;
                }
                
                build_file_word = get_next_word(build_file_contents, build_file_word);
            }
            
            mac_free_file_memory(build_file.contents);
            
            // NOTE(yuval): Build Startup & Build Function Call
            if (build_function_name_count != 0) {
                printf("Build Function: %s\n\n", build_function_name);
                
                if (build_startup(app)) {
                    void* build_file_dll = dlopen("build_file.dylib", RTLD_LAZY | RTLD_GLOBAL);
                    
                    if (build_file_dll) {
                        Build_Function* build_function =
                            (Build_Function*)dlsym(build_file_dll, build_function_name);
                        
                        if (build_function) {
                            // NOTE(yuval): Calling the build function
                            build_function(&app->app_links);
                        } else {
                            // TODO(yuval): Report invalid build function
                        }
                    } else {
                        // TODO(yuval): Diagnostic
                    }
                } else {
                    // TODO(yuval): Diagnostic
                }
            } else {
                // TODO(yuval): Report no build function!
            }
        } else {
            printf("A Build File Was Not Specified\n");
        }
    } else {
        // TODO(yuval): Diagnostic
    }
    
    return 0;
}

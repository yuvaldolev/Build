#include "build.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_STRING_FORMAT_IMPLEMENTATION
#include "yd/yd_string_format.h"

#include "build_lexer.cpp"
#include "build_parser.cpp"

global Time_Events_Queue global_time_events_queue;
global b32 global_build_succeeded = true;

#if 0
struct FunctionVariable
{
    String type;
    String name;
};

internal void
ExitWithMessage(const char* format, ...)
{
    char message[128];
    va_list argList;
    
    va_start(argList, format);
    FormatStringList(message, ArrayCount(message), format, argList);
    va_end(argList);
    
    fprintf(stderr, "ERROR: %s\n", message);
    
    exit(-1);
}

internal void
FormatAndWriteStringToFile(const char* format, ...)
{
    char str[1024];
    va_list argList;
    
    va_start(argList, format);
    umm count = FormatStringList(str, ArrayCount(str), format, argList);
    va_end(argList);
    
    PlatformWriteToFile(str, count);
}

internal void
ParseIntrospectable(Tokenizer* tokenizer)
{
    Token typeToken = GetToken(tokenizer);
    
    if (TokenEquals(typeToken, "struct"))
    {
        PlatformWriteTokenToFile(&typeToken);
        Token nameToken = GetToken(tokenizer);
        PlatformWriteTokenToFile(&nameToken);
        printf("Introspection For: %.*s\n", (s32)nameToken.text.count, nameToken.text.data);
    }
    else
    {
        ExitWithMessage("Invalid Type For Introspection - %S. Only structs are supported for now.",
                        typeToken.text);
    }
}

internal void
SwapFunctionVariables(Tokenizer* tokenizer,
                      FunctionVariable* functionVars,
                      umm maxIndex)
{
    Token token = GetToken(tokenizer);
    
    if (token.type == Token_Identifier)
    {
        Token commaToken = GetToken(tokenizer);
        
        if (commaToken.type == Token_Comma)
        {
            Token aToken = token;
            Token bToken = GetToken(tokenizer);
            
            if (bToken.type == Token_Identifier)
            {
                FunctionVariable* aVariable = 0;
                FunctionVariable* bVariable = 0;
                
                FunctionVariable* varsAt = functionVars;
                for (int index = 0; index < maxIndex; ++index, ++varsAt)
                {
                    if (StringsAreEqual((*varsAt).name, aToken.text))
                    {
                        aVariable = varsAt;
                    }
                    
                    if (StringsAreEqual((*varsAt).name, bToken.text))
                    {
                        bVariable = varsAt;
                    }
                }
                
                if (aVariable && bVariable &&
                    StringsAreEqual(aVariable->type, bVariable->type))
                {
                    FormatAndWriteStringToFile("%S temp = %S;\n",
                                               aVariable->type, aVariable->name);
                    
                    FormatAndWriteStringToFile("%S = %S;\n",
                                               aVariable->name, bVariable->name);
                    
                    FormatAndWriteStringToFile("%S = temp;", bVariable->name);
                }
                else
                {
                    // TODO(yuval): Error message
                }
            }
            else
            {
                // TODO(yuval): Error message
            }
        }
        else
        {
            // TODO(yuval): Error message
        }
    }
    else
    {
        // TODO(yuval): Error message
    }
    
    while (token.type != Token_Semi)
    {
        token = GetToken(tokenizer);
    }
}

internal void
FunctionMeta(Tokenizer* tokenizer)
{
    FunctionVariable functionVars[20] = { };
    umm maxIndex = 0;
    
    umm braceCount = 1;
    while (braceCount)
    {
        Token token = GetToken(tokenizer);
        
        switch (token.type)
        {
            case Token_OpenBrace:
            {
                PlatformWriteTokenToFile(&token);
                ++braceCount;
            } break;
            
            case Token_CloseBrace:
            {
                PlatformWriteTokenToFile(&token);
                --braceCount;
            } break;
            
            case Token_Identifier:
            {
                Token nextToken = GetToken(tokenizer);
                
                if (TokenEquals(token, "SWAP") &&
                    nextToken.type == Token_OpenParen)
                {
                    SwapFunctionVariables(tokenizer, functionVars, maxIndex);
                }
                else
                {
                    if (nextToken.type == Token_Identifier)
                    {
                        FunctionVariable var = { token.text, nextToken.text };
                        functionVars[maxIndex++] = var;
                    }
                    
                    PlatformWriteTokenToFile(&token);
                    PlatformWriteTokenToFile(&nextToken);
                }
            } break;
            
            default:
            {
                PlatformWriteTokenToFile(&token);
            } break;
        }
    }
}

#endif // #if 0

#define COMPILATION_TIMED_BLOCK_NAME "Compilation"
#define LINKAGE_TIMED_BLOCK_NAME "Linkage"

internal void
print_workspace_build_stats(Time_Events_Queue* queue) {
    f32 total_build_time = -1.0f;
    f32 compilation_time = -1.0f;
    f32 linkage_time = -1.0f;
    
    Time_Event* last_timed_block = 0;
    
    Time_Event* event = read_time_event(queue);
    while (event) {
        switch (event->type) {
            case TIME_EVENT_BEGIN_BLOCK: {
                // TODO(yuval): The parent event and the OpenBlock event can change when new events
                // are inserted to the queue, FIX THIS!!!
                event->parent = last_timed_block;
                last_timed_block = event;
            } break;
            
            case TIME_EVENT_END_BLOCK: {
                if (last_timed_block) {
                    f32* time = 0;
                    
                    if (strings_match(last_timed_block->name, "BuildWorkspace")) {
                        time = &total_build_time;
                    }
                    
                    else if (strings_match(last_timed_block->name,
                                           COMPILATION_TIMED_BLOCK_NAME)) {
                        time = &compilation_time;
                    }
                    else if (strings_match(last_timed_block->name,
                                           LINKAGE_TIMED_BLOCK_NAME)) {
                        time = &linkage_time;
                    }
                    
                    if (time) {
                        *time = platform.get_seconds_elapsed(last_timed_block->clock, event->clock);
                    }
                    
                    last_timed_block = last_timed_block->parent;
                } else {
                    // TODO(yuval): Diagnostic
                }
            } break;
        }
        
        event = read_time_event(queue);
    }
    
    if ((total_build_time != -1.0f) && (compilation_time != -1.0f) && (linkage_time != -1.0f)) {
        f32 backend_time = compilation_time + linkage_time;
        f32 frontend_time = total_build_time - backend_time;
        
        printf("Front-end Time: %f seconds\n", frontend_time);
        printf("    Compilation Time: %f seconds\n", compilation_time);
        printf("    Linkage Time: %f seconds\n", linkage_time);
        printf("Back-end Time: %f seconds\n", backend_time);
        printf("Total Build Time: %f seconds\n", total_build_time);
    }
}

internal void
link_workspace(Build_Workspace* workspace, Memory_Arena* arena, b32 is_verbose_build) {
    // TODO(yuval): Instead of using the compiler as the linker use the default system linker!
    Build_Options* build_options = &workspace->options;
    Compiler_Info* compiler_info = 0;
    array_foreach (platform.compilers) {
        if (((it.type == build_options->compiler) ||
             build_options->compiler == BUILD_COMPILER_AUTO) &&
            it.path) {
            compiler_info = &it;
            array_break;
        }
    }
    
    // TODO(yuval): Create append functions for Z strings
    char full_output_path_data[PATH_MAX + 1];
    String full_output_path = make_string(full_output_path_data, 0,
                                          sizeof(full_output_path_data) - 1);
    copy(&full_output_path, build_options->output_path);
    append(&full_output_path, build_options->output_name);
    const char* output_extension = platform.get_output_extension(build_options->output_type);
    append(&full_output_path, output_extension);
    terminate_with_null(&full_output_path);
    
    const char* linker_args[512] = {};
    const char** linker_args_at = linker_args;
    *linker_args_at++ = compiler_info->name;
    
    linker_args_at += workspace->files.count;
    switch (build_options->output_type) {
        case BUILD_OUTPUT_STATIC_LIBRARY: {
            // TODO(yuval): Add support for static libraries
            //LinkerArgs[Workspace->Files.Count + 1] = "";
        } break;
        
        case BUILD_OUTPUT_SHARED_LIBRARY: {
            *linker_args_at++ = "-shared";
        } break;
    }
    
    *linker_args_at++ = "-o";
    *linker_args_at = full_output_path.data;
    
    Temporary_Memory temp_mem = begin_temporary_memory(arena);
    
    for (umm index = 0;
         index < workspace->files.count;
         ++index) {
        // TODO(yuval): Use PushCopyZ and add functions to append and set extensions for
        // null-terminated strings
        String object_file_path = push_copy_string(arena, workspace->files.paths[index]);
        if (set_extension(&object_file_path, ".o")) {
            // TODO(yuval): WE NEED TO ALLOCATE MEMORY FOR THE NULL TERMINATOR!!!!!
            terminate_with_null(&object_file_path);
            linker_args[index + 1] = object_file_path.data;
        } else {
            // TODO(yuval): Diagnostic
        }
    }
    
    if (is_verbose_build) {
        printf("Running Linker: ");
        for (const char** arg = linker_args; *arg; ++arg) {
            printf("%s ", *arg);
        }
        printf("\n\n");
    }
    
    int exit_code = platform.exec_process_and_wait(compiler_info->path,
                                                   (char**)linker_args, arena);
    global_build_succeeded &= (exit_code == 0);
    
    end_temporary_memory(temp_mem);
}

internal void
meta_process_translation_unit(const char* filename, Parser* parser) {
    printf("Processing: %s\n", filename);
    
    // TODO(yuval): Proper conversion to null terminated string
    Read_File_Result file_contents = platform.read_entire_file(filename);
    Code_File file = {make_string_slowly(filename), make_string(file_contents.contents,
                                                                file_contents.contents_size)};
    
    Ast_Translation_Unit* translation_unit = parse_translation_unit(parser, file);
    dump_translation_unit_ast(translation_unit);
}

internal PLATFORM_WORK_QUEUE_CALLBACK(do_compilation_work) {
    Compilation_Work* work = (Compilation_Work*)data;
    
    meta_process_translation_unit(work->filename, work->app->parser);
    
    const char* compiler_args[512] = {};
    compiler_args[0] = work->compiler_info->name;
    compiler_args[1] = "-I";
    compiler_args[2] = platform.build_run_tree_code_path;
    compiler_args[3] = work->filename;
    compiler_args[4] = "-c";
    
    if (work->is_verbose_build) {
        printf("Running Compiler: ");
        for (const char** arg = compiler_args; *arg; ++arg) {
            printf("%s ", *arg);
        }
        printf("\n");
    }
    
    int exit_code = platform.exec_process_and_wait(work->compiler_info->path,
                                                   (char**)compiler_args, &work->app->arena);
    
    global_build_succeeded &= (exit_code == 0);
}

internal void
compile_workspace(Build_Workspace* workspace, Build_Application* app, b32 is_verbose_build) {
    Build_Options* build_options = &workspace->options;
    Compiler_Info* compiler_info = 0;
    array_foreach(platform.compilers) {
        if (((it.type == build_options->compiler) ||
             build_options->compiler == BUILD_COMPILER_AUTO) &&
            it.path) {
            compiler_info = &it;
            array_break;
        }
    }
    
    if (compiler_info) {
        for (umm index = 0; index < workspace->files.count; ++index) {
            Compilation_Work* work = PUSH_STRUCT(&app->arena, Compilation_Work);
            
            // TODO(yuval): Add the workspace's object file path to the file name
            work->app = app;
            work->filename = push_copy_z(&app->arena, workspace->files.paths[index]);
            work->compiler_info = compiler_info;
            work->is_verbose_build = is_verbose_build;
            
#if 0
            // Multi-Threaded
            platform.add_work_queue_entry(platform.work_queue, do_compilation_work, work);
#else
            // Single-Threaded
            do_compilation_work(platform.work_queue, work);
#endif // #if 0
        }
    } else {
        // TODO(yuval): Diagnostic
    }
}

internal BUILD_CREATE_WORKSPACE(app_create_workspace) {
    Build_Workspace* result = &app->workspaces.workspaces[app->workspaces.count++];
    
    result->name = name;
    // TODO(yuval): Default workspace options
    
    return result;
}

internal START_BUILD(app_start_build) {
    Build_Application* the_app = (Build_Application*)app;
    
    // NOTE(yuval): Compilation
    Temporary_Memory temp_mem = begin_temporary_memory(&the_app->arena);
    for (umm index = 0; index < app->workspaces.count; ++index) {
        compile_workspace(&app->workspaces.workspaces[index],
                          the_app, the_app->is_verbose_build);
        
        //PrintWorkspaceBuildStats(&WorkspaceTimeEventsQueue);
    }
    platform.complete_all_work_queue_work(platform.work_queue);
    end_temporary_memory(temp_mem);
    
    // NOTE(yuval): Compilation Succeeded
    if (global_build_succeeded) {
        // NOTE(yuval): Linkage
        // TODO(yuval): Link workspaces according to their depencencies on each other
        for (umm index = 0; index < app->workspaces.count; ++index) {
            link_workspace(&app->workspaces.workspaces[index],
                           &the_app->arena, the_app->is_verbose_build);
        }
    }
}

internal BUILD_WAIT_FOR_MESSAGE(app_wait_for_message) {
    Build_Message result;
    
    return result;
}

#define GENERATED_BUILD_FILE_NAME "build_file.buildgen.cpp"
Platform_API platform;

internal b32
build_startup(Build_Application* app) {
    platform = app->platform_api;
    
    app->app_links.create_workspace_ = app_create_workspace;
    app->app_links.start_build_ = app_start_build;
    app->app_links.wait_for_message_ = app_wait_for_message;
    
    app->parser = BOOTSTRAP_PUSH_STRUCT(Parser, arena);
    // TODO(yuval): Maybe create the default types in the parser struct?
    init_default_types(&app->parser->arena);
    
    // NOTE(yuval): Build File Workspace Setup
    Build_Workspace build_file_workspace = {};
    build_file_workspace.name = BUNDLE_LITERAL("Build File");
    
    Build_Options* options = &build_file_workspace.options;
    options->optimization_level = 0; // TODO(yuval): Use max optimization level
    options->output_type = BUILD_OUTPUT_SHARED_LIBRARY;
    options->output_name = BUNDLE_LITERAL("build_file");
    options->output_path = BUNDLE_LITERAL(""); // TODO(yuval): Add an output path;
    options->compiler = BUILD_COMPILER_AUTO;
    
    build_add_file(&build_file_workspace, BUNDLE_LITERAL(GENERATED_BUILD_FILE_NAME));
    
    // NOTE(yuval): Build File Workspace Building
    Time_Events_Queue build_file_time_events_queue;
    // TODO(yuval): b32 Result = build_workspace(&build_file_workspace, &App->AppArena, &BuildFileTimeEventsQueue, false);
    b32 result = true;
    return result;
}
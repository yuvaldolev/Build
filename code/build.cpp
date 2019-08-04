#include "build.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

global_variable time_events_queue GlobalTimeEventsQueue;

#if 0
#include "build_tokenizer.cpp"
#include "build_parser.cpp"

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

internal string
ReadEntireFileIntoMemory(string FileName)
{
    string Result = {};
    FILE* File = fopen(FileName.Data, "r");
    
    if (File)
    {
        fseek(File, 0, SEEK_END);
        
        // NOTE(yuval): ftell returnes the position indicator in bytes
        Result.Count = ftell(File);
        Result.MemorySize = Result.Count;
        
        fseek(File, 0, SEEK_SET);
        
        // TODO(yuval): Replace malloc with memory arena allocation
        Result.Data = (char*)malloc(Result.MemorySize);
        fread(Result.Data, Result.MemorySize, 1, File);
        
        fclose(File);
    }
    
    return Result;
}

internal void
MetaToolProcessFile(string FileName)
{
    if (!MetaToolIsInitialized)
    {
        InitDefaultTypes();
    }
    
    printf("Processing: %.*s\n", (s32)FileName.Count, FileName.Data);
    
    string FileContents = ReadEntireFileIntoMemory(FileName);
    
    ast_file* File = ParseFile(FileName, FileContents);
    DumpAstFile(File);
}
#endif // #if 0

#define COMPILATION_TIMED_BLOCK_NAME "Compilation"
#define LINKAGE_TIMED_BLOCK_NAME "Linkage"

internal void
PrintWorkspaceBuildStats(time_events_queue* Queue)
{
    f32 TotalBuildTime = -1.0f;
    f32 CompilationTime = -1.0f;
    f32 LinkageTime = -1.0f;
    
    time_event* LastTimedBlock = 0;
    
    time_event* Event = ReadTimeEvent(Queue);
    while (Event)
    {
        switch (Event->Type)
        {
            case TimeEvent_BeginBlock:
            {
                // TODO(yuval): The parent event and the OpenBlock event can change when new events
                // are inserted to the queue, FIX THIS!!!
                Event->Parent = LastTimedBlock;
                LastTimedBlock = Event;
            } break;
            
            case TimeEvent_EndBlock:
            {
                if (LastTimedBlock)
                {
                    f32* Time = 0;
                    
                    if (StringsMatch(LastTimedBlock->Name, "BuildWorkspace"))
                    {
                        Time = &TotalBuildTime;
                    }
                    
                    else if (StringsMatch(LastTimedBlock->Name,
                                          COMPILATION_TIMED_BLOCK_NAME))
                    {
                        Time = &CompilationTime;
                    }
                    else if (StringsMatch(LastTimedBlock->Name,
                                          LINKAGE_TIMED_BLOCK_NAME))
                    {
                        Time = &LinkageTime;
                    }
                    
                    if (Time)
                    {
                        *Time = Platform.GetSecondsElapsed(LastTimedBlock->Clock, Event->Clock);
                    }
                    
                    LastTimedBlock = LastTimedBlock->Parent;
                }
                else
                {
                    // TODO(yuval): Diagnostic
                }
            } break;
        }
        
        Event = ReadTimeEvent(Queue);
    }
    
    if ((TotalBuildTime != -1.0f) && (CompilationTime != -1.0f) && (LinkageTime != -1.0f))
    {
        f32 BackendTime = CompilationTime + LinkageTime;
        f32 FrontendTime = TotalBuildTime - BackendTime;
        
        printf("Front-end Time: %f seconds\n", FrontendTime);
        printf("    Compilation Time: %f seconds\n", CompilationTime);
        printf("    Linkage Time: %f seconds\n", LinkageTime);
        printf("Back-end Time: %f seconds\n", BackendTime);
        printf("Total Build Time: %f seconds\n", TotalBuildTime);
    }
}

internal b32
BuildWorkspace(build_workspace* Workspace, memory_arena* Arena,
               time_events_queue* TimeEventsQueue, b32 IsSilentBuild = false)
{
    TimedFunction(TimeEventsQueue);
    
    yd_b32 SuccessfulBuild = true;
    
    build_options* BuildOptions = &Workspace->Options;
    compiler_info* CompilerInfo = 0;
    For (Platform.Compilers)
    {
        if (((It.Type == BuildOptions->Compiler) ||
             BuildOptions->Compiler == BuildCompiler_Auto) &&
            It.Path)
        {
            CompilerInfo = &It;
            Break;
        }
    }
    
    if (CompilerInfo)
    {
        const char* CompilerArgs[512] = {};
        CompilerArgs[0] = CompilerInfo->Name;
        CompilerArgs[1] = "-I";
        CompilerArgs[2] = Platform.BuildRunTreeCodePath;
        CompilerArgs[4] = "-c";
        
        // TODO(yuval): Create append functions for Z strings
        char FullOutputPathData[PATH_MAX + 1];
        string FullOutputPath = MakeString(FullOutputPathData, 0,
                                           sizeof(FullOutputPath) - 1);
        Copy(&FullOutputPath, BuildOptions->OutputPath);
        Append(&FullOutputPath, BuildOptions->OutputName);
        const char* OutputExtension = Platform.GetOutputExtension(BuildOptions->OutputType);
        Append(&FullOutputPath, OutputExtension);
        TerminateWithNull(&FullOutputPath);
        
        const char* LinkerArgs[512] = {};
        const char** LinkerArgsAt = LinkerArgs;
        *LinkerArgsAt++ = CompilerInfo->Name;
        
        LinkerArgsAt += Workspace->Files.Count;
        switch (BuildOptions->OutputType)
        {
            case BuildOutput_StaticLibrary:
            {
                // TODO(yuval): Add support for static libraries
                //LinkerArgs[Workspace->Files.Count + 1] = "";
            } break;
            
            case BuildOutput_SharedLibrary:
            {
                *LinkerArgsAt++ = "-shared";
            } break;
        }
        
        *LinkerArgsAt++ = "-o";
        *LinkerArgsAt = FullOutputPath.Data;
        
        temporary_memory TempMem = BeginTemporaryMemory(Arena);
        
        {
            TimedBlock(COMPILATION_TIMED_BLOCK_NAME, TimeEventsQueue);
            
            for (umm Index = 0;
                 Index < Workspace->Files.Count;
                 ++Index)
            {
                // TODO(yuval): Do I have to push this string????
                CompilerArgs[3] = PushCopyZ(Arena, Workspace->Files.Paths[Index]);
                
                // TODO(yuval): Use PushCopyZ and add functions to append and set extensions for
                // null-terminated strings
                string ObjectFilePath = PushCopyString(Arena, Workspace->Files.Paths[Index]);
                if (SetExtension(&ObjectFilePath, ".o"))
                {
                    TerminateWithNull(&ObjectFilePath);
                    LinkerArgs[Index + 1] = ObjectFilePath.Data;
                }
                else
                {
                    // TODO(yuval): Diagnostic
                }
                
                if (!IsSilentBuild)
                {
                    printf("Running Compiler: ");
                    for (const char** Arg = CompilerArgs; *Arg; ++Arg)
                    {
                        printf("%s ", *Arg);
                    }
                    printf("\n");
                }
                
                int ExitCode = Platform.ExecProcessAndWait(CompilerInfo->Path, (char**)CompilerArgs,
                                                           Arena);
                SuccessfulBuild &= (ExitCode == 0);
            }
            
            if (!IsSilentBuild)
            {
                printf("\n");
            }
        }
        
        if (SuccessfulBuild)
        {
            TimedBlock(LINKAGE_TIMED_BLOCK_NAME, TimeEventsQueue);
            
            if (!IsSilentBuild)
            {
                printf("Running Linker: ");
                for (const char** Arg = LinkerArgs; *Arg; ++Arg)
                {
                    printf("%s ", *Arg);
                }
                printf("\n\n");
            }
            int ExitCode = Platform.ExecProcessAndWait(CompilerInfo->Path, (char**)LinkerArgs, Arena);
            SuccessfulBuild &= (ExitCode == 0);
        }
        
        EndTemporaryMemory(TempMem);
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
    return SuccessfulBuild;
}

internal BUILD_CREATE_WORKSPACE(AppCreateWorkspace)
{
    build_workspace* Result = &App->Workspaces.Workspaces[App->Workspaces.Count++];
    
    Result->Name = Name;
    // TODO(yuval): Default workspace options
    
    return Result;
}

internal START_BUILD(AppStartBuild)
{
    build_application* TheApp = (build_application*)App;
    
    for (umm Index = 0; Index < App->Workspaces.Count; ++Index)
    {
        time_events_queue WorkspaceTimeEventsQueue;
        BuildWorkspace(&App->Workspaces.Workspaces[Index], &TheApp->AppArena,
                       &WorkspaceTimeEventsQueue);
        //PrintWorkspaceBuildStats(&WorkspaceTimeEventsQueue);
    }
}

internal BUILD_WAIT_FOR_MESSAGE(AppWaitForMessage)
{
    build_message Result;
    
    return Result;
}

#define GENERATED_BUILD_FILE_NAME "build_file.buildgen.cpp"
platform_api Platform;

internal b32
BuildStartup(build_application* App)
{
    Platform = App->PlatformAPI;
    
    App->AppLinks.CreateWorkspace_ = AppCreateWorkspace;
    App->AppLinks.StartBuild_ = AppStartBuild;
    App->AppLinks.WaitForMessage_ = AppWaitForMessage;
    
    // NOTE(yuval): Build File Workspace Setup
    build_workspace BuildFileWorkspace = {};
    BuildFileWorkspace.Name = MakeLitString("Build File");
    
    build_options* Options = &BuildFileWorkspace.Options;
    Options->OptimizationLevel = 0; // TODO(yuval): Use max optimization level
    Options->OutputType = BuildOutput_SharedLibrary;
    Options->OutputName = MakeLitString("build_file");
    Options->OutputPath = MakeLitString(""); // TODO(yuval): Add an output path;
    Options->Compiler = BuildCompiler_Auto;
    
    BuildAddFile(&BuildFileWorkspace, MakeLitString(GENERATED_BUILD_FILE_NAME));
    
    // NOTE(yuval): Build File Workspace Building
    time_events_queue BuildFileTimeEventsQueue;
    b32 Result = BuildWorkspace(&BuildFileWorkspace, &App->AppArena, &BuildFileTimeEventsQueue, false);
    
    return Result;
}
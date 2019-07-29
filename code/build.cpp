#include "build.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#if 0
#include "build_tokenizer.cpp"
#include "build_parser.cpp"

global_variable b32 MetaToolIsInitialized = false;
global_variable time_events_queue GlobalTimeEventsQueue;

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

// TODO(yuval): Move this to another file
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
                        *Time = MacGetSecondsElapsed(LastTimedBlock->Clock, Event->Clock);
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
BuildWorkspace(build_workspace* Workspace, memory_arena* Arena, b32 IsSilentBuild = false)
{
    TimedFunction;
    
    yd_b32 SuccessfulBuild = true;
    
    build_options* BuildOptions = &Workspace->Options;
    compiler_info* CompilerInfo = 0;
    For (GlobalCompilers)
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
        CompilerArgs[2] = GlobalBuildRunTreeCodePath;
        CompilerArgs[4] = "-c";
        
        char FullOutputPath[PATH_MAX];
        ConcatStrings(FullOutputPath, sizeof(FullOutputPath),
                      BuildOptions->OutputPath, BuildOptions->OutputName);
        
        const char* LinkerArgs[512] = {};
        LinkerArgs[0] = CompilerInfo->Name;
        
        // TODO(yuval): Add workspace options for compiling as shared/static library
        // in addition to compiling as an executable
        LinkerArgs[Workspace->Files.Count + 1] = "-o";
        LinkerArgs[Workspace->Files.Count + 2] = FullOutputPath;
        
        temporary_memory TempMem = BeginTemporaryMemory(Arena);
        
        {
            TimedBlock(COMPILATION_TIMED_BLOCK_NAME);
            
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
                    printf("Compiling File: ");
                    for (const char** Arg = CompilerArgs; *Arg; ++Arg)
                    {
                        printf("%s ", *Arg);
                    }
                    printf("\n");
                }
                
                int ExitCode = ExecProcessAndWait(CompilerInfo->Path, (char**)CompilerArgs, Arena);
                SuccessfulBuild &= (ExitCode == 0);
            }
            
            if (!IsSilentBuild)
            {
                printf("\n");
            }
        }
        
        if (SuccessfulBuild)
        {
            TimedBlock(LINKAGE_TIMED_BLOCK_NAME);
            
            if (!IsSilentBuild)
            {
                printf("Linking: ");
                for (const char** Arg = LinkerArgs; *Arg; ++Arg)
                {
                    printf("%s ", *Arg);
                }
                printf("\n\n");
            }
            int ExitCode = ExecProcessAndWait(CompilerInfo->Path, (char**)LinkerArgs, Arena);
            SuccessfulBuild &= (ExitCode == 0);
        }
        
        EndTemporaryMemory(TempMem);
    }
    else
    {
        // TODO(yuval): Diagnostic
    }
    
    return true;
}

BUILD_CREATE_WORKSPACE(AppCreateWorkspace)
{
    build_workspace* Result = &App->Workspaces.Workspaces[App->Workspaces.Count++];
    
    Result->Name = Name;
    // TODO(yuval): Default workspace options
    
    return Result;
}

START_BUILD(AppStartBuild)
{
    for (umm Index = 0; Index < App->Workspaces.Count; ++Index)
    {
        time_events_queue WorkspaceTimeEventsQueue;
        BuildWorkspace(&App->Workspaces.Workspaces[Index], &App->MemoryArena);
        //PrintWorkspaceBuildStats(&WorkspaceTimeEventsQueue);
    }
}

BUILD_WAIT_FOR_MESSAGE(AppWaitForMessage)
{
    build_message Result;
    
    return Result;
}

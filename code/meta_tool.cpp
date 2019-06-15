#include "meta_tool.h"

#include "meta_tool_tokenizer.cpp"
#include "meta_tool_parser.cpp"

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
#endif

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
        
        Result.Data = (char*)malloc(Result.MemorySize);
        fread(Result.Data, Result.MemorySize, 1, File);
        
        fclose(File);
    }
    
    return Result;
}

internal void
MetaToolProcessFile(string FileName)
{
    printf("Processing: %.*s\n", (s32)FileName.Count, FileName.Data);
    
    string FileContents = ReadEntireFileIntoMemory(FileName);
    
#if 0
    tokenizer Tokenizer = Tokenize(FileName, FileContents);
    //ConstructAST(&tokenizer);
    
    b32 parsing = true;
    while (parsing)
    {
        token Token = GetToken(&Tokenizer);
        
        switch (Token.Type)
        {
            case Token_EndOfStream:
            {
                parsing = false;
            } break;
            
            default:
            {
                printf("%d: %.*s\n", Token.Type, (s32)Token.Text.Count, Token.Text.Data);
            } break;
        }
    }
#endif
    
    ParseFile(FileName, FileContents);
}

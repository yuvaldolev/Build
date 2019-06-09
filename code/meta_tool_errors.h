#if !defined(METATOOLERRORS_H)

#include <stdio.h>
#include <stdlib.h>

internal void
PrintErrorMessage(const char* type, const char* message,
                  const String* fileData, String filepath,
                  s32 line, s32 column)
{
    const char* dataAt = fileData->data;
    
    // NOTE(yuval): Getting to the line that contains the error
    for (s32 lineNumber = 1; lineNumber < line; ++lineNumber)
    {
        while (*dataAt && (*dataAt++ != '\n'));
    }
    
    // NOTE(yuval): Ignoring all spacing in the beginning of the line
    s32 firstCharIndex = 0;
    while (IsSpacing(*dataAt))
    {
        ++firstCharIndex;
        ++dataAt;
    }
    
    u32 lineLen = 0;
    const char* lineAt = dataAt;
    
    // NOTE(yuval): Error line length calculation
    while (*lineAt && (*lineAt != '\n'))
    {
        ++lineLen;
        ++lineAt;
    }
    
    // NOTE(yuval): Error location data
    fprintf(stderr, "\n%.*s:%d:%d: %s: %s\n",
            (s32)filepath.count, filepath.data,
            line, column, type, message);
    
    // NOTE(yuval): Error line
    fprintf(stderr, "    %.*s\n    ", lineLen, dataAt);
    
    // NOTE(yuval): Error char
    for (s32 charIndex = firstCharIndex; charIndex < column - 1; ++charIndex)
    {
        fprintf(stderr, ((dataAt[charIndex] == '\t') ? "    " : " "));
    }
    
    fprintf(stderr, "^\n\n");
}

internal void
PrintWarning(const char* message, const String* fileData, String filepath, s32 line, s32 column)
{
    PrintErrorMessage("warning", message, fileData, filepath, line, column);
}

internal void
PrintError(const char* message, const String* fileData, String filepath, s32 line, s32 column)
{
    PrintErrorMessage("error", message, fileData, filepath, line, column);
    exit(1);
}

internal void
WarnToken(Token* token, const char* message)
{
    PrintWarning(message, &token->fileData,
                 token->filename, token->lineNumber,
                 token->columnNumber);
}

internal void
BadToken(Token* token, const char* message)
{
    PrintError(message, &token->fileData,
               token->filename, token->lineNumber,
               token->columnNumber);
}

#define METATOOLERRORS_H
#endif
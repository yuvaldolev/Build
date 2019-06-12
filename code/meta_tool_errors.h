#if !defined(META_TOOL_ERRORS_H)

#include <stdio.h>
#include <stdlib.h>

internal void
PrintErrorMessage(const char* Type, const char* Message,
                  const string* FileData, string FilePath,
                  s32 Line, s32 Column)
{
    const char* DataAt = FileData->Data;
    
    // NOTE(yuval): Getting to the line that contains the error
    For (LineNumber, Range(1, Line))
    {
        while (*DataAt && (*DataAt++ != '\n'));
    }
    
    // NOTE(yuval): Ignoring all spacing in the beginning of the line
    s32 FirstCharIndex = 0;
    while (IsSpacing(*DataAt))
    {
        ++FirstCharIndex;
        ++DataAt;
    }
    
    // NOTE(yuval): Error line length calculation
    u32 LineLen = 0;
    const char* LineAt = DataAt;
    
    while (*LineAt && (*LineAt != '\n'))
    {
        ++LineLen;
        ++LineAt;
    }
    
    // NOTE(yuval): Error location data
    fprintf(stderr, "%.*s:%d:%d: %s: %s\n",
            (s32)FilePath.Count, FilePath.Data,
            Line, Column, Type, Message);
    
    // NOTE(yuval): Error line
    fprintf(stderr, "    %.*s\n    ", LineLen, DataAt);
    
    // NOTE(yuval): Error char
    For (CharIndex, Range(FirstCharIndex, Column - 1))
    {
        fprintf(stderr, ((DataAt[CharIndex] == '\t') ? "    " : " "));
    }
    
    fprintf(stderr, "^\n\n");
}

internal void
ReportWarning(const char* Message, const string* FileData, string FilePath, s32 Line, s32 Column)
{
    PrintErrorMessage("warning", Message, FileData, FilePath, Line, Column);
}

internal void
ReportError(const char* Message, const string* FileData, string FilePath, s32 Line, s32 Column)
{
    PrintErrorMessage("error", Message, FileData, FilePath, Line, Column);
    exit(1);
}

internal void
WarnToken(token* Token, const char* Message)
{
    ReportWarning(Message, &Token->FileData,
                  Token->FileName, Token->LineNumber,
                  Token->ColumnNumber);
}

internal void
BadToken(token* Token, const char* Message)
{
    ReportError(Message, &Token->FileData,
                Token->FileName, Token->LineNumber,
                Token->ColumnNumber);
}

#define META_TOOL_ERRORS_H
#endif
#if !defined(BUILD_ERRORS_H)

#include <stdio.h>
#include <stdlib.h>

internal void
PrintReport(string Type, string FileData, string FilePath,
            s32 Line, s32 Column, const char* Format, va_list ArgList)
{
    const char* DataAt = FileData.Data;
    
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
    
    char Message[4096] = {};
    FormatStringList(Message, sizeof(Message), Format, ArgList);
    
    // NOTE(yuval): Error location data
    fprintf(stderr, "%.*s:%d:%d: %.*s: %s\n",
            (s32)FilePath.Count, FilePath.Data,
            Line, Column, (s32)Type.Count, Type.Data, Message);
    
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
ReportWarningList(string FileData, string FilePath,
                  s32 Line, s32 Column,
                  const char* Format, va_list ArgList)
{
    PrintReport(BundleZ("warning"), FileData, FilePath, Line, Column, Format, ArgList);
}

internal void
ReportWarning(string FileData, string FilePath,
              s32 Line, s32 Column,
              const char* Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    ReportWarningList(FileData, FilePath, Line, Column, Format, ArgList);
    va_end(ArgList);
}

internal void
ReportErrorList(string FileData, string FilePath,
                s32 Line, s32 Column,
                const char* Format, va_list ArgList)
{
    PrintReport(BundleZ("error"), FileData, FilePath, Line, Column, Format, ArgList);
    exit(1);
}

internal void
ReportError(string FileData, string FilePath,
            s32 Line, s32 Column,
            const char* Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    ReportErrorList(FileData, FilePath, Line, Column, Format, ArgList);
    va_end(ArgList);
}

internal void
WarnToken(token* Token, const char* Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    ReportWarningList(Token->FileData, Token->FileName,
                      Token->LineNumber, Token->ColumnNumber,
                      Format, ArgList);
    va_end(ArgList);
}

internal void
BadToken(token* Token, const char* Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    ReportErrorList(Token->FileData, Token->FileName,
                    Token->LineNumber, Token->ColumnNumber,
                    Format, ArgList);
    va_end(ArgList);
}

internal void
AstWarning(ast* Ast, const char* Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    ReportWarningList(Ast->MyFile->FileData, Ast->MyFile->FileName,
                      Ast->MyLine, Ast->MyColumn,
                      Format, ArgList);
    va_end(ArgList);
}

internal void
AstError(ast* Ast, const char* Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    ReportErrorList(Ast->MyFile->FileData, Ast->MyFile->FileName,
                    Ast->MyLine, Ast->MyColumn,
                    Format, ArgList);
    va_end(ArgList);
}

#define BUILD_ERRORS_H
#endif // #if !defined(BUILD_ERRORS_H)
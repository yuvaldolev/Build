#include "meta_tool.cpp"

#include <windows.h>
#include "win32_meta_tool.h"

#include <stdio.h> // TODO(yuval): Remove

#if 0
global_variable HANDLE globalDefaultProcessHeap;
global_variable HANDLE globalOutputFileHandle;
//global_variable char* globalFilesDirname;

void
PlatformExitWithMessage(const char* format, ...)
{
    /*char message[128];
    va_list argList;
    
    va_start(argList, format);
    FormatStringList(message, ArrayCount(message), format, argList);
    va_end(argList);
    
    fprintf(stderr, "ERROR: %s\n", message);
    
    CloseHandle(globalOutputFileHandle);
    Win32RestoreFiles();
    exit(-1); */
}

b32
PlatformWriteToFile(const void* memory, memory_index memorySize)
{
    b32 result = false;
    
    if (globalOutputFileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten;
        
        if (WriteFile(globalOutputFileHandle,
                      memory, (DWORD)memorySize,
                      &bytesWritten, 0))
        {
            result = (bytesWritten == memorySize);
        }
        else
        {
            // TODO(yuval): Print an error message
        }
    }
    else
    {
        // TODO(yuval): Print an error message
    }
    
    return result;
}

b32
PlatformWriteTokenToFile(Token* token)
{
    b32 isString = (token->type == Token_StringLiteral);
    
    if (isString)
    {
        PlatformWriteToFile("\"", 1);
    }
    
    b32 result = PlatformWriteToFile(token->text.data,
                                     token->text.count * sizeof(char));
    
    if (result && isString)
    {
        PlatformWriteToFile("\"", 1);
    }
    
    PlatformWriteToFile(" ", 1);
    
    return result;
}

internal b32
Win32CreateProcessAndWait(char* command)
{
    b32 result = true;
    
    STARTUPINFO startupInfo = { };
    startupInfo.cb = sizeof(STARTUPINFO);
    
    PROCESS_INFORMATION processInfo = { };
    
    if (CreateProcessA(0, command, 0, 0, false,
                       0, 0, 0, &startupInfo, &processInfo))
    {
        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else
    {
        result = false;
    }
    
    return result;
}

internal b32
Win32ExecuteCTimeCommand(const char* command, const char* timingFilename)
{
    char ctimeCommand[MAX_PATH] = { };
    FormatString(ctimeCommand, sizeof(ctimeCommand),
                 "ctime -%s %s", command, timingFilename);
    
    return Win32CreateProcessAndWait(ctimeCommand);
}

internal void
Win32StartCTime(const char* timingFilename)
{
    if (!Win32ExecuteCTimeCommand("begin", timingFilename))
    {
        fprintf(stderr, "WRANING: ctime.exe was not found! Timing data will not be captured\n");
    }
}

internal void
Win32EndCTime(const char* timingFilename)
{
    if (Win32ExecuteCTimeCommand("end", timingFilename))
    {
        DeleteFileA(timingFilename);
    }
}

internal void
Win32RestoreFiles(const char* dirNameZ)
{
    char dirNameData[MAX_PATH] = { };
    memory_index dirNameSize = MAX_PATH * sizeof(char);
    String dirName = MakeString(dirNameZ, dirNameData, dirNameSize);
    
    AppendZ(&dirName, "\\.temp*");
    
    WIN32_FIND_DATAA fileData;
    HANDLE file = FindFirstFileA(dirName.data, &fileData);
    
    if (file != INVALID_HANDLE_VALUE)
    {
        do
        {
            const char* tempFilename = fileData.cFileName;
            
            // NOTE(yuval): We do not need to subtract one for the '-' charater that's after
            // ".temp" because sizeof returns the size plus the terminating character
            const char* originalFilename = tempFilename + sizeof(".temp");
            
            MoveFileExA(tempFilename, originalFilename,
                        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
        }
        while (FindNextFileA(file, &fileData));
        
        FindClose(file);
    }
}

internal void
Win32FreeHeapMemory(void* memory)
{
    if (memory)
    {
        HeapFree(globalDefaultProcessHeap, 0, memory);
    }
}

internal HANDLE
Win32ClearFile(const char* filename)
{
    HANDLE file = CreateFileA(filename,
                              FILE_APPEND_DATA, 0,
                              0,
                              CREATE_ALWAYS,
                              0, 0);
    return file;
}

internal String
Win32ReadEntireFileIntoMemory(const char* filename)
{
    String result = { };
    
    HANDLE fileHandle = CreateFileA(filename,
                                    GENERIC_READ, FILE_SHARE_READ,
                                    0,
                                    OPEN_EXISTING,
                                    0, 0);
    
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER fileSize;
        
        if (GetFileSizeEx(fileHandle, &fileSize))
        {
            u32 fileSize32 = SafeTruncateToU32(fileSize.QuadPart);
            u32 fileDataSize = fileSize32 + sizeof(char);
            
            char* fileData = (char*)HeapAlloc(globalDefaultProcessHeap,
                                              HEAP_ZERO_MEMORY,
                                              fileDataSize);
            if (fileData)
            {
                DWORD bytesRead;
                
                if (ReadFile(fileHandle, fileData, fileSize32, &bytesRead, 0) &&
                    (bytesRead == fileSize32))
                {
                    result.data = fileData;
                    result.count = fileSize32/ sizeof(char);
                    result.memorySize = fileDataSize;
                }
                else
                {
                    // TODO(yuval): Print an error message
                    Win32FreeHeapMemory(fileData);
                }
            }
            else
            {
                // TODO(yuval): Print an error message
            }
        }
        else
        {
            // TODO(yuval): Print an error message
        }
        
        CloseHandle(fileHandle);
    }
    else
    {
        // TODO(yuval): Print an error message
    }
    
    return result;
}

internal void
Win32ProcessFile(char* filename)
{
    if (IsCodeFile(filename))
    {
        String originalFilename = WrapZ(filename);
        
        memory_index tempFilenameMemorySize = sizeof(char) * (originalFilename.count + 7);
        char* tempFilenameMemory = (char*)malloc(tempFilenameMemorySize);
        tempFilenameMemory[tempFilenameMemorySize - 1] = 0;
        String tempFilename = MakeString(".temp-", tempFilenameMemory,
                                         tempFilenameMemorySize);
        
        AppendString(&tempFilename, &originalFilename);
        
        if (CopyFile(filename, tempFilename.data, false))
        {
            String file = Win32ReadEntireFileIntoMemory(filename);
            //globalOutputFileHandle = Win32ClearFile(filename);
            
            if (globalOutputFileHandle != INVALID_HANDLE_VALUE)
            {
                MetaToolProcessFile(originalFilename, file);
                //CloseHandle(globalOutputFileHandle);
                Win32FreeHeapMemory(file.data);
            }
        }
    }
}

int
main(s32 argCount, char** args)
{
    if (argCount >= 2)
    {
        printf("Meta Tool:\n");
        Win32StartCTime("meta_tool.ctm");
        
        char dirNameData[MAX_PATH] = ".";
        memory_index dirNameSize = MAX_PATH * sizeof(char);
        String dirName = { dirNameData, 1, dirNameSize };
        
        b32 dirNameSpecified = (argCount > 2);
        if (dirNameSpecified)
        {
            // TODO(yuval): Maybe use args[2] as the actual memory
            dirName = MakeString(args[2], dirNameData, dirNameSize);
        }
        
        AppendZ(&dirName, "\\*");
        
        printf("Searching for files in: %s\n", dirName.data);
        
        WIN32_FIND_DATAA fileData;
        HANDLE file = FindFirstFileA(dirName.data, &fileData);
        
        if (file != INVALID_HANDLE_VALUE)
        {
            globalDefaultProcessHeap = GetProcessHeap();
            
            if (globalDefaultProcessHeap)
            {
                do
                {
                    Win32ProcessFile(fileData.cFileName);
                }
                while (FindNextFileA(file, &fileData));
                
                FindClose(file);
            }
            else
            {
                // TODO(yuval): Print an error message
            }
        }
        else
        {
            // TODO(yuval): Better error message
            printf("A directory with the name %s was not found!\n", dirName.data);
        }
        
        Win32EndCTime("meta_tool.ctm");
        
        printf("\nCompiling using %s:\n", args[1]);
        Win32StartCTime("compile.ctm");
        
        if (!Win32CreateProcessAndWait(args[1]))
        {
            fprintf(stderr, "Compilation Failed\n");
        }
        
        Win32EndCTime("compile.ctm");
        
        Win32RestoreFiles(dirNameSpecified ? args[2] : ".");
    }
    else
    {
        printf("win32_meta_tool.exe [build-script] [directory-name](optional)");
    }
    
    return 0;
}

#endif

int
main(int argc, const char* argv[])
{
}
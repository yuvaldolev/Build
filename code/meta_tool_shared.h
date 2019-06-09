#if !defined(METATOOLSHARED_H)

#include <stdarg.h>

#define ReadVarArgUnsignedInteger(argList, length) \
((length) == 8) ? va_arg(argList, u64) : (u64)va_arg(argList, u32)

#define ReadVarArgSignedInteger(argList, length) \
((length) == 8) ? va_arg(argList, s64) : (s64)va_arg(argList, s32)

#define ReadVarArgFloat(argList, length) va_arg(argList, f64)

struct FormatDest
{
    char* at;
    umm size;
};

internal void*
Copy(void* destInit, const void* sourceInit, memory_index size)
{
    void* result = 0;
    
    if (sourceInit && destInit)
    {
        u8* source = (u8*)sourceInit;
        u8* dest = (u8*)destInit;
        
        while (size--) { *dest++ = *source++; };
        
        result = destInit;
    }
    
    return result;
}

internal char*
CopyZ(void* dest, const char* source, umm count)
{
    char* destInit = (char*)Copy(dest, source, count * sizeof(char));
    return destInit;
}

internal char*
CopyZ(void* dest, const char* source)
{
    return CopyZ(dest, source, StringLength(source));
}

internal String
MakeString(const char* z, void* memory,
           umm count, memory_index memorySize)
{
    // NOTE(yuval): z is REQUIRED to be null terminated!
    String str = { };
    
    if (memory)
    {
        Assert(count * sizeof(char) <= memorySize);
        
        str.data = CopyZ(memory, z, count);
        str.count = count;
        str.memorySize = memorySize;
    }
    
    return str;
}

internal String
MakeString(const char* z, void* memory, memory_index memorySize)
{
    // NOTE(yuval): z is REQUIRED to be null terminated!
    return MakeString(z, memory, StringLength(z), memorySize);
}

internal String
WrapZ(char* z)
{
    u32 zLength = StringLength(z);
    String str = { z, zLength, zLength * sizeof(char)};
    return str;
}

internal void
AppendZ(String* dest, const char* source, umm count)
{
    if (dest && source)
    {
        if (((dest->count + count) * sizeof(char)) <= dest->memorySize)
        {
            char* destAt = dest->data + dest->count;
            CopyZ(destAt, source, count);
            dest->count += count;
        }
    }
}

internal void
AppendZ(String* dest, const char* source)
{
    AppendZ(dest, source, StringLength(source));
}

internal void
AppendString(String* dest, const String* source)
{
    AppendZ(dest, source->data, source->count);
}

internal void
AdvanceString(String* value, umm count)
{
    if (value->count >= count)
    {
        value->data += count;
        value->count -= count;
        value->memorySize -= (count * sizeof(char));
    }
    else
    {
        value->data += value->count;
        value->count = 0;
        value->memorySize = 0;
    }
}

internal b32
IsSpacing(char c)
{
    b32 result = ((c == ' ') ||
                  (c == '\t') ||
                  (c == '\v') ||
                  (c == '\f'));
    
    return result;
}

internal b32
IsEndOfLine(char c)
{
    b32 result = ((c == '\n') ||
                  (c == '\r'));
    
    return result;
}

internal b32
IsWhitespace(char c)
{
    b32 result = (IsSpacing(c) || IsEndOfLine(c));
    return result;
}

internal b32
IsAlpha(char c)
{
    b32 result = (((c >= 'a') && (c <= 'z')) ||
                  ((c >= 'A') && (c <= 'Z')));
    
    return result;
}

internal b32
IsNumber(char c)
{
    b32 result = ((c >= '0') && (c <= '9'));
    return result;
}


internal b32
StringsAreEqual(const char* a, const char* b)
{
    b32 result = (a == b);
    
    if (a && b)
    {
        if (!result)
        {
            while (*a && *b && (*a == *b))
            {
                ++a;
                ++b;
            }
            
            result = ((*a == 0) && (*b == 0));
        }
    }
    
    return result;
}

internal b32
StringsAreEqual(const char* a, umm aLength, const char* b)
{
    b32 result = false;
    
    if (b)
    {
        const char* at = b;
        
        for (umm index = 0; index < aLength; ++index, ++at)
        {
            if ((*at == 0) || (a[index] != *at))
            {
                return false;
            }
        }
        
        result = (*at == 0);
    }
    else
    {
        result = (aLength == 0);
    }
    
    return result;
}

internal b32
StringsAreEqual(const char* a, umm aLength,
                const char* b, umm bLength)
{
    b32 result = (aLength == bLength);
    
    if (result)
    {
        for (umm index = 0; index < aLength; ++index)
        {
            if (a[index] != b[index])
            {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

internal b32
StringsAreEqual(String a, const char* b, umm length)
{
    b32 result = false;
    
    if (length <= a.count)
    {
        result = StringsAreEqual(a.data, length, b);
    }
    
    return result;
}

internal b32
StringsAreEqual(String a, const char* b)
{
    return StringsAreEqual(a.data, a.count, b);
}

internal b32
StringsAreEqual(String a, String b)
{
    return StringsAreEqual(a.data, a.count, b.data, b.count);
}

internal b32
IsH(const char* fileExtention)
{
    return StringsAreEqual(fileExtention, "h") ||
        StringsAreEqual(fileExtention, "hpp") ||
        StringsAreEqual(fileExtention, "hin");
}

internal b32
IsCPP(const char* fileExtention)
{
    return StringsAreEqual(fileExtention, "c") ||
        StringsAreEqual(fileExtention, "cpp") ||
        StringsAreEqual(fileExtention, "cin");
}

internal const char*
FileExtention(const char* fileName)
{
    const char* fileExtention = 0;
    
    if (fileName)
    {
        const char* fileNameAt = fileName;
        
        while (*fileNameAt)
        {
            if (*fileNameAt == '.' && *(fileNameAt + 1))
            {
                fileExtention = fileNameAt + 1;
                break;
            }
            
            ++fileNameAt;
        }
    }
    
    return fileExtention;
}

internal b32
IsCodeFile(const char* filename)
{
    const char* fileExtention = FileExtention(filename);
    return IsH(fileExtention) || IsCPP(fileExtention);
}

internal s32
S32FromZInternal(const char **atInit)
{
    s32 result = 0;
    
    const char *at = *atInit;
    while((*at >= '0') &&
          (*at <= '9'))
    {
        result *= 10;
        result += (*at - '0');
        ++at;
    }
    
    *atInit = at;
    
    return result;
}

internal s32
S32FromZ(const char *at)
{
    const char *ignored = at;
    s32 result = S32FromZInternal(&ignored);
    return result;
}

internal void
OutChar(FormatDest* dest, char value)
{
    if(dest->size)
    {
        --dest->size;
        *dest->at++ = value;
    }
}

internal void
OutChars(FormatDest* dest, const char* value)
{
    // NOTE(yuval): Make this faster
    while (*value)
    {
        OutChar(dest, *value++);
    }
}

char decChars[] = "0123456789";
char lowerHexChars[] = "0123456789abcdef";
char upperHexChars[] = "0123456789ABCDEF";

internal void
U64ToASCII(FormatDest* dest, u64 value, u32 base, const char* digits)
{
    Assert(base != 0);
    
    char *start = dest->at;
    
    do
    {
        u64 digitIndex = (value % base);
        char digit = digits[digitIndex];
        OutChar(dest, digit);
        
        value /= base;
        
    } while(value != 0);
    
    char *end = dest->at;
    
    while (start < end)
    {
        --end;
        char temp = *end;
        *end = *start;
        *start = temp;
        ++start;
    }
}

internal void
F64ToASCII(FormatDest* dest, f64 value, u32 precision)
{
    if(value < 0)
    {
        OutChar(dest, '-');
        value = -value;
    }
    
    u64 integerPart = (u64)value;
    value -= (f64)integerPart;
    U64ToASCII(dest, integerPart, 10, decChars);
    
    OutChar(dest, '.');
    
    // TODO(yuval): Note that this is NOT an accurate way to do this!
    for(u32 precisionIndex = 0;
        precisionIndex < precision;
        ++precisionIndex)
    {
        value *= 10.0f;
        u32 integer = (u32)value;
        value -= (f32)integer;
        OutChar(dest, decChars[integer]);
    }
}

// NOTE(yuval): Size returned __DOES NOT__ include the null terminator.
internal umm
FormatStringList(char* destInit, umm destSize, const char* format, va_list argList)
{
    FormatDest dest = { destInit, destSize };
    
    if (dest.size)
    {
        const char *at = format;
        
        while (at[0])
        {
            if (*at == '%')
            {
                ++at;
                
                b32 forceSign = false;
                b32 padWithZeros = false;
                b32 leftJustify = false;
                b32 postiveSignIsBlank = false;
                b32 annotateIfNotZero = false;
                
                //
                // NOTE(yuval): Handle the flags
                //
                b32 parsing = true;
                while (parsing)
                {
                    switch (*at)
                    {
                        case '+': { forceSign = true; } break;
                        case '0': { padWithZeros = true; } break;
                        case '-': { leftJustify = true; } break;
                        case ' ': { postiveSignIsBlank = true; } break;
                        case '#': { annotateIfNotZero = true; } break;
                        default: { parsing = false; } break;
                    }
                    
                    if(parsing)
                    {
                        ++at;
                    }
                }
                
                //
                // NOTE(yuval): Handle the width
                //
                b32 widthSpecified = false;
                s32 width = 0;
                
                if (*at == '*')
                {
                    width = va_arg(argList, s32);
                    widthSpecified = true;
                    ++at;
                }
                else if ((*at >= '0') && (*at <= '9'))
                {
                    width = S32FromZInternal(&at);
                    widthSpecified = true;
                }
                
                //
                // NOTE(yuval): Handle the precision
                //
                b32 precisionSpecified = false;
                s32 precision = 0;
                
                if (*at == '.')
                {
                    ++at;
                    
                    if(*at == '*')
                    {
                        precision = va_arg(argList, s32);
                        precisionSpecified = true;
                        ++at;
                    }
                    else if((*at >= '0') && (*at <= '9'))
                    {
                        precision = S32FromZInternal(&at);
                        precisionSpecified = true;
                    }
                    else
                    {
                        Assert(!"Malformed precision specifier!");
                    }
                }
                
                // TODO(yuval): Right now our routine doesn't allow non-specified
                // precisions, so we just set non-specified precisions to a specified value
                if (!precisionSpecified)
                {
                    precision = 6;
                }
                
                //
                // NOTE(yuval): Handle the length
                //
                u32 integerLength = 4;
                u32 floatLength = 8;
                // TODO(yuval): Actually set different values here!
                if ((at[0] == 'h') && (at[1] == 'h'))
                {
                    at += 2;
                }
                else if ((at[0] == 'l') && (at[1] == 'l'))
                {
                    at += 2;
                }
                else if (*at == 'h')
                {
                    ++at;
                }
                else if (*at == 'l')
                {
                    integerLength = 8;
                    ++at;
                }
                else if (*at == 'j')
                {
                    ++at;
                }
                else if (*at == 'z')
                {
                    ++at;
                }
                else if (*at == 't')
                {
                    ++at;
                }
                else if (*at == 'L')
                {
                    ++at;
                }
                
                char tempBuffer[64];
                char* temp = tempBuffer;
                FormatDest tempDest = { temp, ArrayCount(tempBuffer) };
                const char* prefix = "";
                b32 isFloat = false;
                
                switch (*at)
                {
                    case 'd':
                    case 'i':
                    {
                        s64 value = ReadVarArgSignedInteger(argList, integerLength);
                        b32 wasNegative = (value < 0);
                        
                        if (wasNegative)
                        {
                            value = -value;
                        }
                        
                        U64ToASCII(&tempDest, (u64)value, 10, decChars);
                        
                        // TODO(yuval): Make this a common routine once floating
                        // point is available.
                        if (wasNegative)
                        {
                            prefix = "-";
                        }
                        else if (forceSign)
                        {
                            Assert(!postiveSignIsBlank); // NOTE(yuval): Not a problem here, but probably shouldn't be specified?
                            prefix = "+";
                        }
                        else if (postiveSignIsBlank)
                        {
                            prefix = " ";
                        }
                    } break;
                    
                    case 'u':
                    {
                        u64 value = ReadVarArgUnsignedInteger(argList, integerLength);
                        U64ToASCII(&tempDest, value, 10, decChars);
                    } break;
                    
                    case 'o':
                    {
                        u64 value = ReadVarArgUnsignedInteger(argList, integerLength);
                        U64ToASCII(&tempDest, value, 8, decChars);
                        
                        if (annotateIfNotZero && (value != 0))
                        {
                            prefix = "0";
                        }
                    } break;
                    
                    case 'x':
                    {
                        u64 value = ReadVarArgUnsignedInteger(argList, integerLength);
                        U64ToASCII(&tempDest, value, 16, lowerHexChars);
                        
                        if (annotateIfNotZero && (value != 0))
                        {
                            prefix = "0x";
                        }
                    } break;
                    
                    case 'X':
                    {
                        u64 value = ReadVarArgUnsignedInteger(argList, integerLength);
                        U64ToASCII(&tempDest, value, 16, upperHexChars);
                        
                        if (annotateIfNotZero && (value != 0))
                        {
                            prefix = "0X";
                        }
                    } break;
                    
                    // TODO(yuval): Support other kinds of floating point prints
                    // (right now we only do basic decimal output)
                    case 'f':
                    case 'F':
                    case 'e':
                    case 'E':
                    case 'g':
                    case 'G':
                    case 'a':
                    case 'A':
                    {
                        f64 value = ReadVarArgFloat(argList, floatLength);
                        F64ToASCII(&tempDest, value, precision);
                        isFloat = true;
                    } break;
                    
                    case 'c':
                    {
                        int value = va_arg(argList, int);
                        OutChar(&tempDest, (char)value);
                    } break;
                    
                    case 's':
                    {
                        char* string = va_arg(argList, char*);
                        
                        // TODO(yuval): Obey precision, width, etc.
                        
                        temp = string;
                        
                        if (precisionSpecified)
                        {
                            tempDest.size = 0;
                            for (char* scan = string;
                                 *scan && (tempDest.size < precision);
                                 ++scan)
                            {
                                ++tempDest.size;
                            }
                        }
                        else
                        {
                            tempDest.size = StringLength(string);
                        }
                        
                        tempDest.at = string + tempDest.size;
                    } break;
                    
                    case 'S':
                    {
                        String string = va_arg(argList, String);
                        
                        // TODO(yuval): Obey precision, width, etc.
                        
                        temp = string.data;
                        tempDest.size = string.count;
                        
                        if (precisionSpecified && (tempDest.size > precision))
                        {
                            tempDest.size = precision;
                        }
                        
                        tempDest.at = temp + tempDest.size;
                    } break;
                    
                    case 'p':
                    {
                        void* value = va_arg(argList, void*);
                        U64ToASCII(&tempDest, *(umm*)&value, 16, lowerHexChars);
                    } break;
                    
                    case 'n':
                    {
                        int* tabDest = va_arg(argList, int*);
                        *tabDest = (int)(dest.at - destInit);
                    } break;
                    
                    case '%':
                    {
                        OutChar(&dest, '%');
                    } break;
                    
                    default:
                    {
                        Assert(!"Unrecognized format specifier");
                    } break;
                }
                
                if (tempDest.at - temp)
                {
                    smm usePrecision = precision;
                    
                    if (isFloat || !precisionSpecified)
                    {
                        usePrecision = (tempDest.at - temp);
                    }
                    
                    smm prefixLength = StringLength(prefix);
                    smm useWidth = width;
                    smm computedWidth = usePrecision + prefixLength;
                    
                    if (useWidth < computedWidth)
                    {
                        useWidth = computedWidth;
                    }
                    
                    if (padWithZeros)
                    {
                        Assert(!leftJustify); // NOTE(yuval): Not a problem, but no way to do it?
                        leftJustify = false;
                    }
                    
                    if (!leftJustify)
                    {
                        while (useWidth > (usePrecision + prefixLength))
                        {
                            OutChar(&dest, padWithZeros ? '0' : ' ');
                            --useWidth;
                        }
                    }
                    
                    for (const char* pre = prefix;
                         *pre && useWidth;
                         ++pre)
                    {
                        OutChar(&dest, *pre);
                        --useWidth;
                    }
                    
                    if (usePrecision > useWidth)
                    {
                        usePrecision = useWidth;
                    }
                    
                    while (usePrecision > (tempDest.at - temp))
                    {
                        OutChar(&dest, '0');
                        --usePrecision;
                        --useWidth;
                    }
                    
                    while (usePrecision && (tempDest.at != temp))
                    {
                        OutChar(&dest, *temp++);
                        --usePrecision;
                        --useWidth;
                    }
                    
                    if (leftJustify)
                    {
                        while (useWidth)
                        {
                            OutChar(&dest, ' ');
                            --useWidth;
                        }
                    }
                }
                
                if(*at)
                {
                    ++at;
                }
            }
            else
            {
                OutChar(&dest, *at++);
            }
        }
        
        if (dest.size)
        {
            dest.at[0] = 0;
        }
        else
        {
            dest.at[-1] = 0;
        }
    }
    
    umm result = dest.at - destInit;
    return result;
}

// TODO(yuval): Eventually, make this return a string struct
internal umm
FormatString(char* dest, umm destSize, char* format, ...)
{
    va_list argList;
    
    va_start(argList, format);
    umm result = FormatStringList(dest, destSize, format, argList);
    va_end(argList);
    
    return result;
}

#define METATOOLSHARED_H
#endif
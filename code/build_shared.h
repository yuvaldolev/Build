#if !defined(BUILD_SHARED_H)

// TODO(yuval): Move this to a YD Header Only Library!

#include <stdarg.h>

#define ReadVarArgUnsignedInteger(Length, ArgList) (Length == 8) ? \
va_arg(ArgList, u64) : va_arg(ArgList, u32)

#define ReadVarArgSignedInteger(Length, ArgList) (Length == 8) ? \
va_arg(ArgList, s64) : va_arg(ArgList, s32)

#define ReadVarArgFloat(Length, ArgList) va_arg(ArgList, f64)

struct format_dest
{
    char* At;
    umm Size;
};

global char GlobalDecChars[] = "0123456789";
global char GlobalLowerHexChars[] = "0123456789abcdef";
global char GlobalUpperHexChars[] = "0123456789ABCDEF";

internal void
OutChar(format_dest* Dest, char Value)
{
    if (Dest->Size)
    {
        --Dest->Size;
        *Dest->At++ = Value;
    }
}

internal void
OutChars(format_dest* Dest, const char* Value)
{
    // TODO(yuval & eran): Speed this up
    while (*Value)
    {
        OutChar(Dest, *Value++);
    }
}

internal void
OutString(format_dest* Dest, string Value)
{
    while (Value.Count--)
    {
        OutChar(Dest, *Value.Data++);
    }
}

internal void
OutCharsLowercase(format_dest* Dest, const char* Value)
{
    while (*Value)
    {
        OutChar(Dest, ToLower(*Value++));
    }
}


internal void
OutCharsUppercase(format_dest* Dest, const char* Value)
{
    while (*Value)
    {
        OutChar(Dest, ToUpper(*Value++));
    }
}

internal void
U64ToASCII(format_dest* Dest, u64 Value, u32 Base, const char* Digits)
{
    Assert(Base != 0);
    
    char* Start = Dest->At;
    
    do
    {
        u64 DigitIndex = Value % Base;
        char Digit = Digits[DigitIndex];
        OutChar(Dest, Digit);
        Value /= Base;
    }
    while (Value != 0);
    
    char* End = Dest->At;
    
    while (Start < End)
    {
        --End;
        
        // TODO(yuval): Metaprogramming SWAP
        char Temp = *End;
        *End = *Start;
        *Start = Temp;
        ++Start;
    }
}

internal void
F64ToASCII(format_dest* Dest, f64 Value, u32 Precision)
{
    if (Value < 0)
    {
        OutChar(Dest, '-');
        Value = -Value;
    }
    
    u64 IntegerPart = (u64)Value;
    U64ToASCII(Dest, IntegerPart, 10, GlobalDecChars);
    
    Value -= IntegerPart;
    
    OutChar(Dest, '.');
    
    // TODO(yuval): Round to the precision
    for (u32 PrecisionIndex = 0;
         PrecisionIndex < Precision;
         PrecisionIndex++)
    {
        Value *= 10.0;
        
        u32 Integer = (u32)Value;
        OutChar(Dest, GlobalDecChars[Integer]);
        
        Value -= Integer;
    }
}

internal s32
S32FromZInternal(const char** AtInit)
{
    s32 Result = 0;
    const char* At = *AtInit;
    
    while ((*At >= '0') && (*At <= '9'))
    {
        Result *= 10;
        Result += *At - '0';
        ++At;
    }
    
    *AtInit = At;
    return Result;
}

internal s32
S32FromZ(const char* At)
{
    const char* Ignored = At;
    s32 Result = S32FromZInternal(&Ignored);
    return Result;
}

// NOTE(yuval): Size returned __DOES NOT__ include the null terminator.
internal umm
FormatStringList(char* DestInit, umm DestSize,
                 const char* Format, va_list ArgList)
{
    format_dest Dest = {DestInit, DestSize};
    
    if(Dest.Size)
    {
        const char *At = Format;
        while(At[0])
        {
            if(*At == '%')
            {
                ++At;
                
                b32 ForceSign = false;
                b32 PadWithZeros = false;
                b32 LeftJustify = false;
                b32 PostiveSignIsBlank = false;
                b32 AnnotateIfNotZero = false;
                
                b32 Parsing = true;
                
                //
                // NOTE(casey): Handle the flags
                //
                Parsing = true;
                while(Parsing)
                {
                    switch(*At)
                    {
                        case '+': {ForceSign = true;} break;
                        case '0': {PadWithZeros = true;} break;
                        case '-': {LeftJustify = true;} break;
                        case ' ': {PostiveSignIsBlank = true;} break;
                        case '#': {AnnotateIfNotZero = true;} break;
                        default: {Parsing = false;} break;
                    }
                    
                    if(Parsing)
                    {
                        ++At;
                    }
                }
                
                //
                // NOTE(casey): Handle the width
                //
                b32 WidthSpecified = false;
                s32 Width = 0;
                if(*At == '*')
                {
                    Width = va_arg(ArgList, int);
                    WidthSpecified = true;
                    ++At;
                }
                else if((*At >= '0') && (*At <= '9'))
                {
                    Width = S32FromZInternal(&At);
                    WidthSpecified = true;
                }
                
                //
                // NOTE(casey): Handle the precision
                //
                b32 PrecisionSpecified = false;
                u32 Precision = 0;
                if(*At == '.')
                {
                    ++At;
                    
                    if(*At == '*')
                    {
                        Precision = va_arg(ArgList, int);
                        PrecisionSpecified = true;
                        ++At;
                    }
                    else if((*At >= '0') && (*At <= '9'))
                    {
                        Precision = S32FromZInternal(&At);
                        PrecisionSpecified = true;
                    }
                    else
                    {
                        Assert(!"Malformed precision specifier!");
                    }
                }
                
                // TODO(casey): Right now our routine doesn't allow non-specified
                // precisions, so we just set non-specified precisions to a specified value
                if(!PrecisionSpecified)
                {
                    Precision = 6;
                }
                
                //
                // NOTE(casey): Handle the length
                //
                u32 IntegerLength = 4;
                u32 FloatLength = 8;
                // TODO(casey): Actually set different values here!
                if((At[0] == 'h') && (At[1] == 'h'))
                {
                    At += 2;
                }
                else if((At[0] == 'l') && (At[1] == 'l'))
                {
                    At += 2;
                }
                else if(*At == 'h')
                {
                    ++At;
                }
                else if(*At == 'l')
                {
                    IntegerLength = 8;
                    ++At;
                }
                else if(*At == 'j')
                {
                    ++At;
                }
                else if(*At == 'z')
                {
                    ++At;
                }
                else if(*At == 't')
                {
                    ++At;
                }
                else if(*At == 'L')
                {
                    ++At;
                }
                
                char TempBuffer[64];
                char *Temp = TempBuffer;
                format_dest TempDest = {Temp, ArrayCount(TempBuffer)};
                const char *Prefix = "";
                b32 IsFloat = false;
                
                switch(*At)
                {
                    case 'd':
                    case 'i':
                    {
                        s64 Value = ReadVarArgSignedInteger(IntegerLength, ArgList);
                        b32 WasNegative = (Value < 0);
                        if(WasNegative)
                        {
                            Value = -Value;
                        }
                        U64ToASCII(&TempDest, (u64)Value, 10, GlobalDecChars);
                        
                        // TODO(casey): Make this a common routine once floating
                        // point is available.
                        if(WasNegative)
                        {
                            Prefix = "-";
                        }
                        else if(ForceSign)
                        {
                            Assert(!PostiveSignIsBlank); // NOTE(casey): Not a problem here, but probably shouldn't be specified?
                            Prefix = "+";
                        }
                        else if(PostiveSignIsBlank)
                        {
                            Prefix = " ";
                        }
                    } break;
                    
                    case 'u':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 10, GlobalDecChars);
                    } break;
                    
                    case 'o':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 8, GlobalDecChars);
                        if(AnnotateIfNotZero && (Value != 0))
                        {
                            Prefix = "0";
                        }
                    } break;
                    
                    case 'x':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 16, GlobalLowerHexChars);
                        if(AnnotateIfNotZero && (Value != 0))
                        {
                            Prefix = "0x";
                        }
                    } break;
                    
                    case 'X':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 16, GlobalUpperHexChars);
                        if(AnnotateIfNotZero && (Value != 0))
                        {
                            Prefix = "0X";
                        }
                    } break;
                    
                    // TODO(casey): Support other kinds of floating point prints
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
                        f64 Value = ReadVarArgFloat(FloatLength, ArgList);
                        F64ToASCII(&TempDest, Value, Precision);
                        IsFloat = true;
                    } break;
                    
                    case 'c':
                    {
                        int Value = va_arg(ArgList, int);
                        OutChar(&TempDest, (char)Value);
                    } break;
                    
                    case 's':
                    {
                        char* String = va_arg(ArgList, char*);
                        
                        // TODO(casey): Obey precision, width, etc.
                        
                        Temp = String;
                        if(PrecisionSpecified)
                        {
                            TempDest.Size = 0;
                            for(char* Scan = String;
                                *Scan && (TempDest.Size < Precision);
                                ++Scan)
                            {
                                ++TempDest.Size;
                            }
                        }
                        else
                        {
                            TempDest.Size = StringLength(String);
                        }
                        TempDest.At = String + TempDest.Size;
                    } break;
                    
                    case 'S':
                    {
                        string String = va_arg(ArgList, string);
                        
                        // TODO(casey): Obey precision, width, etc.
                        
                        Temp = (char*)String.Data;
                        TempDest.Size = String.Count;
                        if(PrecisionSpecified && (TempDest.Size > Precision))
                        {
                            TempDest.Size = Precision;
                        }
                        TempDest.At = Temp + TempDest.Size;
                    } break;
                    
                    case 'p':
                    {
                        void *Value = va_arg(ArgList, void *);
                        U64ToASCII(&TempDest, *(umm *)&Value, 16, GlobalLowerHexChars);
                    } break;
                    
                    case 'n':
                    {
                        int *TabDest = va_arg(ArgList, int *);
                        *TabDest = (int)(Dest.At - DestInit);
                    } break;
                    
                    case '%':
                    {
                        OutChar(&Dest, '%');
                    } break;
                    
                    default:
                    {
                        Assert(!"Unrecognized format specifier");
                    } break;
                }
                
                if(TempDest.At - Temp)
                {
                    smm UsePrecision = Precision;
                    if(IsFloat || !PrecisionSpecified)
                    {
                        UsePrecision = (TempDest.At - Temp);
                    }
                    
                    smm PrefixLength = StringLength(Prefix);
                    smm UseWidth = Width;
                    smm ComputedWidth = UsePrecision + PrefixLength;
                    if(UseWidth < ComputedWidth)
                    {
                        UseWidth = ComputedWidth;
                    }
                    
                    if(PadWithZeros)
                    {
                        Assert(!LeftJustify); // NOTE(casey): Not a problem, but no way to do it?
                        LeftJustify = false;
                    }
                    
                    if(!LeftJustify)
                    {
                        while(UseWidth > (UsePrecision + PrefixLength))
                        {
                            OutChar(&Dest, PadWithZeros ? '0' : ' ');
                            --UseWidth;
                        }
                    }
                    
                    for(const char *Pre = Prefix;
                        *Pre && UseWidth;
                        ++Pre)
                    {
                        OutChar(&Dest, *Pre);
                        --UseWidth;
                    }
                    
                    if(UsePrecision > UseWidth)
                    {
                        UsePrecision = UseWidth;
                    }
                    while(UsePrecision > (TempDest.At - Temp))
                    {
                        OutChar(&Dest, '0');
                        --UsePrecision;
                        --UseWidth;
                    }
                    while(UsePrecision && (TempDest.At != Temp))
                    {
                        OutChar(&Dest, *Temp++);
                        --UsePrecision;
                        --UseWidth;
                    }
                    
                    if(LeftJustify)
                    {
                        while(UseWidth)
                        {
                            OutChar(&Dest, ' ');
                            --UseWidth;
                        }
                    }
                }
                
                if(*At)
                {
                    ++At;
                }
            }
            else
            {
                OutChar(&Dest, *At++);
            }
        }
        
        if(Dest.Size)
        {
            Dest.At[0] = 0;
        }
        else
        {
            Dest.At[-1] = 0;
        }
    }
    
    umm Result = Dest.At - DestInit;
    return(Result);
}

// TODO(casey): Eventually, make this return a string struct
internal umm
FormatString(char *Dest, umm DestSize, const char *Format, ...)
{
    va_list ArgList;
    
    va_start(ArgList, Format);
    umm Result = FormatStringList(Dest, DestSize, Format, ArgList);
    va_end(ArgList);
    
    return(Result);
}

#define BUILD_SHARED_H
#endif // #if !defined(BUILD_SHARED_H)
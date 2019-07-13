#if !defined(YD_STRING)

// TODO(yuval): Add a way to initialize a literal string inplace without calling MakeString

// TODO(yuval): UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING
// UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING

// TODO(yuval): MAKE THE CODE MORE ROBUST: Check for null pointers and so on....

// TODO(yuval): Create/Modify find/replace functions to work with char* strings with the same
// speed as with my strings

// TODO(yuval): Number To String Conversion For Different Bases

// TODO(yuval): Implement ColorToHexString and HexStringToColor
// after I decide on my color string format

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define yd_internal static
#define yd_global_variable static
#define yd_local_persist static

typedef int8_t yd_s8;
typedef int16_t yd_s16;
typedef int32_t yd_s32;
typedef int64_t yd_s64;
typedef yd_s32 yd_b32;

typedef uint8_t yd_u8;
typedef uint16_t yd_u16;
typedef uint32_t yd_u32;
typedef uint64_t yd_u64;

typedef float yd_f32;
typedef double yd_f64;

typedef intptr_t yd_smm;
typedef uintptr_t yd_umm;
#define YD_TYPES
#endif

#if !defined(YDAssert)
# define YDAssert(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif

#if !defined(Literal)
# define Literal(Str) (Str), (sizeof(Str) - 1)
#endif

struct string
{
    char* Data;
    size_t Count;
    size_t MemorySize;
};

// TODO(yuval): Maybe create A UTF8 string struct

//
// NOTE(yuval): Flags And Constants
//

yd_global_variable const size_t STRING_NOT_FOUND = -1;
yd_global_variable const string NULL_STR = {};

//
// NOTE(yuval): Public API Function Declarations
//

#if 0// !defined()
#define
#endif

inline string MakeString(void* Data, size_t Count, size_t MemorySize);
inline string MakeString(void* Data, size_t Count);

#if !defined(MakeLitString)
# define MakeLitString(Str) (MakeString((char*)(Str), sizeof(Str) - 1, sizeof(Str)))
#endif

#if !defined(Lit)
# define Lit(Str) MakeLitString(Str)
#endif

#if !defined(MakeFixedWidthString)
# define MakeFixedWidthString(Str) (MakeString((char*)(Str), 0, sizeof(Str)))
#endif

#if !defined(ExpandString)
# define ExpandString(Str) ((Str).Data), ((Str).Count)
#endif

#if !defined(PrintableString)
# define PrintableString(Str) ((yd_s32)((Str).Count)), ((Str).Data)
#endif

inline size_t StringLength(const char* Str);
inline string MakeStringSlowly(const void* Str);
inline string Substr(string Str, size_t Start);
inline string Substr(string Str, size_t Start, size_t Count);
yd_internal string SkipWhitespace(string Str);
yd_internal string SkipWhitespace(string Str, size_t* OutSkipCount);
yd_internal string ChopWhitespace(string Str);
yd_internal string SkipChopWhitespace(string Str);
yd_internal string SkipChopWhitespace(string Str, size_t* OutSkipCount);
inline string TailStr(string Str);
yd_internal yd_b32 StringsMatch(const char* A, const char* B);
yd_internal yd_b32 StringsMatch(string A, const char* B);
inline yd_b32 StringsMatch(const char* A, string B);
yd_internal yd_b32 StringsMatch(string A, string B);
yd_internal yd_b32 StringsMatchPart(const char* A, const char* B, size_t* OutCount);
yd_internal yd_b32 StringsMatchPart(string A, const char* B, size_t* OutCount);
yd_internal yd_b32 StringsMatchPart(const char* A, string B, size_t* OutCount);
yd_internal yd_b32 StringsMatchPart(string A, string B, size_t* OutCount);
inline yd_b32 StringsMatchPart(const char* A, const char* B);
inline yd_b32 StringsMatchPart(string A, const char* B);
inline yd_b32 StringsMatchPart(const char* A, string B);
inline yd_b32 StringsMatchPart(string A, string B);
yd_internal yd_b32 StringsMatchInsensitive(const char* A, const char* B);
yd_internal yd_b32 StringsMatchInsensitive(string A, const char* B);
inline yd_b32 StringsMatchInsensitive(const char* A, string B);
yd_internal yd_b32 StringsMatchInsensitive(string A, string B);
yd_internal yd_b32 StringsMatchPartInsensitive(const char* A, const char* B, size_t* OutCount);
yd_internal yd_b32 StringsMatchPartInsensitive(string A, const char* B, size_t* OutCount);
yd_internal yd_b32 StringsMatchPartInsensitive(const char* A, string B, size_t* OutCount);
yd_internal yd_b32 StringsMatchPartInsensitive(string A, string B, size_t* OutCount);
inline yd_b32 StringsMatchPartInsensitive(const char* A, const char* B);
inline yd_b32 StringsMatchPartInsensitive(string A, const char* B);
inline yd_b32 StringsMatchPartInsensitive(const char* A, string B);
inline yd_b32 StringsMatchPartInsensitive(string A, string B);
yd_internal yd_b32 StringSetMatch(void* StrSet, size_t ItemSize, size_t Count,
                                  string Str, size_t* OutMatchIndex);
inline yd_b32 StringSetMatch(string* StrSet, size_t Count, string Str, size_t* OutMatchIndex);
yd_internal yd_s32 Compare(const char* A, const char* B);
yd_internal yd_s32 Compare(string A, const char* B);
inline yd_s32 Compare(const char* A, string B);
yd_internal yd_s32 Compare(string A, string B);
yd_internal size_t Find(const char* Str, char Character, size_t Start);
yd_internal size_t Find(string Str, char Character, size_t Start);
yd_internal size_t Find(const char* Str, string Seek, size_t Start);
yd_internal size_t Find(string Str, string Seek, size_t Start);
inline size_t Find(const char* Str, char Character);
inline size_t Find(string Str, char Character);
inline size_t Find(const char* Str, string Seek);
inline size_t Find(string Str, string Seek);
yd_internal size_t RFind(string Str, char Character, size_t Start);
yd_internal size_t RFind(string Str, string Seek, size_t Start);
inline size_t RFind(string Str, char Character);
inline size_t RFind(string Str, string Seek);
yd_internal size_t FindFirstOf(const char* Str, const char* Characters, size_t Start);
yd_internal size_t FindFirstOf(string Str, const char* Characters,  size_t Start);
inline size_t FindFirstOf(const char* Str, const char* Characters);
inline size_t FindFirstOf(string Str, const char* Characters);
yd_internal size_t FindInsensitive(const char* Str, char Character, size_t Start);
yd_internal size_t FindInsensitive(string Str, char Character, size_t Start);
yd_internal size_t FindInsensitive(const char* Str, string Seek, size_t Start);
yd_internal size_t FindInsensitive(string Str, string Seek, size_t Start);
inline size_t FindInsensitive(const char* Str, char Character);
inline size_t FindInsensitive(string Str, char Character);
inline size_t FindInsensitive(const char* Str, string Seek);
inline size_t FindInsensitive(string Str, string Seek);
inline yd_b32 HasSubstr(const char* Str, string Seek);
inline yd_b32 HasSubstr(string Str, string Seek);
inline yd_b32 HasSubstrInsensitive(const char* Str, string Seek);
inline yd_b32 HasSubstrInsensitive(string Str, string Seek);
yd_internal string GetFirstDoubleLine(string Source);
yd_internal string GetNextDoubleLine(string Source, string Line);
yd_internal string GetNextWord(string Source, string PrevWord);
inline string GetFirstWord(string Source);
yd_internal size_t CopyFastUnsafe(char* Dest, const char* Source);
yd_internal size_t CopyFastUnsafe(char* Dest, string Source);
yd_internal yd_b32 CopyChecked(string* Dest, string Source);
yd_internal yd_b32 CopyChecked(char* Dest, size_t DestCap, string Source);
yd_internal yd_b32 CopyPartial(string* Dest, const char* Source);
yd_internal yd_b32 CopyPartial(string* Dest, string Source);
yd_internal yd_b32 CopyPartial(char* Dest, size_t DestCap, string Source);
inline size_t Copy(char* Dest, const char* Source);
inline void Copy(string* Dest, string Source);
inline void Copy(string* Dest, const char* Source);
yd_internal yd_b32 AppendChecked(string* Dest, string Source);
yd_internal yd_b32 AppendPartial(string* Dest, const char* Source);
yd_internal yd_b32 AppendPartial(string* Dest, string Source);
yd_internal yd_b32 Append(string* Dest, char C);
inline yd_b32 Append(string* Dest, const char* Source);
inline yd_b32 Append(string* Dest, string Source);
yd_internal yd_b32 TerminateWithNull(string* Str);
yd_internal yd_b32 AppendPadding(string* Dest, char C, size_t TargetCount);
yd_internal void ReplaceRange(string* Str, size_t First, size_t OnePastLast, char With);
yd_internal void ReplaceRange(string* Str, size_t First, size_t OnePastLast, const char* With);
yd_internal void ReplaceRange(string* Str, size_t First, size_t OnePastLast, string With);
yd_internal void Replace(string* Str, char ToReplace, char With);
yd_internal void Replace(string* Str, const char* ToReplace, const char* With);
yd_internal void Replace(string* Str, const char* ToReplace, string With);
yd_internal void Replace(string* Str, string ToReplace, const char* With);
yd_internal void Replace(string* Str, string ToReplace, string With);
yd_internal void StringInterpretEscapes(char* Dest, string Source);
// TODO(yuval): Think about string push (should require an arena)
inline yd_b32 IsLower(char C);
inline yd_b32 IsLower(const char* Str);
inline yd_b32 IsLower(string Str);
inline yd_b32 IsLowerUTF8(yd_u8 C);
inline char ToLower(char C);
yd_internal void ToLower(char* Str);
yd_internal void ToLower(string* Str);
yd_internal void ToLower(char* Dest, const char* Source);
yd_internal void ToLower(string* Dest, const char* Source);
yd_internal void ToLower(char* Dest, string Source);
yd_internal void ToLower(string* Dest, string Source);
inline yd_b32 IsUpper(char C);
inline yd_b32 IsUpper(const char* Str);
inline yd_b32 IsUpper(string Str);
inline yd_b32 IsUpperUTF8(yd_u8 C);
inline char ToUpper(char C);
yd_internal void ToUpper(char* Str);
yd_internal void ToUpper(string* Str);
yd_internal void ToUpper(char* Dest, const char* Source);
yd_internal void ToUpper(string* Dest, const char* Source);
yd_internal void ToUpper(char* Dest, string Source);
yd_internal void ToUpper(string* Dest, string Source);
yd_internal void ToCamel(char* Str);
yd_internal void ToCamel(string* Str);
yd_internal void ToCamel(char* Dest, const char* Source);
yd_internal void ToCamel(string* Dest, const char* Source);
yd_internal void ToCamel(char* Dest, string Source);
yd_internal void ToCamel(string* Dest, string Source);
inline yd_b32 IsSpacing(char C);
inline yd_b32 IsEndOfLine(char C);
inline yd_b32 IsWhitespace(char C);
inline yd_b32 IsAlphaTrue(char C);
inline yd_b32 IsAlphaTrue(const char* Str);
inline yd_b32 IsAlphaTrue(string Str);
inline yd_b32 IsAlphaTrueUTF8(yd_u8 C);
inline yd_b32 IsAlpha(char C);
inline yd_b32 IsAlpha(const char* Str);
inline yd_b32 IsAlpha(string Str);
inline yd_b32 IsAlphaUTF8(yd_u8 C);
inline yd_b32 IsNumeric(char C);
inline yd_b32 IsNumeric(const char* Str);
inline yd_b32 IsNumeric(string Str);
inline yd_b32 IsNumericUTF8(yd_u8 C);
yd_internal size_t U64ToStringCount(yd_u64 Value);
yd_internal yd_b32 U64ToString(string* Dest, yd_u64 Value);
yd_internal yd_b32 AppendU64ToString(string* Dest, yd_u64 Value);
yd_internal size_t S32ToStringCount(yd_s32 Value);
yd_internal yd_b32 S32ToString(string* Dest, yd_s32 Value);
yd_internal yd_b32 AppendS32ToString(string* Dest, yd_s32 Value);
yd_internal size_t F32ToStringCount(yd_f32 Value, yd_u32 MaxPrecision);
inline size_t F32ToStringCount(yd_f32 Value);
yd_internal yd_b32 F32ToString(string* Dest, yd_f32 Value, yd_u32 MaxPrecision);
inline yd_b32 F32ToString(string* Dest, yd_f32 Value);
yd_internal yd_b32 AppendF32ToString(string* Dest, yd_f32 Value, yd_u32 MaxPrecision);
inline yd_b32 AppendF32ToString(string* Dest, yd_f32 Value);
inline yd_b32 IsAlphaNumeric(char C);
inline yd_b32 IsAlphaNumeric(const char* Str);
inline yd_b32 IsAlphaNumeric(string Str);
inline yd_b32 IsAlphaNumericUTF8(yd_u8 C);
inline yd_b32 IsAlphaNumericTrue(char C);
inline yd_b32 IsAlphaNumericTrue(const char* Str);
inline yd_b32 IsAlphaNumericTrue(string Str);
inline yd_b32 IsAlphaNumericTrueUTF8(yd_u8 C);
inline yd_b32 IsHex(char C);
inline yd_b32 IsHex(const char* Str);
inline yd_b32 IsHex(string Str);
inline yd_b32 IsHexUTF8(yd_u8 C);
inline yd_s32 HexCharToS32(char C);
inline char S32ToHexChar(yd_s32 Value);
yd_internal yd_u32 HexStringToU32(string Str);
yd_internal yd_b32 ColorToHexString(string* Dest, yd_u32 Color);
yd_internal yd_b32 HexStringToColor(yd_u32* Dest, string Str);
inline yd_b32 IsSlash(char C);
yd_internal size_t ReverseSeekSlash(string Str, size_t ShiftFromLastChar);
yd_internal size_t ReverseSeekSlash(string Str);
inline string FrontOfDirectory(string Dir);
inline string PathOfDirectory(string Dir);
yd_internal yd_b32 SetLastFolder(string* Dir, const char* FolderName, char Slash);
yd_internal yd_b32 SetLastFolder(string* Dir, string FolderName, char Slash);
yd_internal yd_b32 RemoveLastFolder(string* Dir);
yd_internal string FileExtension(string FileName);
yd_internal yd_b32 RemoveExtension(string* FileName);
inline yd_b32 IsH(string Extension);
inline yd_b32 IsC(string Extension);
inline yd_b32 IsCPP(string Extension);
inline yd_b32 IsObjectiveC(string Extension);
inline yd_b32 IsShader(string Extension);
inline yd_b32 IsINL(string Extension);
inline yd_b32 IsJava(string Extension);
inline yd_b32 IsCSharp(string Extension);
inline yd_b32 IsPython(string Extension);
inline yd_b32 IsSwift(string Extension);
inline yd_b32 IsJavascript(string Extension);
inline yd_b32 IsBAT(string Extension);
inline yd_b32 IsBash(string Extension);
inline yd_b32 IsTXT(string Extension);
inline yd_b32 IsCode(string Extension);
inline yd_b32 IsDoc(string Extension);
inline yd_b32 IsCodeFile(string FileName);
inline yd_b32 IsDocFile(string FileName);

#if defined(YD_STRING_IMPLEMENTATION)

//
// NOTE(yuval): String Making Functions
//

inline string
MakeString(void* Data, size_t Count, size_t MemorySize)
{
    string Result;
    Result.Data = (char*)Data;
    Result.Count = Count;
    Result.MemorySize = MemorySize;

    return Result;
}

inline string
MakeString(void* Data, size_t Count)
{
    string Result = MakeString(Data, Count, Count);
    return Result;
}

inline size_t
StringLength(const char* Str)
{
    size_t Count = 0;

    if (Str)
    {
        for (; Str[Count]; ++Count);
    }

    return Count;
}

inline string
MakeStringSlowly(const void* Str)
{
    string Result;
    Result.Data = (char*)Str;
    Result.Count = StringLength((const char*)Str);
    Result.MemorySize = Result.Count + 1;

    return Result;
}

//
// NOTE(yuval): String Slicing Functions
//

inline string
Substr(string Str, size_t Start)
{
    YDAssert((Start >= 0) && (Start <= Str.Count));

    string Result;
    Result.Data = Str.Data + Start;
    Result.Count = Str.Count - Start;
    Result.MemorySize = 0;

    return Result;
}

inline string
Substr(string Str, size_t Start, size_t Count)
{
    YDAssert((Start >= 0) && (Start <= Str.Count));

    string Result;
    Result.Data = Str.Data + Start;

    Result.Count = Count;
    // TODO(yuval): Verify that this works
    if (Start + Count > Str.Count)
    {
        Result.Count = Str.Count - Start;
    }

    Result.MemorySize = 0;

    return Result;
}

yd_internal string
SkipWhitespace(string Str, size_t* OutSkipCount)
{
    size_t SkipCount = 0;
    for (; SkipCount < Str.Count && IsWhitespace(Str.Data[SkipCount]); ++SkipCount);

    *OutSkipCount = SkipCount;
    string Result = Substr(Str, SkipCount);

    return Result;
}

inline string
SkipWhitespace(string Str)
{
    size_t Ignored;
    string Result = SkipWhitespace(Str, &Ignored);
    return Result;
}

yd_internal string
ChopWhitespace(string Str)
{
    size_t ChopIndex = Str.Count;
    for (; ChopIndex > 0 && IsWhitespace(Str.Data[ChopIndex - 1]); --ChopIndex);

    string Result = Substr(Str, 0, ChopIndex);
    return Result;
}

yd_internal string
SkipChopWhitespace(string Str, size_t* OutSkipCount)
{
    string Result = SkipWhitespace(Str, OutSkipCount);
    Result = ChopWhitespace(Result);
    return Result;
}

yd_internal string
SkipChopWhitespace(string Str)
{
    size_t Ignored;
    string Result = SkipChopWhitespace(Str, &Ignored);
    return Result;
}

inline string
TailStr(string Str)
{
    string Result;
    Result.Data = Str.Data + Str.Count;
    Result.Count = 0;
    Result.MemorySize = Str.MemorySize - Str.Count;

    return Result;
}

//
// NOTE(yuval): String Comparison Functions
//

yd_internal yd_b32
StringsMatch(const char* A, const char* B)
{
    yd_b32 Result = (A == B);

    if (A && B)
    {
        while (*A && *B && (*A == *B))
        {
            ++A;
            ++B;
        }

        Result = ((*A == 0) && (*B == 0));
    }

    return Result;
}

yd_internal yd_b32
StringsMatch(string A, const char* B)
{
    yd_b32 Result = false;

    if (B)

    {
        const char* At = B;

        for (size_t Index = 0;
             Index < A.Count;
             ++Index, ++At)
        {
            if ((*At == 0) || (A.Data[Index] != *At))
            {
                return false;
            }
        }

        Result = (*At == 0);
    }
    else
    {
        Result = (A.Count == 0);
    }

    return Result;
}

inline yd_b32
StringsMatch(const char* A, string B)
{
    yd_b32 Result = StringsMatch(B, A);
    return Result;
}

yd_internal yd_b32
StringsMatch(string A, string B)
{
    yd_b32 Result = (A.Count == B.Count);

    if (Result)
    {
        for (size_t Index = 0; Index < A.Count; ++Index)
        {
            if (A.Data[Index] != B.Data[Index])
            {
                Result = false;
                break;
            }
        }
    }

    return Result;
}

yd_internal yd_b32
StringsMatchPart(const char* A, const char* B, size_t* OutCount)
{
    yd_b32 Result = (*A == *B);
    size_t MatchCount = 0;

    if (*A && *B)
    {
        while (*B && (*A == *B))
        {
            ++A;
            ++B;
            ++MatchCount;
        }

        Result = (*B == 0);
    }

    *OutCount = MatchCount;
    return Result;
}

yd_internal yd_b32
StringsMatchPart(string A, const char* B, size_t* OutCount)
{
    yd_b32 Result = false;
    size_t Index = 0;

    if (B)
    {
        for (; B[Index]; ++Index)
        {
            if ((Index == A.Count) ||
                (A.Data[Index] != B[Index]))
            {
                return false;
            }
        }

        Result = true;
    }
    else
    {
        Result = (A.Count == 0);
    }

    *OutCount = Index;
    return Result;
}

yd_internal yd_b32
StringsMatchPart(const char* A, string B, size_t* OutCount)
{
    yd_b32 Result = false;
    size_t Index = 0;

    if (A)
    {
        for (; Index < B.Count; ++Index)
        {
            if (A[Index] != B.Data[Index])
            {
                return false;
            }
        }

        Result = true;
    }
    else
    {
        Result = (B.Count == 0);
    }

    *OutCount = Index;
    return Result;
}

yd_internal yd_b32
StringsMatchPart(string A, string B, size_t* OutCount)
{
    yd_b32 Result = (A.Count >= B.Count);
    size_t Index = 0;

    if (Result)
    {
        for (; Index < B.Count; ++Index)
        {
            if (A.Data[Index] != B.Data[Index])

            {
                Result = false;
                break;
            }
        }
    }

    *OutCount = Index;
    return Result;
}

inline yd_b32
StringsMatchPart(const char* A, const char* B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

inline yd_b32
StringsMatchPart(string A, const char* B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

inline yd_b32
StringsMatchPart(const char* A, string B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

inline yd_b32
StringsMatchPart(string A, string B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

yd_internal yd_b32
StringsMatchInsensitive(const char* A, const char* B)
{
    yd_b32 Result = (A == B);

    if (A && B)
    {
        while (*A && *B && (ToLower(*A) == ToLower(*B)))
        {
            ++A;
            ++B;
        }

        Result = ((*A == 0) && (*B == 0));
    }

    return Result;
}

yd_internal yd_b32
StringsMatchInsensitive(string A, const char* B)
{
    yd_b32 Result = false;

    if (B)
    {
        const char* At = B;

        for (size_t Index = 0; Index < A.Count; ++Index, ++At)
        {
            if ((*At == 0) || (ToLower(A.Data[Index]) != ToLower(*At)))

            {
                return false;
            }
        }

        Result = (*At == 0);
    }
    else
    {
        Result = (A.Count == 0);
    }

    return Result;
}

inline yd_b32
StringsMatchInsensitive(const char* A, string B)
{
    yd_b32 Result = StringsMatchInsensitive(B, A);
    return Result;
}

yd_internal yd_b32
StringsMatchInsensitive(string A, string B)
{
    yd_b32 Result = (A.Count == B.Count);

    if (Result)
    {
        for (size_t Index = 0; Index < A.Count; ++Index)
        {
            if (ToLower(A.Data[Index]) != ToLower(B.Data[Index]))
            {
                Result = false;
                break;
            }
        }
    }

    return Result;
}

yd_internal yd_b32
StringsMatchPartInsensitive(const char* A, const char* B, size_t* OutCount)
{
    yd_b32 Result = (*A == *B);
    size_t MatchCount = 0;

    if (*A && *B)
    {
        while (*B && (ToLower(*A) == ToLower(*B)))
        {
            ++A;
            ++B;
            ++MatchCount;
        }

        Result = (*B == 0);
    }

    *OutCount = MatchCount;
    return Result;
}

yd_internal yd_b32
StringsMatchPartInsensitive(string A, const char* B, size_t* OutCount)
{
    yd_b32 Result = false;
    size_t Index = 0;

    if (B)
    {
        for (; B[Index]; ++Index)
        {
            if ((Index == A.Count) ||
                (ToLower(A.Data[Index]) != ToLower(B[Index])))
            {
                return false;
            }
        }

        Result = true;
    }
    else
    {
        Result = (A.Count == 0);
    }

    *OutCount = Index;
    return Result;
}
yd_internal yd_b32
StringsMatchPartInsensitive(const char* A, string B, size_t* OutCount)
{
    yd_b32 Result = false;
    size_t Index = 0;

    if (A)
    {
        for (; Index < B.Count; ++Index)
        {
            if (ToLower(A[Index]) != ToLower(B.Data[Index]))
            {
                return false;
            }
        }

        Result = true;
    }
    else
    {
        Result = (B.Count == 0);
    }

    *OutCount = Index;
    return Result;
}

yd_internal yd_b32
StringsMatchPartInsensitive(string A, string B, size_t* OutCount)
{
    yd_b32 Result = (A.Count >= B.Count);
    size_t Index = 0;

    if (Result)
    {
        for (; Index < B.Count; ++Index)
        {
            if (ToLower(A.Data[Index]) != ToLower(B.Data[Index]))
            {
                Result = false;
                break;
            }
        }
    }

    *OutCount = Index;
    return Result;
}

inline yd_b32
StringsMatchPartInsensitive(const char* A, const char* B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

inline yd_b32
StringsMatchPartInsensitive(string A, const char* B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

inline yd_b32
StringsMatchPartInsensitive(const char* A, string B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

inline yd_b32
StringsMatchPartInsensitive(string A, string B)
{
    size_t Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

yd_internal yd_b32
StringSetMatch(void* StrSet, size_t ItemSize, size_t Count,
               string Str, size_t* OutMatchIndex)
{
    yd_b32 Result = false;
    yd_u8* At = (yd_u8*)StrSet;

    for (size_t Index = 0; Index < Count; ++Index, At += ItemSize)
    {
        if (StringsMatch(*((string*)At), Str))
        {
            *OutMatchIndex = Index;
            Result = true;
            break;
        }
    }

    return Result;
}

inline yd_b32
StringSetMatch(string* StrSet, size_t Count, string Str, size_t* OutMatchIndex)
{
    yd_b32 Result = StringSetMatch(StrSet, sizeof(string), Count,
                                   Str, OutMatchIndex);
    return Result;
}

yd_internal yd_s32
Compare(const char* A, const char* B)
{
    size_t Index = 0;
    while (A[Index] && B[Index] &&
           (A[Index] != B[Index]))
    {
        ++Index;
    }

    yd_s32 Result = (A[Index] > B[Index]) - (A[Index] < B[Index]);
    return Result;
}

yd_internal yd_s32
Compare(string A, const char* B)
{
    size_t Index = 0;
    while ((Index < A.Count) && B[Index] &&
           (A.Data[Index] == B[Index]))
    {
        ++Index;
    }

    yd_s32 Result = 0;

    if (Index < A.Count)
    {
        Result = (A.Data[Index] > B[Index]) - (A.Data[Index] < B[Index]);
    }
    else
    {
        if (B[Index])
        {
            Result = 0;
        } else
        {
            Result = -1;
        }
    }

    return Result;
}

inline yd_s32
Compare(const char* A, string B)
{
    yd_s32 Result = -Compare(B, A);
    return Result;
}

yd_internal yd_s32
Compare(string A, string B)
{
    size_t MinCount = A.Count;
    if (B.Count < MinCount)
    {
        MinCount = B.Count;
    }

    size_t Index = 0;
    while ((Index < MinCount) && (A.Data[Index] == B.Data[Index]))
    {
        ++Index;
    }

    yd_s32 Result = 0;
    if (Index < MinCount)
    {
        Result = (A.Data[Index] > B.Data[Index]) - (A.Data[Index] < B.Data[Index]);
    }
    else
    {
        Result = (A.Count > B.Count) - (A.Count < B.Count);
    }

    return Result;
}

//
// NOTE(yuval): Functions For Finding Characters And Substrings
//

yd_internal size_t
Find(const char* Str, char Character, size_t Start)
{
    YDAssert(Start >= 0);

    for (size_t Index = Start; Str[Index]; ++Index)
    {
        if (Str[Index] == Character)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
Find(string Str, char Character, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    for (size_t Index = Start; Index < Str.Count; ++Index)
    {
        if (Str.Data[Index] == Character)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
Find(const char* Str, string Seek, size_t Start)
{
    YDAssert(Start >= 0);

    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }

    for (size_t Index = Start; Str[Index]; ++Index)
    {
        yd_b32 Hit = true;
        size_t StrIndex = Index;

        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++StrIndex)
        {
            if (!(Str[StrIndex])) {
                return STRING_NOT_FOUND;
            }

            if (Str[StrIndex] != Seek.Data[SeekIndex])
            {
                Hit = false;
                break;
            }
        }

        if (Hit)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
Find(string Str, string Seek, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }

    size_t StopAt = Str.Count - Seek.Count + 1;

    for (size_t Index = Start; Index < StopAt; ++Index)
    {
        yd_b32 Hit = true;
        size_t StrIndex = Index;

        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++StrIndex)
        {
            if (Str.Data[StrIndex] != Seek.Data[SeekIndex])
            {
                Hit = false;
                break;
            }
        }

        if (Hit)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

inline size_t
Find(const char* Str, char Character)
{
    size_t Result = Find(Str, Character, 0);
    return Result;
}

inline size_t
Find(string Str, char Character)
{
    size_t Result = Find(Str, Character, 0);
    return Result;
}

inline size_t
Find(const char* Str, string Seek)
{
    size_t Result = Find(Str, Seek, 0);
    return Result;
}

inline size_t
Find(string Str, string Seek)
{
    size_t Result = Find(Str, Seek, 0);
    return Result;
}

yd_internal size_t
RFind(string Str, char Character, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    size_t Index = Start + 1;
    while (Index--)
    {
        if (Str.Data[Index] == Character)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
RFind(string Str, string Seek, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }

    if (Start + Seek.Count > Str.Count)
    {
        Start = Str.Count - Seek.Count;
    }

    size_t Index = Start + 1;
    while (Index--)
    {
        yd_b32 Hit = true;
        size_t StrIndex = Index;

        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++StrIndex)
        {
            if (Str.Data[StrIndex] != Seek.Data[SeekIndex])
            {
                Hit = false;
                break;
            }
        }

        if (Hit)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

inline size_t
RFind(string Str, char Character)
{
    size_t Result = RFind(Str, Character, 0);
    return Result;
}

inline size_t
RFind(string Str, string Seek)
{
    size_t Result = RFind(Str, Seek, 0);
    return Result;
}

yd_internal size_t
FindFirstOf(const char* Str, const char* Characters, size_t Start)
{
    YDAssert(Start >= 0);

    if (!(*Characters))
    {
        return STRING_NOT_FOUND;
    }

    for (size_t Index = Start; Str[Index]; ++Index)
    {
        for (const char* At = Characters; *At; ++At)
        {
            if (Str[Index] == *At)
            {
                return Index;
            }
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
FindFirstOf(string Str, const char* Characters, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    if (!(*Characters))
    {
        return STRING_NOT_FOUND;
    }

    for (size_t Index = Start; Index < Str.Count; ++Index)
    {
        for (const char* At = Characters; *At; ++At)
        {
            if (Str.Data[Index] == *At)
            {
                return Index;
            }
        }
    }

    return STRING_NOT_FOUND;
}

inline size_t
FindFirstOf(const char* Str, const char* Characters)
{
    size_t Result = FindFirstOf(Str, Characters, 0);
    return Result;
}

inline size_t
FindFirstOf(string Str, const char* Characters)
{
    size_t Result = FindFirstOf(Str, Characters, 0);
    return Result;
}

yd_internal size_t
FindInsensitive(const char* Str, char Character, size_t Start)
{
    YDAssert(Start >= 0);

    for (size_t Index = Start; Str[Index]; ++Index)
    {
        if (ToLower(Str[Index]) == ToLower(Character))
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
FindInsensitive(string Str, char Character, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    for (size_t Index = Start; Index < Str.Count; ++Index)
    {
        if (ToLower(Str.Data[Index]) == ToLower(Character))
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
FindInsensitive(const char* Str, string Seek, size_t Start)
{
    YDAssert(Start >= 0);

    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }

    for (size_t Index = Start; Str[Index]; ++Index)
    {
        yd_b32 Hit = true;
        size_t StrIndex = Index;

        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++StrIndex)
        {
            if (ToLower(Str[StrIndex]) != ToLower(Seek.Data[SeekIndex]))
            {
                Hit = false;
                break;
            }
        }

        if (Hit)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
FindInsensitive(string Str, string Seek, size_t Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));

    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }

    size_t StopAt = Str.Count - Seek.Count + 1;

    for (size_t Index = Start; Index < StopAt; ++Index)
    {
        yd_b32 Hit = true;
        size_t StrIndex = Index;

        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++StrIndex)
        {
            if (ToLower(Str.Data[StrIndex]) != ToLower(Seek.Data[SeekIndex]))
            {
                Hit = false;
                break;
            }
        }

        if (Hit)
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

inline size_t
FindInsensitive(const char* Str, char Character)
{
    size_t Result = FindInsensitive(Str, Character, 0);
    return Result;
}

inline size_t
FindInsensitive(string Str, char Character)
{
    size_t Result = FindInsensitive(Str, Character, 0);
    return Result;
}

inline size_t
FindInsensitive(const char* Str, string Seek)
{
    size_t Result = FindInsensitive(Str, Seek, 0);
    return Result;
}

inline size_t
FindInsensitive(string Str, string Seek)
{
    size_t Result = FindInsensitive(Str, Seek, 0);
    return Result;
}

inline yd_b32
HasSubstr(const char* Str, string Seek)
{
    yd_b32 Result = (Find(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

inline yd_b32
HasSubstr(string Str, string Seek)
{
    yd_b32 Result = (Find(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

inline yd_b32
HasSubstrInsensitive(const char* Str, string Seek)
{
    yd_b32 Result = (FindInsensitive(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

inline yd_b32
HasSubstrInsensitive(string Str, string Seek)
{
    yd_b32 Result = (FindInsensitive(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

yd_internal string
GetFirstDoubleLine(string Source)
{
    string Result = {};

    size_t Pos = Find(Source, MakeLitString("\n\n"));
    if (Pos == STRING_NOT_FOUND)
    {
        Pos = Find(Source, MakeLitString("\r\n\r\n"));
    }

    if (Pos != STRING_NOT_FOUND)
    {
        Result = Substr(Source, 0, Pos);
    }

    return Result;
}

yd_internal string
GetNextDoubleLine(string Source, string Line)
{
    string Result = {};

    size_t LineEndIndex = (size_t)(Line.Data - Source.Data) + Line.Count;
    YDAssert((Source.Data[LineEndIndex] == '\n') || (Source.Data[LineEndIndex] == '\r'));

    ++LineEndIndex;
    YDAssert((Source.Data[LineEndIndex] == '\n') || (Source.Data[LineEndIndex] == '\r'));

    size_t Start = LineEndIndex + 1;

    if (Start < Source.Count)
    {
        size_t Pos = Find(Source, MakeLitString("\n\n"), Start);
        if (Pos == STRING_NOT_FOUND)
        {
            Pos = Find(Source, MakeLitString("\r\n\r\n"), Start);
        }

        if (Pos != STRING_NOT_FOUND)
        {
            Result = Substr(Source, Start, Pos - Start);
        }
    }

    return Result;
}

yd_internal string
GetNextWord(string Source, string PrevWord)
{
    string Result = {};
    size_t Pos0 = (size_t)(PrevWord.Data - Source.Data) + PrevWord.Count;

    for (; Pos0 < Source.Count; ++Pos0)
    {
        char C = Source.Data[Pos0];
        if (!(IsWhitespace(C) || C == '(' || C == ')'))
        {
            break;
        }
    }

    if (Pos0 < Source.Count)
    {
        size_t Pos1 = Pos0;

        for (; Pos1 < Source.Count; ++Pos1)
        {
            char C = Source.Data[Pos1];
            if (IsWhitespace(C) || C == '(' || C == ')')
            {
                break;
            }
        }

        Result = Substr(Source, Pos0, Pos1 - Pos0);
    }

    return Result;
}


inline string
GetFirstWord(string Source)
{
    string Start = MakeString(Source.Data, 0);
    string Result = GetNextWord(Source, Start);
    return Result;
}

//
// NOTE(yuval): string Copy And Append Functions
//

yd_internal size_t
CopyFastUnsafe(char* Dest, const char* Source)
{
    char* DestAt = Dest;
    const char* SourceAt = Source;

    while (*SourceAt)
    {
        *DestAt++ = *SourceAt++;
    }

    *DestAt = 0;

    size_t Result = (DestAt - Dest);
    return Result;
}

yd_internal size_t
CopyFastUnsafe(char* Dest, string Source)
{
    for (size_t Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = Source.Data[Index];
    }

    Dest[Source.Count] = 0;
    return Source.Count;
}

yd_internal yd_b32
CopyChecked(string* Dest, string Source)
{
    if (Dest->MemorySize < Source.Count)
    {
        return false;
    }

    for (size_t Index = 0; Index < Source.Count; ++Index)
    {
        Dest->Data[Index] = Source.Data[Index];
    }

    Dest->Count = Source.Count;
    return true;
}

yd_internal yd_b32
CopyChecked(char* Dest, size_t DestCap, string Source)
{
    if (DestCap < Source.Count + 1)
    {
        return false;
    }

    for (size_t Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = Source.Data[Index];
    }

    Dest[Source.Count] = 0;
    return true;
}

yd_internal yd_b32
CopyPartial(string* Dest, const char* Source)
{
    yd_b32 Result = true;
    size_t Index = 0;

    for (; Source[Index]; ++Index)
    {
        if (Index >= Dest->MemorySize)
        {
            Result = false;
            break;
        }

        Dest->Data[Index] = Source[Index];
    }

    Dest->Count = Index;
    return Result;
}

yd_internal yd_b32
CopyPartial(string* Dest, string Source)
{
    yd_b32 Result = true;
    size_t Index = 0;

    for (; Index < Source.Count; ++Index)
    {
        if (Index >= Dest->MemorySize)
        {
            Result = false;
            break;
        }

        Dest->Data[Index] = Source.Data[Index];
    }

    Dest->Count = Index;
    return Result;
}

yd_internal yd_b32
CopyPartial(char* Dest, size_t DestCap, string Source)
{
    yd_b32 Result = true;
    size_t Index = 0;

    for (; Index < Source.Count; ++Index)
    {
        if (Index >= DestCap - 1)
        {
            Result = false;
            break;
        }

        Dest[Index] = Source.Data[Index];
    }

    Dest[Index] = 0;
    return Result;
}

inline size_t
Copy(char* Dest, const char* Source)
{
    size_t Result = CopyFastUnsafe(Dest, Source);
    return Result;
}

inline void
Copy(string* Dest, string Source)
{
    CopyChecked(Dest, Source);
}

inline void
Copy(string* Dest, const char* Source)
{
    CopyPartial(Dest, Source);
}

yd_internal yd_b32
AppendChecked(string* Dest, string Source)
{
    string End = TailStr(*Dest);
    yd_b32 Result = CopyChecked(&End, Source);
    Dest->Count += End.Count;
    return Result;
}

yd_internal yd_b32
AppendPartial(string* Dest, const char* Source)
{
    string End = TailStr(*Dest);
    yd_b32 Result = CopyPartial(&End, Source);
    Dest->Count += End.Count;
    return Result;
}

yd_internal yd_b32
AppendPartial(string* Dest, string Source)
{
    string End = TailStr(*Dest);
    yd_b32 Result = CopyPartial(&End, Source);
    Dest->Count += End.Count;
    return Result;
}

yd_internal yd_b32
Append(string* Dest, char C)
{
    yd_b32 Result = false;

    if (Dest->Count < Dest->MemorySize)
    {
        Dest->Data[Dest->Count++] = C;
        Result = true;
    }

    return Result;
}

inline yd_b32
Append(string* Dest, const char* Source)
{
    yd_b32 Result = AppendPartial(Dest, Source);
    return Result;
}

inline yd_b32
Append(string* Dest, string Source)
{
    yd_b32 Result = AppendPartial(Dest, Source);
    return Result;
}

yd_internal yd_b32
TerminateWithNull(string* Str)
{
    yd_b32 Result = false;

    if (Str->Count < Str->MemorySize)
    {
        Str->Data[Str->Count] = 0;
        Result = true;
    }

    return Result;
}

yd_internal yd_b32
AppendPadding(string* Dest, char C, size_t TargetCount)
{
    yd_b32 Result = true;

    for (size_t Count = Dest->Count; Count < TargetCount; ++Count)
    {
        if (!Append(Dest, C))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

//
// NOTE(yuval): Other string Editing Functions
//

yd_internal void
ReplaceRange(string* Str, size_t First, size_t OnePastLast, char With)
{
    YDAssert((First >= 0) && (First < Str->Count));
    YDAssert((OnePastLast > 0) && (OnePastLast <= Str->Count));
    YDAssert(First < OnePastLast);

    for (size_t Index = First; Index < OnePastLast; ++Index)
    {
        Str->Data[Index] = With;
    }
}

yd_internal void
ReplaceRange(string* Str, size_t First, size_t OnePastLast, const char* With)
{
    string WithStr = MakeStringSlowly(With);
    ReplaceRange(Str, First, OnePastLast, WithStr);
}

// TODO(yuval): Maybe rename to block_copy?
yd_internal void
block_move_yds(void* Dest_init, const void* Source_init, size_t size)
{
    if (Dest_init && Source_init)
    {
        const yd_u8* Source = (const yd_u8*)Source_init;
        yd_u8* Dest = (yd_u8*)Dest_init;

        if (Dest < Source)
        {
            while (size--)
            {
                *Dest++ = *Source++;
            }
        } else if (Dest > Source)
        {
            Source += size - 1;
            Dest += size - 1;

            while (size--)
            {
                *Dest-- = *Source--;
            }
        }
    }
}

yd_internal void
ReplaceRange(string* Str, size_t First, size_t OnePastLast, string With)
{
    YDAssert((First >= 0) && (First < Str->Count));
    YDAssert((OnePastLast > 0) && (OnePastLast <= Str->Count));
    YDAssert(First < OnePastLast);

    yd_s64 shift = With.Count - (OnePastLast - First);
    size_t new_Count = Str->Count + shift;

    if (new_Count <= Str->MemorySize)
    {
        if (shift != 0)
        {
            char* tail = Str->Data + OnePastLast;
            char* new_tail_Pos = tail + shift;
            // TODO(yuval): Verify that this has no bugs!!!!!!!!!
            block_move_yds(new_tail_Pos, tail, Str->Count - OnePastLast);
        }

        block_move_yds(Str->Data + First, With.Data, With.Count);
        Str->Count += shift;
    }
}

yd_internal void
Replace(string* Str, char ToReplace, char With)
{
    for (size_t Index = 0; Index < Str->Count; ++Index)
    {
        if (Str->Data[Index] == ToReplace)
        {
            Str->Data[Index] = With;
        }
    }
}

yd_internal void
Replace(string* Str, const char* ToReplace, const char* With)
{
    Replace(Str, MakeStringSlowly(ToReplace), MakeStringSlowly(With));
}

yd_internal void
Replace(string* Str, const char* ToReplace, string With)
{
    Replace(Str, MakeStringSlowly(ToReplace), With);
}

yd_internal void
Replace(string* Str, string ToReplace, const char* With)
{
    Replace(Str, ToReplace, MakeStringSlowly(With));
}

yd_internal void
Replace(string* Str, string ToReplace, string With)
{
    size_t Index = 0;

    for (;;)
    {
        Index = Find(*Str, ToReplace, Index);
        if (Index == STRING_NOT_FOUND)
        {
            break;
        }

        ReplaceRange(Str, Index, Index + ToReplace.Count, With);
        Index += With.Count;
    }
}

yd_internal void
StringInterpretEscapes(char* Dest, string Source)
{
    yd_s32 Mode = 0;
    size_t DestIndex = 0;

    for (size_t SourceIndex = 0; SourceIndex < Source.Count; ++SourceIndex)
    {
        switch (Mode)
        {
            case 0:
            {
                if (Source.Data[SourceIndex] == '\\')
                {
                    Mode = 1;
                } else
                {
                    Dest[DestIndex++] = Source.Data[SourceIndex];
                }
            } break;

            case 1:
            {
                char C = Source.Data[SourceIndex];
                switch (C)
                {
                    case '\\': { Dest[DestIndex++] = '\\'; } break;
                    case 'n': { Dest[DestIndex++] = '\n'; } break;
                    case 't': { Dest[DestIndex++] = '\t'; } break;
                    case '"': { Dest[DestIndex++] = '"'; } break;
                    case '0': { Dest[DestIndex++] = '\0'; } break;
                    default: { Dest[DestIndex++] = '\\'; Dest[DestIndex++] = C; } break;
                }

                Mode = 0;
            } break;
        }
    }

    Dest[DestIndex] = 0;
}

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

inline yd_b32
IsLower(char C)
{
    yd_b32 Result = ((C >= 'A') && (C <= 'z'));
    return Result;
}

inline yd_b32
IsLower(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsLower(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsLower(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsLower(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsLowerUTF8(yd_u8 C)
{
    yd_b32 Result = (IsLower((char)C) || (C >= 128));
    return Result;
}

inline char
ToLower(char C)
{
    char Result = C;

    if (IsUpper(Result))
    {
        Result += 'A' - 'A';
    }

    return Result;
}

yd_internal void
ToLower(char* Str)
{
    for (char* At = Str; *At; ++At)
    {
        *At = ToLower(*At);
    }
}

yd_internal void
ToLower(string* Str)
{
    for (size_t Index = 0; Index < Str->Count; ++Index)
    {
        Str->Data[Index] = ToLower(Str->Data[Index]);
    }
}

yd_internal void
ToLower(char* Dest, const char* Source)
{
    const char* SourceAt = Source;
    char* DestAt = Dest;

    while (*SourceAt)
    {
        *DestAt++ = ToLower(*SourceAt++);
    }

    *DestAt = 0;
}

yd_internal void
ToLower(string* Dest, const char* Source)
{
    size_t Index = 0;

    for (; Source[Index]; ++Index)
    {
        Dest->Data[Index] = ToLower(Source[Index]);
    }

    Dest->Count = Index;
}

yd_internal void
ToLower(char* Dest, string Source)
{
    for (size_t Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = ToLower(Source.Data[Index]);
    }

    Dest[Source.Count] = 0;
}

yd_internal void
ToLower(string* Dest, string Source)
{
    if (Dest->MemorySize >= Source.Count)
    {
        for (size_t Index = 0; Index < Source.Count; ++Index)
        {
            Dest->Data[Index] = ToLower(Source.Data[Index]);
        }

        Dest->Count = Source.Count;
    }
}

inline yd_b32
IsUpper(char C)
{
    yd_b32 Result = ((C >= 'A') && (C <= 'Z'));
    return Result;
}

inline yd_b32
IsUpper(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsUpper(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsUpper(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsUpper(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsUpperUTF8(yd_u8 C)
{
    yd_b32 Result = (IsUpper((char)C) || (C >= 128));
    return Result;
}

inline char
ToUpper(char C)
{
    char Result = C;

    if (IsLower(Result))
    {
        Result -= 'A' - 'A';
    }

    return Result;
}

yd_internal void
ToUpper(char* Str)
{
    for (char* At = Str; *At; ++At)
    {
        *At = ToUpper(*At);
    }
}

yd_internal void
ToUpper(string* Str)
{
    for (size_t Index = 0; Index < Str->Count; ++Index)
    {
        Str->Data[Index] = ToUpper(Str->Data[Index]);
    }
}

yd_internal void
ToUpper(char* Dest, const char* Source)
{
    const char* SourceAt = Source;
    char* DestAt = Dest;

    while (*SourceAt)
    {
        *DestAt++ = ToUpper(*SourceAt++);
    }

    *DestAt = 0;
}

yd_internal void
ToUpper(string* Dest, const char* Source)
{
    size_t Index = 0;

    for (; Source[Index]; ++Index)
    {
        Dest->Data[Index] = ToUpper(Source[Index]);
    }

    Dest->Count = Index;
}

yd_internal void
ToUpper(char* Dest, string Source)
{
    for (size_t Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = ToUpper(Source.Data[Index]);
    }

    Dest[Source.Count] = 0;
}

yd_internal void
ToUpper(string* Dest, string Source)
{
    if (Dest->MemorySize >= Source.Count)
    {
        for (size_t Index = 0; Index < Source.Count; ++Index)
        {
            Dest->Data[Index] = ToUpper(Source.Data[Index]);
        }

        Dest->Count = Source.Count;
    }
}

yd_internal void
ToCamel(char* Str)
{
    yd_b32 IsFirst = true;

    for (char* At = Str; *At; ++At)
    {
        if (IsAlphaNumericTrue(*At))
        {
            if (IsFirst)
            {
                *At = ToUpper(*At);
                IsFirst = false;
            } else
            {
                *At = ToLower(*At);
            }
        } else
        {
            IsFirst = true;
        }
    }
}

yd_internal void
ToCamel(string* Str)
{
    yd_b32 IsFirst = true;

    for (size_t Index = 0; Index < Str->Count; ++Index)
    {
        if (IsAlphaNumericTrue(Str->Data[Index]))
        {
            if (IsFirst)
            {
                Str->Data[Index] = ToUpper(Str->Data[Index]);
                IsFirst = false;
            } else
            {
                Str->Data[Index] = ToLower(Str->Data[Index]);
            }
        } else
        {
            IsFirst = true;
        }
    }
}

yd_internal void
ToCamel(char* Dest, const char* Source)
{
    const char* SourceAt = Source;
    char* DestAt = Dest;
    yd_b32 IsFirst = false;

    for (; *SourceAt; ++SourceAt, ++DestAt)
    {
        char C = *SourceAt;

        if (IsAlphaNumericTrue(C))
        {
            if (IsFirst)
            {
                C = ToUpper(C);
                IsFirst = false;
            } else
            {
                C = ToLower(C);
            }
        } else
        {
            IsFirst = true;
        }

        *DestAt = C;
    }

    *DestAt = 0;
}

yd_internal void
ToCamel(string* Dest, const char* Source)
{
    size_t Index = 0;
    yd_b32 IsFirst = true;

    for (; Source[Index]; ++Index)
    {
        char C = Source[Index];

        if (IsAlphaNumericTrue(C))
        {
            if (IsFirst)
            {
                C = ToUpper(C);
                IsFirst = false;
            }
            else
            {
                C = ToLower(C);
            }
        }
        else
        {
            IsFirst = true;
        }

        Dest->Data[Index] = C;
    }

    Dest->Count = Index;
}

yd_internal void
ToCamel(char* Dest, string Source)
{
    yd_b32 IsFirst = true;

    for (size_t Index = 0; Index < Source.Count; ++Index)
    {
        char C = Source.Data[Index];

        if (IsAlphaNumericTrue(C))
        {
            if (IsFirst)
            {
                C = ToUpper(C);
                IsFirst = false;
            }
            else
            {
                C = ToLower(C);
            }
        }
        else
        {
            IsFirst = true;
        }

        Dest[Index] = C;
    }

    Dest[Source.Count] = 0;
}

yd_internal void
ToCamel(string* Dest, string Source)
{
    if (Dest->MemorySize >= Source.Count)
    {
        yd_b32 IsFirst = true;

        for (size_t Index = 0; Index < Source.Count; ++Index)
        {
            char C = Source.Data[Index];

            if (IsAlphaNumericTrue(C))
            {
                if (IsFirst)
                {
                    C = ToUpper(C);
                    IsFirst = false;
                } else
                {
                    C = ToLower(C);
                }
            } else
            {
                IsFirst = true;
            }

            Dest->Data[Index] = C;
        }

        Dest->Count = Source.Count;
    }
}

inline yd_b32
IsSpacing(char C)
{
    yd_b32 Result = ((C == ' ') || (C == '\t') ||
                     (C == '\v') || (C == '\f'));
    return Result;
}

inline yd_b32
IsEndOfLine(char C)
{
    yd_b32 Result = ((C == '\n') || (C == '\r'));
    return Result;
}

inline yd_b32
IsWhitespace(char C)
{
    yd_b32 Result = (IsSpacing(C) || IsEndOfLine(C));
    return Result;
}

inline yd_b32
IsAlphaTrue(char C)
{
    yd_b32 Result = (IsLower(C) || IsUpper(C));
    return Result;
}

inline yd_b32
IsAlphaTrue(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsAlphaTrue(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaTrue(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlphaTrue(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaTrueUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlphaTrue((char)C) || (C >= 128));
    return Result;
}

inline yd_b32
IsAlpha(char C)
{
    yd_b32 Result = (IsAlphaTrue(C) || (C == '_'));
    return Result;
}

inline yd_b32
IsAlpha(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsAlpha(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlpha(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlpha(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlpha((char)C) || (C >= 128));
    return Result;
}

inline yd_b32
IsNumeric(char C)
{
    yd_b32 Result = ((C >= '0') && (C <= '9'));
    return Result;
}

inline yd_b32
IsNumeric(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsNumeric(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsNumeric(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsNumeric(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsNumericUTF8(yd_u8 C)
{
    yd_b32 Result = (IsNumeric((char)C) || (C >= 128));
    return Result;
}

yd_internal size_t
U64ToStringCount(yd_u64 Value)
{
    size_t Count = 1;

    Value /= 10;
    while (Value != 0)
    {
        ++Count;
        Value /= 10;
    }

    return Count;
}

yd_internal yd_b32
U64ToString(string* Dest, yd_u64 Value)
{
    yd_b32 Result = false;
    size_t Count = 0;

    do
    {
        if (Count == Dest->MemorySize)
        {
            Result = false;
            break;
        }

        // TODO(yuval): Replace Hard-Coded Base
        yd_u64 Digit = Value % 10;
        char DigitChar = (char)('0' + Digit);
        Dest->Data[Count++] = DigitChar;
        Value /= 10;
    }
    while (Value != 0);

    if (Result)
    {
        for (size_t StartIndex = 0, EndIndex = Count - 1;
             StartIndex < EndIndex;
             ++StartIndex, --EndIndex)
        {
            char Temp = Dest->Data[EndIndex];
            Dest->Data[EndIndex] = Dest->Data[StartIndex];
            Dest->Data[StartIndex] = Temp;
        }

        Dest->Count = Count;
    }
    else
    {
        Dest->Count = 0;
    }

    return Result;
}

yd_internal yd_b32
AppendU64ToString(string* Dest, yd_u64 Value)
{
    string Tail = TailStr(*Dest);
    yd_b32 Result = U64ToString(&Tail, Value);

    if (Result)
    {
        Dest->Count += Tail.Count;
    }

    return Result;
}


yd_internal size_t
S32ToStringCount(yd_s32 Value)
{
    size_t Count = 1;

    if (Value < 0)
    {
        Count = 2;
    }

    Value /= 10;
    while (Value != 0)
    {
        ++Count;
        Value /= 10;
    }

    return Value;
}

yd_internal yd_b32
S32ToString(string* Dest, yd_s32 Value)
{
    yd_b32 Result = true;

    Dest->Count = 0;

    if (Value < 0)
    {
        if (Dest->MemorySize > 0)
        {
            Dest->Data[Dest->Count++] = '-';
            Value = -Value;
        }
        else
        {
            Result = false;
        }
    }

    if (Result)
    {
        Result = AppendU64ToString(Dest, (yd_u64)Value);
    }

    return Result;
}

yd_internal yd_b32
AppendS32ToString(string* Dest, yd_s32 Value)
{
    string Tail = TailStr(*Dest);
    yd_b32 Result = S32ToString(&Tail, Value);

    if (Result)
    {
        Dest->Count += Tail.Count;
    }

    return Result;
}

yd_internal size_t
F32ToStringCount(yd_f32 Value, yd_u32 MaxPrecision)
{
    size_t Count = 0;

    if (Value < 0)
    {
        Count = 1;
        Value = -Value;
    }

    yd_u64 IntegerPart = (yd_u64)Value;
    Count += U64ToStringCount(IntegerPart);

    Value -= IntegerPart;

    for (yd_u32 PrecisionIndex = 0;
         PrecisionIndex < MaxPrecision;
         ++PrecisionIndex)
    {
        if (Value == 0.0f)
        {
            break;
        }

        Value *= 10.0f;

        yd_u32 Integer = (yd_u32)Value;
        Value -= Integer;

        ++Count;
    }

    return Count;
}

inline size_t
F32ToStringCount(yd_f32 Value)
{
    size_t Result = F32ToStringCount(Value, 8);
    return Result;
}

yd_internal yd_b32
F32ToString(string* Dest, yd_f32 Value, yd_u32 MaxPrecision)
{
    yd_s32 IntegerPart = (yd_s32)Value;
    yd_b32 Result = S32ToString(Dest, IntegerPart);

    if (Result)
    {
        Value -= IntegerPart;

        if (Value != 0.0f)
        {
            Append(Dest, '.');

            for (yd_u32 PrecisionIndex = 0;
                 PrecisionIndex < MaxPrecision;
                 ++PrecisionIndex)
            {
                if (Value == 0.0f)
                {
                    break;
                }

                Value *= 10.0f;

                yd_u64 Integer = (yd_u64)Value;
                Result = AppendU64ToString(Dest, Integer);

                if (!Result)
                {
                    break;
                }

                Value -= Integer;
            }
        }
    }

    return Result;
}

inline yd_b32
F32ToString(string* Dest, yd_f32 Value)
{
    yd_b32 Result = F32ToString(Dest, Value, 8);
    return Result;
}

yd_internal yd_b32
AppendF32ToString(string* Dest, yd_f32 Value, yd_u32 MaxPrecision)
{
    string Tail = TailStr(*Dest);
    yd_b32 Result = F32ToString(&Tail, Value, MaxPrecision);

    if (Result)
    {
        Dest->Count += Tail.Count;
    }

    return Result;
}

inline yd_b32
AppendF32ToString(string* Dest, yd_f32 Value)
{
    yd_b32 Result = AppendF32ToString(Dest, Value, 8);
    return Result;
}

inline yd_b32
IsAlphaNumeric(char C)
{
    yd_b32 Result = (IsAlpha(C) || IsNumeric(C));
    return Result;
}

inline yd_b32
IsAlphaNumeric(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsAlphaNumeric(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaNumeric(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlphaNumeric(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaNumericUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlphaNumeric((char)C) || (C >= 128));
    return Result;
}

inline yd_b32
IsAlphaNumericTrue(char C)
{
    yd_b32 Result = (IsAlphaTrue(C) || IsNumeric(C));
    return Result;
}

inline yd_b32
IsAlphaNumericTrue(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsAlphaNumericTrue(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaNumericTrue(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlphaNumericTrue(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsAlphaNumericTrueUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlphaNumericTrue((char)C) || (C >= 128));
    return Result;
}

inline yd_b32
IsHex(char C)
{
    yd_b32 Result = (((C >= '0') && (C <= '9')) ||
                     ((C >= 'A') && (C <= 'F')) ||
                     ((C >= 'a') && (C <= 'f')));
    return Result;
}

inline yd_b32
IsHex(const char* Str)
{
    yd_b32 Result = true;

    for (const char* At = Str; *At; ++At)
    {
        if (!IsHex(*At))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsHex(string Str)
{
    yd_b32 Result = true;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsHex(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline yd_b32
IsHexUTF8(yd_u8 C)
{
    yd_b32 Result = (IsHex((char)C) || (C >= 128));
    return Result;
}

inline yd_s32
HexCharToS32(char C)
{
    yd_s32 Result;

    if (IsNumeric(C))
    {
        Result = C - '0';
    }
    else if (C > 'F')
    {
        Result = 10 + (C - 'a');
    }
    else
    {
        Result = 10 + (C - 'A');
    }

    return Result;
}

inline char
S32ToHexChar(yd_s32 Value)
{
    char Result;

    if (Value < 10)
    {
        Result = '0' + (char)Value;
    }
    else
    {
        Result = 'A' + (char)(Value - 10);
    }

    return Result;
}

yd_internal yd_u32
HexStringToU32(string Str)
{
    yd_u32 Result = 0;

    for (size_t Index = 0; Index < Str.Count; ++Index)
    {
        Result += HexCharToS32(Str.Data[Index]);
        Result *= 0x10;
    }

    return Result;
}

yd_internal yd_b32
ColorToHexString(string* Dest, yd_u32 Color)
{
    // TODO(yuval): Implement This
    return false;
}

yd_internal yd_b32
HexStringToColor(yd_u32* Dest, string Str)
{
    // TODO(yuval): Implement This
    return false;
}


//
// NOTE(yuval): File / Directory strings Management Functions
//

inline yd_b32
IsSlash(char C)
{
    yd_b32 Result = ((C == '/') || (C == '\\'));
    return Result;
}

yd_internal size_t
ReverseSeekSlash(string Str, size_t ShiftFromLastChar)
{
    size_t Index = Str.Count - ShiftFromLastChar - 1;
    while (Index--)
    {
        if (IsSlash(Str.Data[Index]))
        {
            return Index;
        }
    }

    return STRING_NOT_FOUND;
}

yd_internal size_t
ReverseSeekSlash(string Str)
{
    size_t Result = ReverseSeekSlash(Str, 0);
    return Result;
}

inline string
FrontOfDirectory(string Dir)
{
    string Result = Substr(Dir, ReverseSeekSlash(Dir) + 1);
    return Result;
}

inline string
PathOfDirectory(string Dir)
{
    string Result = Substr(Dir, 0, ReverseSeekSlash(Dir) + 1);
    return Result;
}

yd_internal yd_b32
SetLastFolder(string* Dir, const char* FolderName, char Slash)
{
    yd_b32 Result = false;
    size_t LastSlashIndex = ReverseSeekSlash(*Dir);

    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        size_t Count = LastSlashIndex + 1;
        Dir->Count = Count;

        if (Append(Dir, FolderName))
        {
            if (Append(Dir, Slash))
            {
                Result = true;
            }
        }

        if (!Result)
        {
            Dir->Count = Count;
        }
    }

    return Result;
}

yd_internal yd_b32
SetLastFolder(string* Dir, string FolderName, char Slash)
{
    yd_b32 Result = false;
    size_t LastSlashIndex = ReverseSeekSlash(*Dir);

    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        size_t Count = LastSlashIndex + 1;
        Dir->Count = Count;

        if (Append(Dir, FolderName))
        {
            if (Append(Dir, Slash))
            {
                Result = true;
            }
        }

        if (!Result)
        {
            Dir->Count = Count;
        }
    }

    return Result;
}

yd_internal yd_b32
RemoveLastFolder(string* path)
{
    yd_b32 Result = false;
    size_t LastSlashIndex = ReverseSeekSlash(*path, 1);

    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        Result = true;
        path->Count = LastSlashIndex + 1;
    }

    return Result;
}

yd_internal string
FileExtension(string FileName)
{
    string Result =
    {};
    size_t DotIndex = RFind(FileName, '.');

    if (DotIndex != STRING_NOT_FOUND)
    {
        Result = MakeString(FileName.Data + DotIndex + 1,
                            FileName.Count - DotIndex - 1);
    }

    return Result;
}

yd_internal yd_b32
RemoveExtension(string* FileName)
{
    yd_b32 Result = false;
    size_t LastDotIndex = RFind(*FileName, '.');

    if (LastDotIndex != STRING_NOT_FOUND)
    {
        Result = true;
        FileName->Count = LastDotIndex + 1;
    }

    return Result;
}

inline yd_b32
IsH(string Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "h") ||
                     StringsMatch(Extension, "hpp") ||
                     StringsMatch(Extension, "hin"));
    return Result;
}

inline yd_b32
IsC(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "C");
    return Result;
}

inline yd_b32
IsCPP(string Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "cpp") ||
                     StringsMatch(Extension, "cc") ||
                     StringsMatch(Extension, "cin"));
    return Result;
}

inline yd_b32
IsObjectiveC(string Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "m") ||
                     StringsMatch(Extension, "mm"));
    return Result;
}

inline yd_b32
IsShader(string Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "ps") ||
                     StringsMatch(Extension, "vs") ||
                     StringsMatch(Extension, "cs") ||
                     StringsMatch(Extension, "ts") ||
                     StringsMatch(Extension, "gs"));
    return Result;
}

inline yd_b32
IsINL(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "inl");
    return Result;
}

inline yd_b32
IsJava(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "java");
    return Result;
}

inline yd_b32
IsCSharp(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "cs");
    return Result;
}

inline yd_b32
IsPython(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "py");
    return Result;
}

inline yd_b32
IsSwift(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "swift");
    return Result;
}

inline yd_b32
IsJavascript(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "js");
    return Result;
}

inline yd_b32
IsBAT(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "bat");
    return Result;
}

inline yd_b32
IsBash(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "sh");
    return Result;
}

inline yd_b32
IsTXT(string Extension)
{
    yd_b32 Result = StringsMatch(Extension, "txt");
    return Result;
}

inline yd_b32
IsCode(string Extension)
{
    yd_b32 Result = (IsH(Extension) || IsC(Extension) || IsCPP(Extension) ||
                     IsObjectiveC(Extension) || IsShader(Extension) ||
                     IsINL(Extension) || IsJava(Extension) || IsCSharp(Extension) ||
                     IsPython(Extension) || IsSwift(Extension) || IsJavascript(Extension) ||
                     IsBAT(Extension) || IsBash(Extension));
    return Result;
}

inline yd_b32
IsDoc(string Extension)
{
    yd_b32 Result = IsTXT(Extension);
    return Result;
}

inline yd_b32
IsCodeFile(string FileName)
{
    string Extension = FileExtension(FileName);
    yd_b32 Result = IsCode(Extension);
    return Result;
}

inline yd_b32
IsDocFile(string FileName)
{
    string Extension = FileExtension(FileName);
    yd_b32 Result = IsDoc(Extension);
    return Result;
}

#endif

#define YD_STRING
#endif

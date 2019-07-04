#if !defined(YD_STRING)

// TODO(yuval): UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING
// UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING

// TODO(yuval): MAKE THE CODE MORE ROBUST: Check for null pointers and so on....

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define internal_yd static
#define global_variable_yd static
#define local_persist_yd static

typedef int8_t s8_yd;
typedef int16_t s16_yd;
typedef int32_t s32_yd;
typedef int64_t s64_yd;
typedef s32_yd b32_yd;

typedef uint8_t u8_yd;
typedef uint16_t u16_yd;
typedef uint32_t u32_yd;
typedef uint64_t u64_yd;

typedef float f32_yd;
typedef double f64_yd;

typedef intptr_t smm_yd;
typedef uintptr_t umm_yd;
#define YD_TYPES
#endif

#if !defined(AssertYD)
# define AssertYD(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif

#if !defined(Literal)
# define Literal(Str) (Str), (sizeof(Str) - 1)
#endif

struct string{
    char* Data;
    size_t Count;
    size_t MemorySize;
};

// TODO(yuval): Maybe create A UTF8 string struct

//
// NOTE(yuval): Flags And Constants
//

global_variable_yd const size_t STRING_NOT_FOUND = -1;
global_variable_yd const string NULL_STR = {};

//
// NOTE(yuval): Public API Function Declarations
//

inline string MakeString(void* Data, size_t Count, size_t MemorySize);
inline string MakeString(void* Data, size_t Count);

#if !defined(MakeLitString)
# define MakeLitString(Str) (MakeString((char*)(Str), sizeof(Str) - 1, sizeof(Str)))
#endif

#if !defined(lit)
# define lit(Str) MakeLitString(Str)
#endif

#if !defined(MakeFixedWidthString)
# define MakeFixedWidthString(Str) (MakeString((char*)(Str), 0, sizeof(Str)))
#endif

#if !defined(ExpandString)
# define ExpandString(Str) ((Str).Data), ((Str).Count)
#endif

inline size_t StringLength(const char* Str);
inline string MakeStringSlowly(void* Str);
inline string Substr(string Str, size_t Start);
inline string Substr(string Str, size_t Start, size_t Count);
internal_yd string SkipWhitespace(string Str);
internal_yd string SkipWhitespace(string Str, size_t* OutSkipCount);
internal_yd string ChopWhitespace(string Str);
internal_yd string SkipChopWhitespace(string Str);
internal_yd string SkipChopWhitespace(string Str, size_t* OutSkipCount);
inline string TailStr(string Str);
internal_yd b32_yd StringsMatch(const char* A, const char* B);
internal_yd b32_yd StringsMatch(string A, const char* B);
inline b32_yd StringsMatch(const char* A, string B);
internal_yd b32_yd StringsMatch(string A, string B);
internal_yd b32_yd StringsMatchPart(const char* A, const char* B, size_t* OutCount);
internal_yd b32_yd StringsMatchPart(string A, const char* B, size_t* OutCount);
internal_yd b32_yd StringsMatchPart(const char* A, string B, size_t* OutCount);
internal_yd b32_yd StringsMatchPart(string A, string B, size_t* OutCount);
inline b32_yd StringsMatchPart(const char* A, const char* B);
inline b32_yd StringsMatchPart(string A, const char* B);
inline b32_yd StringsMatchPart(const char* A, string B);
inline b32_yd StringsMatchPart(string A, string B);
internal_yd b32_yd StringsMatchInsensitive(const char* A, const char* B);
internal_yd b32_yd StringsMatchInsensitive(string A, const char* B);
inline b32_yd StringsMatchInsensitive(const char* A, string B);
internal_yd b32_yd StringsMatchInsensitive(string A, string B);
internal_yd b32_yd StringsMatchPartInsensitive(const char* A, const char* B, size_t* OutCount);
internal_yd b32_yd StringsMatchPartInsensitive(string A, const char* B, size_t* OutCount);
internal_yd b32_yd StringsMatchPartInsensitive(const char* A, string B, size_t* OutCount);
internal_yd b32_yd StringsMatchPartInsensitive(string A, string B, size_t* OutCount);
inline b32_yd StringsMatchPartInsensitive(const char* A, const char* B);
inline b32_yd StringsMatchPartInsensitive(string A, const char* B);
inline b32_yd StringsMatchPartInsensitive(const char* A, string B);
inline b32_yd StringsMatchPartInsensitive(string A, string B);
internal_yd b32_yd StringSetMatch(void* StrSet, size_t ItemSize, size_t Count,
                                  string Str, size_t* OutMatchIndex);
inline b32_yd StringSetMatch(string* StrSet, size_t Count, string Str, size_t* OutMatchIndex);
internal_yd s32_yd Compare(const char* A, const char* B);
internal_yd s32_yd Compare(string A, const char* B);
inline s32_yd Compare(const char* A, string B);
internal_yd s32_yd Compare(string A, string B);
internal_yd size_t Find(const char* Str, char Character, size_t Start);
internal_yd size_t Find(string Str, char Character, size_t Start);
internal_yd size_t Find(const char* Str, const char* Characters, size_t Start);
internal_yd size_t Find(string Str, const char* Characters, size_t Start);
internal_yd size_t Find(const char* Str, string Seek, size_t Start);
internal_yd size_t Find(string Str, string Seek, size_t Start);
inline size_t Find(const char* Str, char Character);
inline size_t Find(string Str, char Character);
inline size_t Find(const char* Str, const char* Characters);
inline size_t Find(string Str, const char* Characters);
inline size_t Find(const char* Str, string Seek);
inline size_t Find(string Str, string Seek);
internal_yd size_t RFind(string Str, char Character, size_t Start);
internal_yd size_t RFind(string Str, const char* Characters, size_t Start);
internal_yd size_t RFind(string Str, string Seek, size_t Start);
inline size_t RFind(string Str, char Character);
inline size_t RFind(string Str, const char* Characters);
inline size_t RFind(string Str, string Seek);
internal_yd size_t FindFirstOf(const char* Str, const char* Characters, size_t Start);
internal_yd size_t FindFirstOf(string Str, const char* Characters,  size_t Start);
inline size_t FindFirstOf(const char* Str, const char* Characters);
inline size_t FindFirstOf(string Str, const char* Characters);
internal_yd size_t FindInsensitive(const char* Str, char Character, size_t Start);
internal_yd size_t FindInsensitive(string Str, char Character, size_t Start);
internal_yd size_t FindInsensitive(const char* Str, const char* Characters, size_t Start);
internal_yd size_t FindInsensitive(string Str, const char* Characters, size_t Start);
internal_yd size_t FindInsensitive(const char* Str, string Seek, size_t Start);
internal_yd size_t FindInsensitive(string Str, string Seek, size_t Start);
inline size_t FindInsensitive(const char* Str, char Character);
inline size_t FindInsensitive(string Str, char Character);
inline size_t FindInsensitive(const char* Str, const char* Characters);
inline size_t FindInsensitive(string Str, const char* Characters);
inline size_t FindInsensitive(const char* Str, string Seek);
inline size_t FindInsensitive(string Str, string Seek);
inline b32_yd HasSubstr(const char* Str, string Seek);
inline b32_yd HasSubstr(string Str, string Seek);
inline b32_yd HasSubstrInsensitive(const char* Str, string Seek);
inline b32_yd HasSubstrInsensitive(string Str, string Seek);
internal_yd string GetFirstDoubleLine(string Source);
internal_yd string GetNextDoubleLine(string Source, string Line);
internal_yd string GetNextWord(string Source, string PrevWord);
inline string GetFirstWord(string Source);
internal_yd size_t CopyFastUnsafe(char* Dest, const char* Source);
internal_yd size_t CopyFastUnsafe(char* Dest, string Source);
internal_yd b32_yd CopyChecked(string* Dest, string Source);
internal_yd b32_yd CopyChecked(char* Dest, size_t DestCap, string Source);
internal_yd b32_yd CopyPartial(string* Dest, const char* Source);
internal_yd b32_yd CopyPartial(string* Dest, string Source);
internal_yd b32_yd CopyPartial(char* Dest, size_t DestCap, string Source);
inline size_t Copy(char* Dest, const char* Source);
inline void Copy(string* Dest, string Source);
inline void Copy(string* Dest, const char* Source);
internal_yd b32_yd AppendChecked(string* Dest, string Source);
internal_yd b32_yd AppendPartial(string* Dest, const char* Source);
internal_yd b32_yd AppendPartial(string* Dest, string Source);
internal_yd b32_yd Append(string* Dest, char C);
inline b32_yd Append(string* Dest, const char* Source);
inline b32_yd Append(string* Dest, string Source);
internal_yd b32_yd TerminateWithNull(string* Str);
internal_yd b32_yd AppendPadding(string* Dest, char C, size_t TargetCount);
internal_yd void RaplaceRange(string* Str, size_t First, size_t OnePastLast, char With);
internal_yd void RaplaceRange(string* Str, size_t First, size_t OnePastLast, const char* With);
internal_yd void RaplaceRange(string* Str, size_t First, size_t OnePastLast, string With);
internal_yd void Replace(string* Str, char ToReplace, char With);
internal_yd void Replace(string* Str, const char* ToReplace, const char* With);
internal_yd void Replace(string* Str, const char* ToReplace, string With);
internal_yd void Replace(string* Str, string ToReplace, const char* With);
internal_yd void Replace(string* Str, string ToReplace, string With);
internal_yd void StringInterpretEscapes(char* Dest, string Source);
// TODO(yuval): Think about string push (should require an arena)
inline b32_yd IsLower(char C);
inline b32_yd IsLower(const char* Str);
inline b32_yd IsLower(string Str);
inline b32_yd IsLowerUTF8(u8_yd C);
inline char ToLower(char C);
internal_yd void ToLower(char* Str);
internal_yd void ToLower(string* Str);
internal_yd void ToLower(char* Dest, const char* Source);
internal_yd void ToLower(string* Dest, const char* Source);
internal_yd void ToLower(char* Dest, string Source);
internal_yd void ToLower(string* Dest, string Source);
inline b32_yd IsUpper(char C);
inline b32_yd IsUpper(const char* Str);
inline b32_yd IsUpper(string Str);
inline b32_yd IsUpperUTF8(u8_yd C);
inline char ToUpper(char C);
internal_yd void ToUpper(char* Str);
internal_yd void ToUpper(string* Str);
internal_yd void ToUpper(char* Dest, const char* Source);
internal_yd void ToUpper(string* Dest, const char* Source);
internal_yd void ToUpper(char* Dest, string Source);
internal_yd void ToUpper(string* Dest, string Source);
internal_yd void ToCamel(char* Str);
internal_yd void ToCamel(string* Str);
internal_yd void ToCamel(char* Dest, const char* Source);
internal_yd void ToCamel(string* Dest, const char* Source);
internal_yd void ToCamel(char* Dest, string Source);
internal_yd void ToCamel(string* Dest, string Source);
inline b32_yd IsSpacing(char C);
inline b32_yd IsEndOfLine(char C);
inline b32_yd IsWhitespace(char C);
inline b32_yd IsAlphaTrue(char C);
inline b32_yd IsAlphaTrue(const char* Str);
inline b32_yd IsAlphaTrue(string Str);
inline b32_yd IsAlphaTrueUTF8(u8_yd C);
inline b32_yd IsAlpha(char C);
inline b32_yd IsAlpha(const char* Str);
inline b32_yd IsAlpha(string Str);
inline b32_yd IsAlphaUTF8(u8_yd C);
inline b32_yd IsNumeric(char C);
inline b32_yd IsNumeric(const char* Str);
inline b32_yd IsNumeric(string Str);
inline b32_yd IsNumericUTF8(u8_yd C);
internal_yd size_t S32ToStringCount(s32_yd Value);
internal_yd b32_yd S32ToString(string* Dest, s32_yd Value);
internal_yd b32_yd AppendS32ToString(string* Dest, s32_yd Value);
internal_yd size_t U64TostringCount(u64_yd Value);
internal_yd b32_yd U64Tostring(string* Dest, u64_yd Value);
internal_yd b32_yd AppendU64Tostring(string* Dest, u64_yd Value);
internal_yd size_t F32TostringCount(f32_yd Value);
internal_yd b32_yd F32Tostring(string* Dest, f32_yd Value);
internal_yd b32_yd AppendF32Tostring(string* Dest, f32_yd Value);
inline b32_yd IsAlphaNumeric(char C);
inline b32_yd IsAlphaNumeric(const char* Str);
inline b32_yd IsAlphaNumeric(string Str);
inline b32_yd IsAlphaNumericUTF8(u8_yd C);
inline b32_yd IsAlphaNumericTrue(char C);
inline b32_yd IsAlphaNumericTrue(const char* Str);
inline b32_yd IsAlphaNumericTrue(string Str);
inline b32_yd IsAlphaNumericTrueUTF8(u8_yd C);
inline b32_yd IsHex(char C);
inline b32_yd IsHex(const char* Str);
inline b32_yd IsHex(string Str);
inline b32_yd IsHexUTF8(u8_yd C);
inline s32_yd HexCharToS32(char C);
inline char S32ToHexChar(s32_yd Value);
internal_yd u32_yd HexStringToU32(string Str);
internal_yd b32_yd ColorToHexString(string* Dest, u32_yd Color);
internal_yd b32_yd HexStringToColor(u32_yd* Dest, string Str);
inline b32_yd IsSlash(char C);
internal_yd size_t ReverseSeekSlash(string Str, size_t ShiftFromLastChar);
internal_yd size_t ReverseSeekSlash(string Str);
inline string FrontOfDirectory(string Dir);
inline string PathOfDirectory(string Dir);
internal_yd b32_yd SetLastFolder(string* Dir, const char* FolderName, char Slash);
internal_yd b32_yd SetLastFolder(string* Dir, string FolderName, char Slash);
internal_yd b32_yd RemoveLastFolder(string* Dir);
internal_yd string FileExtension(string FileName);
internal_yd b32_yd RemoveExtension(string* FileName);
inline b32_yd IsH(string Extension);
inline b32_yd IsC(string Extension);
inline b32_yd IsCPP(string Extension);
inline b32_yd IsObjectiveC(string Extension);
inline b32_yd IsShader(string Extension);
inline b32_yd IsINL(string Extension);
inline b32_yd IsJava(string Extension);
inline b32_yd IsCSharp(string Extension);
inline b32_yd IsPython(string Extension);
inline b32_yd IsSwift(string Extension);
inline b32_yd IsJavascript(string Extension);
inline b32_yd IsBAT(string Extension);
inline b32_yd IsBash(string Extension);
inline b32_yd IsTXT(string Extension);
inline b32_yd IsCode(string Extension);
inline b32_yd IsDoc(string Extension);
inline b32_yd IsCodeFile(string FileName);
inline b32_yd IsDocFile(string FileName);

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
MakeStringSlowly(void* Str)
{
    string Result;
    Result.Data = (char*)Str;
    Result.Count = StringLength(Str);
    Result.MemorySize = Result.Count + 1;
    
    return Result;
}

//
// NOTE(yuval): String Slicing Functions
//

inline string
Substr(string Str, size_t Start)
{
    AssertYD((Start >= 0) && (Start <= Str.Count));
    
    string Result;
    Result.Data = Str.Data + Start;
    Result.Size = Str.Count - Start;
    Result.MemorySize = 0;
    
    return Result;
}

inline string
Substr(string Str, size_t Start, size_t Count)
{
    AssertYD((Start >= 0) && (Start <= Str.Count));
    
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

internal_yd string
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

internal_yd string
ChopWhitespace(string Str)
{
    size_t ChopIndex = Str.Count;
    for (; ChopIndex > 0 && IsWhitespace(Str.Data[ChopIndex - 1]); --ChopIndex);
    
    string Result = Substr(Str, 0, ChopIndex);
    return Result;
}

internal_yd string
SkipChopWhitespace(string Str, size_t* OutSkipCount)
{
    string Result = SkipWhitespace(Str, OutSkipCount);
    Result = ChopWhitespace(Result);
    return Result;
}

internal_yd string
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

internal_yd b32_yd
StringsMatch(const char* A, const char* B)
{
    b32_yd Result = (A == B);
    
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

internal_yd b32_yd
StringsMatch(string A, const char* B)
{
    b32_yd Result = false;
    
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

inline b32_yd
StringsMatch(const char* A, string B)
{
    b32_yd Result = StringsMatch(B, A);
    return Result;
}

internal_yd b32_yd
StringsMatch(string A, string B)
{
    b32_yd Result = (A.Count == B.Count);
    
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

internal_yd b32_yd
StringsMatchPart(const char* A, const char* B, size_t* OutCount)
{
    b32_yd Result = (*A == *B);
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

internal_yd b32_yd
StringsMatchPart(string A, const char* B, size_t* OutCount)
{
    b32_yd Result = false;
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

internal_yd b32_yd
StringsMatchPart(const char* A, string B, size_t* OutCount) {
    b32_yd Result = false;
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

internal_yd b32_yd
StringsMatchPart(string A, string B, size_t* OutCount)
{
    b32_yd Result = (A.Count >= B.Count);
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

inline b32_yd
StringsMatchPart(const char* A, const char* B)
{
    size_t Ignored;
    b32_yd Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

inline b32_yd
StringsMatchPart(string A, const char* B)
{
    size_t Ignored;
    b32_yd Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

inline b32_yd
StringsMatchPart(const char* A, string B)
{
    size_t Ignored;
    b32_yd Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

inline b32_yd
StringsMatchPart(string A, string B)
{
    size_t Ignored;
    b32_yd Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

internal_yd b32_yd
StringsMatchInsensitive(const char* A, const char* B)
{
    b32_yd Result = (A == B);
    
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

internal_yd b32_yd
StringsMatchInsensitive(string A, const char* B)
{
    b32_yd Result = false;
    
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

inline b32_yd
StringsMatchInsensitive(const char* A, string B)
{
    b32_yd Result = StringsMatchInsensitive(B, A);
    return Result;
}

internal_yd b32_yd
StringsMatchInsensitive(string A, string B)
{
    b32_yd Result = (A.Count == B.Count);
    
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

internal_yd b32_yd
StringsMatchPartInsensitive(const char* A, const char* B, size_t* OutCount)
{
    b32_yd Result = (*A == *B);
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

internal_yd b32_yd
StringsMatchPartInsensitive(string A, const char* B, size_t* OutCount)
{
    b32_yd Result = false;
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
    } else {
        Result = (A.Count == 0);
    }
    
    *OutCount = Index;
    return Result;
}
internal_yd b32_yd
StringsMatchPartInsensitive(const char* A, string B, size_t* OutCount) {
    b32_yd Result = false;
    size_t Index = 0;
    
    if (A) {
        for (; Index < B.Count; ++Index) {
            if (ToLower(A[Index]) != ToLower(B.Data[Index])) {
                return false;
            }
        }
        
        Result = true;
    } else {
        Result = (B.Count == 0);
    }
    
    *OutCount = Index;
    return Result;
}

internal_yd b32_yd
StringsMatchPartInsensitive(string A, string B, size_t* OutCount) {
    b32_yd Result = (A.Count >= B.Count);
    size_t Index = 0;
    
    if (Result) {
        for (; Index < B.Count; ++Index) {
            if (ToLower(A.Data[Index]) != ToLower(B.Data[Index])) {
                Result = false;
                break;
            }
        }
    }
    
    *OutCount = Index;
    return Result;
}

inline b32_yd
StringsMatchPartInsensitive(const char* A, const char* B) {
    size_t Ignored;
    b32_yd Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

inline b32_yd
StringsMatchPartInsensitive(string A, const char* B) {
    size_t Ignored;
    b32_yd Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

inline b32_yd
StringsMatchPartInsensitive(const char* A, string B) {
    size_t Ignored;
    b32_yd Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

inline b32_yd
StringsMatchPartInsensitive(string A, string B) {
    size_t Ignored;
    b32_yd Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

internal_yd b32_yd
StringSetMatch(void* StrSet, size_t ItemSize, size_t Count,
               string Str, size_t* OutMatchIndex) {
    b32_yd Result = false;
    u8_yd* At = (u8_yd*)StrSet;
    
    for (size_t Index = 0; Index < Count; ++Index, At += ItemSize) {
        if (StringsMatch(*((string*)At), Str)) {
            *OutMatchIndex = Index;
            Result = true;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
StringSetMatch(string* StrSet, size_t Count, string Str, size_t* OutMatchIndex) {
    b32_yd Result = StringSetMatch(StrSet, sizeof(string), Count,
                                   Str, OutMatchIndex);
    return Result;
}

internal_yd s32_yd
Compare(const char* A, const char* B) {
    s32_yd Index = 0;
    while (A[Index] && B[Index] &&
           (A[Index] != B[Index])) {
        ++Index;
    }
    
    s32_yd Result = (A[Index] > B[Index]) - (A[Index] < B[Index]);
    return Result;
}

internal_yd s32_yd
Compare(string A, const char* B) {
    s32_yd Index = 0;
    while ((Index < A.Count) && B[Index] &&
           (A.Data[Index] == B[Index])) {
        ++Index;
    }
    
    s32_yd Result = 0;
    if (Index < A.Count) {
        Result = (A.Data[Index] > B[Index]) - (A.Data[Index] < B[Index]);
    } else {
        if (B[Index]) {
            Result = 0;
        } else {
            Result = -1;
        }
    }
    
    return Result;
}

inline s32_yd
Compare(const char* A, string B) {
    s32_yd Result = -Compare(B, A);
    return Result;
}

internal_yd s32_yd
Compare(string A, string B) {
    size_t min_Count = A.Count;
    if (B.Count < min_Count) {
        min_Count = B.Count;
    }
    
    s32_yd Index = 0;
    while ((Index < min_Count) && (A.Data[i] == B.Data[i])) {
        ++Index;
    }
    
    s32_yd Result = 0;
    if (Index < min_Count) {
        Result = (A.Data[Index] > B.Data[Index]) - (A.Data[Index] < B.Data[Index]);
    } else {
        Result = (A.Count > B.Count) - (A.Count < B.Count);
    }
    
    return Result;
}

//
// NOTE(yuval): Functions For Finding Characters And Substrings
//

internal_yd size_t
Find(const char* Str, char Character, size_t Start) {
    AssertYD(Start >= 0);
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        if (Str[Index] == Character) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
Find(string Str, char Character, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    for (size_t Index = Start; Index < Str.Count; ++Index) {
        if (Str.Data[Index] == Character) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
Find(const char* Str, const char* Characters, size_t Start) {
    AssertYD(Start >= 0);
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (const char* At = Characters; *At; ++At, ++Str_Index) {
            if (Str[Str_Index] != *At) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
Find(string Str, const char* Characters,  size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = Str.Count - StringLength(Characters) + 1;
    
    for (size_t Index = Start; Index < stop_at; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (const char* At = Characters; *At; ++At, ++Str_Index) {
            if (Str.Data[Str_Index] != *At) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
Find(const char* Str, string Seek, size_t Start) {
    AssertYD(Start >= 0);
    
    if (Seek.Count == 0) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++Str_Index) {
            if (Str[Str_Index] != Seek.Data[SeekIndex]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
Find(string Str, string Seek, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (Seek.Count == 0) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = Str.Count - Seek.Count + 1;
    
    for (size_t Index = Start; Index < stop_at; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++Str_Index) {
            if (Str.Data[Str_Index] != Seek.Data[SeekIndex]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

inline size_t
Find(const char* Str, char Character) {
    size_t Result = Find(Str, Character, 0);
    return Result;
}

inline size_t
Find(string Str, char Character) {
    size_t Result = Find(Str, Character, 0);
    return Result;
}

inline size_t
Find(const char* Str, const char* Characters) {
    size_t Result = Find(Str, Characters, 0);
    return Result;
}

inline size_t
Find(string Str, const char* Characters) {
    size_t Result = Find(Str, Characters, 0);
    return Result;
}

inline size_t
Find(const char* Str, string Seek) {
    size_t Result = Find(Str, Seek, 0);
    return Result;
}

inline size_t
Find(string Str, string Seek) {
    size_t Result = Find(Str, Seek, 0);
    return Result;
}

internal_yd size_t
RFind(string Str, char Character, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    for (size_t Index = Start; Index >= 0; --Index) {
        if (Str.Data[Index] == Character) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
RFind(string Str, const char* Characters, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    size_t Characters_Count = StringLength(Characters);
    if (Start + Characters_Count > Str.Count) {
        Start = Str.Count - Characters_Count;
    }
    
    for (size_t Index = Start; Index >= 0; --Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (const char* At = Characters; *At; ++At, ++Str_Index) {
            if (Str.Data[Str_Index] != *At) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}
internal_yd size_t
RFind(string Str, string Seek, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (Seek.Count == 0) {
        return STRING_NOT_FOUND;
    }
    
    size_t Characters_Count = StringLength(Characters);
    if (Start + Characters_Count > Str.Count) {
        Start = Str.Count - Characters_Count;
    }
    
    for (size_t Index = Start; Index >= 0; --Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++Str_Index) {
            if (Str.Data[Str_Index] != Seek.Data[SeekIndex]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

inline size_t
RFind(string Str, char Character) {
    size_t Result = RFind(Str, Character, 0);
    return Result;
}

inline size_t
RFind(string Str, const char* Characters) {
    size_t Result = RFind(Str, Characters, 0);
    return Result;
}

inline size_t
RFind(string Str, string Seek) {
    size_t Result = RFind(Str, Seek, 0);
    return Result;
}

internal_yd size_t
FindFirstOf(const char* Str, const char* Characters, size_t Start) {
    AssertYD(Start >= 0);
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        for (const char* At = Characters; *At; ++At) {
            if (Str[Index] == *At) {
                return Index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
FindFirstOf(string Str, const char* Characters, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t Index = Start; Index < Str.Count; ++Index) {
        for (const char* At = Characters; *At; ++At) {
            if (Str.Data[Index] == *At) {
                return Index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

inline size_t
FindFirstOf(const char* Str, const char* Characters) {
    size_t Result = FindFirstOf(Str, Characters, 0);
    return Result;
}

inline size_t
FindFirstOf(string Str, const char* Characters) {
    size_t Result = FindFirstOf(Str, Characters, 0);
    return Result;
}

internal_yd size_t
FindInsensitive(const char* Str, char Character, size_t Start) {
    AssertYD(Start >= 0);
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        if (ToLower(Str[Index]) == ToLower(Character)) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
FindInsensitive(string Str, char Character, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    for (size_t Index = Start; Index < Str.Count; ++Index) {
        if (ToLower(Str.Data[Index]) == ToLower(Character)) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
FindInsensitive(const char* Str, const char* Characters, size_t Start) {
    AssertYD(Start >= 0);
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (const char* At = Characters; *At; ++At, ++Str_Index) {
            if (ToLower(Str[Str_Index]) != ToLower(*At)) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
FindInsensitive(string Str, const char* Characters, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (!(*Characters)) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = Str.Count - StringLength(Characters) + 1;
    
    for (size_t Index = Start; Index < stop_at; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (const char* At = Characters; *At; ++At, ++Str_Index) {
            if (ToLower(Str.Data[Str_Index]) != ToLower(*At)) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
FindInsensitive(const char* Str, string Seek, size_t Start) {
    AssertYD(Start >= 0);
    
    if (Seek.Count == 0) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t Index = Start; Str[Index]; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++Str_Index) {
            if (ToLower(Str[Str_Index]) != ToLower(Seek.Data[SeekIndex])) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
FindInsensitive(string Str, string Seek, size_t Start) {
    AssertYD((Start >= 0) && (Start < Str.Count));
    
    if (Seek.Count == 0) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = Str.Count - Seek.Count + 1;
    
    for (size_t Index = Start; Index < stop_at; ++Index) {
        b32_yd hit = true;
        size_t Str_Index = Index;
        
        for (size_t SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++Str_Index) {
            if (ToLower(Str.Data[Str_Index]) != ToLower(Seek.Data[SeekIndex])) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

inline size_t
FindInsensitive(const char* Str, char Character) {
    size_t Result = FindInsensitive(Str, Character, 0);
    return Result;
}

inline size_t
FindInsensitive(string Str, char Character) {
    size_t Result = FindInsensitive(Str, Character, 0);
    return Result;
}

inline size_t
FindInsensitive(const char* Str, const char* Characters) {
    size_t Result = FindInsensitive(Str, Characters, 0);
    return Result;
}

inline size_t
FindInsensitive(string Str, const char* Characters) {
    size_t Result = FindInsensitive(Str, Characters, 0);
    return Result;
}

inline size_t
FindInsensitive(const char* Str, string Seek) {
    size_t Result = FindInsensitive(Str, Seek, 0);
    return Result;
}

inline size_t
FindInsensitive(string Str, string Seek) {
    size_t Result = FindInsensitive(Str, Seek, 0);
    return Result;
}

inline b32_yd
HasSubstr(const char* Str, string Seek) {
    b32_yd Result = (Find(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

inline b32_yd
HasSubstr(string Str, string Seek) {
    b32_yd Result = (Find(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

inline b32_yd
HasSubstrInsensitive(const char* Str, string Seek) {
    b32_yd Result = (FindInsensitive(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

inline b32_yd
HasSubstrInsensitive(string Str, string Seek) {
    b32_yd Result = (FindInsensitive(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}


internal_yd string
GetFirstDoubleLine(string Source) {
    string Result = {};
    
    size_t pos = Find(Source, MakeLitString("\n\n"));
    if (pos == STRING_NOT_FOUND) {
        pos = Find(Source, MakeLitString("\r\n\r\n"));
    }
    
    if (pos != STRING_NOT_FOUND) {
        Result = Substr(Str, 0, pos);
    }
    
    return Result;
}

internal_yd string
GetNextDoubleLine(string Source, string Line) {
    string Result = {};
    
    size_t Line_end_Index = (size_t)(Line.Data - Source.Data) + Line.Count;
    assert((Source.Data[Line_end_Index] == '\n') || (Source.Data[Line_end_Index] == '\r'));
    
    ++Line_end_Index;
    assert((Source.Data[Line_end_Index] == '\n') || (Source.Data[Line_end_Index] == '\r'));
    
    size_t Start = Line_end_Index + 1;
    
    if (Start < Source.Count) {
        size_t pos = Find(Source, MakeLitString("\n\n"), Start);
        if (pos == STRING_NOT_FOUND) {
            pos = Find(Source, MakeLitString("\r\n\r\n"), Start);
        }
        
        if (pos != STRING_NOT_FOUND) {
            Result = Substr(Source, Start, pos - Start);
        }
    }
    
    return Result;
}

internal_yd string
GetNextWord(string Source, string PrevWord) {
    string Result = {};
    size_t pos0 = (size_t)(PrevWord.Data - Source.Data) + PrevWord.Count;
    
    for (; pos0 < Source.Count; ++pos0) {
        char C = Source.Data[pos0];
        if (!(IsWhitespace(C) || C == '(' || C == ')')) {
            break;
        }
    }
    
    if (pos0 < Source.Count) {
        size_t pos1 = pos;
        
        for (; pos1 < Source.Count; ++pos1) {
            char C = Source.Data[pos1];
            if (IsWhitespace(C) || C == '(' || C == ')') {
                break;
            }
        }
        
        word = Substr(Source, pos0, pos1 - pos0);
    }
    
    return word;
}


inline string
GetFirstWord(string Source) {
    string Start = MakeString(Source.Data, 0);
    string Result = GetNextWord(Source, Start);
    return Result;
}

//
// NOTE(yuval): string Copy And Append Functions
//

internal_yd size_t
CopyFastUnsafe(char* Dest, const char* Source) {
    char* Dest_at = Dest;
    char* Source_at = Source;
    
    while (*Source_at) {
        *Dest_at++ = *Source_at++;
    }
    
    *Dest_at = 0;
    
    size_t Result = (Dest_at - Dest);
    return Result;
}

internal_yd s32_yd
CopyFastUnsafe(char* Dest, string Source) {
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        Dest[Index] = Source.Data[Index];
    }
    
    Dest[Index] = 0;
    return Source.Count;
}

internal_yd b32_yd
CopyChecked(string* Dest, string Source) {
    if (Dest->MemorySize < Source.Count) {
        return false;
    }
    
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        Dest->Data[Index] = Source.Data[Index];
    }
    
    Dest->Count = Source.Count;
    return true;
}

internal_yd b32_yd
CopyChecked(char* Dest, size_t DestCap, string Source) {
    if (DestCap < Source.Count + 1) {
        return false;
    }
    
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        Dest[Index] = Source.Data[Index];
    }
    
    Dest[Index] = 0;
    return true;
}

internal_yd b32_yd
CopyPartial(string* Dest, const char* Source) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Source[Index]; ++Index) {
        if (Index >= Dest->MemorySize) {
            Result = false;
            break;
        }
        
        Dest->Data[Index] = Source[Index];
    }
    
    Dest->Count = Index;
    return Result;
}

internal_yd b32_yd
CopyPartial(string* Dest, string Source) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        if (Index >= Dest->MemorySize) {
            Result = false;
            break;
        }
        
        Dest->Data[Index] = Source.Data[Index];
    }
    
    Dest->Count = Index;
    return Result;
}

internal_yd b32_yd
CopyPartial(char* Dest, size_t DestCap, string Source) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        if (Index >= DestCap - 1) {
            Result = false;
            break;
        }
        
        Dest[Index] = Source.Data[Index];
    }
    
    Dest[Index] = 0;
    return Result;
}

inline size_t
Copy(char* Dest, const char* Source) {
    size_t Result = CopyFastUnsafe(Dest, Source);
    return Result;
}

inline void
Copy(string* Dest, string Source) {
    CopyChecked(Dest, Source);
}

inline void
Copy(string* Dest, const char* Source) {
    CopyPartial(Dest, Source);
}

internal_yd b32_yd
AppendChecked(string* Dest, string Source) {
    string end = TailStr(*Dest);
    b32_yd Result = CopyChecked(&end, Source);
    Dest->Count += end.Count;
    return Result;
}

internal_yd b32_yd
AppendPartial(string* Dest, const char* Source) {
    string end = TailStr(*Dest);
    b32_yd Result = CopyPartial(&end, Source);
    Dest->Count += end.Count;
    return Result;
}

internal_yd b32_yd
AppendPartial(string* Dest, string Source) {
    string end = TailStr(*Dest);
    b32_yd Result = CopyPartial(&end, Source);
    Dest->Count += end.Count;
    return Result;
}

internal_yd b32_yd
Append(string* Dest, char C) {
    b32_yd Result = false;
    
    if (Dest->Count < Dest->MemorySize) {
        Dest->Data[Dest->Count++] = C;
        Result = true;
    }
    
    return Result;
}

inline b32_yd
Append(string* Dest, const char* Source) {
    b32_yd Result = AppendPartial(Dest, Source);
    return Result;
}

inline b32_yd
Append(string* Dest, string Source) {
    b32_yd Result = AppendPartial(Dest, Source);
    return Result;
}

internal_yd b32_yd
TerminateWithNull(string* Str) {
    b32_yd Result = false;
    
    if (Str->Count < Str->MemorySize) {
        Str->Data[Str->Count] = 0;
        Result = true;
    }
    
    return Result;
}

internal_yd b32_yd
AppendPadding(string* Dest, char C, size_t TargetCount) {
    b32_yd Result = true;
    
    for (size_t Count = Dest->Count; Count < TargetCount; ++Count) {
        if (!Append(Dest, C)) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

//
// NOTE(yuval): Other string Editing Functions
//

internal_yd void
RaplaceRange(string* Str, size_t First, size_t OnePastLast, char With) {
    AssertYD((First >= 0) && (from < Str->Count));
    AssertYD((OnePastLast > 0) && (OnePastLast <= Str->Count));
    AssertYD(First < OnePastLast);
    
    for (size_t Index = from; Index < OnePastLast; ++Index) {
        Str->Data[Index] = With;
    }
}

internal_yd void
RaplaceRange(string* Str, size_t First, size_t OnePastLast, const char* With) {
    string WithStr = MakeStringSlowly(With);
    RaplaceRange(Str, First, OnePastLast, WithStr);
}

// TODO(yuval): Maybe rename to block_copy?
internal_yd void
block_move_yds(void* Dest_init, const void* Source_init, size_t size) {
    if (Dest_init && Source_init) {
        const u8* Source = (const u8*)Source_init;
        u8* Dest = (u8*)Dest_init;
        
        if (Dest < Source) {
            while (size--) {
                *Dest++ = *Source++;
            }
        } else if (Dest > Source) {
            Source += size - 1;
            Dest += size - 1;
            
            while (size--) {
                *Dest-- = *Source--;
            }
        }
    }
}

internal_yd void
RaplaceRange(string* Str, size_t First, size_t OnePastLast, string With) {
    AssertYD((First >= 0) && (from < Str->Count));
    AssertYD((OnePastLast > 0) && (OnePastLast <= Str->Count));
    AssertYD(First < OnePastLast);
    
    s64_yd shift = With.Count - (OnePastLast - First);
    size_t new_Count = Str->Count + shift;
    
    if (new_Count <= Str->MemorySize) {
        if (shift != 0) {
            char* tail = Str->Data + OnePastLast;
            char* new_tail_pos = tail + shift;
            // TODO(yuval): Verify that this has no bugs!!!!!!!!!
            block_move_yds(new_tail_pos, tail, Str->Count - OnePastLast);
        }
        
        block_move_yds(Str->Data + First, With.Data, With.Count);
        Str->Count += shift;
    }
}

internal_yd void
Replace(string* Str, char ToReplace, char With) {
    for (size_t Index = 0; Index < Str->Count; ++Index) {
        if (Str->Data[Index] == ToReplace) {
            Str->Data[Index] = With;
        }
    }
}

internal_yd void
Replace(string* Str, const char* ToReplace, const char* With) {
    Replace(Str, MakeStringSlowly(ToReplace), MakeStringSlowly(With));
}

internal_yd void
Replace(string* Str, const char* ToReplace, string With) {
    Replace(Str, MakeStringSlowly(ToReplace), With);
}

internal_yd void
Replace(string* Str, string ToReplace, const char* With) {
    Replace(Str, ToReplace, MakeStringSlowly(With));
}

internal_yd void
Replace(string* Str, string ToReplace, string With) {
    size_t Index = 0;
    
    for (;;) {
        Index = Find(*Str, ToReplace, Index);
        if (Index == STRING_NOT_FOUND) {
            break;
        }
        
        RaplaceRange(Str, Index, Index + ToReplace.Count, With);
        Index += With.Count;
    }
}

internal_yd void
StringInterpretEscapes(char* Dest, string Source) {
    s32_yd mode = 0;
    size_t Dest_Index = 0;
    
    for (size_t Source_Index = 0; Source_Index < Source.Count; ++Source_Index) {
        switch (mode) {
            case 0: {
                if (Source.Data[Source_Index] = '\\') {
                    mode = 1;
                } else {
                    Dest[Dest_Index++] = Source.Data[Source_Index];
                }
            } break;
            
            case 1: {
                char C = Source.Data[Source_Index];
                switch (C) {
                    case '\\': { Dest[Dest_Index++] = '\\'; } break;
                    case 'n': { Dest[Dest_Index++] = '\n'; } break;
                    case 't': { Dest[Dest_Index++] = '\t'; } break;
                    case '"': { Dest[Dest_Index++] = '"'; } break;
                    case '0': { Dest[Dest_Index++] = '\0'; } break;
                    default: { Dest[Dest_Index++] = '\\'; Dest[Dest_Index++] = C; } break;
                }
                
                mode = 0;
            } break;
        }
    }
    
    Dest[Dest_Index] = 0;
}

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

inline b32_yd
IsLower(char C) {
    b32_yd Result = ((C >= 'A') && (C <= 'z'));
    return Result;
}

inline b32_yd
IsLower(const char* Str) {
    b32_yd Result = true;
    
    for (const char* At = Str; *At; ++At) {
        if (!IsLower(*At)) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsLower(string Str) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Index < Str.Count; ++Index) {
        if (!IsLower(Str.Data[Index])) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsLowerUTF8(u8_yd C) {
    b32_yd Result = (IsLower((char)C) || (C >= 128));
    return Result;
}

inline char
ToLower(char C) {
    char Result = C;
    
    if (IsUpper(Result)) {
        Result += 'A' - 'A';
    }
    
    return Result;
}

internal_yd void
ToLower(char* Str) {
    for (char* At = Str; *At; ++At) {
        *At = ToLower(*At);
    }
}

internal_yd void
ToLower(string* Str) {
    for (size_t Index = 0; Index < Str->Count; ++Index) {
        Str->Data[Index] = ToLower(Str->Data[Index]);
    }
}

internal_yd void
ToLower(char* Dest, const char* Source) {
    const char* Source_at = Source;
    char* Dest_at = Dest;
    
    while (*Source_at) {
        *Dest_at++ = ToLower(*Source_at++);
    }
    
    *Dest_at = 0;
}

internal_yd void
ToLower(string* Dest, const char* Source) {
    size_t Index = 0;
    
    for (; Source[Index]; ++Index) {
        Dest->Data[Index] = ToLower(Source[Index]);
    }
    
    Dest->Count = Index;
}

internal_yd void
ToLower(char* Dest, string Source) {
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        Dest[Index] = ToLower(Source.Data[Index]);
    }
    
    Dest[Source.Count] = 0;
}

internal_yd void
ToLower(string* Dest, string Source) {
    if (Dest->MemorySize >= Source.Count) {
        for (size_t Index = 0; Index < Source.Count; ++Index) {
            Dest->Data[Index] = ToLower(Source.Data[Index]);
        }
        
        Dest->Count = Source.Count;
    }
}

inline b32_yd
IsUpper(char C) {
    b32_yd Result = ((C >= 'A') && (C <= 'Z'));
    return Result;
}

inline b32_yd
IsUpper(const char* Str) {
    b32_yd Result = true;
    
    for (const char* At = Str; *At; ++At) {
        if (!IsUpper(*At)) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsUpper(string Str) {
    b32_yd Result;
    
    for (size_t Index = 0; Index < Str.Count; ++Index) {
        if (!IsUpper(Str.Data[Index])) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsUpperUTF8(u8_yd C) {
    b32_yd Result = (IsUpper((char)C) || (C >= 128));
    return Result;
}

inline char
ToUpper(char C) {
    char Result = C;
    
    if (IsLower(Result)) {
        Result -= 'A' - 'A';
    }
    
    return Result;
}

internal_yd void
ToUpper(char* Str) {
    for (char* At = Str; *At; ++At) {
        *At = ToUpper(*At);
    }
}

internal_yd void
ToUpper(string* Str) {
    for (size_t Index = 0; Index < Str->Count; ++Index) {
        Str->Data[Index] = ToUpper(Str->Data[Index]);
    }
}

internal_yd void
ToUpper(char* Dest, const char* Source) {
    const char* Source_at = Source;
    char* Dest_at = Dest;
    
    while (*Source_at) {
        *Dest_at++ = ToUpper(*Source_at++);
    }
    
    *Dest_at = 0;
}

internal_yd void
ToUpper(string* Dest, const char* Source) {
    size_t Index = 0;
    
    for (; Source[Index]; ++Index) {
        Dest->Data[Index] = ToUpper(Source[Index]);
    }
    
    Dest->Count = Index;
}

internal_yd void
ToUpper(char* Dest, string Source) {
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        Dest[Index] = ToUpper(Source.Data[Index]);
    }
    
    Dest[Source.Count] = 0;
}

internal_yd void
ToUpper(string* Dest, string Source) {
    if (Dest->MemorySize >= Source.Count) {
        for (size_t Index = 0; Index < Source.Count; ++Index) {
            Dest->Data[Index] = ToUpper(Source.Data[Index]);
        }
        
        Dest->Count = Source.Count;
    }
}

internal_yd void
ToCamel(char* Str) {
    b32_yd IsFirst = true;
    
    for (char* At = Str; *At; ++At) {
        if (IsAlphaNumericTrue(*At)) {
            if (IsFirst) {
                *At = ToUpper(*At);
                IsFirst = false;
            } else {
                *At = ToLower(*At);
            }
        } else {
            IsFirst = true;
        }
    }
}

internal_yd void
ToCamel(string* Str) {
    b32_yd IsFirst = true;
    
    for (size_t Index = 0; Index < Str->Count; ++Index) {
        if (IsAlphaNumericTrue(Str->Data[Index])) {
            if (IsFirst) {
                Str->Data[Index] = ToUpper(Str->Data[Index]);
                IsFirst = false;
            } else {
                Str->Data[Index] = ToLower(Str->Data[Index]);
            }
        } else {
            IsFirst = true;
        }
    }
}

internal_yd void
ToCamel(char* Dest, const char* Source) {
    const char* Source_at = Source;
    char* Dest_at = Dest;
    b32_yd IsFirst = false;
    
    for (; *Source_at; ++Source_at, ++Dest_at) {
        char C = *Source_at;
        
        if (IsAlphaNumericTrue(C)) {
            if (IsFirst) {
                C = ToUpper(C);
                IsFirst = false;
            } else {
                C = ToLower(C);
            }
        } else {
            IsFirst = true;
        }
        
        *Dest_at = C;
    }
    
    *Dest_at = 0;
}

internal_yd void
ToCamel(string* Dest, const char* Source) {
    size_t Index = 0;
    b32_yd IsFirst = true;
    
    for (; Source[Index]; ++Index) {
        char C = Source[Index];
        
        if (IsAlphaNumericTrue(C)) {
            if (IsFirst) {
                C = ToUpper(C);
                IsFirst = false;
            } else {
                C = ToLower(C);
            }
        } else {
            IsFirst = true;
        }
        
        Dest->Data[Index] = C;
    }
    
    Dest->Count = Index;
}

internal_yd void
ToCamel(char* Dest, string Source) {
    b32_yd IsFirst = true;
    
    for (size_t Index = 0; Index < Source.Count; ++Index) {
        char C = Source.Data[Index];
        
        if (IsAlphaNumericTrue(C)) {
            if (IsFirst) {
                C = ToUpper(C);
                IsFirst = false;
            } else {
                C = ToLower(C);
            }
        } else {
            IsFirst = true;
        }
        
        Dest[Index] = C;
    }
    
    Dest[Source.Count] = Index;
}

internal_yd void
ToCamel(string* Dest, string Source) {
    if (Dest->MemorySize >= Source.Count) {
        b32_yd IsFirst = true;
        
        for (size_t Index = 0; Index < Source.Count; ++Index) {
            char C = Source.Data[Index];
            
            if (IsAlphaNumericTrue(C)) {
                if (IsFirst) {
                    C = ToUpper(C);
                    IsFirst = false;
                } else {
                    C = ToLower(C);
                }
            } else {
                IsFirst = true;
            }
            
            Dest->Data[Index] = C;
        }
        
        Dest->Count = Source.Count;
    }
}

inline b32_yd
IsSpacing(char C) {
    b32_yd Result = ((C == ' ') || (C == '\t') ||
                     (C == '\v') || (C == '\f'));
    return Result;
}

inline b32_yd
IsEndOfLine(char C) {
    b32_yd Result = ((C == '\n') || (C == '\r'));
    return Result;
}

inline b32_yd
IsWhitespace(char C) {
    b32_yd Result = (IsSpacing(C) || IsEndOfLine(C));
}

inline b32_yd
IsAlphaTrue(char C) {
    b32_yd Result = (IsLower(C) || IsUpper(C));
}

inline b32_yd
IsAlphaTrue(const char* Str) {
    b32_yd Result = true;
    
    for (const char* At = Str; *At; ++At) {
        if (!IsAlphaTrue(*At)) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsAlphaTrue(string Str) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Index < Str.Count; ++Index) {
        if (!IsAlphaTrue(Str.Data[Index])) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsAlphaTrueUTF8(u8_yd C) {
    b32_yd Result = (IsAlphaTrue((char)C) || (C >= 128));
    return Result;
}

inline b32_yd
IsAlpha(char C) {
    b32_yd Result = (IsAlphaTrue(C) || (C == '_'));
    return Result;
}

inline b32_yd
IsAlpha(const char* Str) {
    b32_yd Result = true;
    
    for (const char* At = Str; *At; ++At) {
        if (!IsAlpha(*At)) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsAlpha(string Str) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Index < Str.Count; ++Index) {
        if (!IsAlpha(Str.Data[Index])) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsAlphaUTF8(u8_yd C) {
    b32_yd Result = (IsAlpha((char)C) || (C >= 128));
    return Result;
}

inline b32_yd
IsNumeric(char C) {
    b32_yd Result = ((C >= '0') && (C <= '9'));
    return Result;
}

inline b32_yd
IsNumeric(const char* Str) {
    b32_yd Result = true;
    
    for (const char* At = Str; *At; ++At) {
        if (!IsNumeric(*At)) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsNumeric(string Str) {
    b32_yd Result = true;
    
    for (size_t Index = 0; Index < Str.Count; ++Index) {
        if (!IsNumeric(Str.Data[Index])) {
            Result = false;
            break;
        }
    }
    
    return Result;
}

inline b32_yd
IsNumericUTF8(u8_yd C) {
    b32_yd Result = (IsNumeric((char)C) || (C >= 128));
    return Result;
}

internal_yd size_t
S32ToStringCount(s32_yd Value) {
    size_t Count = 1;
    
    if (Value < 0) {
        Count = 2;
    }
    
    Value /= 10;
    while (Value != 0) {
        ++Count;
        Value /= 10;
    }
    
    return Value;
}

internal_yd b32_yd
S32ToString(string* Dest, s32_yd Value) {
    
}

internal_yd b32_yd
AppendS32ToString(string* Dest, s32_yd Value) {
    
}


//
// NOTE(yuval): File / Directory strings Management Functions
//

internal_yd size_t
ReverseSeekSlash(string Str, size_t ShiftFromLastChar) {
    for (size_t Index = Str.size - ShiftFromLastChar - 1;
         Index >= 0;
         --Index) {
        if (IsSlash(Str[Index])) {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
ReverseSeekSlash(string Str) {
    size_t Result = ReverseSeekSlash(Str, 0);
    return Result;
}

inline string
FrontOfDirectory(string Dir) {
    string Result = Substr(Dir, ReverseSeekSlash(Dir) + 1);
    return Result;
}

inline string
PathOfDirectory(string Dir) {
    string Result = Substr(Dir, 0, ReverseSeekSlash(Dir) + 1);
    return Result;
}

internal_yd b32_yd
SetLastFolder(string* Dir, const char* FolderName, char Slash) {
    b32_yd Result = false;
    size_t LastSlashIndex = ReverseSeekSlash(*Dir);
    
    if (LastSlashIndex != STRING_NOT_FOUND) {
        size_t Count = LastSlashIndex + 1;
        Dir->Count = Count;
        
        if (Append(Dir, FolderName)) {
            if (Append(Dir, Slash)) {
                Result = true;
            }
        }
        
        if (!Result) {
            Dir->Count = Count;
        }
    }
    
    return Result;
}

internal_yd b32_yd
SetLastFolder(string* Dir, string FolderName, char Slash) {
    b32_yd Result = false;
    size_t LastSlashIndex = ReverseSeekSlash(*Dir);
    
    if (LastSlashIndex != STRING_NOT_FOUND) {
        size_t Count = LastSlashIndex + 1;
        Dir->Count = Count;
        
        if (Append(Dir, FolderName)) {
            if (Append(Dir, Slash)) {
                Result = true;
            }
        }
        
        if (!Result) {
            Dir->Count = Count;
        }
    }
    
    return Result;
}

internal_yd b32_yd
RemoveLastFolder(string* path) {
    b32_yd Result = false;
    size_t LastSlashIndex = ReverseSeekSlash(*path, 1);
    
    if (LastSlashIndex != STRING_NOT_FOUND) {
        Result = true;
        path->Count = LastSlashIndex + 1;
    }
    
    return Result;
}

internal_yd string
FileExtension(string FileName) {
    string Result = {};
    size_t DotIndex = RFind(FileName, '.');
    
    if (DotIndex != STRING_NOT_FOUND) {
        Result = MakeString(FileName.Data + DotIndex + 1,
                            FileName.Count - DotIndex - 1);
    }
    
    return Result;
}

internal_yd b32_yd
RemoveExtension(string* FileName) {
    b32_yd Result = false;
    size_t LastDotIndex = RFind(FileName, '.');
    
    if (LastDotIndex != STRING_NOT_FOUND) {
        Result = true;
        FileName->Count = LastDotIndex + 1;
    }
    
    return Result;
}

inline b32_yd
IsH(string Extension) {
    b32_yd Result = (StringsMatch(Extension, "h") ||
                     StringsMatch(Extension, "hpp") ||
                     StringsMatch(Extension, "hin"));
    return Result;
}

inline b32_yd
IsC(string Extension) {
    b32_yd Result = StringsMatch(Extension, "C");
    return Result;
}

inline b32_yd
IsCPP(string Extension) {
    b32_yd Result = (StringsMatch(Extension, "cpp") ||
                     StringsMatch(Extension, "cc") ||
                     StringsMatch(Extension, "cin"));
    return Result;
}

inline b32_yd
IsObjectiveC(string Extension) {
    b32_yd Result = (StringsMatch(Extension, "m") ||
                     StringsMatch(Extension, "mm"));
    return Result;
}

inline b32_yd
IsShader(string Extension) {
    b32_yd Result = (StringsMatch(Extension, "ps") ||
                     StringsMatch(Extension, "vs") ||
                     StringsMatch(Extension, "cs") ||
                     StringsMatch(Extension, "ts") ||
                     StringsMatch(Extension, "gs"));
    return Result;
}

inline b32_yd
IsINL(string Extension) {
    b32_yd Result = StringsMatch(Extension, "inl");
    return Result;
}

inline b32_yd
IsJava(string Extension) {
    b32_yd Result = StringsMatch(Extension, "java");
    return Result;
}

inline b32_yd
IsCSharp(string Extension) {
    b32_yd Result = StringsMatch(Extension, "cs");
    return Result;
}

inline b32_yd
IsPython(string Extension) {
    b32_yd Result = StringsMatch(Extension, "py");
    return Result;
}

inline b32_yd
IsSwift(string Extension) {
    b32_yd Result = StringsMatch(Extension, "swift");
    return Result;
}

inline b32_yd
IsJavascript(string Extension) {
    b32_yd Result = StringsMatch(Extension, "js");
    return Result;
}

inline b32_yd
IsBAT(string Extension) {
    b32_yd Result = StringsMatch(Extension, "bat");
    return Result;
}

inline b32_yd
IsBash(string Extension) {
    b32_yd Result = StringsMatch(Extension, "sh");
    return Result;
}

inline b32_yd
IsTXT(string Extension) {
    b32_yd Result = StringsMatch(Extension, "txt");
    return Result;
}

inline b32_yd
IsCode(string Extension) {
    b32_yd Result = (IsH(Extension) || is_c(Extension) || IsCPP(Extension) ||
                     IsObjectiveC(Extension) || IsShader(Extension) ||
                     IsINL(Extension) || IsJava(Extension) || IsCSharp(Extension) ||
                     IsPython(Extension) || IsSwift(Extension) || IsJavascript(Extension) ||
                     IsBAT(Extension) || IsBash(Extension));
    return Result;
}

inline b32_yd
IsDoc(string Extension) {
    b32_yd Result = IsTXT(Extension);
    return Result;
}

inline b32_yd
IsCodeFile(string FileName) {
    string Extension = FileExtension(FileName);
    b32_yd Result = IsCode(Extension);
    return Result;
}

inline b32_yd
IsDocFile(string FileName) {
    string Extension = FileExtension(FileName);
    b32_yd Result = IsDoc(Extension);
    return Result;
}

#endif

#define YD_STRING
#endif

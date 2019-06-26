#if !defined(YD_STRING_H)

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
typedef s32 b32_yd;

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

#if !defined(YD_ASSERT)
#define AssertYD(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#define YD_ASSERT
#endif

#define Literal(Str) (Str), (sizeof(Str) - 1)

struct string
{
    char* Data;
    size_t Count;
    size_t MemorySize;
};

// TODO(yuval): Maybe create a UTF8 string struct

global_variable_yd string NullString = {};

inline string MakeString(void* Data, size_t Count, size_t MemorySize);
inline string MakeString(void* Data, size_t Count);

#if !defined(MakeLitString)
#define LitString(Str) (MakeString((char*)(Str), sizeof(Str) - 1, sizeof(Str)))
#endif

#if !defined(Lit)
#define Lit(Str) LitString(Str)
#endif

#if !defined(FixedWidthString)
#define FixedWidthString(Str) (MakeString((char*)(Str), 0, sizeof(Str)))
#endif

#if !defined(ExpandStr)
#define ExpandStr(Str) ((Str).Data), ((Str).Count)
#endif

inline size_t StringLength(const char* Str);
inline string MakeStringSlowly(void* Str);
inline string Substr(string Str, size_t Start);
inline string Substr(string Str, size_t Start, size_t Count);
internal string SkipWhitespace(string Str);
internal string SkipWhitespace(string Str, size_t* OutSkipCount);
internal string ChopWhitespace(string Str);
internal string SkipChopWhitespace(string Str);
internal string SkipChopWhitespace(string Str, size_t* OutSkipCount);
inline string TailStr(string Str);
internal b32_yd StringsMatch(const char* A, const char* B);
internal b32_yd StringsMatch(string A, const char* B);
inline b32_yd StringsMatch(const char* A, string B);
internal b32_yd StringsMatch(string A, string B);
internal b32_yd StringsMatchPart(const char* A, const char* B, size_t* OutCount);
internal b32_yd StringsMatchPart(string A, const char* B, size_t* OutCount);
internal b32_yd StringsMatchPart(const char* A, string B, size_t* OutCount);
internal b32_yd StringsMatchPart(string A, string B, size_t* OutCount);
inline b32_yd StringsMatchPart(const char* A, const char* B);
inline b32_yd StringsMatchPart(string A, const char* B);
inline b32_yd StringsMatchPart(const char* A, string B);
inline b32_yd StringsMatchPart(string A, string B);
internal b32_yd StringsMatchInsensitive(const char* A, const char* B);
internal b32_yd StringsMatchInsensitive(string A, const char* B);
inline b32_yd StringsMatchInsensitive(const char* A, string B);
internal b32_yd StringsMatchInsensitive(string A, string B);
internal b32_yd StringsMatchPartInsensitive(const char* A, const char* B, size_t* OutCount);
internal b32_yd StringsMatchPartInsensitive(string A, const char* B, size_t* OutCount);
internal b32_yd StringsMatchPartInsensitive(const char* A, string B, size_t* OutCount);
internal b32_yd StringsMatchPartInsensitive(string A, string B, size_t* OutCount);
inline b32_yd StringsMatchPartInsensitive(const char* A, const char* B);
inline b32_yd StringsMatchPartInsensitive(string A, const char* B);
inline b32_yd StringsMatchPartInsensitive(const char* A, string B);
inline b32_yd StringsMatchPartInsensitive(string A, string B);
internal s32_yd Compare(const char* A, const char* B);
internal s32_yd Compare(string A, const char* B);
inline s32_yd Compare(const char* A, string B);
internal s32_yd Compare(string A, string B);
internal size_t Find(const char* Str, size_t Start, char Character);
internal size_t Find(string Str, size_t Start, char Character);
internal size_t RFind(string Str, size_t Start, char Character);
internal size_t Find(const char* Str, size_t Start, char* Characters);
internal size_t Find(string Str, size_t Start, char* Characters);
internal size_t FindSubstr(const char* Str, size_t Start, string Seek);
internal size_t FindSubstr(string Str, size_t Start, string Seek);
internal size_t RFindSubstr(string Str, size_t Start, string Seek);
internal size_t FindSubstrInsensitive(const char* Str, size_t Start, string Seek);
internal size_t FindSubstrInsensitive(String Str, size_t Start, string Seek);
inline b32_yd HasSubstr(const char* Str, string Seek);
inline b32_yd HasSubstr(string Str, string Seek);
inline b32_yd HasSubstrInsensitive(const char* Str, string Seek);
inline b32_yd HasSubstrInsensitive(string Str, string Seek);
internal s32_yd CopyFastUnsafe(char* Dest, const char* Src);
internal s32_yd CopyFastUnsafe(char* Dest, string Src);
internal b32_yd CopyChecked(string* Dest, string Src);
internal b32_yd CopyChecked(char* Dest, size_t DestCount, string Src);
internal b32_yd CopyPartial(string* Dest, const char* Src);
internal b32_yd CopyPartial(string* Dest, string Src);
internal b32_yd CopyPartial(char* Dest, size_t DestCount, string Src);
inline s32_yd Copy(char* Dest, const char* Src);
inline void Copy(string* Dest, string Src);
inline void Copy(string* Dest, const char* Src);
internal b32_yd AppendChecked(string* Dest, string Src);
internal b32_yd AppendPartial(string* Dest, const char* Src);
internal b32_yd AppendPartial(string* Dest, string Src);
internal b32_yd Append(string* Dest, char C);
inline b32_yd Append(string* Dest, string Src);
inline b32_yd Append(string* Dest, const char* Src);
internal b32_yd TerminateWithNull(string* Str);
internal b32_yd AppendPadding(string* Dest, char C, size_t TargetSize);
internal void StringInterpretEscapes(char* Dest, string Src);
internal void ReplaceChar(string* Str, char Replace, char With);
internal void ReplaceStr(string* Str, string Replace, string With);
internal void ReplaceStr(string* Str, string Replace, const char* With);
internal void ReplaceStr(string* Str, const char* Replace, string With);
internal void ReplaceStr(string* Str, const char* Replace, const char* With);
internal size_t S32ToStrCount(s32_yd Value);
internal b32_yd S32ToStr(string* Dest, s32_yd Value);
internal b32_yd AppendS32ToStr(string* Dest, s32_yd Value);
internal size_t U64ToStrCount(u64_yd Value);
internal b32_yd U64ToStr(string* Dest, u64_yd Value);
internal b32_yd AppendU64ToStr(string* Dest, u64_yd Value);
internal size_t F32ToStrCount(f32_yd Value);
internal b32_yd F32ToStr(string* Dest, f32_yd Value);
internal b32_yd AppendF32ToStr(string* Dest, f32_yd Value);
internal s32_yd ReverseSeekSlash(string Str, s32_yd Pos);
inline s32_yd ReverseSeekSlash(string Str);
inline string FrontOfDirectory(string Dir);
inline string PathOfDirectory(string Dir);
internal b32_yd SetLastFolder(string* Dir, const char* FolderName, char Slash);
internal b32_yd SetLastFolder(string* Dir, string FolderName, char Slash);
internal string FileExtention(string FileName);
internal b32_yd RemoveExtention(string* FileName);
internal b32_yd RemoveLastFolder(string* Path);
internal b32_yd StringSetMatch(void* StrSet, size_t ItemCount, size_t Count,
                               string Str, size_t* OutMatchIndex);
internal b32_yd StringSetMatch(string* StrSet, size_t Count, string Str, size_t* OutMatchIndex);
internal string GetFirstDoubleLine(string Source);
internal string GetNextDoubleLine(string Source, string Line);
internal string GetFirstWord(string Source);
internal string GetNextWord(string Source, string PrevWord);
// TODO(yuval): Think about string push (should require an arena)
inline b32_yd IsH(string Extention);
inline b32_yd IsCPP(string Extention);
inline b32_yd IsObjectiveC(string Extention);
inline b32_yd IsCode(string Extention);
inline b32_yd IsCodeFile(string FileName);
inline b32_yd IsLower(char C);
inline b32_yd IsLower(const char* Str);
inline b32_yd IsLower(string Str);
inline b32_yd IsLowerUTF8(u8_yd C);
inline b32_yd IsUpper(char C);
inline b32_yd IsUpper(const char* Str);
inline b32_yd IsUpper(string Str);
inline b32_yd IsUpperUTF8(u8_yd C);
inline char ToLower(char C);
internal void ToLower(char* Str);
internal void ToLower(string* Str);
internal void ToLower(char* Dest, const char* Src);
internal void ToLower(string* Dest, const char* Src);
internal void ToLower(char* Dest, string Src);
internal void ToLower(string* Dest, string Src);
inline char ToUpper(char C);
internal void ToUpper(char* Str);
internal void ToUpper(string* Str);
internal void ToUpper(char* Dest, const char* Src);
internal void ToUpper(string* Dest, const char* Src);
internal void ToUpper(char* Dest, string Src);
internal void ToUpper(string* Dest, string Src);
internal void ToCamel(char* Str);
internal void ToCamel(string* Str);
internal void ToCamel(char* Dest, const char* Src);
internal void ToCamel(string* Dest, const char* Src);
internal void ToCamel(char* Dest, string Src);
internal void ToCamel(string* Dest, string Src);
inline b32_yd IsSpacing(char C);
inline b32_yd IsSpacing(const char* Str);
inline b32_yd IsSpacing(string Str);
inline b32_yd IsEndOfLine(char C);
inline b32_yd IsEndOfLine(const char* Str);
inline b32_yd IsEndOfLine(string Str);
inline b32_yd IsWhitespace(char C);
inline b32_yd IsWhitespace(const char* Str);
inline b32_yd IsWhitespace(string Str);
inline b32_yd IsAlpha(char C);
inline b32_yd IsAlpha(const char* Str);
inline b32_yd IsAlpha(string Str);
inline b32_yd IsAlphaUTF8(u8_yd C);
inline b32_yd IsAlphaTrue(char C);
inline b32_yd IsAlphaTrue(const char* Str);
inline b32_yd IsAlphaTrue(string Str);
inline b32_yd IsAlphaTrueUTF8(u8_yd C);
inline b32_yd IsNumeric(char C);
inline b32_yd IsNumeric(const char* Str);
inline b32_yd IsNumeric(string Str);
inline b32_yd IsNumericUTF8(u8_yd C);
internal s32_yd ToNumeric(const char* Str);
internal s32_yd ToNumeric(string Str);
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
internal u32_yd HexStrToU32(string Str);
internal b32_yd ColorToHexStr(string* Dest, u32_yd Color);
internal b32_yd HexStrToColor(u32_yd* Dest, string Str);
inline b32_yd IsSlash(char C);

#define YD_STRING_H
#endif
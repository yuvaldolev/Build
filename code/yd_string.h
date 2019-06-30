#if !defined(YD_STRING)

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

#if !defined(assert_yd)
# define assert_yd(expression) if (!(expression)) { *(volatile int*)0 = 0; }
#endif

#if !defined(literal)
# define literal(str) (str), (sizeof(str) - 1)
#endif

struct String {
    char* data;
    size_t count;
    size_t memory_size;
};

// TODO(yuval): Maybe create a UTF8 String struct

#if !defined(STRING_NOT_FOUND)
# define STRING_NOT_FOUND -1
#endif

global_variable_yd String null_str = {};

inline String make_string(void* data, size_t count, size_t memory_size);
inline String make_string(void* data, size_t count);

#if !defined(make_lit_string)
# define make_lit_string(str) (make_string((char*)(str), sizeof(str) - 1, sizeof(str)))
#endif

#if !defined(lit)
# define lit(str) make_lit_string(str)
#endif

#if !defined(make_fixed_width_string)
# define make_fixed_width_string(str) (make_string((char*)(str), 0, sizeof(str)))
#endif

#if !defined(expand_string)
# define expand_string(str) ((str).data), ((str).count)
#endif

inline size_t string_length(const char* str);
inline String make_string_slowly(void* str);
inline String substr(String str, size_t start);
inline String substr(String str, size_t start, size_t count);
internal_yd String skip_whitespace(String str);
internal_yd String skip_whitespace(String str, size_t* out_skip_count);
internal_yd String chop_whitespace(String str);
internal_yd String skip_chop_whitespace(String str);
internal_yd String skip_chop_whitespace(String str, size_t* out_skip_count);
inline String tailstr(String str);
internal_yd b32_yd strings_match(const char* a, const char* b);
internal_yd b32_yd strings_match(String a, const char* b);
inline b32_yd strings_match(const char* a, String b);
internal_yd b32_yd strings_match(String a, String b);
internal_yd b32_yd strings_match_part(const char* a, const char* b, size_t* out_count);
internal_yd b32_yd strings_match_part(String a, const char* b, size_t* out_count);
internal_yd b32_yd strings_match_part(const char* a, String b, size_t* out_count);
internal_yd b32_yd strings_match_part(String a, String b, size_t* out_count);
inline b32_yd strings_match_part(const char* a, const char* b);
inline b32_yd strings_match_part(String a, const char* b);
inline b32_yd strings_match_part(const char* a, String b);
inline b32_yd strings_match_part(String a, String b);
internal_yd b32_yd strings_match_insensitive(const char* a, const char* b);
internal_yd b32_yd strings_match_insensitive(String a, const char* b);
inline b32_yd strings_match_insensitive(const char* a, String b);
internal_yd b32_yd strings_match_insensitive(String a, String b);
internal_yd b32_yd strings_match_part_insensitive(const char* a, const char* b, size_t* out_count);
internal_yd b32_yd strings_match_part_insensitive(String a, const char* b, size_t* out_count);
internal_yd b32_yd strings_match_part_insensitive(const char* a, String b, size_t* out_count);
internal_yd b32_yd strings_match_part_insensitive(String a, String b, size_t* out_count);
inline b32_yd strings_match_part_insensitive(const char* a, const char* b);
inline b32_yd strings_match_part_insensitive(String a, const char* b);
inline b32_yd strings_match_part_insensitive(const char* a, String b);
inline b32_yd strings_match_part_insensitive(String a, String b);
internal_yd s32_yd compare(const char* a, const char* b);
internal_yd s32_yd compare(String a, const char* b);
inline s32_yd compare(const char* a, String b);
internal_yd s32_yd compare(String a, String b);
internal_yd size_t find(const char* str, size_t start, char character);
internal_yd size_t find(String str, size_t start, char character);
internal_yd size_t find(const char* str, size_t start, const char* characters);
internal_yd size_t find(String str, size_t start, const char* characters);
internal_yd size_t find(const char* str, size_t start, String seek);
internal_yd size_t find(String str, size_t start, String seek);
internal_yd size_t rfind(String str, size_t start, char character);
internal_yd size_t rfind(String str, size_t start, const char* characters);
internal_yd size_t rfind(String str, size_t start, String seek);
internal_yd size_t find_first_of(const char* str, size_t start, const char* characters);
internal_yd size_t find_first_of(String str, size_t start, const char* characters);
internal_yd size_t find_insensitive(const char* str, size_t start, char character);
internal_yd size_t find_insensitive(String str, size_t start, char character);
internal_yd size_t find_insensitive(const char* str, size_t start, const char* characters);
internal_yd size_t find_insensitive(String str, size_t start, const char* characters);
internal_yd size_t find_insensitive(const char* str, size_t start, String seek);
internal_yd size_t find_insensitive(String str, size_t start, String seek);
inline b32_yd HasSubstr(const char* str, String Seek);
inline b32_yd HasSubstr(String str, String Seek);
inline b32_yd HasSubstrInsensitive(const char* str, String Seek);
inline b32_yd HasSubstrInsensitive(String str, String Seek);
internal_yd s32_yd CopyFastUnsafe(char* Dest, const char* Src);
internal_yd s32_yd CopyFastUnsafe(char* Dest, String Src);
internal_yd b32_yd CopyChecked(String* Dest, String Src);
internal_yd b32_yd CopyChecked(char* Dest, size_t Destcount, String Src);
internal_yd b32_yd CopyPartial(String* Dest, const char* Src);
internal_yd b32_yd CopyPartial(String* Dest, String Src);
internal_yd b32_yd CopyPartial(char* Dest, size_t Destcount, String Src);
inline s32_yd Copy(char* Dest, const char* Src);
inline void Copy(String* Dest, String Src);
inline void Copy(String* Dest, const char* Src);
internal_yd b32_yd AppendChecked(String* Dest, String Src);
internal_yd b32_yd AppendPartial(String* Dest, const char* Src);
internal_yd b32_yd AppendPartial(String* Dest, String Src);
internal_yd b32_yd Append(String* Dest, char C);
inline b32_yd Append(String* Dest, String Src);
inline b32_yd Append(String* Dest, const char* Src);
internal_yd b32_yd TerminateWithNull(String* str);
internal_yd b32_yd AppendPadding(String* Dest, char C, size_t TargetSize);
internal_yd void StringInterpretEscapes(char* Dest, String Src);
internal_yd void ReplaceChar(String* str, char Replace, char With);
internal_yd void ReplaceStr(String* str, String Replace, String With);
internal_yd void ReplaceStr(String* str, String Replace, const char* With);
internal_yd void ReplaceStr(String* str, const char* Replace, String With);
internal_yd void ReplaceStr(String* str, const char* Replace, const char* With);
internal_yd b32_yd StringSetMatch(void* strSet, size_t Itemcount, size_t count,
                                  String str, size_t* OutMatchIndex);
internal_yd b32_yd StringSetMatch(String* strSet, size_t count, String str, size_t* OutMatchIndex);
internal_yd String GetFirstDoubleLine(String Source);
internal_yd String GetNextDoubleLine(String Source, String Line);
internal_yd String GetFirstWord(String Source);
internal_yd String GetNextWord(String Source, String PrevWord);
// TODO(yuval): Think about String push (should require an arena)
internal_yd s32_yd ReverseSeekSlash(String str, s32_yd Pos);
inline s32_yd ReverseSeekSlash(String str);
inline String FrontOfDirectory(String Dir);
inline String PathOfDirectory(String Dir);
internal_yd b32_yd SetLastFolder(String* Dir, const char* FolderName, char Slash);
internal_yd b32_yd SetLastFolder(String* Dir, String FolderName, char Slash);
internal_yd String FileExtention(String FileName);
internal_yd b32_yd RemoveExtention(String* FileName);
internal_yd b32_yd RemoveLastFolder(String* Path);
inline b32_yd IsH(String Extention);
inline b32_yd IsCPP(String Extention);
inline b32_yd IsObjectiveC(String Extention);
inline b32_yd IsCode(String Extention);
inline b32_yd IsCodeFile(String FileName);
inline b32_yd IsLower(char C);
inline b32_yd IsLower(const char* str);
inline b32_yd IsLower(String str);
inline b32_yd IsLowerUTF8(u8_yd C);
inline b32_yd IsUpper(char C);
inline b32_yd IsUpper(const char* str);
inline b32_yd IsUpper(String str);
inline b32_yd IsUpperUTF8(u8_yd C);
inline char to_lower(char C);
internal_yd void ToLower(char* str);
internal_yd void ToLower(String* str);
internal_yd void ToLower(char* Dest, const char* Src);
internal_yd void ToLower(String* Dest, const char* Src);
internal_yd void ToLower(char* Dest, String Src);
internal_yd void ToLower(String* Dest, String Src);
inline char ToUpper(char C);
internal_yd void ToUpper(char* str);
internal_yd void ToUpper(String* str);
internal_yd void ToUpper(char* Dest, const char* Src);
internal_yd void ToUpper(String* Dest, const char* Src);
internal_yd void ToUpper(char* Dest, String Src);
internal_yd void ToUpper(String* Dest, String Src);
internal_yd void ToCamel(char* str);
internal_yd void ToCamel(String* str);
internal_yd void ToCamel(char* Dest, const char* Src);
internal_yd void ToCamel(String* Dest, const char* Src);
internal_yd void ToCamel(char* Dest, String Src);
internal_yd void ToCamel(String* Dest, String Src);
inline b32_yd IsSpacing(char C);
inline b32_yd IsSpacing(const char* str);
inline b32_yd IsSpacing(String str);
inline b32_yd IsEndOfLine(char C);
inline b32_yd IsEndOfLine(const char* str);
inline b32_yd IsEndOfLine(String str);
inline b32_yd is_whitespace(char C);
inline b32_yd IsWhitespace(const char* str);
inline b32_yd IsWhitespace(String str);
inline b32_yd IsAlpha(char C);
inline b32_yd IsAlpha(const char* str);
inline b32_yd IsAlpha(String str);
inline b32_yd IsAlphaUTF8(u8_yd C);
inline b32_yd IsAlphaTrue(char C);
inline b32_yd IsAlphaTrue(const char* str);
inline b32_yd IsAlphaTrue(String str);
inline b32_yd IsAlphaTrueUTF8(u8_yd C);
inline b32_yd IsNumeric(char C);
inline b32_yd IsNumeric(const char* str);
inline b32_yd IsNumeric(String str);
inline b32_yd IsNumericUTF8(u8_yd C);
internal_yd s32_yd ToNumeric(const char* str);
internal_yd s32_yd ToNumeric(String str);
internal_yd size_t S32ToStringCount(s32_yd Value);
internal_yd b32_yd S32ToString(String* Dest, s32_yd Value);
internal_yd b32_yd AppendS32ToString(String* Dest, s32_yd Value);
internal_yd size_t U64ToStringCount(u64_yd Value);
internal_yd b32_yd U64ToString(String* Dest, u64_yd Value);
internal_yd b32_yd AppendU64ToString(String* Dest, u64_yd Value);
internal_yd size_t F32ToStringCount(f32_yd Value);
internal_yd b32_yd F32ToString(String* Dest, f32_yd Value);
internal_yd b32_yd AppendF32ToString(String* Dest, f32_yd Value);
inline b32_yd IsAlphaNumeric(char C);
inline b32_yd IsAlphaNumeric(const char* str);
inline b32_yd IsAlphaNumeric(String str);
inline b32_yd IsAlphaNumericUTF8(u8_yd C);
inline b32_yd IsAlphaNumericTrue(char C);
inline b32_yd IsAlphaNumericTrue(const char* str);
inline b32_yd IsAlphaNumericTrue(String str);
inline b32_yd IsAlphaNumericTrueUTF8(u8_yd C);
inline b32_yd IsHex(char C);
inline b32_yd IsHex(const char* str);
inline b32_yd IsHex(String str);
inline b32_yd IsHexUTF8(u8_yd C);
inline s32_yd HexCharToS32(char C);
inline char S32ToHexChar(s32_yd Value);
internal_yd u32_yd HexStringToU32(String str);
internal_yd b32_yd ColorToHexString(String* Dest, u32_yd Color);
internal_yd b32_yd HexStringToColor(u32_yd* Dest, String str);
inline b32_yd IsSlash(char C);

#if defined(YD_STRING_IMPLEMENTATION)

//
// NOTE(yuval): String Making Functions
//

inline String
make_string(void* data, size_t count, size_t memory_size) {
    String result;
    result.data = (char*)data;
    result.count = count;
    result.memory_size = memory_size;
    
    return result;
}

inline String
make_string(void* data, size_t count) {
    String result = make_string(data, count, count);
    return result;
}

inline size_t
string_length(const char* str)
{
    size_t count = 0;
    
    if (str) {
        for (; str[count]; ++count);
    }
    
    return count;
}

inline String
make_string_slowly(void* str) {
    String result;
    result.data = (char*)str;
    result.count = string_length(str);
    result.memory_size = result.count + 1;
    
    return result;
}

//
// NOTE(yuval): String Slicing Functions
//

inline String
substr(String str, size_t start) {
    String result;
    result.data = str.data + start;
    result.Size = str.count - start;
    result.memory_size = 0;
    
    return result;
}

inline String
substr(String str, size_t start, size_t count) {
    String result;
    result.data = str.data + start;
    result.count = count;
    // TODO(yuval): Verify that this works
    if (start + count > str.count) {
        result.count = str.count - start;
    }
    result.memory_size = 0;
    
    return result;
}

internal_yd String
skip_whitespace(String str, size_t* out_skip_count) {
    size_t skip_count = 0;
    for (; skip_count < str.count && is_whitespace(str.data[skip_count]); ++skip_count);
    
    *out_skip_count = skip_count;
    String result = substr(str, skip_count);
    return result;
}

inline String
skip_whitespace(String str) {
    size_t ignored;
    String result = skip_whitespace(str, &ignored);
    return result;
}

internal_yd String
chop_whitespace(String str) {
    size_t chop_index = str.count;
    for (; chop_index > 0 && is_whitespace(str.data[chop_index - 1]); --chop_index);
    
    String result = substr(str, 0, ChopIndex);
    return result;
}

internal_yd String
skip_chop_whitespace(String str, size_t* out_skip_count) {
    String result = skip_whitespace(str, out_skip_count);
    result = chop_whitespace(result);
    return result;
}

internal_yd String
skip_chop_whitespace(String str) {
    size_t ignored;
    String result = skip_chop_whitespace(str, &ignored);
    return result;
}

inline String
tailstr(String str) {
    String result;
    result.data = str.data + str.count;
    result.count = 0;
    result.memory_size = str.memory_size - str.count;
    
    return result;
}

//
// NOTE(yuval): String Comparison Functions
//

internal_yd b32_yd
strings_match(const char* a, const char* b) {
    b32_yd result = (a == b);
    
    if (a && b) {
        while (*a && *b && (*a == *b)) {
            ++a;
            ++b;
        }
        
        result = ((*a == 0) && (*b == 0));
    }
    
    return result;
}

internal_yd b32_yd
strings_match(String a, const char* b) {
    b32_yd result = false;
    
    if (b) {
        const char* at = b;
        
        for (size_t index = 0;
             index < a.count;
             ++index, ++at) {
            if ((*at == 0) || (a.data[index] != *at)) {
                return false;
            }
        }
        
        result = (*at == 0);
    } else {
        result = (a.count == 0);
    }
    
    return result;
}

inline b32_yd
strings_match(const char* a, String b) {
    b32_yd result = strings_match(b, a);
    return result;
}

internal_yd b32_yd
strings_match(String a, String b) {
    b32_yd result = (a.count == b.count);
    
    if (result) {
        For (index, Range(a.count)) {
            if (a.data[index] != b.data[index]) {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

internal_yd b32_yd
strings_match_part(const char* a, const char* b, size_t* out_count) {
    b32_yd result = (*a == *b);
    size_t match_count = 0;
    
    if (*a && *b) {
        while (*b && (*a == *b)) {
            ++a;
            ++b;
            ++match_count;
        }
        
        result = (*b == 0);
    }
    
    *out_count = match_count;
    return result;
}

internal_yd b32_yd
strings_match_part(String a, const char* b, size_t* out_count) {
    b32_yd result = false;
    size_t index = 0;
    
    if (b) {
        for (; b[index]; ++index) {
            if ((index == a.count) ||
                (a.data[index] != b[index])) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (a.count == 0);
    }
    
    *out_count = index;
    return result;
}

internal_yd b32_yd
strings_match_part(const char* a, String b, size_t* out_count) {
    b32_yd result = false;
    size_t index = 0;
    
    if (a) {
        for (; index < b.count; ++index) {
            if (a[index] != b.data[index]) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (b.count == 0);
    }
    
    *out_count = index;
    return result;
}
internal_yd b32_yd
strings_match_part(String a, String b, size_t* out_count) {
    b32_yd result = (a.count >= b.count);
    size_t index = 0;
    
    if (result) {
        for (; index < b.count; ++index) {
            if (a.data[index] != b.data[index]) {
                result = false;
                break;
            }
        }
    }
    
    *out_count = index;
    return result;
}

inline b32_yd
strings_match_part(const char* a, const char* b) {
    size_t ignored;
    b32_yd result = strings_match_part(a, b, &ignored);
    return result;
}

inline b32_yd
strings_match_part(String a, const char* b) {
    size_t ignored;
    b32_yd result = strings_match_part(a, b, &ignored);
    return result;
}

inline b32_yd
strings_match_part(const char* a, String b) {
    size_t ignored;
    b32_yd result = strings_match_part(a, b, &ignored);
    return result;
}

inline b32_yd
strings_match_part(String a, String b) {
    size_t ignored;
    b32_yd result = strings_match_part(a, b, &ignored);
    return result;
}

internal_yd b32_yd
strings_match_insensitive(const char* a, const char* b) {
    b32_yd result = (a == b);
    
    if (a && b) {
        while (*a && *b && (to_lower(*a) == to_lower(*b))) {
            ++a;
            ++b;
        }
        
        result = ((*a == 0) && (*b == 0));
    }
    
    return result;
}

internal_yd b32_yd
strings_match_insensitive(String a, const char* b) {
    b32_yd result = false;
    
    if (b) {
        const char* at = b;
        
        for (size_t index = 0; index < a.count; ++index, ++at) {
            if ((*at == 0) || (to_lower(a.data[index]) != to_lower(*at))) {
                return false;
            }
        }
        
        result = (*at == 0);
    } else {
        result = (a.count == 0);
    }
    
    return result;
}

inline b32_yd
strings_match_insensitive(const char* a, String b) {
    b32_yd result = strings_match_insensitive(b, a);
    return result;
}

internal_yd b32_yd
strings_match_insensitive(String a, String b) {
    b32_yd result = (a.count == b.count);
    
    if (result) {
        For (index, Range(a.count)) {
            if (to_lower(a.data[index]) != to_lower(b.data[index])) {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

internal_yd b32_yd
strings_match_part_insensitive(const char* a, const char* b, size_t* out_count) {
    b32_yd result = (*a == *b);
    size_t match_count = 0;
    
    if (*a && *b) {
        while (*b && (to_lower(*a) == to_lower(*b))) {
            ++a;
            ++b;
            ++match_count;
        }
        
        result = (*b == 0);
    }
    
    *out_count = match_count;
    return result;
}

internal_yd b32_yd
strings_match_part_insensitive(String a, const char* b, size_t* out_count) {
    b32_yd result = false;
    size_t index = 0;
    
    if (b) {
        for (; b[index]; ++index) {
            if ((index == a.count) ||
                (to_lower(a.data[index]) != to_lower(b[index]))) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (a.count == 0);
    }
    
    *out_count = index;
    return result;
}
internal_yd b32_yd
strings_match_part_insensitive(const char* a, String b, size_t* out_count) {
    b32_yd result = false;
    size_t index = 0;
    
    if (a) {
        for (; index < b.count; ++index) {
            if (to_lower(a[index]) != to_lower(b.data[index])) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (b.count == 0);
    }
    
    *out_count = index;
    return result;
}

internal_yd b32_yd
strings_match_part_insensitive(String a, String b, size_t* out_count) {
    b32_yd result = (a.count >= b.count);
    size_t index = 0;
    
    if (result) {
        for (; index < b.count; ++index) {
            if (to_lower(a.data[index]) != to_lower(b.data[index])) {
                result = false;
                break;
            }
        }
    }
    
    *out_count = index;
    return result;
}

inline b32_yd
strings_match_part_insensitive(const char* a, const char* b) {
    size_t ignored;
    b32_yd result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

inline b32_yd
strings_match_part_insensitive(String a, const char* b) {
    size_t ignored;
    b32_yd result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

inline b32_yd
strings_match_part_insensitive(const char* a, String b) {
    size_t ignored;
    b32_yd result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

inline b32_yd
strings_match_part_insensitive(String a, String b) {
    size_t ignored;
    b32_yd result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

internal_yd s32_yd
compare(const char* a, const char* b) {
    s32_yd index = 0;
    while (a[index] && b[index] &&
           (a[index] != b[index])) {
        ++index;
    }
    
    s32_yd result = (a[index] > b[index]) - (a[index] < b[index]);
    return result;
}

internal_yd s32_yd
compare(String a, const char* b) {
    s32_yd index = 0;
    while ((index < a.count) && b[index] &&
           (a.data[index] == b[index])) {
        ++index;
    }
    
    s32_yd result = 0;
    if (index < a.count) {
        result = (a.data[index] > b[index]) - (a.data[index] < b[index]);
    } else {
        if (b[index]) {
            result = 0;
        } else {
            result = -1;
        }
    }
    
    return result;
}

inline s32_yd
compare(const char* a, String b) {
    s32_yd result = -compare(b, a);
    return result;
}

internal_yd s32_yd
compare(String a, String b) {
    size_t min_count = a.count;
    if (b.count < min_count) {
        min_count = b.count;
    }
    
    s32_yd index = 0;
    while ((index < min_count) && (a.data[i] == b.data[i])) {
        ++index;
    }
    
    s32_yd result = 0;
    if (index < min_count) {
        result = (a.data[index] > b.data[index]) - (a.data[index] < b.data[index]);
    } else {
        result = (a.count > b.count) - (a.count < b.count);
    }
    
    return result;
}

//
// NOTE(yuval): Functions For Finding Characters And Substrings
//

internal_yd size_t
find(const char* str, size_t start, char character) {
    assert_yd(start >= 0);
    
    for (size_t index = start; str[index]; ++index) {
        if (str[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(String str, size_t start, char character) {
    assert_yd((start >= 0) && (start < str.count));
    
    for (size_t index = start; index < str.count; ++index) {
        if (str.data[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(const char* str, size_t start, const char* characters) {
    assert_yd(start >= 0);
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t index = start; str[index]; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (const char* at = characters; *at; ++at, ++str_index) {
            if (str[str_index] != *at) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(String str, size_t start, const char* characters) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = str.count - string_length(characters) + 1;
    
    for (size_t index = start; index < stop_at; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (const char* at = characters; *at; ++at, ++str_index) {
            if (str.data[str_index] != *at) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(const char* str, size_t start, String seek) {
    assert_yd(start >= 0);
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t index = start; str[index]; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (size_t seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (str[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(String str, size_t start, String seek) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = str.count - seek.count + 1;
    
    for (size_t index = start; index < stop_at; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (size_t seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (str.data[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
rfind(String str, size_t start, char character) {
    assert_yd((start >= 0) && (start < str.count));
    
    for (size_t index = start; index >= 0; --index) {
        if (str.data[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
rfind(String str, size_t start, const char* characters) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    size_t characters_count = string_length(characters);
    if (start + characters_count > str.count) {
        start = str.count - characters_count;
    }
    
    for (size_t index = start; index >= 0; --index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (const char* at = characters; *at; ++at, ++str_index) {
            if (str.data[str_index] != *at) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}
internal_yd size_t
rfind(String str, size_t start, String seek) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    size_t characters_count = string_length(characters);
    if (start + characters_count > str.count) {
        start = str.count - characters_count;
    }
    
    for (size_t index = start; index >= 0; --index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (size_t seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (str.data[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_first_of(const char* str, size_t start, const char* characters) {
    assert_yd(start >= 0);
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t index = start; str[index]; ++index) {
        for (const char* at = characters; *at; ++at) {
            if (str[index] == *at) {
                return index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_first_of(String str, size_t start, const char* characters) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t index = start; index < str.count; ++index) {
        for (const char* at = characters; *at; ++at) {
            if (str.data[index] == *at) {
                return index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(const char* str, size_t start, char character) {
    assert_yd(start >= 0);
    
    for (size_t index = start; str[index]; ++index) {
        if (to_lower(str[index]) == to_lower(character)) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(String str, size_t start, char character) {
    assert_yd((start >= 0) && (start < str.count));
    
    for (size_t index = start; index < str.count; ++index) {
        if (to_lower(str.data[index]) == to_lower(character)) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(const char* str, size_t start, const char* characters) {
    assert_yd(start >= 0);
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t index = start; str[index]; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (const char* at = characters; *at; ++at, ++str_index) {
            if (to_lower(str[str_index]) != to_lower(*at)) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(String str, size_t start, const char* characters) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = str.count - string_length(characters) + 1;
    
    for (size_t index = start; index < stop_at; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (const char* at = characters; *at; ++at, ++str_index) {
            if (to_lower(str.data[str_index]) != to_lower(*at)) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(const char* str, size_t start, String seek) {
    assert_yd(start >= 0);
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    for (size_t index = start; str[index]; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (size_t seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (to_lower(str[str_index]) != to_lower(seek.data[seek_index])) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(String str, size_t start, String seek) {
    assert_yd((start >= 0) && (start < str.count));
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    size_t stop_at = str.count - seek.count + 1;
    
    for (size_t index = start; index < stop_at; ++index) {
        b32_yd hit = true;
        size_t str_index = index;
        
        for (size_t seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (to_lower(str.data[str_index]) != to_lower(seek.data[seek_index])) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

#endif

#define YD_STRING
#endif
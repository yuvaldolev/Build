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

global_variable_yd const size_t STRING_NOT_FOUND = -1;
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
internal_yd b32_yd string_set_match(void* str_set, size_t item_size, size_t count,
                                    String str, size_t* out_match_index);
inline b32_yd string_set_match(String* str_set, size_t count, String str, size_t* out_match_index);
internal_yd s32_yd compare(const char* a, const char* b);
internal_yd s32_yd compare(String a, const char* b);
inline s32_yd compare(const char* a, String b);
internal_yd s32_yd compare(String a, String b);
internal_yd size_t find(const char* str, char character, size_t start);
internal_yd size_t find(String str, char character, size_t start);
internal_yd size_t find(const char* str, const char* characters, size_t start);
internal_yd size_t find(String str, const char* characters, size_t start);
internal_yd size_t find(const char* str, String seek, size_t start);
internal_yd size_t find(String str, String seek, size_t start);
inline size_t find(const char* str, char character);
inline size_t find(String str, char character);
inline size_t find(const char* str, const char* characters);
inline size_t find(String str, const char* characters);
inline size_t find(const char* str, String seek);
inline size_t find(String str, String seek);
internal_yd size_t rfind(String str, char character, size_t start);
internal_yd size_t rfind(String str, const char* characters, size_t start);
internal_yd size_t rfind(String str, String seek, size_t start);
inline size_t rfind(String str, char character);
inline size_t rfind(String str, const char* characters);
inline size_t rfind(String str, String seek);
internal_yd size_t find_first_of(const char* str, const char* characters, size_t start);
internal_yd size_t find_first_of(String str, const char* characters,  size_t start);
inline size_t find_first_of(const char* str, const char* characters);
inline size_t find_first_of(String str, const char* characters);
internal_yd size_t find_insensitive(const char* str, char character, size_t start);
internal_yd size_t find_insensitive(String str, char character, size_t start);
internal_yd size_t find_insensitive(const char* str, const char* characters, size_t start);
internal_yd size_t find_insensitive(String str, const char* characters, size_t start);
internal_yd size_t find_insensitive(const char* str, String seek, size_t start);
internal_yd size_t find_insensitive(String str, String seek, size_t start);
inline size_t find_insensitive(const char* str, char character);
inline size_t find_insensitive(String str, char character);
inline size_t find_insensitive(const char* str, const char* characters);
inline size_t find_insensitive(String str, const char* characters);
inline size_t find_insensitive(const char* str, String seek);
inline size_t find_insensitive(String str, String seek);
inline b32_yd has_substr(const char* str, String seek);
inline b32_yd has_substr(String str, String seek);
inline b32_yd has_substr_insensitive(const char* str, String seek);
inline b32_yd has_substr_insensitive(String str, String seek);
internal_yd String get_first_double_line(String source);
internal_yd String get_next_double_line(String source, String line);
internal_yd String get_next_word(String source, String prev_word);
inline String get_first_word(String source);
internal_yd size_t copy_fast_unsafe(char* dest, const char* source);
internal_yd size_t copy_fast_unsafe(char* dest, String source);
internal_yd b32_yd copy_checked(String* dest, String source);
internal_yd b32_yd copy_checked(char* dest, size_t dest_cap, String source);
internal_yd b32_yd copy_partial(String* dest, const char* source);
internal_yd b32_yd copy_partial(String* dest, String source);
internal_yd b32_yd copy_partial(char* dest, size_t dest_cap, String source);
inline size_t copy(char* dest, const char* source);
inline void copy(String* dest, String source);
inline void copy(String* dest, const char* source);
internal_yd b32_yd append_checked(String* dest, String source);
internal_yd b32_yd append_partial(String* dest, const char* source);
internal_yd b32_yd append_partial(String* dest, String source);
internal_yd b32_yd append(String* dest, char c);
inline b32_yd append(String* dest, const char* source);
inline b32_yd append(String* dest, String source);
internal_yd b32_yd terminate_with_null(String* str);
internal_yd b32_yd append_padding(String* dest, char c, size_t target_count);
internal_yd void replace_range(String* str, size_t first, size_t one_past_last, char with);
internal_yd void replace_range(String* str, size_t first, size_t one_past_last, const char* with);
internal_yd void replace_range(String* str, size_t first, size_t one_past_last, String with);
internal_yd void replace(String* str, char to_replace, char with);
internal_yd void replace(String* str, const char* to_replace, const char* with);
internal_yd void replace(String* str, const char* to_replace, String with);
internal_yd void replace(String* str, String to_replace, const char* with);
internal_yd void replace(String* str, String to_replace, String with);
internal_yd void string_interpret_escapes(char* dest, String source);
internal_yd size_t reverse_seek_slash(String str, size_t shift_from_last_char);
internal_yd size_t reverse_seek_slash(String str);
inline String front_of_directory(String dir);
inline String path_of_directory(String dir);
internal_yd b32_yd set_last_folder(String* dir, const char* folder_name, char slash);
internal_yd b32_yd set_last_folder(String* dir, String folder_name, char slash);
internal_yd b32_yd remove_last_folder(String* dir);
internal_yd String file_extension(String filename);
internal_yd b32_yd remove_extension(String* filename);
inline b32_yd is_h(String extension);
inline b32_yd is_c(String extension);
inline b32_yd is_cpp(String extension);
inline b32_yd is_objective_c(String extension);
inline b32_yd is_shader(String extension);
inline b32_yd is_inl(String extension);
inline b32_yd is_java(String extension);
inline b32_yd is_csharp(String extension);
inline b32_yd is_python(String extension);
inline b32_yd is_swift(String extension);
inline b32_yd is_javascript(String extension);
inline b32_yd is_bat(String extension);
inline b32_yd is_bash(String extension);
inline b32_yd is_txt(String extension);
inline b32_yd is_code(String extension);
inline b32_yd is_doc(String extension);
inline b32_yd is_code_file(String filename);
inline b32_yd is_doc_file(String filename);
// TODO(yuval): Think about String push (should require an arena)
inline b32_yd is_lower(char c);
inline b32_yd is_lower(const char* str);
inline b32_yd is_lower(String str);
inline b32_yd is_lower_utf8(u8_yd c);
inline b32_yd is_upper(char c);
inline b32_yd is_upper(const char* str);
inline b32_yd is_upper(String str);
inline b32_yd is_upper_utf8(u8_yd c);
inline char to_lower(char c);
internal_yd void to_lower(char* str);
internal_yd void to_lower(String* str);
internal_yd void to_lower(char* dest, const char* source);
internal_yd void to_lower(String* dest, const char* source);
internal_yd void to_lower(char* dest, String source);
internal_yd void to_lower(String* dest, String source);
inline char to_upper(char c);
internal_yd void to_upper(char* str);
internal_yd void to_upper(String* str);
internal_yd void to_upper(char* dest, const char* source);
internal_yd void to_upper(String* dest, const char* source);
internal_yd void to_upper(char* dest, String source);
internal_yd void to_upper(String* dest, String source);
internal_yd void to_camel(char* str);
internal_yd void to_camel(String* str);
internal_yd void to_camel(char* dest, const char* source);
internal_yd void to_camel(String* dest, const char* source);
internal_yd void to_camel(char* dest, String source);
internal_yd void to_camel(String* dest, String source);
inline b32_yd is_spacing(char c);
inline b32_yd is_end_of_line(char c);
inline b32_yd is_whitespace(char c);
inline b32_yd is_alpha_true(char c);
inline b32_yd is_alpha_true(const char* str);
inline b32_yd is_alpha_true(String str);
inline b32_yd is_alpha_true_utf8(u8_yd c);
inline b32_yd is_alpha(char c);
inline b32_yd is_alpha(const char* str);
inline b32_yd is_alpha(String str);
inline b32_yd is_alpha_utf8(u8_yd c);
inline b32_yd IsNumeric(char c);
inline b32_yd IsNumeric(const char* str);
inline b32_yd IsNumeric(String str);
inline b32_yd IsNumeric_utf8(u8_yd c);
internal_yd s32_yd ToNumeric(const char* str);
internal_yd s32_yd ToNumeric(String str);
internal_yd size_t S32ToStringCount(s32_yd Value);
internal_yd b32_yd S32ToString(String* dest, s32_yd Value);
internal_yd b32_yd AppendS32ToString(String* dest, s32_yd Value);
internal_yd size_t U64ToStringCount(u64_yd Value);
internal_yd b32_yd U64ToString(String* dest, u64_yd Value);
internal_yd b32_yd AppendU64ToString(String* dest, u64_yd Value);
internal_yd size_t F32ToStringCount(f32_yd Value);
internal_yd b32_yd F32ToString(String* dest, f32_yd Value);
internal_yd b32_yd AppendF32ToString(String* dest, f32_yd Value);
inline b32_yd is_alpha_numeric(char C);
inline b32_yd is_alpha_numeric(const char* str);
inline b32_yd is_alpha_numeric(String str);
inline b32_yd is_alpha_numeric_utf8(u8_yd C);
inline b32_yd is_alpha_numeric_true(char C);
inline b32_yd is_alpha_numeric_true(const char* str);
inline b32_yd is_alpha_numeric_true(String str);
inline b32_yd is_alpha_numeric_true_utf8(u8_yd C);
inline b32_yd IsHex(char C);
inline b32_yd IsHex(const char* str);
inline b32_yd IsHex(String str);
inline b32_yd IsHex_utf8(u8_yd C);
inline s32_yd HexCharToS32(char C);
inline char S32ToHexChar(s32_yd Value);
internal_yd u32_yd HexStringToU32(String str);
internal_yd b32_yd ColorToHexString(String* dest, u32_yd Color);
internal_yd b32_yd HexStringToColor(u32_yd* dest, String str);
inline b32_yd is_slash(char C);

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

internal_yd b32_yd
string_set_match(void* str_set, size_t item_size, size_t count,
                 String str, size_t* out_match_index) {
    b32_yd result = false;
    u8_yd* at = (u8_yd*)str_set;
    
    for (size_t index = 0; index < count; ++index, at += item_size) {
        if (strings_match(*((String*)at), str)) {
            *out_match_index = index;
            result = true;
            break;
        }
    }
    
    return result;
}

inline b32_yd
string_set_match(String* str_set, size_t count, String str, size_t* out_match_index) {
    b32_yd result = string_set_match(str_set, sizeof(String), count,
                                     str, out_match_index);
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
find(const char* str, char character, size_t start) {
    assert_yd(start >= 0);
    
    for (size_t index = start; str[index]; ++index) {
        if (str[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(String str, char character, size_t start) {
    assert_yd((start >= 0) && (start < str.count));
    
    for (size_t index = start; index < str.count; ++index) {
        if (str.data[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find(const char* str, const char* characters, size_t start) {
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
find(String str, const char* characters,  size_t start) {
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
find(const char* str, String seek, size_t start) {
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
find(String str, String seek, size_t start) {
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

inline size_t
find(const char* str, char character) {
    size_t result = find(str, character, 0);
    return result;
}

inline size_t
find(String str, char character) {
    size_t result = find(str, character, 0);
    return result;
}

inline size_t
find(const char* str, const char* characters) {
    size_t result = find(str, characters, 0);
    return result;
}

inline size_t
find(String str, const char* characters) {
    size_t result = find(str, characters, 0);
    return result;
}

inline size_t
find(const char* str, String seek) {
    size_t result = find(str, seek, 0);
    return result;
}

inline size_t
find(String str, String seek) {
    size_t result = find(str, seek, 0);
    return result;
}

internal_yd size_t
rfind(String str, char character, size_t start) {
    assert_yd((start >= 0) && (start < str.count));
    
    for (size_t index = start; index >= 0; --index) {
        if (str.data[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
rfind(String str, const char* characters, size_t start) {
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
rfind(String str, String seek, size_t start) {
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

inline size_t
rfind(String str, char character) {
    size_t result = rfind(str, character, 0);
    return result;
}

inline size_t
rfind(String str, const char* characters) {
    size_t result = rfind(str, characters, 0);
    return result;
}

inline size_t
rfind(String str, String seek) {
    size_t result = rfind(str, seek, 0);
    return result;
}

internal_yd size_t
find_first_of(const char* str, const char* characters, size_t start) {
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
find_first_of(String str, const char* characters, size_t start) {
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

inline size_t
find_first_of(const char* str, const char* characters) {
    size_t result = find_first_of(str, characters, 0);
    return result;
}

inline size_t
find_first_of(String str, const char* characters) {
    size_t result = find_first_of(str, characters, 0);
    return result;
}

internal_yd size_t
find_insensitive(const char* str, char character, size_t start) {
    assert_yd(start >= 0);
    
    for (size_t index = start; str[index]; ++index) {
        if (to_lower(str[index]) == to_lower(character)) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(String str, char character, size_t start) {
    assert_yd((start >= 0) && (start < str.count));
    
    for (size_t index = start; index < str.count; ++index) {
        if (to_lower(str.data[index]) == to_lower(character)) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
find_insensitive(const char* str, const char* characters, size_t start) {
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
find_insensitive(String str, const char* characters, size_t start) {
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
find_insensitive(const char* str, String seek, size_t start) {
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
find_insensitive(String str, String seek, size_t start) {
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

inline size_t
find_insensitive(const char* str, char character) {
    size_t result = find_insensitive(str, character, 0);
    return result;
}

inline size_t
find_insensitive(String str, char character) {
    size_t result = find_insensitive(str, character, 0);
    return result;
}

inline size_t
find_insensitive(const char* str, const char* characters) {
    size_t result = find_insensitive(str, characters, 0);
    return result;
}

inline size_t
find_insensitive(String str, const char* characters) {
    size_t result = find_insensitive(str, characters, 0);
    return result;
}

inline size_t
find_insensitive(const char* str, String seek) {
    size_t result = find_insensitive(str, seek, 0);
    return result;
}

inline size_t
find_insensitive(String str, String seek) {
    size_t result = find_insensitive(str, seek, 0);
    return result;
}

inline b32_yd
has_substr(const char* str, String seek) {
    b32_yd result = (find(str, seek) != STRING_NOT_FOUND);
    return result;
}

inline b32_yd
has_substr(String str, String seek) {
    b32_yd result = (find(str, seek) != STRING_NOT_FOUND);
    return result;
}

inline b32_yd
has_substr_insensitive(const char* str, String seek) {
    b32_yd result = (find_insensitive(str, seek) != STRING_NOT_FOUND);
    return result;
}

inline b32_yd
has_substr_insensitive(String str, String seek) {
    b32_yd result = (find_insensitive(str, seek) != STRING_NOT_FOUND);
    return result;
}


internal_yd String
get_first_double_line(String source) {
    String result = {};
    
    size_t pos = find(source, make_lit_string("\n\n"));
    if (pos == STRING_NOT_FOUND) {
        pos = find(source, make_lit_string("\r\n\r\n"));
    }
    
    if (pos != STRING_NOT_FOUND) {
        result = substr(str, 0, pos);
    }
    
    return result;
}

internal_yd String
get_next_double_line(String source, String line) {
    String result = {};
    
    size_t line_end_index = (size_t)(line.data - source.data) + line.count;
    assert((source.data[line_end_index] == '\n') || (source.data[line_end_index] == '\r'));
    
    ++line_end_index;
    assert((source.data[line_end_index] == '\n') || (source.data[line_end_index] == '\r'));
    
    size_t start = line_end_index + 1;
    
    if (start < source.count) {
        size_t pos = find(source, make_lit_string("\n\n"), start);
        if (pos == STRING_NOT_FOUND) {
            pos = find(source, make_lit_string("\r\n\r\n"), start);
        }
        
        if (pos != STRING_NOT_FOUND) {
            result = substr(source, start, pos - start);
        }
    }
    
    return result;
}

internal_yd String
get_next_word(String source, String prev_word) {
    String result = {};
    size_t pos0 = (size_t)(prev_word.data - source.data) + prev_word.count;
    
    for (; pos0 < source.count; ++pos0) {
        char c = source.data[pos0];
        if (!(is_whitespace(c) || c == '(' || c == ')')) {
            break;
        }
    }
    
    if (pos0 < source.count) {
        size_t pos1 = pos;
        
        for (; pos1 < source.count; ++pos1) {
            char c = source.data[pos1];
            if (is_whitespace(c) || c == '(' || c == ')') {
                break;
            }
        }
        
        word = substr(source, pos0, pos1 - pos0);
    }
    
    return word;
}


inline String
get_first_word(String source) {
    String start = make_string(source.data, 0);
    String result = get_next_word(source, start);
    return result;
}

//
// NOTE(yuval): String Copy And Append Functions
//

internal_yd size_t
copy_fast_unsafe(char* dest, const char* source) {
    char* dest_at = dest;
    char* source_at = source;
    
    while (*source_at) {
        *dest_at++ = *source_at++;
    }
    
    *dest_at = 0;
    
    size_t result = (dest_at - dest);
    return result;
}

internal_yd s32_yd
copy_fast_unsafe(char* dest, String source) {
    for (size_t index = 0; index < source.count; ++index) {
        dest[index] = source.data[index];
    }
    
    dest[index] = 0;
    return source.count;
}

internal_yd b32_yd
copy_checked(String* dest, String source) {
    if (dest->memory_size < source.count) {
        return false;
    }
    
    for (size_t index = 0; index < source.count; ++index) {
        dest->data[index] = source.data[index];
    }
    
    dest->count = source.count;
    return true;
}

internal_yd b32_yd
copy_checked(char* dest, size_t dest_cap, String source) {
    if (dest_cap < source.count + 1) {
        return false;
    }
    
    for (size_t index = 0; index < source.count; ++index) {
        dest[index] = source.data[index];
    }
    
    dest[index] = 0;
    return true;
}

internal_yd b32_yd
copy_partial(String* dest, const char* source) {
    b32_yd result = true;
    
    for (size_t index = 0; source[index]; ++index) {
        if (index >= dest->memory_size) {
            result = false;
            break;
        }
        
        dest->data[index] = source[index];
    }
    
    dest->count = index;
    return result;
}

internal_yd b32_yd
copy_partial(String* dest, String source) {
    b32_yd result = true;
    
    for (size_t index = 0; index < source.count; ++index) {
        if (index >= dest->memory_size) {
            result = false;
            break;
        }
        
        dest->data[index] = source.data[index];
    }
    
    dest->count = index;
    return result;
}

internal_yd b32_yd
copy_partial(char* dest, size_t dest_cap, String source) {
    b32_yd result = true;
    
    for (size_t index = 0; index < source.count; ++index) {
        if (index >= dest_cap - 1) {
            result = false;
            break;
        }
        
        dest[index] = source.data[index];
    }
    
    dest[index] = 0;
    return result;
}

inline size_t
copy(char* dest, const char* source) {
    size_t result = copy_fast_unsafe(dest, source);
    return result;
}

inline void
copy(String* dest, String source) {
    copy_checked(dest, source);
}

inline void
copy(String* dest, const char* source) {
    copy_partial(dest, source);
}

internal_yd b32_yd
append_checked(String* dest, String source) {
    String end = tailstr(*dest);
    b32_yd result = copy_checked(&end, source);
    dest->count += end.count;
    return result;
}

internal_yd b32_yd
append_partial(String* dest, const char* source) {
    String end = tailstr(*dest);
    b32_yd result = copy_partial(&end, source);
    dest->count += end.count;
    return result;
}

internal_yd b32_yd
append_partial(String* dest, String source) {
    String end = tailstr(*dest);
    b32_yd result = copy_partial(&end, source);
    dest->count += end.count;
    return result;
}

internal_yd b32_yd
append(String* dest, char c) {
    b32_yd result = false;
    
    if (dest->count < dest->memory_size) {
        dest->data[dest->count++] = c;
        result = true;
    }
    
    return result;
}

inline b32_yd
append(String* dest, const char* source) {
    b32_yd result = append_partial(dest, source);
    return result;
}

inline b32_yd
append(String* dest, String source) {
    b32_yd result = append_partial(dest, source);
    return result;
}

internal_yd b32_yd
terminate_with_null(String* str) {
    b32_yd result = false;
    
    if (str->count < str->memory_size) {
        str->data[str->count] = 0;
        result = true;
    }
    
    return result;
}

internal_yd b32_yd
append_padding(String* dest, char c, size_t target_count) {
    b32_yd result = true;
    
    for (size_t count = dest->count; count < target_count; ++count) {
        if (!append(dest, c)) {
            result = false;
            break;
        }
    }
    
    return result;
}

//
// NOTE(yuval): Other String Editing Functions
//

internal_yd void
replace_range(String* str, size_t first, size_t one_past_last, char with) {
    assert_yd((first >= 0) && (from < str->count));
    assert_yd((one_past_last > 0) && (one_past_last <= str->count));
    assert_yd(first < one_past_last);
    
    for (size_t index = from; index < one_past_last; ++index) {
        str->data[index] = with;
    }
}

internal_yd void
replace_range(String* str, size_t first, size_t one_past_last, const char* with) {
    String with_str = make_string_slowly(with);
    replace_range(str, first, one_past_last, with_str);
}

// TODO(yuval): Maybe rename to block_copy?
internal_yd void
block_move_yds(void* dest_init, const void* source_init, size_t size) {
    if (dest_init && source_init) {
        const u8* source = (const u8*)source_init;
        u8* dest = (u8*)dest_init;
        
        if (dest < source) {
            while (size--) {
                *dest++ = *source++;
            }
        } else if (dest > source) {
            source += size - 1;
            dest += size - 1;
            
            while (size--) {
                *dest-- = *source--;
            }
        }
    }
}

internal_yd void
replace_range(String* str, size_t first, size_t one_past_last, String with) {
    assert_yd((first >= 0) && (from < str->count));
    assert_yd((one_past_last > 0) && (one_past_last <= str->count));
    assert_yd(first < one_past_last);
    
    s64_yd shift = with.count - (one_past_last - first);
    size_t new_count = str->count + shift;
    
    if (new_count <= str->memory_size) {
        if (shift != 0) {
            char* tail = str->data + one_past_last;
            char* new_tail_pos = tail + shift;
            // TODO(yuval): Verify that this has no bugs!!!!!!!!!
            block_move_yds(new_tail_pos, tail, str->count - one_past_last);
        }
        
        block_move_yds(str->data + first, with.data, with.count);
        str->count += shift;
    }
}

internal_yd void
replace(String* str, char to_replace, char with) {
    for (size_t index = 0; index < str->count; ++index) {
        if (str->data[index] == to_replace) {
            str->data[index] = with;
        }
    }
}

internal_yd void
replace(String* str, const char* to_replace, const char* with) {
    replace(str, make_string_slowly(to_replace), make_string_slowly(with));
}

internal_yd void
replace(String* str, const char* to_replace, String with) {
    replace(str, make_string_slowly(to_replace), with);
}

internal_yd void
replace(String* str, String to_replace, const char* with) {
    replace(str, to_replace, make_string_slowly(with));
}

internal_yd void
replace(String* str, String to_replace, String with) {
    size_t index = 0;
    
    for (;;) {
        index = find(*str, to_replace, index);
        if (index == STRING_NOT_FOUND) {
            break;
        }
        
        replace_range(str, index, index + to_replace.count, with);
        index += with.count;
    }
}

internal_yd void
string_interpret_escapes(char* dest, String source) {
    s32_yd mode = 0;
    size_t dest_index = 0;
    
    for (size_t source_index = 0; source_index < source.count; ++source_index) {
        switch (mode) {
            case 0: {
                if (source.data[source_index] = '\\') {
                    mode = 1;
                } else {
                    dest[dest_index++] = source.data[source_index];
                }
            } break;
            
            case 1: {
                char c = source.data[source_index];
                switch (c) {
                    case '\\': { dest[dest_index++] = '\\'; } break;
                    case 'n': { dest[dest_index++] = '\n'; } break;
                    case 't': { dest[dest_index++] = '\t'; } break;
                    case '"': { dest[dest_index++] = '"'; } break;
                    case '0': { dest[dest_index++] = '\0'; } break;
                    default: { dest[dest_index++] = '\\'; dest[dest_index++] = c; } break;
                }
                
                mode = 0;
            } break;
        }
    }
    
    dest[dest_index] = 0;
}

//
// NOTE(yuval): File / Directory Strings Management Functions
//

internal_yd size_t
reverse_seek_slash(String str, size_t shift_from_last_char) {
    for (size_t index = str.size - shift_from_last_char - 1;
         index >= 0;
         --index) {
        if (is_slash(str[index])) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

internal_yd size_t
reverse_seek_slash(String str) {
    size_t result = reverse_seek_slash(str, 0);
    return result;
}

inline String
front_of_directory(String dir) {
    String result = substr(dir, reverse_seek_slash(dir) + 1);
    return result;
}

inline String
path_of_directory(String dir) {
    String result = substr(dir, 0, reverse_seek_slash(dir) + 1);
    return result;
}

internal_yd b32_yd
set_last_folder(String* dir, const char* folder_name, char slash) {
    b32_yd result = false;
    size_t last_slash_index = reverse_seek_slash(*dir);
    
    if (last_slash_index != STRING_NOT_FOUND) {
        size_t count = last_slash_index + 1;
        dir->count = count;
        
        if (append(dir, folder_name)) {
            if (append(dir, slash)) {
                result = true;
            }
        }
        
        if (!result) {
            dir->count = count;
        }
    }
    
    return result;
}

internal_yd b32_yd
set_last_folder(String* dir, String folder_name, char slash) {
    b32_yd result = false;
    size_t last_slash_index = reverse_seek_slash(*dir);
    
    if (last_slash_index != STRING_NOT_FOUND) {
        size_t count = last_slash_index + 1;
        dir->count = count;
        
        if (append(dir, folder_name)) {
            if (append(dir, slash)) {
                result = true;
            }
        }
        
        if (!result) {
            dir->count = count;
        }
    }
    
    return result;
}

internal_yd b32_yd
remove_last_folder(String* path) {
    b32_yd result = false;
    size_t last_slash_index = reverse_seek_slash(*path, 1);
    
    if (last_slash_index != STRING_NOT_FOUND) {
        result = true;
        path->count = last_slash_index + 1;
    }
    
    return result;
}

internal_yd String
file_extension(String filename) {
    String result = {};
    size_t dot_index = rfind(filename, '.');
    
    if (dot_index != STRING_NOT_FOUND) {
        result = make_string(filename.data + dot_index + 1,
                             filename.count - dot_index - 1);
    }
    
    return result;
}

internal_yd b32_yd
remove_extension(String* filename) {
    b32_yd result = false;
    size_t last_dot_index = rfind(filename, '.');
    
    if (last_dot_index != STRING_NOT_FOUND) {
        result = true;
        filename->count = last_dot_index + 1;
    }
    
    return result;
}

inline b32_yd
is_h(String extension) {
    b32_yd result = (strings_match(extension, "h") ||
                     strings_match(extension, "hpp") ||
                     strings_match(extension, "hin"));
    return result;
}

inline b32_yd
is_c(String extension) {
    b32_yd result = strings_match(extension, "c");
    return result;
}

inline b32_yd
is_cpp(String extension) {
    b32_yd result = (strings_match(extension, "cpp") ||
                     strings_match(extension, "cc") ||
                     strings_match(extension, "cin"));
    return result;
}

inline b32_yd
is_objective_c(String extension) {
    b32_yd result = (strings_match(extension, "m") ||
                     strings_match(extension, "mm"));
    return result;
}

inline b32_yd
is_shader(String extension) {
    b32_yd result = (strings_match(extension, "ps") ||
                     strings_match(extension, "vs") ||
                     strings_match(extension, "cs") ||
                     strings_match(extension, "ts") ||
                     strings_match(extension, "gs"));
    return result;
}

inline b32_yd
is_inl(String extension) {
    b32_yd result = strings_match(extension, "inl");
    return result;
}

inline b32_yd
is_java(String extension) {
    b32_yd result = strings_match(extension, "java");
    return result;
}

inline b32_yd
is_csharp(String extension) {
    b32_yd result = strings_match(extension, "cs");
    return result;
}

inline b32_yd
is_python(String extension) {
    b32_yd result = strings_match(extension, "py");
    return result;
}

inline b32_yd
is_swift(String extension) {
    b32_yd result = strings_match(extension, "swift");
    return result;
}

inline b32_yd
is_javascript(String extension) {
    b32_yd result = strings_match(extension, "js");
    return result;
}

inline b32_yd
is_bat(String extension) {
    b32_yd result = strings_match(extension, "bat");
    return result;
}

inline b32_yd
is_bash(String extension) {
    b32_yd result = strings_match(extension, "sh");
    return result;
}

inline b32_yd
is_txt(String extension) {
    b32_yd result = strings_match(extension, "txt");
    return result;
}

inline b32_yd
is_code(String extension) {
    b32_yd result = (is_h(extension) || is_c(extension) || is_cpp(extension) ||
                     is_objective_c(extension) || is_shader(extension) ||
                     is_inl(extension) || is_java(extension) || is_csharp(extension) ||
                     is_python(extension) || is_swift(extension) || is_javascript(extension) ||
                     is_bat(extension) || is_bash(extension));
    return result;
}

inline b32_yd
is_doc(String extension) {
    b32_yd result = is_txt(extension);
    return result;
}

inline b32_yd
is_code_file(String filename) {
    String extension = file_extension(filename);
    b32_yd result = is_code(extension);
    return result;
}

inline b32_yd
is_doc_file(String filename) {
    String extension = file_extension(filename);
    b32_yd result = is_doc(extension);
    return result;
}

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

inline b32_yd
is_lower(char c) {
    b32_yd result = ((c >= 'a') && (c <= 'z'));
    return result;
}

inline b32_yd
is_lower(const char* str) {
    b32_yd result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_lower(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_lower(String str) {
    b32_yd result = true;
    
    for (size_t index = 0; index < str.count; ++index) {
        if (!is_lower(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_lower_utf8(u8_yd c) {
    b32_yd result = (is_lower((char)c) || (c >= 128));
    return result;
}

inline b32_yd
is_upper(char c) {
    b32_yd result = ((c >= 'A') && (c <= 'Z'));
    return result;
}

inline b32_yd
is_upper(const char* str) {
    b32_yd result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_upper(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_upper(String str) {
    b32_yd result;
    
    for (size_t index = 0; index < str.count; ++index) {
        if (!is_upper(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_upper_utf8(u8_yd c) {
    b32_yd result = (is_upper((char)c) || (c >= 128));
    return result;
}

inline char
to_lower(char c) {
    char result = c;
    
    if (is_upper(result)) {
        result += 'a' - 'A';
    }
    
    return result;
}

internal_yd void
to_lower(char* str) {
    for (char* at = str; *at; ++at) {
        *at = to_lower(*at);
    }
}

internal_yd void
to_lower(String* str) {
    for (size_t index = 0; index < str->count; ++index) {
        str->data[index] = to_lower(str->data[index]);
    }
}

internal_yd void
to_lower(char* dest, const char* source) {
    const char* source_at = source;
    char* dest_at = dest;
    
    while (*source_at) {
        *dest_at++ = to_lower(*source_at++);
    }
    
    *dest_at = 0;
}

internal_yd void
to_lower(String* dest, const char* source) {
    size_t index = 0;
    
    for (; source[index]; ++index) {
        dest->data[index] = to_lower(source[index]);
    }
    
    dest->count = index;
}

internal_yd void
to_lower(char* dest, String source) {
    for (size_t index = 0; index < source.count; ++index) {
        dest[index] = to_lower(source.data[index]);
    }
    
    dest[source.count] = 0;
}

internal_yd void
to_lower(String* dest, String source) {
    if (dest->memory_size >= source.count) {
        for (size_t index = 0; index < source.count; ++index) {
            dest->data[index] = to_lower(source.data[index]);
        }
        
        dest->count = source.count;
    }
}

inline char
to_upper(char c) {
    char result = c;
    
    if (is_lower(result)) {
        result -= 'a' - 'A';
    }
    
    return result;
}

internal_yd void
to_upper(char* str) {
    for (char* at = str; *at; ++at) {
        *at = to_upper(*at);
    }
}

internal_yd void
to_upper(String* str) {
    for (size_t index = 0; index < str->count; ++index) {
        str->data[index] = to_upper(str->data[index]);
    }
}

internal_yd void
to_upper(char* dest, const char* source) {
    const char* source_at = source;
    char* dest_at = dest;
    
    while (*source_at) {
        *dest_at++ = to_upper(*source_at++);
    }
    
    *dest_at = 0;
}

internal_yd void
to_upper(String* dest, const char* source) {
    size_t index = 0;
    
    for (; source[index]; ++index) {
        dest->data[index] = to_upper(source[index]);
    }
    
    dest->count = index;
}

internal_yd void
to_upper(char* dest, String source) {
    for (size_t index = 0; index < source.count; ++index) {
        dest[index] = to_upper(source.data[index]);
    }
    
    dest[source.count] = 0;
}

internal_yd void
to_upper(String* dest, String source) {
    if (dest->memory_size >= source.count) {
        for (size_t index = 0; index < source.count; ++index) {
            dest->data[index] = to_upper(source.data[index]);
        }
        
        dest->count = source.count;
    }
}

internal_yd void
to_camel(char* str) {
    b32_yd is_first = true;
    
    for (char* at = str; *at; ++at) {
        if (is_alpha_numeric_true(*at)) {
            if (is_first) {
                *at = to_upper(*at);
                is_first = false;
            } else {
                *at = to_lower(*at);
            }
        } else {
            is_first = true;
        }
    }
}

internal_yd void
to_camel(String* str) {
    b32_yd is_first = true;
    
    for (size_t index = 0; index < str->count; ++index) {
        if (is_alpha_numeric_true(str->data[index])) {
            if (is_first) {
                str->data[index] = to_upper(str->data[index]);
                is_first = false;
            } else {
                str->data[index] = to_lower(str->data[index]);
            }
        } else {
            is_first = true;
        }
    }
}

internal_yd void
to_camel(char* dest, const char* source) {
    const char* source_at = source;
    char* dest_at = dest;
    b32_yd is_first = false;
    
    for (; *source_at; ++source_at, ++dest_at) {
        char c = *source_at;
        
        if (is_alpha_numeric_true(c)) {
            if (is_first) {
                c = to_upper(c);
                is_first = false;
            } else {
                c = to_lower(c);
            }
        } else {
            is_first = true;
        }
        
        *dest_at = c;
    }
    
    *dest_at = 0;
}

internal_yd void
to_camel(String* dest, const char* source) {
    size_t index = 0;
    b32_yd is_first = true;
    
    for (; source[index]; ++index) {
        char c = source[index];
        
        if (is_alpha_numeric_true(c)) {
            if (is_first) {
                c = to_upper(c);
                is_first = false;
            } else {
                c = to_lower(c);
            }
        } else {
            is_first = true;
        }
        
        dest->data[index] = c;
    }
    
    dest->count = index;
}

internal_yd void
to_camel(char* dest, String source) {
    b32_yd is_first = true;
    
    for (size_t index = 0; index < source.count; ++index) {
        char c = source.data[index];
        
        if (is_alpha_numeric_true(c)) {
            if (is_first) {
                c = to_upper(c);
                is_first = false;
            } else {
                c = to_lower(c);
            }
        } else {
            is_first = true;
        }
        
        dest[index] = c;
    }
    
    dest[source.count] = index;
}

internal_yd void
to_camel(String* dest, String source) {
    if (dest->memory_size >= source.count) {
        b32_yd is_first = true;
        
        for (size_t index = 0; index < source.count; ++index) {
            char c = source.data[index];
            
            if (is_alpha_numeric_true(c)) {
                if (is_first) {
                    c = to_upper(c);
                    is_first = false;
                } else {
                    c = to_lower(c);
                }
            } else {
                is_first = true;
            }
            
            dest->data[index] = c;
        }
        
        dest->count = source.count;
    }
}

inline b32_yd
is_spacing(char c) {
    b32_yd result = ((c == ' ') || (c == '\t') ||
                     (c == '\v') || (c == '\f'));
    return result;
}

inline b32_yd
is_end_of_line(char c) {
    b32_yd result = ((c == '\n') || (c == '\r'));
    return result;
}

inline b32_yd
is_whitespace(char c) {
    b32_yd result = (is_spacing(c) || is_end_of_line(c));
}

inline b32_yd
is_alpha_true(char c) {
    b32_yd result = (is_lower(c) || is_upper(c));
}

inline b32_yd
is_alpha_true(const char* str) {
    b32_yd result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_true(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_alpha_true(String str) {
    b32_yd result = true;
    
    for (size_t index = 0; index < str.count; ++index) {
        if (!is_alpha_true(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_alpha_true_utf8(u8_yd c) {
    b32_yd result = (is_alpha_true((char)c) || (c >= 128));
    return result;
}

inline b32_yd
is_alpha(char c) {
    b32_yd result = (is_alpha_true(c) || (c == '_'));
    return result;
}

inline b32_yd
is_alpha(const char* str) {
    b32_yd result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_alpha(String str) {
    b32_yd result = true;
    
    for (size_t index = 0; index < str.count; ++index) {
        if (!is_alpha(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

inline b32_yd
is_alpha_utf8(u8_yd c) {
    b32_yd result = (is_alpha(c) || (c >= 128));
}

#endif

#define YD_STRING
#endif
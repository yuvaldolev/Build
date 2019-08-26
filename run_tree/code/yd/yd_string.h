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
#define yd_global static
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
#endif // #if !defined(YD_TYPES)

#if !defined(YD_ASSERT)
# define YDAssert(expression) if (!(expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YD_ASSERT)

struct String
{
    char* data;
    yd_umm count;
    yd_umm capacity;
};

// TODO(yuval): Maybe create A UTF8 string struct

#if !defined(LITERIAL)
# define LITERAL(str) (str), (sizeof(str) - 1)
#endif // #if !defined(LITERIAL)

#if !defined(EXPAND_STRING)
# define EXPAND_STRING(str) ((str).data), ((str).count)
#endif // #if !defined(EXPAND_STRING)

#if !defined(PRINTABLE_STRING)
# define PRINTABLE_STRING(str) ((yd_s32)((str).count)), ((str).data)
#endif // #if !defined(PRINTABLE_STRING)

//
// NOTE(yuval): Flags And Constants
//

yd_global const String NULL_STRING = {};
yd_global const yd_umm STRING_NOT_FOUND = -1;

//
// NOTE(yuval): Public API Function Declarations
//

String skip_whitespace(String str, yd_umm* out_skip_count);
String chop_whitespace(String str);

yd_b32 strings_match(const char* a, const char* b);
yd_b32 strings_match(String a, const char* b);
yd_b32 strings_match(String a, String b);
yd_b32 strings_match(const char* a, yd_umm a_count, const char* b);
yd_b32 strings_match(const char* a, yd_umm a_count,
                     const char* b, yd_umm b_count);
yd_b32 strings_match(String a, const char* b, yd_umm count);
yd_b32 strings_match_part(const char* a, const char* b, yd_umm* out_count);
yd_b32 strings_match_part(String a, const char* b, yd_umm* out_count);
yd_b32 strings_match_part(const char* a, String b, yd_umm* out_count);
yd_b32 strings_match_part(String a, String b, yd_umm* out_count);
yd_b32 strings_match_insensitive(const char* a, const char* b);
yd_b32 strings_match_insensitive(String a, const char* b);
yd_b32 strings_match_insensitive(String a, String b);
yd_b32 strings_match_part_insensitive(const char* a, const char* b, yd_umm* out_count);
yd_b32 strings_match_part_insensitive(String a, const char* b, yd_umm* out_count);
yd_b32 strings_match_part_insensitive(const char* a, String b, yd_umm* out_count);
yd_b32 strings_match_part_insensitive(String a, String b, yd_umm* out_count);
yd_b32 string_set_match(void* str_set, yd_umm item_size, yd_umm count,
                        String str, yd_umm* out_match_index);

yd_s32 compare(const char* a, const char* b);
yd_s32 compare(String a, const char* b);
yd_s32 compare(String a, String b);

yd_umm find(const char* str, char character, yd_umm start);
yd_umm find(String str, char character, yd_umm start);
yd_umm find(const char* str, String Seek, yd_umm start);
yd_umm find(String str, String seek, yd_umm start);
yd_umm rfind(const char* str, yd_umm count, char character, yd_umm start);
yd_umm rfind(String str, char character, yd_umm start);
yd_umm rfind(const char* str, yd_umm count, String seek, yd_umm start);
yd_umm rfind(String str, String seek, yd_umm start);
yd_umm find_first_of(const char* str, const char* characters, yd_umm start);
yd_umm find_first_of(String str, const char* characters,  yd_umm start);
yd_umm find_insensitive(const char* str, char character, yd_umm start);
yd_umm find_insensitive(String str, char character, yd_umm start);
yd_umm find_insensitive(const char* str, String seek, yd_umm start);
yd_umm find_insensitive(String str, String seek, yd_umm start);

String get_first_double_line(String source);
String get_next_double_line(String source, String line);
String get_next_word(String source, String prev_word);

yd_umm copy_fast_unsafe(char* dest, const char* source);
yd_umm copy_fast_unsafe(char* dest, String source);
yd_b32 copy_checked(String* dest, String source);
yd_b32 copy_checked(char* dest, yd_umm dest_cap, String source);
yd_b32 copy_partial(String* dest, const char* source);
yd_b32 copy_partial(String* dest, String source);
yd_b32 copy_partial(char* dest, yd_umm dest_cap, String source);

yd_b32 append_checked(String* dest, String source);
yd_b32 append_partial(String* dest, const char* source);
yd_b32 append_partial(String* dest, String source);
yd_b32 append(String* dest, char c);
yd_b32 terminate_with_null(String* str);
yd_b32 append_padding(String* dest, char c, yd_umm target_count);

yd_b32 concat(String* dest, String source_a, String source_b);

void replace_range(String* str, yd_umm first, yd_umm one_past_last, char with);
void replace_range(String* str, yd_umm first, yd_umm one_past_last, const char* with);
void replace_range(String* str, yd_umm first, yd_umm one_past_last, String with);
void replace(String* str, char to_replace, char ׳ith);
void replace(String* str, const char* to_replace, const char* with);
void replace(String* str, const char* to_replace, String with);
void replace(String* str, String to_replace, const char* with);
void replace(String* str, String to_replace, String with);
void string_interpret_escapes(char* dest, String source);

#if defined(YD_MEMORY)
char* push_z(Memory_Arena* arena, yd_umm count,
             Arena_Push_Params params = default_arena_params());
String push_string(Memory_Arena* arena, yd_umm capacity,
                   Arena_Push_Params params = default_arena_params());
char* push_copy_z(Memory_Arena* arena, const char* source,
                  Arena_Push_Params params = default_arena_params());
String push_copy_string(Memory_Arena* arena, const char* source,
                        Arena_Push_Params params = default_arena_params());
char* push_copy_z(Memory_Arena* arena, String source,
                  Arena_Push_Params params = default_arena_params());
String push_copy_string(Memory_Arena* arena, String source,
                        Arena_Push_Params params = default_arena_params());
#endif // #if defined(YD_MEMORY)

void to_lower(char* str);
void to_lower(String* str);
void to_lower(char* dest, const char* source);
void to_lower(String* dest, const char* source);
void to_lower(char* dest, String source);
void to_lower(String* dest, String source);
void to_upper(char* str);
void to_upper(String* str);
void to_upper(char* dest, const char* source);
void to_upper(String* dest, const char* source);
void to_upper(char* dest, String source);
void to_upper(String* dest, String source);
void to_camel(char* str);
void to_camel(String* str);
void to_camel(char* dest, const char* source);
void to_camel(String* dest, const char* source);
void to_camel(char* dest, String source);
void to_camel(String* dest, String source);
yd_umm u64_to_string_count(yd_u64 value);
yd_b32 u64_to_string(String* dest, yd_u64 value);
yd_b32 append_u64_to_string(String* dest, yd_u64 value);
yd_umm s32_to_string_count(yd_s32 value);
yd_b32 s32_to_string(String* dest, yd_s32 value);
yd_b32 append_s32_to_string(String* dest, yd_s32 value);
yd_umm f32_to_string_count(yd_f32 value, yd_u32 max_precision);
yd_b32 f32_to_string(String* dest, yd_f32 value, yd_u32 max_precision);
yd_b32 append_f32_to_string(String* dest, yd_f32 value, yd_u32 max_precision);
yd_u32 hex_string_to_u32(String str);
yd_b32 color_to_hex_string(String* dest, yd_u32 color);
yd_b32 hex_string_to_color(yd_u32* dest, String str);

yd_umm reverse_seek_slash(String str, yd_umm shift_from_last_char);
yd_umm reverse_seek_slash(String str);
yd_b32 set_last_folder(String* dir, const char* folder_name, char slash);
yd_b32 set_last_folder(String* dir, String folder_name, char slash);
yd_b32 remove_last_folder(String* dir);
String file_extension(String filename);
yd_b32 set_extension(String* filename, const char* extension);
yd_b32 set_extension(String* filename, String extension);
yd_b32 remove_extension(String* filename);

//
// NOTE(yuval): Public API Inline Functions
//

//
// NOTE(yuval): String Making Functions
//

yd_internal inline String
make_string(void* data, yd_umm count, yd_umm capacity) {
    String result;
    result.data = (char*)data;
    result.count = count;
    result.capacity = capacity;
    
    return result;
}

yd_internal inline String
make_string(void* data, yd_umm count) {
    String result = make_string(data, count, count);
    return result;
}

yd_internal inline yd_umm
string_length(const char* str) {
    yd_umm count = 0;
    
    if (str) {
        for (; str[count]; ++count);
    }
    
    return count;
}

yd_internal inline String
make_string_slowly(const void* str) {
    String result;
    result.data = (char*)str;
    result.count = string_length((const char*)str);
    result.capacity = result.count + 1;
    
    return result;
}

#if !defined(MAKE_LIT_STRING)
# define MAKE_LIT_STRING(str) (make_string((char*)(str), sizeof(str) - 1, sizeof(str)))
#endif // #if !defined(MAKE_LIT_STRING)

#if !defined(MAKE_FIXED_WIDTH_STRING)
# define MAKE_FIXED_WIDTH_STRING(str) (make_string((char*)(str), 0, sizeof(str)))
#endif // #if !defined(MAKE_FIXED_WIDTH_STRING)

//
// NOTE(yuval): Utility String Functions
//

yd_internal inline yd_b32
is_null_string(String str) {
    yd_b32 result = ((str.data = NULL_STRING.data) &&
                     (str.count == NULL_STRING.count) &&
                     (str.capacity == NULL_STRING.capacity));
    
    return result;
}

//
// NOTE(yuval): String Slicing Functions
//

yd_internal inline String
substr(String str, yd_umm start) {
    YDAssert((start >= 0) && (start <= str.count));
    
    String result;
    result.data = str.data + start;
    result.count = str.count - start;
    result.capacity = 0;
    
    return result;
}

yd_internal inline String
substr(String str, yd_umm Start, yd_umm count) {
    YDAssert((Start >= 0) && (Start <= str.count));
    
    String result;
    result.data = str.data + start;
    
    result.count = count;
    // TODO(yuval): Verify that this works
    if (start + count > str.count) {
        result.count = str.count - start;
    }
    
    result.capacity = 0;
    
    return result;
}

yd_internal inline String
skip_whitespace(String str) {
    yd_umm ignored;
    String result = skip_whitespace(str, &ignored);
    return result;
}

yd_internal inline String
skip_chop_whitespace(String str, yd_umm* ou_skip_count) {
    String result = skip_whitespace(str, out_skip_count);
    result = chop_whitespace(result);
    return result;
}

yd_internal inline String
skip_chop_whitespace(String str) {
    yd_umm ignored;
    String result = skip_chop_whitespace(str, &ignored);
    return result;
}

yd_internal inline String
tailstr(String str) {
    String result;
    result.data = str.data + str.count;
    result.count = 0;
    result.capacity = str.capacity - str.count;
    
    return result;
}

yd_internal inline void
advance_string(String* value, yd_umm count) {
    if (value->count >= count) {
        value->data += count;
        value->count -= count;
        value->capacity -= (count * sizeof(char));
    } else {
        value->data += value->count;
        value->count = 0;
        value->capacity = 0;
    }
}

//
// NOTE(yuval): String Comparison Functions
//

yd_internal inline yd_b32
strings_match(const char* a, String b) {
    yd_b32 result = strings_match(b, a);
    return result;
}

yd_internal inline yd_b32
strings_match(const char* a, const char* b, yd_umm b_count) {
    yd_b32 result = strings_match(b, b_count, a);
    return result;
}

yd_internal inline yd_b32
strings_match(const char* a, String b, yd_umm count) {
    yd_b32 result = strings_match(b, a, count);
    return result;
}

yd_internal inline yd_b32
strings_match_part(const char* a, const char* b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_part(String a, const char* b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_part(const char* a, String b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_part(String a, String b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_insensitive(const char* a, String b) {
    yd_b32 result = strings_match_insensitive(b, a);
    return result;
}

yd_internal inline yd_b32
strings_match_part_insensitive(const char* a, const char* b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_part_insensitive(String a, const char* b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_part_insensitive(const char* a, String b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
strings_match_part_insensitive(String a, String b) {
    yd_umm ignored;
    yd_b32 result = strings_match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
string_set_match(String* str_set, yd_umm count,
                 String str, yd_umm* out_match_index) {
    yd_b32 result = string_set_match(str_set, sizeof(String), count,
                                     str, out_match_index);
    return result;
}

yd_internal inline yd_s32
compare(const char* a, String b) {
    yd_s32 result = -compare(b, a);
    return result;
}

//
// NOTE(yuval): Functions For Finding Characters And SubStrings
//

yd_internal inline yd_umm
find(const char* str, char character) {
    yd_umm result = Find(str, Character, 0);
    return result;
}

yd_internal inline yd_umm
find(String str, char character) {
    yd_umm result = find(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find(const char* str, String seek) {
    yd_umm result = find(str, seek, 0);
    return result;
}

yd_internal inline yd_umm
find(String str, String seek) {
    yd_umm result = find(str, seek, 0);
    return result;
}

yd_internal inline yd_umm
rfind(const char* str, yd_umm count, char character) {
    yd_umm result = rfind(str, count, character, count - 1);
    return result;
}

yd_internal inline yd_umm
rfind(String str, char character) {
    yd_umm result = rfind(str, character, str.count - 1);
    return result;
}

yd_internal inline yd_umm
rfind(const char* str, yd_umm count, String seek) {
    yd_umm result = rfind(str, count, seek, count - 1);
    return result;
}

yd_internal inline yd_umm
rfind(String str, String seek) {
    yd_umm result = RFind(str, seek, str.count - 1);
    return result;
}

yd_internal inline yd_umm
find_first_of(const char* str, const char* characters) {
    yd_umm result = find_first_of(str, characters, 0);
    return result;
}

yd_internal inline yd_umm
find_first_of(String str, const char* characters) {
    yd_umm result = find_first_of(str, characters, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(const char* str, char character) {
    yd_umm result = find_insensitive(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(String str, char character) {
    yd_umm result = find_insensitive(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(const char* str, String seek) {
    yd_umm result = find_insensitive(str, seek, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(String str, String seek) {
    yd_umm result = find_insensitive(str, seek, 0);
    return result;
}

yd_internal inline yd_b32
has_substr(const char* str, String seek) {
    yd_b32 result = (find(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline yd_b32
has_substr(String str, String seek) {
    yd_b32 result = (find(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline yd_b32
has_substr_insensitive(const char* str, String seek) {
    yd_b32 result = (find_insensitive(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline yd_b32
has_substr_insensitive(String str, String seek) {
    yd_b32 result = (find_insensitive(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline String
get_first_word(String Source) {
    String start = make_string(source.data, 0);
    String result = get_next_word(source, start);
    return result;
}

//
// NOTE(yuval): String Copy And Append Functions
//

yd_internal inline yd_umm
copy(char* dest, const char* source) {
    yd_umm result = copy_fast_unsafe(dest, source);
    return result;
}

yd_internal inline yd_umm
copy(char* dest, String source) {
    yd_umm result = copy_fast_unsafe(dest, source);
    return result;
}

yd_internal inline void
copy(String* dest, String source) {
    copy_checked(dest, source);
}

yd_internal inline void
copy(String* dest, const char* source) {
    copy_partial(dest, source);
}

yd_internal inline yd_b32
append(String* dest, const char* source) {
    yd_b32 result = append_partial(dest, source);
    return result;
}

yd_internal inline yd_b32
append(String* dest, String source) {
    yd_b32 result = append_partial(dest, source);
    return result;
}

//
// NOTE(yuval): String Concatenation Functions
//

yd_internal inline yd_b32
concat_strings(char* dest, yd_umm dest_count,
               const char* source_a, yd_umm source_a_count,
               const char* source_b, yd_umm source_b_count) {
    String dest_string = make_string(dest, dest_count - 1);
    String source_a_string = make_string((char*)source_a, source_a_count);
    String source_b_string = make_string((char*)source_b, source_b_count);
    
    yd_b32 result = concat_strings(&dest_string, source_a_string, source_b_string);
    dest[dest_string.count] = 0;
    
    return result;
}

yd_internal inline yd_b32
concat_strings(char* dest, yd_umm dest_count,
               String source_a, String source_b) {
    String dest_string = make_string(dest, dest_count - 1);
    
    yd_b32 result = concat_strings(&dest_string, source_a, source_b);
    dest[dest_string.count] = 0;
    
    return result;
}

yd_internal inline yd_b32
concat_strings(String* dest,
               const char* source_a, yd_umm source_a_count,
               const char* Source_b, yd_umm source_b_count) {
    String source_a_string = make_string((char*)source_a, source_a_count);
    String source_b_string = make_string((char*)source_b, source_b_count);
    
    yd_b32 result = concat_strings(dest, source_a_string, source_b_string);
    return result;
}

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

yd_internal inline yd_b32
is_lower(char c)
{
    yd_b32 result = ((c >= 'A') && (c <= 'z'));
    return result;
}

yd_internal inline yd_b32
is_lower(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_lower(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_lower(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_lower(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_lower_utf8(yd_u8 c) {
    yd_b32 result = (is_lower((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32 is_upper(char c);

yd_internal inline char
to_lower(char c) {
    char result = c;
    
    if (is_upper(result)) {
        result += 'a' - 'A';
    }
    
    return result;
}

yd_internal inline yd_b32
is_upper(char c) {
    yd_b32 result = ((c >= 'A') && (c <= 'Z'));
    return result;
}

yd_internal inline yd_b32
is_upper(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_upper(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_upper(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_upper(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_upper_utf8(yd_u8 c) {
    yd_b32 result = (is_upper((char)c) || (c >= 128));
    return result;
}

yd_internal inline char
to_upper(char c) {
    char result = c;
    
    if (is_lower(result)) {
        result -= 'a' - 'A';
    }
    
    return result;
}

yd_internal inline yd_b32
is_spacing(char c) {
    yd_b32 result = ((c == ' ') || (c == '\t') ||
                     (c == '\v') || (c == '\f'));
    return result;
}

yd_internal inline yd_b32
is_end_of_line(char c) {
    yd_b32 result = ((c == '\n') || (c == '\r'));
    return result;
}

yd_internal inline yd_b32
is_whitespace(char c) {
    yd_b32 result = (is_spacing(c) || is_end_of_line(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_true(char c) {
    yd_b32 result = (is_lower(c) || is_upper(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_true(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_true(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_true(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_alpha_true(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_true_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha_true((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_alpha(char c) {
    yd_b32 result = (is_alpha_true(c) || (c == '_'));
    return result;
}

yd_internal inline yd_b32
is_alpha(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at)
    {
        if (!is_alpha(*at))
        {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index)
    {
        if (!is_alpha(str.data[index]))
        {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_numeric(char c) {
    yd_b32 result = ((c >= '0') && (c <= '9'));
    return result;
}

yd_internal inline yd_b32
is_numeric(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_numeric(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_numeric(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_numeric(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_numeric_utf8(yd_u8 c) {
    yd_b32 result = (is_numeric((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_umm
f32_to_string_count(yd_f32 value) {
    yd_umm result = f32_to_string_count(value, 8);
    return result;
}

yd_internal inline yd_b32
f32_to_string(String* dest, yd_f32 value) {
    yd_b32 result = f32_to_string(dest, value, 8);
    return result;
}

yd_internal inline yd_b32
append_f32_to_string(String* dest, yd_f32 value) {
    yd_b32 result = append_f32_to_string(dest, value, 8);
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric(char c) {
    yd_b32 result = (is_alpha(c) || is_numeric(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_numeric(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_alpha_numeric(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha_numeric((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true(char c) {
    yd_b32 result = (is_alpha_true(c) || is_numeric(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_numeric_true(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true(String str)
{
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_alpha_numeric_true(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha_numeric_true((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_hex(char c) {
    yd_b32 result = (((c >= '0') && (c <= '9')) ||
                     ((c >= 'A') && (c <= 'F')) ||
                     ((c >= 'a') && (c <= 'f')));
    return result;
}

yd_internal inline yd_b32
is_hex(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_hex(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_hex(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_hex(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_hex_utf8(yd_u8 c) {
    yd_b32 result = (is_hex((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_s32
hex_char_to_s32(char c) {
    yd_s32 result;
    
    if (is_numeric(c)) {
        result = c - '0';
    } else if (c > 'F') {
        result = 10 + (c - 'a');
    } else {
        result = 10 + (c - 'A');
    }
    
    return result;
}

yd_internal inline char
s32_to_hex_char(yd_s32 value) {
    char result;
    
    if (value < 10) {
        result = '0' + (char)value;
    } else {
        result = 'A' + (char)(value - 10);
    }
    
    return result;
}

//
// NOTE(yuval): File / Directory Strings Management Functions
//

yd_internal inline yd_b32
IsSlash(char c) {
    yd_b32 result = ((c == '/') || (c == '\\'));
    return result;
}

yd_internal inline String
front_of_directory(String dir) {
    String result = substr(dir, reverse_seek_slash(dir) + 1);
    return result;
}

yd_internal inline String
path_of_directory(String dir) {
    String result = substr(dir, 0, reverse_seek_slash(dir) + 1);
    return result;
}

yd_internal inline yd_b32
is_h(String extension) {
    yd_b32 result = (strings_match(extension, "h") ||
                     strings_match(extension, "hpp") ||
                     strings_match(extension, "hin"));
    return result;
}

yd_internal inline yd_b32
is_c(String extension) {
    yd_b32 result = strings_match(extension, "c");
    return result;
}

yd_internal inline yd_b32
is_cpp(String extension) {
    yd_b32 result = (strings_match(extension, "cpp") ||
                     strings_match(extension, "cc") ||
                     strings_match(extension, "cin"));
    return result;
}

yd_internal inline yd_b32
is_objective_c(String extension) {
    yd_b32 result = (strings_match(extension, "m") ||
                     strings_match(extension, "mm"));
    return result;
}

yd_internal inline yd_b32
is_shader(String extension) {
    yd_b32 result = (strings_match(extension, "ps") ||
                     strings_match(extension, "vs") ||
                     strings_match(extension, "cs") ||
                     strings_match(extension, "ts") ||
                     strings_match(extension, "gs"));
    return result;
}

yd_internal inline yd_b32
is_inl(String extension) {
    yd_b32 result = strings_match(extension, "inl");
    return result;
}

yd_internal inline yd_b32
is_java(String extension) {
    yd_b32 result = strings_match(extension, "java");
    return result;
}

yd_internal inline yd_b32
is_csharp(String extension) {
    yd_b32 result = strings_match(extension, "cs");
    return result;
}

yd_internal inline yd_b32
is_python(String extension) {
    yd_b32 result = strings_match(extension, "py");
    return result;
}

yd_internal inline yd_b32
is_swift(String extension) {
    yd_b32 result = strings_match(extension, "swift");
    return result;
}

yd_internal inline yd_b32
is_javascript(String extension) {
    yd_b32 result = strings_match(extension, "js");
    return result;
}

yd_internal inline yd_b32
is_bat(String extension) {
    yd_b32 result = strings_match(extension, "bat");
    return result;
}

yd_internal inline yd_b32
is_bash(String extension) {
    yd_b32 result = strings_match(extension, "sh");
    return result;
}

yd_internal inline yd_b32
is_txt(String extension) {
    yd_b32 result = strings_match(extension, "txt");
    return result;
}

yd_internal inline yd_b32
is_code(String extension) {
    yd_b32 result = (is_h(extension) || is_c(extension) || is_cpp(extension) ||
                     is_objective_c(extension) || is_shader(extension) ||
                     is_inl(extension) || is_java(extension) || is_csharp(extension) ||
                     is_python(extension) || is_swift(extension) || is_javascript(extension) ||
                     is_bat(extension) || is_bash(extension));
    return result;
}

yd_internal inline yd_b32
is_doc(String extension) {
    yd_b32 result = is_txt(extension);
    return result;
}

yd_internal inline yd_b32
is_code_file(String filename) {
    String extension = file_extension(filename);
    yd_b32 result = is_code(extension);
    return result;
}

yd_internal inline yd_b32
is_doc_file(String filename)
{
    String extension = file_extension(filename);
    yd_b32 result = is_doc(extension);
    return result;
}

#define YD_STRING
#endif // #if !defined(YD_STRING)

//
// NOTE(yuval): Exported Function Implementations
//

#if defined(YD_STRING_IMPLEMENTATION)

//
// NOTE(yuval): String Slicing Functions
//

String
SkipWhitespace(String str, yd_umm* OutSkipcount)
{
    yd_umm Skipcount = 0;
    for (; Skipcount < str.count && is_whitespace(str.data[Skipcount]); ++Skipcount);
    
    *OutSkipcount = Skipcount;
    String result = substr(str, Skipcount);
    
    return result;
}

String
ChopWhitespace(String str)
{
    yd_umm Chopindex = str.count;
    for (; Chopindex > 0 && is_whitespace(str.data[Chopindex - 1]); --Chopindex);
    
    String result = substr(str, 0, Chopindex);
    return result;
}

//
// NOTE(yuval): String Comparison Functions
//

yd_b32
strings_match(const char* A, const char* B)
{
    yd_b32 result = (A == B);
    
    if (A && B)
    {
        while (*A && *B && (*A == *B))
        {
            ++A;
            ++B;
        }
        
        result = ((*A == 0) && (*B == 0));
    }
    
    return result;
}

yd_b32
strings_match(String A, const char* B)
{
    yd_b32 result = false;
    
    if (B)
    
    {
        const char* at = B;
        
        for (yd_umm index = 0;
             index < A.count;
             ++index, ++at)
        {
            if ((*at == 0) || (A.data[index] != *at))
            {
                return false;
            }
        }
        
        result = (*at == 0);
    }
    else
    {
        result = (A.count == 0);
    }
    
    return result;
}

yd_b32
strings_match(String A, String B)
{
    yd_b32 result = (A.count == B.count);
    
    if (result)
    {
        for (yd_umm index = 0; index < A.count; ++index)
        {
            if (A.data[index] != B.data[index])
            {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

yd_b32
strings_match(const char* A, yd_umm a_count, const char* B)
{
    yd_b32 result = false;
    
    if (B)
    {
        const char* at = B;
        
        for (yd_umm index = 0; index < a_count; ++index)
        {
            if ((*at == 0) || (A[index] != *at))
            {
                return false;
            }
        }
        
        result = (*at == 0);
    }
    else
    {
        result = (a_count == 0);
    }
    
    return result;
}

yd_b32
strings_match(const char* A, yd_umm a_count,
              const char* B, yd_umm b_count)
{
    b32 result = (a_count == b_count);
    
    if (result)
    {
        for (yd_umm index = 0; index < a_count; ++index)
        {
            if (A[index] != B[index])
            {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

yd_b32
strings_match(String A, const char* B, yd_umm count)
{
    b32 result = true;
    
    for (yd_umm index = 0; index < count; ++index)
    {
        if (A.data[index] != B[index])
        {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_b32
strings_match_part(const char* A, const char* B, yd_umm* out_count)
{
    yd_b32 result = (*A == *B);
    yd_umm match_count = 0;
    
    if (*A && *B)
    {
        while (*B && (*A == *B))
        {
            ++A;
            ++B;
            ++match_count;
        }
        
        result = (*B == 0);
    }
    
    *out_count = match_count;
    return result;
}

yd_b32
strings_match_part(String A, const char* B, yd_umm* out_count)
{
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (B)
    {
        for (; B[index]; ++index)
        {
            if ((index == A.count) ||
                (A.data[index] != B[index]))
            {
                return false;
            }
        }
        
        result = true;
    }
    else
    {
        result = (A.count == 0);
    }
    
    *out_count = index;
    return result;
}

yd_b32
strings_match_part(const char* A, String B, yd_umm* out_count)
{
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (A)
    {
        for (; index < B.count; ++index)
        {
            if (A[index] != B.data[index])
            {
                return false;
            }
        }
        
        result = true;
    }
    else
    {
        result = (B.count == 0);
    }
    
    *out_count = index;
    return result;
}

yd_b32
strings_match_part(String A, String B, yd_umm* out_count)
{
    yd_b32 result = (A.count >= B.count);
    yd_umm index = 0;
    
    if (result)
    {
        for (; index < B.count; ++index)
        {
            if (A.data[index] != B.data[index])
            
            {
                result = false;
                break;
            }
        }
    }
    
    *out_count = index;
    return result;
}

yd_b32
strings_matchInsensitive(const char* A, const char* B)
{
    yd_b32 result = (A == B);
    
    if (A && B)
    {
        while (*A && *B && (to_lower(*A) == to_lower(*B)))
        {
            ++A;
            ++B;
        }
        
        result = ((*A == 0) && (*B == 0));
    }
    
    return result;
}

yd_b32
strings_matchInsensitive(String A, const char* B)
{
    yd_b32 result = false;
    
    if (B)
    {
        const char* at = B;
        
        for (yd_umm index = 0; index < A.count; ++index, ++at)
        {
            if ((*at == 0) || (to_lower(A.data[index]) != to_lower(*at)))
            
            {
                return false;
            }
        }
        
        result = (*at == 0);
    }
    else
    {
        result = (A.count == 0);
    }
    
    return result;
}

yd_b32
strings_matchInsensitive(String A, String B)
{
    yd_b32 result = (A.count == B.count);
    
    if (result)
    {
        for (yd_umm index = 0; index < A.count; ++index)
        {
            if (to_lower(A.data[index]) != to_lower(B.data[index]))
            {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

yd_b32
strings_match_part_insensitive(const char* A, const char* B, yd_umm* out_count)
{
    yd_b32 result = (*A == *B);
    yd_umm match_count = 0;
    
    if (*A && *B)
    {
        while (*B && (to_lower(*A) == to_lower(*B)))
        {
            ++A;
            ++B;
            ++match_count;
        }
        
        result = (*B == 0);
    }
    
    *out_count = match_count;
    return result;
}

yd_b32
strings_match_part_insensitive(String A, const char* B, yd_umm* out_count)
{
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (B)
    {
        for (; B[index]; ++index)
        {
            if ((index == A.count) ||
                (to_lower(A.data[index]) != to_lower(B[index])))
            {
                return false;
            }
        }
        
        result = true;
    }
    else
    {
        result = (A.count == 0);
    }
    
    *out_count = index;
    return result;
}
yd_b32
strings_match_part_insensitive(const char* A, String B, yd_umm* out_count)
{
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (A)
    {
        for (; index < B.count; ++index)
        {
            if (to_lower(A[index]) != to_lower(B.data[index]))
            {
                return false;
            }
        }
        
        result = true;
    }
    else
    {
        result = (B.count == 0);
    }
    
    *out_count = index;
    return result;
}

yd_b32
strings_match_part_insensitive(String A, String B, yd_umm* out_count)
{
    yd_b32 result = (A.count >= B.count);
    yd_umm index = 0;
    
    if (result)
    {
        for (; index < B.count; ++index)
        {
            if (to_lower(A.data[index]) != to_lower(B.data[index]))
            {
                result = false;
                break;
            }
        }
    }
    
    *out_count = index;
    return result;
}

yd_b32
StringSetMatch(void* StrSet, yd_umm ItemSize, yd_umm count,
               String str, yd_umm* OutMatchindex)
{
    yd_b32 result = false;
    yd_u8* at = (yd_u8*)StrSet;
    
    for (yd_umm index = 0; index < count; ++index, at += ItemSize)
    {
        if (strings_match(*((String*)at), str))
        {
            *OutMatchindex = index;
            result = true;
            break;
        }
    }
    
    return result;
}

yd_s32
Compare(const char* A, const char* B)
{
    yd_umm index = 0;
    while (A[index] && B[index] &&
           (A[index] != B[index]))
    {
        ++index;
    }
    
    yd_s32 result = (A[index] > B[index]) - (A[index] < B[index]);
    return result;
}

yd_s32
Compare(String A, const char* B)
{
    yd_umm index = 0;
    while ((index < A.count) && B[index] &&
           (A.data[index] == B[index]))
    {
        ++index;
    }
    
    yd_s32 result = 0;
    
    if (index < A.count)
    {
        result = (A.data[index] > B[index]) - (A.data[index] < B[index]);
    }
    else
    {
        if (B[index])
        {
            result = 0;
        } else
        {
            result = -1;
        }
    }
    
    return result;
}

yd_s32
Compare(String A, String B)
{
    yd_umm Mincount = A.count;
    if (B.count < Mincount)
    {
        Mincount = B.count;
    }
    
    yd_umm index = 0;
    while ((index < Mincount) && (A.data[index] == B.data[index]))
    {
        ++index;
    }
    
    yd_s32 result = 0;
    if (index < Mincount)
    {
        result = (A.data[index] > B.data[index]) - (A.data[index] < B.data[index]);
    }
    else
    {
        result = (A.count > B.count) - (A.count < B.count);
    }
    
    return result;
}

//
// NOTE(yuval): Functions For Finding Characters And SubStrings
//

yd_umm
Find(const char* str, char character, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    for (yd_umm index = Start; str[index]; ++index)
    {
        if (str[index] == Character)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
Find(String str, char character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    for (yd_umm index = Start; index < str.count; ++index)
    {
        if (str.data[index] == Character)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
Find(const char* str, String Seek, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    if (Seek.count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = Start; str[index]; ++index)
    {
        yd_b32 Hit = true;
        yd_umm Strindex = index;
        
        for (yd_umm Seekindex = 0;
             Seekindex < Seek.count;
             ++Seekindex, ++Strindex)
        {
            if (!(str[Strindex])) {
                return STRING_NOT_FOUND;
            }
            
            if (str[Strindex] != Seek.data[Seekindex])
            {
                Hit = false;
                break;
            }
        }
        
        if (Hit)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
Find(String str, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    if (Seek.count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    yd_umm Stopat = str.count - Seek.count + 1;
    
    for (yd_umm index = Start; index < Stopat; ++index)
    {
        yd_b32 Hit = true;
        yd_umm Strindex = index;
        
        for (yd_umm Seekindex = 0;
             Seekindex < Seek.count;
             ++Seekindex, ++Strindex)
        {
            if (str.data[Strindex] != Seek.data[Seekindex])
            {
                Hit = false;
                break;
            }
        }
        
        if (Hit)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
RFind(const char* str, yd_umm count, char character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < count));
    
    yd_umm index = Start + 1;
    while (index--)
    {
        if (str[index] == Character)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
RFind(String str, char character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    yd_umm index = Start + 1;
    while (index--)
    {
        if (str.data[index] == Character)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
RFind(const char* str, yd_umm count, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < count));
    
    if (count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    // TODO(yuval): This will overflow if Seek's count is Bigger then str's count
    // An additional check is needed
    if (Start + Seek.count > count)
    {
        Start = count - Seek.count;
    }
    
    yd_umm index = Start + 1;
    while (index--)
    {
        yd_b32 Hit = true;
        yd_umm Strindex = index;
        
        for (yd_umm Seekindex = 0;
             Seekindex < Seek.count;
             ++Seekindex, ++Strindex)
        {
            if (str[Strindex] != Seek.data[Seekindex])
            {
                Hit = false;
                break;
            }
        }
        
        if (Hit)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
RFind(String str, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    if (Seek.count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    // TODO(yuval): This will overflow if Seek's count is Bigger then str's count
    // An additional check is needed
    if (Start + Seek.count > str.count)
    {
        Start = str.count - Seek.count;
    }
    
    yd_umm index = Start + 1;
    while (index--)
    {
        yd_b32 Hit = true;
        yd_umm Strindex = index;
        
        for (yd_umm Seekindex = 0;
             Seekindex < Seek.count;
             ++Seekindex, ++Strindex)
        {
            if (str.data[Strindex] != Seek.data[Seekindex])
            {
                Hit = false;
                break;
            }
        }
        
        if (Hit)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindFirstOf(const char* str, const char* Characters, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    if (!(*Characters))
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = Start; str[index]; ++index)
    {
        for (const char* at = Characters; *at; ++at)
        {
            if (str[index] == *at)
            {
                return index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindFirstOf(String str, const char* Characters, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    if (!(*Characters))
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = Start; index < str.count; ++index)
    {
        for (const char* at = Characters; *at; ++at)
        {
            if (str.data[index] == *at)
            {
                return index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindInsensitive(const char* str, char character, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    for (yd_umm index = Start; str[index]; ++index)
    {
        if (to_lower(str[index]) == to_lower(Character))
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindInsensitive(String str, char character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    for (yd_umm index = Start; index < str.count; ++index)
    {
        if (to_lower(str.data[index]) == to_lower(Character))
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindInsensitive(const char* str, String Seek, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    if (Seek.count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = Start; str[index]; ++index)
    {
        yd_b32 Hit = true;
        yd_umm Strindex = index;
        
        for (yd_umm Seekindex = 0;
             Seekindex < Seek.count;
             ++Seekindex, ++Strindex)
        {
            if (to_lower(str[Strindex]) != to_lower(Seek.data[Seekindex]))
            {
                Hit = false;
                break;
            }
        }
        
        if (Hit)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindInsensitive(String str, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < str.count));
    
    if (Seek.count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    yd_umm Stopat = str.count - Seek.count + 1;
    
    for (yd_umm index = Start; index < Stopat; ++index)
    {
        yd_b32 Hit = true;
        yd_umm Strindex = index;
        
        for (yd_umm Seekindex = 0;
             Seekindex < Seek.count;
             ++Seekindex, ++Strindex)
        {
            if (to_lower(str.data[Strindex]) != to_lower(Seek.data[Seekindex]))
            {
                Hit = false;
                break;
            }
        }
        
        if (Hit)
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

String
GetFirstDoubleLine(String Source)
{
    String result = {};
    
    yd_umm Pos = Find(Source, MakeLitString("\n\n"));
    if (Pos == STRING_NOT_FOUND)
    {
        Pos = Find(Source, MakeLitString("\r\n\r\n"));
    }
    
    if (Pos != STRING_NOT_FOUND)
    {
        result = substr(Source, 0, Pos);
    }
    
    return result;
}

String
GetNextDoubleLine(String Source, String Line)
{
    String result = {};
    
    yd_umm LineEndindex = (yd_umm)(Line.data - Source.data) + Line.count;
    YDAssert((Source.data[LineEndindex] == '\n') || (Source.data[LineEndindex] == '\r'));
    
    ++LineEndindex;
    YDAssert((Source.data[LineEndindex] == '\n') || (Source.data[LineEndindex] == '\r'));
    
    yd_umm Start = LineEndindex + 1;
    
    if (Start < Source.count)
    {
        yd_umm Pos = Find(Source, MakeLitString("\n\n"), Start);
        if (Pos == STRING_NOT_FOUND)
        {
            Pos = Find(Source, MakeLitString("\r\n\r\n"), Start);
        }
        
        if (Pos != STRING_NOT_FOUND)
        {
            result = substr(Source, Start, Pos - Start);
        }
    }
    
    return result;
}

String
GetNextWord(String Source, String PrevWord)
{
    String result = {};
    yd_umm Pos0 = (yd_umm)(PrevWord.data - Source.data) + PrevWord.count;
    
    for (; Pos0 < Source.count; ++Pos0)
    {
        char c = Source.data[Pos0];
        if (!(is_whitespace(c) || c == '(' || c == ')'))
        {
            break;
        }
    }
    
    if (Pos0 < Source.count)
    {
        yd_umm Pos1 = Pos0;
        
        for (; Pos1 < Source.count; ++Pos1)
        {
            char c = Source.data[Pos1];
            if (is_whitespace(c) || c == '(' || c == ')')
            {
                break;
            }
        }
        
        result = substr(Source, Pos0, Pos1 - Pos0);
    }
    
    return result;
}

//
// NOTE(yuval): String Copy And Append Functions
//

yd_umm
CopyFastUnsafe(char* dest, const char* Source)
{
    char* destat = dest;
    const char* Sourceat = Source;
    
    while (*Sourceat)
    {
        *destat++ = *Sourceat++;
    }
    
    *destat = 0;
    
    yd_umm result = (destat - dest);
    return result;
}

yd_umm
CopyFastUnsafe(char* dest, String Source)
{
    for (yd_umm index = 0; index < Source.count; ++index)
    {
        dest[index] = Source.data[index];
    }
    
    dest[Source.count] = 0;
    return Source.count;
}

yd_b32
CopyChecked(String* dest, String Source)
{
    if (dest->capacity < Source.count)
    {
        return false;
    }
    
    for (yd_umm index = 0; index < Source.count; ++index)
    {
        dest->data[index] = Source.data[index];
    }
    
    dest->count = Source.count;
    return true;
}

yd_b32
CopyChecked(char* dest, yd_umm destCap, String Source)
{
    if (destCap < Source.count + 1)
    {
        return false;
    }
    
    for (yd_umm index = 0; index < Source.count; ++index)
    {
        dest[index] = Source.data[index];
    }
    
    dest[Source.count] = 0;
    return true;
}

yd_b32
CopyPartial(String* dest, const char* Source)
{
    yd_b32 result = true;
    yd_umm index = 0;
    
    for (; Source[index]; ++index)
    {
        if (index >= dest->capacity)
        {
            result = false;
            break;
        }
        
        dest->data[index] = Source[index];
    }
    
    dest->count = index;
    return result;
}

yd_b32
CopyPartial(String* dest, String Source)
{
    yd_b32 result = true;
    yd_umm index = 0;
    
    for (; index < Source.count; ++index)
    {
        if (index >= dest->capacity)
        {
            result = false;
            break;
        }
        
        dest->data[index] = Source.data[index];
    }
    
    dest->count = index;
    return result;
}

yd_b32
CopyPartial(char* dest, yd_umm destCap, String Source)
{
    yd_b32 result = true;
    yd_umm index = 0;
    
    for (; index < Source.count; ++index)
    {
        if (index >= destCap - 1)
        {
            result = false;
            break;
        }
        
        dest[index] = Source.data[index];
    }
    
    dest[index] = 0;
    return result;
}

yd_b32
AppendChecked(String* dest, String Source)
{
    String End = TailStr(*dest);
    yd_b32 result = CopyChecked(&End, Source);
    dest->count += End.count;
    return result;
}

yd_b32
AppendPartial(String* dest, const char* Source)
{
    String End = TailStr(*dest);
    yd_b32 result = CopyPartial(&End, Source);
    dest->count += End.count;
    return result;
}

yd_b32
AppendPartial(String* dest, String Source)
{
    String End = TailStr(*dest);
    yd_b32 result = CopyPartial(&End, Source);
    dest->count += End.count;
    return result;
}

yd_b32
Append(String* dest, char c)
{
    yd_b32 result = false;
    
    if (dest->count < dest->capacity)
    {
        dest->data[dest->count++] = c;
        result = true;
    }
    
    return result;
}

yd_b32
TerminateWithNull(String* str)
{
    yd_b32 result = false;
    
    if (str->count < str->capacity)
    {
        str->data[str->count] = 0;
        result = true;
    }
    
    return result;
}

yd_b32
AppendPadding(String* dest, char c, yd_umm Targetcount)
{
    yd_b32 result = true;
    
    for (yd_umm count = dest->count; count < Targetcount; ++count)
    {
        if (!Append(dest, c))
        {
            result = false;
            break;
        }
    }
    
    return result;
}

//
// NOTE(yuval): String Concatenation
//

yd_b32
ConcatStrings(String* dest, String SourceA, String SourceB)
{
    yd_b32 CanFitConcat = (dest->capacity >= SourceA.count + SourceB.count);
    
    if (CanFitConcat)
    {
        yd_umm dest_index = 0;
        
        for (yd_umm Sourceindex = 0;
             Sourceindex < SourceA.count;
             ++Sourceindex, ++dest_index)
        {
            dest->data[dest_index] = SourceA.data[Sourceindex];
        }
        
        for (yd_umm Sourceindex = 0;
             Sourceindex < SourceB.count;
             ++Sourceindex, ++dest_index)
        {
            dest->data[dest_index] = SourceB.data[Sourceindex];
        }
        
        dest->count = SourceA.count + SourceB.count;
    }
    
    return CanFitConcat;
}

//
// NOTE(yuval): Other String Editing Functions
//

void
ReplaceRange(String* str, yd_umm First, yd_umm OnePastLast, char With)
{
    YDAssert((First >= 0) && (First < str->count));
    YDAssert((OnePastLast > 0) && (OnePastLast <= str->count));
    YDAssert(First < OnePastLast);
    
    for (yd_umm index = First; index < OnePastLast; ++index)
    {
        str->data[index] = With;
    }
}

void
ReplaceRange(String* str, yd_umm First, yd_umm OnePastLast, const char* With)
{
    String WithStr = MakeStringSlowly(With);
    ReplaceRange(str, First, OnePastLast, WithStr);
}

// TODO(yuval): Maybe rename to block_copy?
yd_internal void
YDStringBlockMove(void* dest_init, const void* Source_init, yd_umm size)
{
    if (dest_init && Source_init)
    {
        const yd_u8* Source = (const yd_u8*)Source_init;
        yd_u8* dest = (yd_u8*)dest_init;
        
        if (dest < Source)
        {
            while (size--)
            {
                *dest++ = *Source++;
            }
        } else if (dest > Source)
        {
            Source += size - 1;
            dest += size - 1;
            
            while (size--)
            {
                *dest-- = *Source--;
            }
        }
    }
}

void
ReplaceRange(String* str, yd_umm First, yd_umm OnePastLast, String With)
{
    YDAssert((First >= 0) && (First < str->count));
    YDAssert((OnePastLast > 0) && (OnePastLast <= str->count));
    YDAssert(First < OnePastLast);
    
    yd_s64 shift = With.count - (OnePastLast - First);
    yd_umm new_count = str->count + shift;
    
    if (new_count <= str->capacity)
    {
        if (shift != 0)
        {
            char* tail = str->data + OnePastLast;
            char* new_tail_Pos = tail + shift;
            // TODO(yuval): Verify that this has no bugs!!!!!!!!!
            YDStringBlockMove(new_tail_Pos, tail, str->count - OnePastLast);
        }
        
        YDStringBlockMove(str->data + First, With.data, With.count);
        str->count += shift;
    }
}

void
Replace(String* str, char ToReplace, char With)
{
    for (yd_umm index = 0; index < str->count; ++index)
    {
        if (str->data[index] == ToReplace)
        {
            str->data[index] = With;
        }
    }
}

void
Replace(String* str, const char* ToReplace, const char* With)
{
    Replace(str, MakeStringSlowly(ToReplace), MakeStringSlowly(With));
}

void
Replace(String* str, const char* ToReplace, String With)
{
    Replace(str, MakeStringSlowly(ToReplace), With);
}

void
Replace(String* str, String ToReplace, const char* With)
{
    Replace(str, ToReplace, MakeStringSlowly(With));
}

void
Replace(String* str, String ToReplace, String With)
{
    yd_umm index = 0;
    
    for (;;)
    {
        index = Find(*str, ToReplace, index);
        if (index == STRING_NOT_FOUND)
        {
            break;
        }
        
        ReplaceRange(str, index, index + ToReplace.count, With);
        index += With.count;
    }
}

void
StringInterpretEscapes(char* dest, String Source)
{
    yd_s32 Mode = 0;
    yd_umm dest_index = 0;
    
    for (yd_umm Sourceindex = 0; Sourceindex < Source.count; ++Sourceindex)
    {
        switch (Mode)
        {
            case 0:
            {
                if (Source.data[Sourceindex] == '\\')
                {
                    Mode = 1;
                } else
                {
                    dest[dest_index++] = Source.data[Sourceindex];
                }
            } break;
            
            case 1:
            {
                char c = Source.data[Sourceindex];
                switch (c)
                {
                    case '\\': { dest[dest_index++] = '\\'; } break;
                    case 'n': { dest[dest_index++] = '\n'; } break;
                    case 't': { dest[dest_index++] = '\t'; } break;
                    case '"': { dest[dest_index++] = '"'; } break;
                    case '0': { dest[dest_index++] = '\0'; } break;
                    default: { dest[dest_index++] = '\\'; dest[dest_index++] = c; } break;
                }
                
                Mode = 0;
            } break;
        }
    }
    
    dest[dest_index] = 0;
}

//
// NOTE(yuval): String Arena Allocation Functions
//

// TODO(yuval): This functions should all be inline!

#if defined(YD_MEMORY)
char*
PushZ(memory_arena* Arena, yd_umm count, arena_push_params Params)
{
    char* result = PushArray(Arena, char, count + 1, Params);
    
    if (result)
    {
        result[count] = 0;
    }
    
    return result;
}

String
PushString(memory_arena* Arena, yd_umm capacity, arena_push_params Params)
{
    String result = {};
    result.data = PushArray(Arena, char, capacity, Params);
    result.count = 0;
    
    if (result.data)
    {
        result.capacity = capacity;
    }
    
    return result;
}

char*
PushCopyZ(memory_arena* Arena, const char* Source, arena_push_params Params)
{
    yd_umm Size = StringLength(Source);
    char* result = (char*)PushCopy(Arena, Source, Size + 1, Params);
    
    return result;
}

String
PushCopyString(memory_arena* Arena, const char* Source, arena_push_params Params)
{
    String result = {};
    
    yd_umm Size = StringLength(Source);
    result.data = (char*)PushCopy(Arena, Source, Size, Params);
    
    if (result.data)
    {
        result.count = Size;
        result.capacity = Size;
    }
    
    return result;
}

char*
PushCopyZ(memory_arena* Arena, String Source, arena_push_params Params)
{
    char* result = PushArray(Arena, char, Source.count + 1, Params);
    
    if (result)
    {
        CopyArray(result, Source.data, Source.count);
        result[Source.count] = 0;
    }
    
    return result;
}


String
PushCopyString(memory_arena* Arena, String Source, arena_push_params Params)
{
    String result = {};
    result.data = (char*)PushCopy(Arena, Source.data, Source.count, Params);
    
    if (result.data)
    {
        result.count = Source.count;
        result.capacity = Source.count;
    }
    
    return result;
}
#endif // #if defined(YD_MEMORY)

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

void
to_lower(char* str)
{
    for (char* at = str; *at; ++at)
    {
        *at = to_lower(*at);
    }
}

void
to_lower(String* str)
{
    for (yd_umm index = 0; index < str->count; ++index)
    {
        str->data[index] = to_lower(str->data[index]);
    }
}

void
to_lower(char* dest, const char* Source)
{
    const char* Sourceat = Source;
    char* destat = dest;
    
    while (*Sourceat)
    {
        *destat++ = to_lower(*Sourceat++);
    }
    
    *destat = 0;
}

void
to_lower(String* dest, const char* Source)
{
    yd_umm index = 0;
    
    for (; Source[index]; ++index)
    {
        dest->data[index] = to_lower(Source[index]);
    }
    
    dest->count = index;
}

void
to_lower(char* dest, String Source)
{
    for (yd_umm index = 0; index < Source.count; ++index)
    {
        dest[index] = to_lower(Source.data[index]);
    }
    
    dest[Source.count] = 0;
}

void
to_lower(String* dest, String Source)
{
    if (dest->capacity >= Source.count)
    {
        for (yd_umm index = 0; index < Source.count; ++index)
        {
            dest->data[index] = to_lower(Source.data[index]);
        }
        
        dest->count = Source.count;
    }
}

void
to_upper(char* str)
{
    for (char* at = str; *at; ++at)
    {
        *at = to_upper(*at);
    }
}

void
to_upper(String* str)
{
    for (yd_umm index = 0; index < str->count; ++index)
    {
        str->data[index] = to_upper(str->data[index]);
    }
}

void
to_upper(char* dest, const char* Source)
{
    const char* Sourceat = Source;
    char* destat = dest;
    
    while (*Sourceat)
    {
        *destat++ = to_upper(*Sourceat++);
    }
    
    *destat = 0;
}

void
to_upper(String* dest, const char* Source)
{
    yd_umm index = 0;
    
    for (; Source[index]; ++index)
    {
        dest->data[index] = to_upper(Source[index]);
    }
    
    dest->count = index;
}

void
to_upper(char* dest, String Source)
{
    for (yd_umm index = 0; index < Source.count; ++index)
    {
        dest[index] = to_upper(Source.data[index]);
    }
    
    dest[Source.count] = 0;
}

void
to_upper(String* dest, String Source)
{
    if (dest->capacity >= Source.count)
    {
        for (yd_umm index = 0; index < Source.count; ++index)
        {
            dest->data[index] = to_upper(Source.data[index]);
        }
        
        dest->count = Source.count;
    }
}

void
ToCamel(char* str)
{
    yd_b32 IsFirst = true;
    
    for (char* at = str; *at; ++at)
    {
        if (is_alpha_numeric_true(*at))
        {
            if (IsFirst)
            {
                *at = to_upper(*at);
                IsFirst = false;
            } else
            {
                *at = to_lower(*at);
            }
        } else
        {
            IsFirst = true;
        }
    }
}

void
ToCamel(String* str)
{
    yd_b32 IsFirst = true;
    
    for (yd_umm index = 0; index < str->count; ++index)
    {
        if (is_alpha_numeric_true(str->data[index]))
        {
            if (IsFirst)
            {
                str->data[index] = to_upper(str->data[index]);
                IsFirst = false;
            } else
            {
                str->data[index] = to_lower(str->data[index]);
            }
        } else
        {
            IsFirst = true;
        }
    }
}

void
ToCamel(char* dest, const char* Source)
{
    const char* Sourceat = Source;
    char* destat = dest;
    yd_b32 IsFirst = false;
    
    for (; *Sourceat; ++Sourceat, ++destat)
    {
        char c = *Sourceat;
        
        if (is_alpha_numeric_true(c))
        {
            if (IsFirst)
            {
                c = to_upper(c);
                IsFirst = false;
            } else
            {
                c = to_lower(c);
            }
        } else
        {
            IsFirst = true;
        }
        
        *destat = c;
    }
    
    *destat = 0;
}

void
ToCamel(String* dest, const char* Source)
{
    yd_umm index = 0;
    yd_b32 IsFirst = true;
    
    for (; Source[index]; ++index)
    {
        char c = Source[index];
        
        if (is_alpha_numeric_true(c))
        {
            if (IsFirst)
            {
                c = to_upper(c);
                IsFirst = false;
            }
            else
            {
                c = to_lower(c);
            }
        }
        else
        {
            IsFirst = true;
        }
        
        dest->data[index] = c;
    }
    
    dest->count = index;
}

void
ToCamel(char* dest, String Source)
{
    yd_b32 IsFirst = true;
    
    for (yd_umm index = 0; index < Source.count; ++index)
    {
        char c = Source.data[index];
        
        if (is_alpha_numeric_true(c))
        {
            if (IsFirst)
            {
                c = to_upper(c);
                IsFirst = false;
            }
            else
            {
                c = to_lower(c);
            }
        }
        else
        {
            IsFirst = true;
        }
        
        dest[index] = c;
    }
    
    dest[Source.count] = 0;
}

void
ToCamel(String* dest, String Source)
{
    if (dest->capacity >= Source.count)
    {
        yd_b32 IsFirst = true;
        
        for (yd_umm index = 0; index < Source.count; ++index)
        {
            char c = Source.data[index];
            
            if (is_alpha_numeric_true(c))
            {
                if (IsFirst)
                {
                    c = to_upper(c);
                    IsFirst = false;
                } else
                {
                    c = to_lower(c);
                }
            } else
            {
                IsFirst = true;
            }
            
            dest->data[index] = c;
        }
        
        dest->count = Source.count;
    }
}

yd_umm
U64ToStringcount(yd_u64 value)
{
    yd_umm count = 1;
    
    value /= 10;
    while (value != 0)
    {
        ++count;
        value /= 10;
    }
    
    return count;
}

yd_b32
U64ToString(String* dest, yd_u64 value)
{
    yd_b32 result = false;
    yd_umm count = 0;
    
    do
    {
        if (count == dest->capacity)
        {
            result = false;
            break;
        }
        
        // TODO(yuval): Replace Hard-Coded Base
        yd_u64 Digit = value % 10;
        char DigitChar = (char)('0' + Digit);
        dest->data[count++] = DigitChar;
        value /= 10;
    }
    while (value != 0);
    
    if (result)
    {
        for (yd_umm Startindex = 0, Endindex = count - 1;
             Startindex < Endindex;
             ++Startindex, --Endindex)
        {
            char Temp = dest->data[Endindex];
            dest->data[Endindex] = dest->data[Startindex];
            dest->data[Startindex] = Temp;
        }
        
        dest->count = count;
    }
    else
    {
        dest->count = 0;
    }
    
    return result;
}

yd_b32
AppendU64ToString(String* dest, yd_u64 value)
{
    String Tail = TailStr(*dest);
    yd_b32 result = U64ToString(&Tail, value);
    
    if (result)
    {
        dest->count += Tail.count;
    }
    
    return result;
}


yd_umm
S32ToStringcount(yd_s32 value)
{
    yd_umm count = 1;
    
    if (value < 0)
    {
        count = 2;
    }
    
    value /= 10;
    while (value != 0)
    {
        ++count;
        value /= 10;
    }
    
    return value;
}

yd_b32
S32ToString(String* dest, yd_s32 value)
{
    yd_b32 result = true;
    
    dest->count = 0;
    
    if (value < 0)
    {
        if (dest->capacity > 0)
        {
            dest->data[dest->count++] = '-';
            value = -value;
        }
        else
        {
            result = false;
        }
    }
    
    if (result)
    {
        result = AppendU64ToString(dest, (yd_u64)value);
    }
    
    return result;
}

yd_b32
AppendS32ToString(String* dest, yd_s32 value)
{
    String Tail = TailStr(*dest);
    yd_b32 result = S32ToString(&Tail, value);
    
    if (result)
    {
        dest->count += Tail.count;
    }
    
    return result;
}

yd_umm
F32ToStringcount(yd_f32 value, yd_u32 MaxPrecision)
{
    yd_umm count = 0;
    
    if (value < 0)
    {
        count = 1;
        value = -value;
    }
    
    yd_u64 IntegerPart = (yd_u64)value;
    count += U64ToStringcount(IntegerPart);
    
    value -= IntegerPart;
    
    for (yd_u32 Precisionindex = 0;
         Precisionindex < MaxPrecision;
         ++Precisionindex)
    {
        if (value == 0.0f)
        {
            break;
        }
        
        value *= 10.0f;
        
        yd_u32 Integer = (yd_u32)value;
        value -= Integer;
        
        ++count;
    }
    
    return count;
}

yd_b32
F32ToString(String* dest, yd_f32 value, yd_u32 MaxPrecision)
{
    yd_s32 IntegerPart = (yd_s32)value;
    yd_b32 result = S32ToString(dest, IntegerPart);
    
    if (result)
    {
        value -= IntegerPart;
        
        if (value != 0.0f)
        {
            Append(dest, '.');
            
            for (yd_u32 Precisionindex = 0;
                 Precisionindex < MaxPrecision;
                 ++Precisionindex)
            {
                if (value == 0.0f)
                {
                    break;
                }
                
                value *= 10.0f;
                
                yd_u64 Integer = (yd_u64)value;
                result = AppendU64ToString(dest, Integer);
                
                if (!result)
                {
                    break;
                }
                
                value -= Integer;
            }
        }
    }
    
    return result;
}

yd_b32
AppendF32ToString(String* dest, yd_f32 value, yd_u32 MaxPrecision)
{
    String Tail = TailStr(*dest);
    yd_b32 result = F32ToString(&Tail, value, MaxPrecision);
    
    if (result)
    {
        dest->count += Tail.count;
    }
    
    return result;
}

yd_u32
HexStringToU32(String str)
{
    yd_u32 result = 0;
    
    for (yd_umm index = 0; index < str.count; ++index)
    {
        result += HexCharToS32(str.data[index]);
        result *= 0x10;
    }
    
    return result;
}

yd_b32
ColorToHexString(String* dest, yd_u32 Color)
{
    // TODO(yuval): Implement This
    return false;
}

yd_b32
HexStringToColor(yd_u32* dest, String str)
{
    // TODO(yuval): Implement This
    return false;
}

//
// NOTE(yuval): File / directory Strings Management Functions
//

yd_umm
reverse_seek_slash(String str, yd_umm ShiftFromLastChar)
{
    yd_umm index = str.count - ShiftFromLastChar - 1;
    while (index--)
    {
        if (IsSlash(str.data[index]))
        {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
reverse_seek_slash(String str)
{
    yd_umm result = reverse_seek_slash(str, 0);
    return result;
}

#if 0
yd_b32
SetLastFolder(char* dir, yd_umm count, const char* FolderName, char Slash)
{
    yd_b32 result = false;
    yd_umm LastSlashindex = reverse_seek_slash(dir, count);
    
    if (LastSlashindex != STRING_NOT_FOUND)
    {
        yd_umm Newcount = LastSlashindex + 1;
        Newcount = Append(dir, count, FolderName));
        {
            if (Append())
        }
    }
}
#endif // #if 0

yd_b32
SetLastFolder(String* dir, const char* FolderName, char Slash)
{
    yd_b32 result = false;
    yd_umm LastSlashindex = reverse_seek_slash(*dir);
    
    if (LastSlashindex != STRING_NOT_FOUND)
    {
        yd_umm count = LastSlashindex + 1;
        dir->count = count;
        
        if (Append(dir, FolderName))
        {
            if (Append(dir, Slash))
            {
                result = true;
            }
        }
        
        if (!result)
        {
            dir->count = count;
        }
    }
    
    return result;
}

#if 0
yd_b32
SetLastFolder(char* dir, yd_umm count, String FolderName, char Slash)
{
    
}
#endif // #if 0

yd_b32
SetLastFolder(String* dir, String FolderName, char Slash)
{
    yd_b32 result = false;
    yd_umm LastSlashindex = reverse_seek_slash(*dir);
    
    if (LastSlashindex != STRING_NOT_FOUND)
    {
        yd_umm count = LastSlashindex + 1;
        dir->count = count;
        
        if (Append(dir, FolderName))
        {
            if (Append(dir, Slash))
            {
                result = true;
            }
        }
        
        if (!result)
        {
            dir->count = count;
        }
    }
    
    return result;
}

yd_b32
RemoveLastFolder(String* path)
{
    yd_b32 result = false;
    yd_umm LastSlashindex = reverse_seek_slash(*path, 1);
    
    if (LastSlashindex != STRING_NOT_FOUND)
    {
        result = true;
        path->count = LastSlashindex + 1;
    }
    
    return result;
}

String
Fileextension(String filename)
{
    String result = {};
    yd_umm Dotindex = RFind(filename, '.');
    
    if (Dotindex != STRING_NOT_FOUND)
    {
        result = MakeString(filename.data + Dotindex + 1,
                            filename.count - Dotindex - 1);
    }
    
    return result;
}

yd_b32
Setextension(String* filename, const char* extension)
{
    yd_b32 result = false;
    yd_umm LastDotindex = RFind(*filename, '.');
    
    if (LastDotindex != STRING_NOT_FOUND)
    {
        yd_umm count = LastDotindex;
        filename->count = count;
        
        if (Append(filename, extension))
        {
            result = true;
        }
        
        if (!result)
        {
            filename->count = count;
        }
    }
    
    return result;
}

yd_b32
Setextension(String* filename, String extension)
{
    yd_b32 result = false;
    yd_umm LastDotindex = RFind(*filename, '.');
    
    if (LastDotindex != STRING_NOT_FOUND)
    {
        yd_umm count = LastDotindex + 1;
        filename->count = count;
        
        if (Append(filename, extension))
        {
            result = true;
        }
        
        if (!result)
        {
            filename->count = count - 1;
        }
    }
    
    return result;
}

yd_b32
Removeextension(String* filename)
{
    yd_b32 result = false;
    yd_umm LastDotindex = RFind(*filename, '.');
    
    if (LastDotindex != STRING_NOT_FOUND)
    {
        result = true;
        filename->count = LastDotindex;
    }
    
    return result;
}

#endif // #if defined(YD_STRING_IMPLEMENTATION)

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
void to_lower(String* Dest, const char* source);
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
    
    return Result;
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
# define MAKE_FIXED_WIDTH_STRING(Str) (make_string((char*)(str), 0, sizeof(str)))
#endif // #if !defined(MAKE_FIXED_WIDTH_STRING)

//
// NOTE(yuval): Utility String Functions
//

yd_internal inline yd_b32
is_null_string(String str) {
    yd_b32 Result = ((str.data = NULL_STRING.Data) &&
                     (str.count == NULL_STRING.Count) &&
                     (str.capacity == NULL_STRING.capacity));
    
    return Result;
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
    
    return Result;
}

yd_internal inline String
Substr(String Str, yd_umm Start, yd_umm Count) {
    YDAssert((Start >= 0) && (Start <= Str.Count));
    
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
tailstr(String str)
{
    String result;
    result.data = str.data + str.count;
    result.count = 0;
    result.capacity = str.capacity - str.Count;
    
    return result;
}

yd_internal inline void
advance_string(String* value, yd_umm count)
{
    if (value->count >= count)
    {
        value->data += count;
        value->count -= count;
        value->capacity -= (count * sizeof(char));
    }
    else
    {
        value->data += value->count;
        value->count = 0;
        value->capacity = 0;
    }
}

//
// NOTE(yuval): String Comparison Functions
//

yd_internal inline yd_b32
StringsMatch(const char* A, String B)
{
    yd_b32 Result = StringsMatch(B, A);
    return Result;
}

yd_internal inline yd_b32
StringsMatch(const char* A, const char* B, yd_umm BCount)
{
    yd_b32 Result = StringsMatch(B, BCount, A);
    return Result;
}

yd_internal inline yd_b32
StringsMatch(const char* A, String B, yd_umm Count)
{
    yd_b32 Result = StringsMatch(B, A, Count);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPart(const char* A, const char* B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPart(String A, const char* B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPart(const char* A, String B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPart(String A, String B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPart(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchInsensitive(const char* A, String B)
{
    yd_b32 Result = StringsMatchInsensitive(B, A);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPartInsensitive(const char* A, const char* B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPartInsensitive(String A, const char* B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPartInsensitive(const char* A, String B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringsMatchPartInsensitive(String A, String B)
{
    yd_umm Ignored;
    yd_b32 Result = StringsMatchPartInsensitive(A, B, &Ignored);
    return Result;
}

yd_internal inline yd_b32
StringSetMatch(String* StrSet, yd_umm Count, String Str, yd_umm* OutMatchIndex)
{
    yd_b32 Result = StringSetMatch(StrSet, sizeof(String), Count,
                                   Str, OutMatchIndex);
    return Result;
}

yd_internal inline yd_s32
Compare(const char* A, String B)
{
    yd_s32 Result = -Compare(B, A);
    return Result;
}

//
// NOTE(yuval): Functions For Finding Characters And SubStrings
//

yd_internal inline yd_umm
Find(const char* Str, char Character)
{
    yd_umm Result = Find(Str, Character, 0);
    return Result;
}

yd_internal inline yd_umm
Find(String Str, char Character)
{
    yd_umm Result = Find(Str, Character, 0);
    return Result;
}

yd_internal inline yd_umm
Find(const char* Str, String Seek)
{
    yd_umm Result = Find(Str, Seek, 0);
    return Result;
}

yd_internal inline yd_umm
Find(String Str, String Seek)
{
    yd_umm Result = Find(Str, Seek, 0);
    return Result;
}

yd_internal inline yd_umm
RFind(const char* Str, yd_umm Count, char Character)
{
    yd_umm Result = RFind(Str, Count, Character, Count - 1);
    return Result;
}

yd_internal inline yd_umm
RFind(String Str, char Character)
{
    yd_umm Result = RFind(Str, Character, Str.Count - 1);
    return Result;
}

yd_internal inline yd_umm
RFind(const char* Str, yd_umm Count, String Seek)
{
    yd_umm Result = RFind(Str, Count, Seek, Count - 1);
    return Result;
}

yd_internal inline yd_umm
RFind(String Str, String Seek)
{
    yd_umm Result = RFind(Str, Seek, Str.Count - 1);
    return Result;
}

yd_internal inline yd_umm
FindFirstOf(const char* Str, const char* Characters)
{
    yd_umm Result = FindFirstOf(Str, Characters, 0);
    return Result;
}

yd_internal inline yd_umm
FindFirstOf(String Str, const char* Characters)
{
    yd_umm Result = FindFirstOf(Str, Characters, 0);
    return Result;
}

yd_internal inline yd_umm
FindInsensitive(const char* Str, char Character)
{
    yd_umm Result = FindInsensitive(Str, Character, 0);
    return Result;
}

yd_internal inline yd_umm
FindInsensitive(String Str, char Character)
{
    yd_umm Result = FindInsensitive(Str, Character, 0);
    return Result;
}

yd_internal inline yd_umm
FindInsensitive(const char* Str, String Seek)
{
    yd_umm Result = FindInsensitive(Str, Seek, 0);
    return Result;
}

yd_internal inline yd_umm
FindInsensitive(String Str, String Seek)
{
    yd_umm Result = FindInsensitive(Str, Seek, 0);
    return Result;
}

yd_internal inline yd_b32
HasSubstr(const char* Str, String Seek)
{
    yd_b32 Result = (Find(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

yd_internal inline yd_b32
HasSubstr(String Str, String Seek)
{
    yd_b32 Result = (Find(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

yd_internal inline yd_b32
HasSubstrInsensitive(const char* Str, String Seek)
{
    yd_b32 Result = (FindInsensitive(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

yd_internal inline yd_b32
HasSubstrInsensitive(String Str, String Seek)
{
    yd_b32 Result = (FindInsensitive(Str, Seek) != STRING_NOT_FOUND);
    return Result;
}

yd_internal inline String
GetFirstWord(String Source)
{
    String Start = MakeString(Source.Data, 0);
    String Result = GetNextWord(Source, Start);
    return Result;
}

//
// NOTE(yuval): String Copy And Append Functions
//

yd_internal inline yd_umm
Copy(char* Dest, const char* Source)
{
    yd_umm Result = CopyFastUnsafe(Dest, Source);
    return Result;
}

yd_internal inline yd_umm
Copy(char* Dest, String Source)
{
    yd_umm Result = CopyFastUnsafe(Dest, Source);
    return Result;
}

yd_internal inline void
Copy(String* Dest, String Source)
{
    CopyChecked(Dest, Source);
}

yd_internal inline void
Copy(String* Dest, const char* Source)
{
    CopyPartial(Dest, Source);
}

yd_internal inline yd_b32
Append(String* Dest, const char* Source)
{
    yd_b32 Result = AppendPartial(Dest, Source);
    return Result;
}

yd_internal inline yd_b32
Append(String* Dest, String Source)
{
    yd_b32 Result = AppendPartial(Dest, Source);
    return Result;
}

//
// NOTE(yuval): String Concatenation Functions
//

yd_internal inline yd_b32
ConcatStrings(char* Dest, yd_umm DestCount,
              const char* SourceA, yd_umm SourceACount,
              const char* SourceB, yd_umm SourceBCount)
{
    String DestString = MakeString(Dest, DestCount - 1);
    String SourceAString = MakeString((char*)SourceA, SourceACount);
    String SourceBString = MakeString((char*)SourceB, SourceBCount);
    
    yd_b32 Result = ConcatStrings(&DestString, SourceAString, SourceBString);
    Dest[DestString.Count] = 0;
    
    return Result;
}

yd_internal inline yd_b32
ConcatStrings(char* Dest, yd_umm DestCount,
              String SourceA, String SourceB)
{
    String DestString = MakeString(Dest, DestCount - 1);
    
    yd_b32 Result = ConcatStrings(&DestString, SourceA, SourceB);
    Dest[DestString.Count] = 0;
    
    return Result;
}

yd_internal inline yd_b32
ConcatStrings(String* Dest,
              const char* SourceA, yd_umm SourceACount,
              const char* SourceB, yd_umm SourceBCount)
{
    String SourceAString = MakeString((char*)SourceA, SourceACount);
    String SourceBString = MakeString((char*)SourceB, SourceBCount);
    
    yd_b32 Result = ConcatStrings(Dest, SourceAString, SourceBString);
    return Result;
}

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

yd_internal inline yd_b32
IsLower(char C)
{
    yd_b32 Result = ((C >= 'A') && (C <= 'z'));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsLower(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsLower(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsLowerUTF8(yd_u8 C)
{
    yd_b32 Result = (IsLower((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_b32 IsUpper(char C);

yd_internal inline char
ToLower(char C)
{
    char Result = C;
    
    if (IsUpper(Result))
    {
        Result += 'A' - 'A';
    }
    
    return Result;
}

yd_internal inline yd_b32
IsUpper(char C)
{
    yd_b32 Result = ((C >= 'A') && (C <= 'Z'));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsUpper(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsUpper(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsUpperUTF8(yd_u8 C)
{
    yd_b32 Result = (IsUpper((char)C) || (C >= 128));
    return Result;
}

yd_internal inline char
ToUpper(char C)
{
    char Result = C;
    
    if (IsLower(Result))
    {
        Result -= 'A' - 'A';
    }
    
    return Result;
}

yd_internal inline yd_b32
IsSpacing(char C)
{
    yd_b32 Result = ((C == ' ') || (C == '\t') ||
                     (C == '\v') || (C == '\f'));
    return Result;
}

yd_internal inline yd_b32
IsEndOfLine(char C)
{
    yd_b32 Result = ((C == '\n') || (C == '\r'));
    return Result;
}

yd_internal inline yd_b32
IsWhitespace(char C)
{
    yd_b32 Result = (IsSpacing(C) || IsEndOfLine(C));
    return Result;
}

yd_internal inline yd_b32
IsAlphaTrue(char C)
{
    yd_b32 Result = (IsLower(C) || IsUpper(C));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsAlphaTrue(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlphaTrue(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsAlphaTrueUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlphaTrue((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_b32
IsAlpha(char C)
{
    yd_b32 Result = (IsAlphaTrue(C) || (C == '_'));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsAlpha(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlpha(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsAlphaUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlpha((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_b32
IsNumeric(char C)
{
    yd_b32 Result = ((C >= '0') && (C <= '9'));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsNumeric(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsNumeric(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsNumericUTF8(yd_u8 C)
{
    yd_b32 Result = (IsNumeric((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_umm
F32ToStringCount(yd_f32 Value)
{
    yd_umm Result = F32ToStringCount(Value, 8);
    return Result;
}

yd_internal inline yd_b32
F32ToString(String* Dest, yd_f32 Value)
{
    yd_b32 Result = F32ToString(Dest, Value, 8);
    return Result;
}

yd_internal inline yd_b32
AppendF32ToString(String* Dest, yd_f32 Value)
{
    yd_b32 Result = AppendF32ToString(Dest, Value, 8);
    return Result;
}

yd_internal inline yd_b32
IsAlphaNumeric(char C)
{
    yd_b32 Result = (IsAlpha(C) || IsNumeric(C));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsAlphaNumeric(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlphaNumeric(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsAlphaNumericUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlphaNumeric((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_b32
IsAlphaNumericTrue(char C)
{
    yd_b32 Result = (IsAlphaTrue(C) || IsNumeric(C));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsAlphaNumericTrue(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsAlphaNumericTrue(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsAlphaNumericTrueUTF8(yd_u8 C)
{
    yd_b32 Result = (IsAlphaNumericTrue((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_b32
IsHex(char C)
{
    yd_b32 Result = (((C >= '0') && (C <= '9')) ||
                     ((C >= 'A') && (C <= 'F')) ||
                     ((C >= 'a') && (C <= 'f')));
    return Result;
}

yd_internal inline yd_b32
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

yd_internal inline yd_b32
IsHex(String Str)
{
    yd_b32 Result = true;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        if (!IsHex(Str.Data[Index]))
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_internal inline yd_b32
IsHexUTF8(yd_u8 C)
{
    yd_b32 Result = (IsHex((char)C) || (C >= 128));
    return Result;
}

yd_internal inline yd_s32
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

yd_internal inline char
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

//
// NOTE(yuval): File / Directory Strings Management Functions
//

yd_internal inline yd_b32
IsSlash(char C)
{
    yd_b32 Result = ((C == '/') || (C == '\\'));
    return Result;
}

yd_internal inline String
FrontOfDirectory(String Dir)
{
    String Result = Substr(Dir, ReverseSeekSlash(Dir) + 1);
    return Result;
}

yd_internal inline String
PathOfDirectory(String Dir)
{
    String Result = Substr(Dir, 0, ReverseSeekSlash(Dir) + 1);
    return Result;
}

yd_internal inline yd_b32
IsH(String Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "h") ||
                     StringsMatch(Extension, "hpp") ||
                     StringsMatch(Extension, "hin"));
    return Result;
}

yd_internal inline yd_b32
IsC(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "C");
    return Result;
}

yd_internal inline yd_b32
IsCPP(String Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "cpp") ||
                     StringsMatch(Extension, "cc") ||
                     StringsMatch(Extension, "cin"));
    return Result;
}

yd_internal inline yd_b32
IsObjectiveC(String Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "m") ||
                     StringsMatch(Extension, "mm"));
    return Result;
}

yd_internal inline yd_b32
IsShader(String Extension)
{
    yd_b32 Result = (StringsMatch(Extension, "ps") ||
                     StringsMatch(Extension, "vs") ||
                     StringsMatch(Extension, "cs") ||
                     StringsMatch(Extension, "ts") ||
                     StringsMatch(Extension, "gs"));
    return Result;
}

yd_internal inline yd_b32
IsINL(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "inl");
    return Result;
}

yd_internal inline yd_b32
IsJava(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "java");
    return Result;
}

yd_internal inline yd_b32
IsCSharp(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "cs");
    return Result;
}

yd_internal inline yd_b32
IsPython(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "py");
    return Result;
}

yd_internal inline yd_b32
IsSwift(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "swift");
    return Result;
}

yd_internal inline yd_b32
IsJavascript(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "js");
    return Result;
}

yd_internal inline yd_b32
IsBAT(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "bat");
    return Result;
}

yd_internal inline yd_b32
IsBash(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "sh");
    return Result;
}

yd_internal inline yd_b32
IsTXT(String Extension)
{
    yd_b32 Result = StringsMatch(Extension, "txt");
    return Result;
}

yd_internal inline yd_b32
IsCode(String Extension)
{
    yd_b32 Result = (IsH(Extension) || IsC(Extension) || IsCPP(Extension) ||
                     IsObjectiveC(Extension) || IsShader(Extension) ||
                     IsINL(Extension) || IsJava(Extension) || IsCSharp(Extension) ||
                     IsPython(Extension) || IsSwift(Extension) || IsJavascript(Extension) ||
                     IsBAT(Extension) || IsBash(Extension));
    return Result;
}

yd_internal inline yd_b32
IsDoc(String Extension)
{
    yd_b32 Result = IsTXT(Extension);
    return Result;
}

yd_internal inline yd_b32
IsCodeFile(String FileName)
{
    String Extension = FileExtension(FileName);
    yd_b32 Result = IsCode(Extension);
    return Result;
}

yd_internal inline yd_b32
IsDocFile(String FileName)
{
    String Extension = FileExtension(FileName);
    yd_b32 Result = IsDoc(Extension);
    return Result;
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
SkipWhitespace(String Str, yd_umm* OutSkipCount)
{
    yd_umm SkipCount = 0;
    for (; SkipCount < Str.Count && IsWhitespace(Str.Data[SkipCount]); ++SkipCount);
    
    *OutSkipCount = SkipCount;
    String Result = Substr(Str, SkipCount);
    
    return Result;
}

String
ChopWhitespace(String Str)
{
    yd_umm ChopIndex = Str.Count;
    for (; ChopIndex > 0 && IsWhitespace(Str.Data[ChopIndex - 1]); --ChopIndex);
    
    String Result = Substr(Str, 0, ChopIndex);
    return Result;
}

//
// NOTE(yuval): String Comparison Functions
//

yd_b32
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

yd_b32
StringsMatch(String A, const char* B)
{
    yd_b32 Result = false;
    
    if (B)
    
    {
        const char* At = B;
        
        for (yd_umm Index = 0;
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

yd_b32
StringsMatch(String A, String B)
{
    yd_b32 Result = (A.Count == B.Count);
    
    if (Result)
    {
        for (yd_umm Index = 0; Index < A.Count; ++Index)
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

yd_b32
StringsMatch(const char* A, yd_umm ACount, const char* B)
{
    yd_b32 Result = false;
    
    if (B)
    {
        const char* At = B;
        
        for (yd_umm Index = 0; Index < ACount; ++Index)
        {
            if ((*At == 0) || (A[Index] != *At))
            {
                return false;
            }
        }
        
        Result = (*At == 0);
    }
    else
    {
        Result = (ACount == 0);
    }
    
    return Result;
}

yd_b32
StringsMatch(const char* A, yd_umm ACount,
             const char* B, yd_umm BCount)
{
    b32 Result = (ACount == BCount);
    
    if (Result)
    {
        for (yd_umm Index = 0; Index < ACount; ++Index)
        {
            if (A[Index] != B[Index])
            {
                Result = false;
                break;
            }
        }
    }
    
    return Result;
}

yd_b32
StringsMatch(String A, const char* B, yd_umm Count)
{
    b32 Result = true;
    
    for (yd_umm Index = 0; Index < Count; ++Index)
    {
        if (A.Data[Index] != B[Index])
        {
            Result = false;
            break;
        }
    }
    
    return Result;
}

yd_b32
StringsMatchPart(const char* A, const char* B, yd_umm* out_count)
{
    yd_b32 Result = (*A == *B);
    yd_umm MatchCount = 0;
    
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
    
    *out_count = MatchCount;
    return Result;
}

yd_b32
StringsMatchPart(String A, const char* B, yd_umm* out_count)
{
    yd_b32 Result = false;
    yd_umm Index = 0;
    
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
    
    *out_count = Index;
    return Result;
}

yd_b32
StringsMatchPart(const char* A, String B, yd_umm* out_count)
{
    yd_b32 Result = false;
    yd_umm Index = 0;
    
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
    
    *out_count = Index;
    return Result;
}

yd_b32
StringsMatchPart(String A, String B, yd_umm* out_count)
{
    yd_b32 Result = (A.Count >= B.Count);
    yd_umm Index = 0;
    
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
    
    *out_count = Index;
    return Result;
}

yd_b32
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

yd_b32
StringsMatchInsensitive(String A, const char* B)
{
    yd_b32 Result = false;
    
    if (B)
    {
        const char* At = B;
        
        for (yd_umm Index = 0; Index < A.Count; ++Index, ++At)
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

yd_b32
StringsMatchInsensitive(String A, String B)
{
    yd_b32 Result = (A.Count == B.Count);
    
    if (Result)
    {
        for (yd_umm Index = 0; Index < A.Count; ++Index)
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

yd_b32
StringsMatchPartInsensitive(const char* A, const char* B, yd_umm* out_count)
{
    yd_b32 Result = (*A == *B);
    yd_umm MatchCount = 0;
    
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
    
    *out_count = MatchCount;
    return Result;
}

yd_b32
StringsMatchPartInsensitive(String A, const char* B, yd_umm* out_count)
{
    yd_b32 Result = false;
    yd_umm Index = 0;
    
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
    
    *out_count = Index;
    return Result;
}
yd_b32
StringsMatchPartInsensitive(const char* A, String B, yd_umm* out_count)
{
    yd_b32 Result = false;
    yd_umm Index = 0;
    
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
    
    *out_count = Index;
    return Result;
}

yd_b32
StringsMatchPartInsensitive(String A, String B, yd_umm* out_count)
{
    yd_b32 Result = (A.Count >= B.Count);
    yd_umm Index = 0;
    
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
    
    *out_count = Index;
    return Result;
}

yd_b32
StringSetMatch(void* StrSet, yd_umm ItemSize, yd_umm Count,
               String Str, yd_umm* OutMatchIndex)
{
    yd_b32 Result = false;
    yd_u8* At = (yd_u8*)StrSet;
    
    for (yd_umm Index = 0; Index < Count; ++Index, At += ItemSize)
    {
        if (StringsMatch(*((String*)At), Str))
        {
            *OutMatchIndex = Index;
            Result = true;
            break;
        }
    }
    
    return Result;
}

yd_s32
Compare(const char* A, const char* B)
{
    yd_umm Index = 0;
    while (A[Index] && B[Index] &&
           (A[Index] != B[Index]))
    {
        ++Index;
    }
    
    yd_s32 Result = (A[Index] > B[Index]) - (A[Index] < B[Index]);
    return Result;
}

yd_s32
Compare(String A, const char* B)
{
    yd_umm Index = 0;
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

yd_s32
Compare(String A, String B)
{
    yd_umm MinCount = A.Count;
    if (B.Count < MinCount)
    {
        MinCount = B.Count;
    }
    
    yd_umm Index = 0;
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
// NOTE(yuval): Functions For Finding Characters And SubStrings
//

yd_umm
Find(const char* Str, char Character, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    for (yd_umm Index = Start; Str[Index]; ++Index)
    {
        if (Str[Index] == Character)
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
Find(String Str, char Character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    for (yd_umm Index = Start; Index < Str.Count; ++Index)
    {
        if (Str.Data[Index] == Character)
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
Find(const char* Str, String Seek, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm Index = Start; Str[Index]; ++Index)
    {
        yd_b32 Hit = true;
        yd_umm StrIndex = Index;
        
        for (yd_umm SeekIndex = 0;
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

yd_umm
Find(String Str, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    yd_umm StopAt = Str.Count - Seek.Count + 1;
    
    for (yd_umm Index = Start; Index < StopAt; ++Index)
    {
        yd_b32 Hit = true;
        yd_umm StrIndex = Index;
        
        for (yd_umm SeekIndex = 0;
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

yd_umm
RFind(const char* Str, yd_umm Count, char Character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Count));
    
    yd_umm Index = Start + 1;
    while (Index--)
    {
        if (Str[Index] == Character)
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
RFind(String Str, char Character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    yd_umm Index = Start + 1;
    while (Index--)
    {
        if (Str.Data[Index] == Character)
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
RFind(const char* Str, yd_umm Count, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Count));
    
    if (Count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    // TODO(yuval): This will overflow if Seek's Count is Bigger then Str's Count
    // An additional check is needed
    if (Start + Seek.Count > Count)
    {
        Start = Count - Seek.Count;
    }
    
    yd_umm Index = Start + 1;
    while (Index--)
    {
        yd_b32 Hit = true;
        yd_umm StrIndex = Index;
        
        for (yd_umm SeekIndex = 0;
             SeekIndex < Seek.Count;
             ++SeekIndex, ++StrIndex)
        {
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

yd_umm
RFind(String Str, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    // TODO(yuval): This will overflow if Seek's Count is Bigger then Str's Count
    // An additional check is needed
    if (Start + Seek.Count > Str.Count)
    {
        Start = Str.Count - Seek.Count;
    }
    
    yd_umm Index = Start + 1;
    while (Index--)
    {
        yd_b32 Hit = true;
        yd_umm StrIndex = Index;
        
        for (yd_umm SeekIndex = 0;
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

yd_umm
FindFirstOf(const char* Str, const char* Characters, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    if (!(*Characters))
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm Index = Start; Str[Index]; ++Index)
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

yd_umm
FindFirstOf(String Str, const char* Characters, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    if (!(*Characters))
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm Index = Start; Index < Str.Count; ++Index)
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

yd_umm
FindInsensitive(const char* Str, char Character, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    for (yd_umm Index = Start; Str[Index]; ++Index)
    {
        if (ToLower(Str[Index]) == ToLower(Character))
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindInsensitive(String Str, char Character, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    for (yd_umm Index = Start; Index < Str.Count; ++Index)
    {
        if (ToLower(Str.Data[Index]) == ToLower(Character))
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
FindInsensitive(const char* Str, String Seek, yd_umm Start)
{
    YDAssert(Start >= 0);
    
    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm Index = Start; Str[Index]; ++Index)
    {
        yd_b32 Hit = true;
        yd_umm StrIndex = Index;
        
        for (yd_umm SeekIndex = 0;
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

yd_umm
FindInsensitive(String Str, String Seek, yd_umm Start)
{
    YDAssert((Start >= 0) && (Start < Str.Count));
    
    if (Seek.Count == 0)
    {
        return STRING_NOT_FOUND;
    }
    
    yd_umm StopAt = Str.Count - Seek.Count + 1;
    
    for (yd_umm Index = Start; Index < StopAt; ++Index)
    {
        yd_b32 Hit = true;
        yd_umm StrIndex = Index;
        
        for (yd_umm SeekIndex = 0;
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

String
GetFirstDoubleLine(String Source)
{
    String Result = {};
    
    yd_umm Pos = Find(Source, MakeLitString("\n\n"));
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

String
GetNextDoubleLine(String Source, String Line)
{
    String Result = {};
    
    yd_umm LineEndIndex = (yd_umm)(Line.Data - Source.Data) + Line.Count;
    YDAssert((Source.Data[LineEndIndex] == '\n') || (Source.Data[LineEndIndex] == '\r'));
    
    ++LineEndIndex;
    YDAssert((Source.Data[LineEndIndex] == '\n') || (Source.Data[LineEndIndex] == '\r'));
    
    yd_umm Start = LineEndIndex + 1;
    
    if (Start < Source.Count)
    {
        yd_umm Pos = Find(Source, MakeLitString("\n\n"), Start);
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

String
GetNextWord(String Source, String PrevWord)
{
    String Result = {};
    yd_umm Pos0 = (yd_umm)(PrevWord.Data - Source.Data) + PrevWord.Count;
    
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
        yd_umm Pos1 = Pos0;
        
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

//
// NOTE(yuval): String Copy And Append Functions
//

yd_umm
CopyFastUnsafe(char* Dest, const char* Source)
{
    char* DestAt = Dest;
    const char* SourceAt = Source;
    
    while (*SourceAt)
    {
        *DestAt++ = *SourceAt++;
    }
    
    *DestAt = 0;
    
    yd_umm Result = (DestAt - Dest);
    return Result;
}

yd_umm
CopyFastUnsafe(char* Dest, String Source)
{
    for (yd_umm Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = Source.Data[Index];
    }
    
    Dest[Source.Count] = 0;
    return Source.Count;
}

yd_b32
CopyChecked(String* Dest, String Source)
{
    if (Dest->capacity < Source.Count)
    {
        return false;
    }
    
    for (yd_umm Index = 0; Index < Source.Count; ++Index)
    {
        Dest->Data[Index] = Source.Data[Index];
    }
    
    Dest->Count = Source.Count;
    return true;
}

yd_b32
CopyChecked(char* Dest, yd_umm DestCap, String Source)
{
    if (DestCap < Source.Count + 1)
    {
        return false;
    }
    
    for (yd_umm Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = Source.Data[Index];
    }
    
    Dest[Source.Count] = 0;
    return true;
}

yd_b32
CopyPartial(String* Dest, const char* Source)
{
    yd_b32 Result = true;
    yd_umm Index = 0;
    
    for (; Source[Index]; ++Index)
    {
        if (Index >= Dest->capacity)
        {
            Result = false;
            break;
        }
        
        Dest->Data[Index] = Source[Index];
    }
    
    Dest->Count = Index;
    return Result;
}

yd_b32
CopyPartial(String* Dest, String Source)
{
    yd_b32 Result = true;
    yd_umm Index = 0;
    
    for (; Index < Source.Count; ++Index)
    {
        if (Index >= Dest->capacity)
        {
            Result = false;
            break;
        }
        
        Dest->Data[Index] = Source.Data[Index];
    }
    
    Dest->Count = Index;
    return Result;
}

yd_b32
CopyPartial(char* Dest, yd_umm DestCap, String Source)
{
    yd_b32 Result = true;
    yd_umm Index = 0;
    
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

yd_b32
AppendChecked(String* Dest, String Source)
{
    String End = TailStr(*Dest);
    yd_b32 Result = CopyChecked(&End, Source);
    Dest->Count += End.Count;
    return Result;
}

yd_b32
AppendPartial(String* Dest, const char* Source)
{
    String End = TailStr(*Dest);
    yd_b32 Result = CopyPartial(&End, Source);
    Dest->Count += End.Count;
    return Result;
}

yd_b32
AppendPartial(String* Dest, String Source)
{
    String End = TailStr(*Dest);
    yd_b32 Result = CopyPartial(&End, Source);
    Dest->Count += End.Count;
    return Result;
}

yd_b32
Append(String* Dest, char C)
{
    yd_b32 Result = false;
    
    if (Dest->Count < Dest->capacity)
    {
        Dest->Data[Dest->Count++] = C;
        Result = true;
    }
    
    return Result;
}

yd_b32
TerminateWithNull(String* Str)
{
    yd_b32 Result = false;
    
    if (Str->Count < Str->capacity)
    {
        Str->Data[Str->Count] = 0;
        Result = true;
    }
    
    return Result;
}

yd_b32
AppendPadding(String* Dest, char C, yd_umm TargetCount)
{
    yd_b32 Result = true;
    
    for (yd_umm Count = Dest->Count; Count < TargetCount; ++Count)
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
// NOTE(yuval): String Concatenation
//

yd_b32
ConcatStrings(String* Dest, String SourceA, String SourceB)
{
    yd_b32 CanFitConcat = (Dest->capacity >= SourceA.Count + SourceB.Count);
    
    if (CanFitConcat)
    {
        yd_umm DestIndex = 0;
        
        for (yd_umm SourceIndex = 0;
             SourceIndex < SourceA.Count;
             ++SourceIndex, ++DestIndex)
        {
            Dest->Data[DestIndex] = SourceA.Data[SourceIndex];
        }
        
        for (yd_umm SourceIndex = 0;
             SourceIndex < SourceB.Count;
             ++SourceIndex, ++DestIndex)
        {
            Dest->Data[DestIndex] = SourceB.Data[SourceIndex];
        }
        
        Dest->Count = SourceA.Count + SourceB.Count;
    }
    
    return CanFitConcat;
}

//
// NOTE(yuval): Other String Editing Functions
//

void
ReplaceRange(String* Str, yd_umm First, yd_umm OnePastLast, char With)
{
    YDAssert((First >= 0) && (First < Str->Count));
    YDAssert((OnePastLast > 0) && (OnePastLast <= Str->Count));
    YDAssert(First < OnePastLast);
    
    for (yd_umm Index = First; Index < OnePastLast; ++Index)
    {
        Str->Data[Index] = With;
    }
}

void
ReplaceRange(String* Str, yd_umm First, yd_umm OnePastLast, const char* With)
{
    String WithStr = MakeStringSlowly(With);
    ReplaceRange(Str, First, OnePastLast, WithStr);
}

// TODO(yuval): Maybe rename to block_copy?
yd_internal void
YDStringBlockMove(void* Dest_init, const void* Source_init, yd_umm size)
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

void
ReplaceRange(String* Str, yd_umm First, yd_umm OnePastLast, String With)
{
    YDAssert((First >= 0) && (First < Str->Count));
    YDAssert((OnePastLast > 0) && (OnePastLast <= Str->Count));
    YDAssert(First < OnePastLast);
    
    yd_s64 shift = With.Count - (OnePastLast - First);
    yd_umm new_Count = Str->Count + shift;
    
    if (new_Count <= Str->capacity)
    {
        if (shift != 0)
        {
            char* tail = Str->Data + OnePastLast;
            char* new_tail_Pos = tail + shift;
            // TODO(yuval): Verify that this has no bugs!!!!!!!!!
            YDStringBlockMove(new_tail_Pos, tail, Str->Count - OnePastLast);
        }
        
        YDStringBlockMove(Str->Data + First, With.Data, With.Count);
        Str->Count += shift;
    }
}

void
Replace(String* Str, char ToReplace, char With)
{
    for (yd_umm Index = 0; Index < Str->Count; ++Index)
    {
        if (Str->Data[Index] == ToReplace)
        {
            Str->Data[Index] = With;
        }
    }
}

void
Replace(String* Str, const char* ToReplace, const char* With)
{
    Replace(Str, MakeStringSlowly(ToReplace), MakeStringSlowly(With));
}

void
Replace(String* Str, const char* ToReplace, String With)
{
    Replace(Str, MakeStringSlowly(ToReplace), With);
}

void
Replace(String* Str, String ToReplace, const char* With)
{
    Replace(Str, ToReplace, MakeStringSlowly(With));
}

void
Replace(String* Str, String ToReplace, String With)
{
    yd_umm Index = 0;
    
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

void
StringInterpretEscapes(char* Dest, String Source)
{
    yd_s32 Mode = 0;
    yd_umm DestIndex = 0;
    
    for (yd_umm SourceIndex = 0; SourceIndex < Source.Count; ++SourceIndex)
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
// NOTE(yuval): String Arena Allocation Functions
//

// TODO(yuval): This functions should all be inline!

#if defined(YD_MEMORY)
char*
PushZ(memory_arena* Arena, yd_umm Count, arena_push_params Params)
{
    char* Result = PushArray(Arena, char, Count + 1, Params);
    
    if (Result)
    {
        Result[Count] = 0;
    }
    
    return Result;
}

String
PushString(memory_arena* Arena, yd_umm capacity, arena_push_params Params)
{
    String Result = {};
    Result.Data = PushArray(Arena, char, capacity, Params);
    Result.Count = 0;
    
    if (Result.Data)
    {
        Result.capacity = capacity;
    }
    
    return Result;
}

char*
PushCopyZ(memory_arena* Arena, const char* Source, arena_push_params Params)
{
    yd_umm Size = StringLength(Source);
    char* Result = (char*)PushCopy(Arena, Source, Size + 1, Params);
    
    return Result;
}

String
PushCopyString(memory_arena* Arena, const char* Source, arena_push_params Params)
{
    String Result = {};
    
    yd_umm Size = StringLength(Source);
    Result.Data = (char*)PushCopy(Arena, Source, Size, Params);
    
    if (Result.Data)
    {
        Result.Count = Size;
        Result.capacity = Size;
    }
    
    return Result;
}

char*
PushCopyZ(memory_arena* Arena, String Source, arena_push_params Params)
{
    char* Result = PushArray(Arena, char, Source.Count + 1, Params);
    
    if (Result)
    {
        CopyArray(Result, Source.Data, Source.Count);
        Result[Source.Count] = 0;
    }
    
    return Result;
}


String
PushCopyString(memory_arena* Arena, String Source, arena_push_params Params)
{
    String Result = {};
    Result.Data = (char*)PushCopy(Arena, Source.Data, Source.Count, Params);
    
    if (Result.Data)
    {
        Result.Count = Source.Count;
        Result.capacity = Source.Count;
    }
    
    return Result;
}
#endif // #if defined(YD_MEMORY)

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

void
ToLower(char* Str)
{
    for (char* At = Str; *At; ++At)
    {
        *At = ToLower(*At);
    }
}

void
ToLower(String* Str)
{
    for (yd_umm Index = 0; Index < Str->Count; ++Index)
    {
        Str->Data[Index] = ToLower(Str->Data[Index]);
    }
}

void
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

void
ToLower(String* Dest, const char* Source)
{
    yd_umm Index = 0;
    
    for (; Source[Index]; ++Index)
    {
        Dest->Data[Index] = ToLower(Source[Index]);
    }
    
    Dest->Count = Index;
}

void
ToLower(char* Dest, String Source)
{
    for (yd_umm Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = ToLower(Source.Data[Index]);
    }
    
    Dest[Source.Count] = 0;
}

void
ToLower(String* Dest, String Source)
{
    if (Dest->capacity >= Source.Count)
    {
        for (yd_umm Index = 0; Index < Source.Count; ++Index)
        {
            Dest->Data[Index] = ToLower(Source.Data[Index]);
        }
        
        Dest->Count = Source.Count;
    }
}

void
ToUpper(char* Str)
{
    for (char* At = Str; *At; ++At)
    {
        *At = ToUpper(*At);
    }
}

void
ToUpper(String* Str)
{
    for (yd_umm Index = 0; Index < Str->Count; ++Index)
    {
        Str->Data[Index] = ToUpper(Str->Data[Index]);
    }
}

void
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

void
ToUpper(String* Dest, const char* Source)
{
    yd_umm Index = 0;
    
    for (; Source[Index]; ++Index)
    {
        Dest->Data[Index] = ToUpper(Source[Index]);
    }
    
    Dest->Count = Index;
}

void
ToUpper(char* Dest, String Source)
{
    for (yd_umm Index = 0; Index < Source.Count; ++Index)
    {
        Dest[Index] = ToUpper(Source.Data[Index]);
    }
    
    Dest[Source.Count] = 0;
}

void
ToUpper(String* Dest, String Source)
{
    if (Dest->capacity >= Source.Count)
    {
        for (yd_umm Index = 0; Index < Source.Count; ++Index)
        {
            Dest->Data[Index] = ToUpper(Source.Data[Index]);
        }
        
        Dest->Count = Source.Count;
    }
}

void
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

void
ToCamel(String* Str)
{
    yd_b32 IsFirst = true;
    
    for (yd_umm Index = 0; Index < Str->Count; ++Index)
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

void
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

void
ToCamel(String* Dest, const char* Source)
{
    yd_umm Index = 0;
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

void
ToCamel(char* Dest, String Source)
{
    yd_b32 IsFirst = true;
    
    for (yd_umm Index = 0; Index < Source.Count; ++Index)
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

void
ToCamel(String* Dest, String Source)
{
    if (Dest->capacity >= Source.Count)
    {
        yd_b32 IsFirst = true;
        
        for (yd_umm Index = 0; Index < Source.Count; ++Index)
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

yd_umm
U64ToStringCount(yd_u64 Value)
{
    yd_umm Count = 1;
    
    Value /= 10;
    while (Value != 0)
    {
        ++Count;
        Value /= 10;
    }
    
    return Count;
}

yd_b32
U64ToString(String* Dest, yd_u64 Value)
{
    yd_b32 Result = false;
    yd_umm Count = 0;
    
    do
    {
        if (Count == Dest->capacity)
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
        for (yd_umm StartIndex = 0, EndIndex = Count - 1;
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

yd_b32
AppendU64ToString(String* Dest, yd_u64 Value)
{
    String Tail = TailStr(*Dest);
    yd_b32 Result = U64ToString(&Tail, Value);
    
    if (Result)
    {
        Dest->Count += Tail.Count;
    }
    
    return Result;
}


yd_umm
S32ToStringCount(yd_s32 Value)
{
    yd_umm Count = 1;
    
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

yd_b32
S32ToString(String* Dest, yd_s32 Value)
{
    yd_b32 Result = true;
    
    Dest->Count = 0;
    
    if (Value < 0)
    {
        if (Dest->capacity > 0)
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

yd_b32
AppendS32ToString(String* Dest, yd_s32 Value)
{
    String Tail = TailStr(*Dest);
    yd_b32 Result = S32ToString(&Tail, Value);
    
    if (Result)
    {
        Dest->Count += Tail.Count;
    }
    
    return Result;
}

yd_umm
F32ToStringCount(yd_f32 Value, yd_u32 MaxPrecision)
{
    yd_umm Count = 0;
    
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

yd_b32
F32ToString(String* Dest, yd_f32 Value, yd_u32 MaxPrecision)
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

yd_b32
AppendF32ToString(String* Dest, yd_f32 Value, yd_u32 MaxPrecision)
{
    String Tail = TailStr(*Dest);
    yd_b32 Result = F32ToString(&Tail, Value, MaxPrecision);
    
    if (Result)
    {
        Dest->Count += Tail.Count;
    }
    
    return Result;
}

yd_u32
HexStringToU32(String Str)
{
    yd_u32 Result = 0;
    
    for (yd_umm Index = 0; Index < Str.Count; ++Index)
    {
        Result += HexCharToS32(Str.Data[Index]);
        Result *= 0x10;
    }
    
    return Result;
}

yd_b32
ColorToHexString(String* Dest, yd_u32 Color)
{
    // TODO(yuval): Implement This
    return false;
}

yd_b32
HexStringToColor(yd_u32* Dest, String Str)
{
    // TODO(yuval): Implement This
    return false;
}

//
// NOTE(yuval): File / Directory Strings Management Functions
//

yd_umm
ReverseSeekSlash(String Str, yd_umm ShiftFromLastChar)
{
    yd_umm Index = Str.Count - ShiftFromLastChar - 1;
    while (Index--)
    {
        if (IsSlash(Str.Data[Index]))
        {
            return Index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
ReverseSeekSlash(String Str)
{
    yd_umm Result = ReverseSeekSlash(Str, 0);
    return Result;
}

#if 0
yd_b32
SetLastFolder(char* Dir, yd_umm Count, const char* FolderName, char Slash)
{
    yd_b32 Result = false;
    yd_umm LastSlashIndex = ReverseSeekSlash(Dir, Count);
    
    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        yd_umm NewCount = LastSlashIndex + 1;
        NewCount = Append(Dir, Count, FolderName));
        {
            if (Append())
        }
    }
}
#endif // #if 0

yd_b32
SetLastFolder(String* Dir, const char* FolderName, char Slash)
{
    yd_b32 Result = false;
    yd_umm LastSlashIndex = ReverseSeekSlash(*Dir);
    
    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        yd_umm Count = LastSlashIndex + 1;
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

#if 0
yd_b32
SetLastFolder(char* Dir, yd_umm Count, String FolderName, char Slash)
{
    
}
#endif // #if 0

yd_b32
SetLastFolder(String* Dir, String FolderName, char Slash)
{
    yd_b32 Result = false;
    yd_umm LastSlashIndex = ReverseSeekSlash(*Dir);
    
    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        yd_umm Count = LastSlashIndex + 1;
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

yd_b32
RemoveLastFolder(String* path)
{
    yd_b32 Result = false;
    yd_umm LastSlashIndex = ReverseSeekSlash(*path, 1);
    
    if (LastSlashIndex != STRING_NOT_FOUND)
    {
        Result = true;
        path->Count = LastSlashIndex + 1;
    }
    
    return Result;
}

String
FileExtension(String FileName)
{
    String Result = {};
    yd_umm DotIndex = RFind(FileName, '.');
    
    if (DotIndex != STRING_NOT_FOUND)
    {
        Result = MakeString(FileName.Data + DotIndex + 1,
                            FileName.Count - DotIndex - 1);
    }
    
    return Result;
}

yd_b32
SetExtension(String* FileName, const char* Extension)
{
    yd_b32 Result = false;
    yd_umm LastDotIndex = RFind(*FileName, '.');
    
    if (LastDotIndex != STRING_NOT_FOUND)
    {
        yd_umm Count = LastDotIndex;
        FileName->Count = Count;
        
        if (Append(FileName, Extension))
        {
            Result = true;
        }
        
        if (!Result)
        {
            FileName->Count = Count;
        }
    }
    
    return Result;
}

yd_b32
SetExtension(String* FileName, String Extension)
{
    yd_b32 Result = false;
    yd_umm LastDotIndex = RFind(*FileName, '.');
    
    if (LastDotIndex != STRING_NOT_FOUND)
    {
        yd_umm Count = LastDotIndex + 1;
        FileName->Count = Count;
        
        if (Append(FileName, Extension))
        {
            Result = true;
        }
        
        if (!Result)
        {
            FileName->Count = Count - 1;
        }
    }
    
    return Result;
}

yd_b32
RemoveExtension(String* FileName)
{
    yd_b32 Result = false;
    yd_umm LastDotIndex = RFind(*FileName, '.');
    
    if (LastDotIndex != STRING_NOT_FOUND)
    {
        Result = true;
        FileName->Count = LastDotIndex;
    }
    
    return Result;
}

#endif // #if defined(YD_STRING_IMPLEMENTATION)

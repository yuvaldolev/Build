#if !defined(BUILD_ERRORS_H)

#include <stdio.h>
#include <stdlib.h>

internal void
print_report(String type, Code_File file, s32 line, s32 column,
             const char* format, va_list arg_list) {
    const char* data_at = file.contents.data;
    
    // NOTE(yuval): Getting to the line that contains the error
    for (s32 line_number = 1; line_number < line; ++line_number) {
        while (*data_at && (*data_at++ != '\n'));
    }
    
    // NOTE(yuval): Ignoring all spacing in the beginning of the line
    s32 first_char_index = 0;
    while (is_spacing(*data_at)) {
        ++first_char_index;
        ++data_at;
    }
    
    // NOTE(yuval): Error line length calculation
    u32 line_len = 0;
    const char* line_at = data_at;
    
    while (*line_at && (*line_at != '\n')) {
        ++line_len;
        ++line_at;
    }
    
    char message[4096] = {};
    format_string_list(message, sizeof(message), format, arg_list);
    
    // NOTE(yuval): Error location data
    fprintf(stderr, "%.*s:%d:%d: %.*s: %s\n",
            PRINTABLE_STRING(file.name),
            line, column, PRINTABLE_STRING(type), message);
    
    // NOTE(yuval): Error line
    fprintf(stderr, "    %.*s\n    ", line_len, data_at);
    
    // NOTE(yuval): Error char
    for (s32 char_index = first_char_index;
         char_index < column - 1;
         ++char_index) {
        fprintf(stderr, ((data_at[char_index] == '\t') ? "    " : " "));
    }
    
    fprintf(stderr, "^\n\n");
}

internal void
report_error_list(Code_File file, s32 line, s32 column,
                  const char* format, va_list arg_list) {
    print_report(MAKE_LIT_STRING("error"), file, line, column, format, arg_list);
}

internal void
report_error(Code_File file, s32 line, s32 column, const char* format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    report_error_list(file, line, column, format, arg_list);
    va_end(arg_list);
}

internal void
report_error(Token* token, const char* format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    report_error_list(token->file, token->line_number,
                      token->column_number, format, arg_list);
    va_end(arg_list);
}

internal void
report_error(Ast* ast, const char* format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    report_error_list(ast->my_file, ast->my_line,
                      ast->my_column, format, arg_list);
    va_end(arg_list);
}

internal void
report_warning_list(Code_File file, s32 line, s32 column,
                    const char* format, va_list arg_list) {
    print_report(MAKE_LIT_STRING("warning"), file, line, column, format, arg_list);
}

internal void
report_warning(Code_File file, s32 line, s32 column, const char* format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    report_warning_list(file, line, column, format, arg_list);
    va_end(arg_list);
}

internal void
report_warning(Token* token, const char* format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    report_warning_list(token->file, token->line_number,
                        token->column_number, format, arg_list);
    va_end(arg_list);
}

internal void
report_warning(Ast* ast, const char* format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    report_warning_list(ast->my_file, ast->my_line,
                        ast->my_column, format, arg_list);
    va_end(arg_list);
}

#define BUILD_ERRORS_H
#endif // #if !defined(BUILD_ERRORS_H)
// TODO(yuval): Maybe add support for LINE, FILE, etc. preprocessor tokens
// TODO(yuval): Add C++ support (class, new, delete, try, catch...)
global Token_Name_And_Type global_keywords[] = {
#define KEYWORD_TOKEN_TYPE(type, name) {name, JOIN2(TOKEN_, type)},
    KEYWORD_TOKEN_TYPES
#undef KEYWORD_TOKEN_TYPE
    
        // NOTE(yuval): BoolConstant can be true as well as false
    {"false", TOKEN_BOOL_CONSTANT}
};

global Token_Name_And_Type global_pp_keywords[] = {
#define PP_KEYWORD_TOKEN_TYPE(type, name) {name, JOIN2(TOKEN_, type)},
    PP_KEYWORD_TOKEN_TYPES
        PP_KEYWORD_TOKEN_TYPES_UPPER
#undef PP_KEYWORD_TOKEN_TYPE
};

internal b32
is_null_token(Token token) {
    b32 result = (token.type == TOKEN_NULL);
    return result;
}

internal b32
token_equals(Token token, const char* match) {
    b32 result = strings_match(token.text, match);
    return result;
}

internal String
get_token_type_name(Token_Type type) {
    switch (type) {
#define TOKEN_TYPE(type) case JOIN2(TOKEN_, type): { return BUNDLE_LITERAL(#type); }
        TOKEN_TYPES
#undef TOKEN_TYPE
        
#define KEYWORD_TOKEN_TYPE(type, ...) case JOIN2(TOKEN_, type): { return BUNDLE_LITERAL(#type); }
            KEYWORD_TOKEN_TYPES
#undef KEYWORD_TOKEN_TYPE
    }
    
    return BUNDLE_LITERAL("Unknown");
}

internal void
refill(Lexer* lexer) {
    if (lexer->input.count == 0) {
        lexer->at[0] = 0;
        lexer->at[1] = 0;
    } else if (lexer->input.count == 1) {
        lexer->at[0] = lexer->input.data[0];
        lexer->at[1] = 0;
    } else {
        lexer->at[0] = lexer->input.data[0];
        lexer->at[1] = lexer->input.data[1];
    }
}

internal void
advance_chars(Lexer* lexer, umm count) {
    lexer->column_number += count;
    advance_string(&lexer->input, count);
    refill(lexer);
}

internal void
get_literal(Token* token, Lexer* lexer, char enclosing) {
    while (lexer->at[0] && lexer->at[0] != enclosing) {
        if ((lexer->at[0] == '\\') && lexer->at[1]) {
            advance_chars(lexer, 1);
        }
        
        advance_chars(lexer, 1);
    }
    
    if (lexer->at[0] == enclosing) {
        advance_chars(lexer, 1);
    } else {
        report_error(token, "unclosed literal: %c expected", enclosing);
    }
}

internal Token
get_token_raw(Lexer* lexer) {
    Token token = {};
    token.file = lexer->file;
    token.line_number = lexer->line_number;
    token.column_number = lexer->column_number;
    token.text = lexer->input;
    
    if (is_alpha(lexer->at[0])) {
        b32 is_keyword = false;
        
        array_foreach (global_keywords) {
            if (strings_match_part(lexer->input, it.name)) {
                // TODO(yuval): Copy-and-paste - string_length is called twice
                advance_chars(lexer, string_length(it.name));
                token.type = it.type;
                is_keyword = true;
                array_break;
            }
        }
        
        if (!is_keyword) {
            token.type = TOKEN_IDENTIFIER;
            
            advance_chars(lexer, 1);
            
            while (is_alpha(lexer->at[0]) ||
                   is_numeric(lexer->at[0]) ||
                   (lexer->at[0] == '_')) {
                advance_chars(lexer, 1);
            }
        }
    } else {
        char c = lexer->at[0];
        advance_chars(lexer, 1);
        
        switch (c) {
            case '\0': { token.type = TOKEN_END_OF_STREAM; } break;
            
            case '?': { token.type = TOKEN_TERNARY; } break;
            case '[': { token.type = TOKEN_OPEN_BRACKET; } break;
            case ']': { token.type = TOKEN_CLOSE_BRACKET; } break;
            case '(': { token.type = TOKEN_OPEN_PAREN; } break;
            case ')': { token.type = TOKEN_CLOSE_PAREN; } break;
            case '{': { token.type = TOKEN_OPEN_BRACE; } break;
            case '}': { token.type = TOKEN_CLOSE_BRACE; } break;
            case '.': { token.type = TOKEN_DOT; } break;
            case '~': { token.type = TOKEN_TILDE; } break;
            case ';': { token.type = TOKEN_SEMI; } break;
            case ',': { token.type = TOKEN_COMMA; } break;
            case '@': { token.type = TOKEN_AT; } break;
            
            case '&': {
                if (lexer->at[0] == '&') {
                    token.type = TOKEN_AMP_AMP;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_AMP_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_AMP;
                }
            } break;
            
            case '*': {
                if (lexer->at[0] == '=') {
                    token.type = TOKEN_STAR_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_STAR;
                }
            } break;
            
            case '+': {
                if (lexer->at[0] == '+') {
                    token.type = TOKEN_PLUS_PLUS;
                    advance_chars(lexer, 1);
                }
                else if (lexer->at[0] == '=') {
                    token.type = TOKEN_PLUS_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_PLUS;
                }
            } break;
            
            case '-': {
                if (lexer->at[0] == '-') {
                    token.type = TOKEN_MINUS_MINUS;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_MINUS_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_MINUS;
                }
            } break;
            
            case '!': {
                if (lexer->at[0] == '=') {
                    token.type = TOKEN_NOT_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_NOT;
                }
            } break;
            
            case '%': {
                if (lexer->at[0] == '=') {
                    token.type = TOKEN_PERCENT_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_PERCENT;
                }
            } break;
            
            case '<': {
                if (lexer->at[0] == '<') {
                    token.type = TOKEN_LESS_LESS;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_LESS_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_LESS;
                }
            } break;
            
            case '>': {
                if (lexer->at[0] == '>') {
                    token.type = TOKEN_GREATER_GREATER;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_GREATER_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_GREATER;
                }
            } break;
            
            case '^': {
                if (lexer->at[0] == '^') {
                    token.type = TOKEN_CARET_CARET;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_CARET_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_CARET;
                }
            } break;
            
            case '|': {
                if (lexer->at[0] == '|') {
                    token.type = TOKEN_PIPE_PIPE;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_PIPE_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_PIPE;
                }
            } break;
            
            case ':': {
                if (lexer->at[0] == ':') {
                    token.type = TOKEN_COLON_COLON;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_COLON;
                }
            } break;
            
            case '=': {
                if (lexer->at[0] == '=') {
                    token.type = TOKEN_EQUAL_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_EQUAL;
                }
            } break;
            
            case '#': {
                if (lexer->at[0] == '#') {
                    token.type = TOKEN_HASH_HASH;
                    advance_chars(lexer, 1);
                } else {
                    b32 is_keyword = false;
                    
                    array_foreach (global_pp_keywords) {
                        if (strings_match_part(lexer->input, it.name)) {
                            // TODO(yuval): Copy-and-paste - string_length is called twice
                            advance_chars(lexer, string_length(it.name));
                            token.type = it.type;
                            is_keyword = true;
                            array_break;
                        }
                    }
                    
                    if (!is_keyword) {
                        token.type = TOKEN_HASH;
                    }
                }
            } break;
            
            case '/': {
                if (lexer->at[0] == '/') {
                    token.type = TOKEN_COMMENT;
                    advance_chars(lexer, 2);
                    
                    while (lexer->at[0] && !is_end_of_line(lexer->at[0])) {
                        advance_chars(lexer, 1);
                    }
                } else if (lexer->at[0] == '*') {
                    token.type = TOKEN_COMMENT;
                    advance_chars(lexer, 2);
                    
                    while (lexer->at[0] &&
                           !((lexer->at[0] == '*') && (lexer->at[1]))) {
                        if (((lexer->at[0] == '\r') && (lexer->at[1] == '\n')) ||
                            ((lexer->at[0] == '\n') && (lexer->at[1] == '\r'))) {
                            advance_chars(lexer, 1);
                        }
                        
                        if (is_end_of_line(lexer->at[0])) {
                            ++lexer->line_number;
                        }
                        
                        advance_chars(lexer, 1);
                    }
                } else if (lexer->at[0] == '=') {
                    token.type = TOKEN_SLASH_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.type = TOKEN_SLASH;
                }
            } break;
            
            case '"': {
                token.type = TOKEN_STRING_LITERAL;
                get_literal(&token, lexer, '"');
            } break;
            
            case '\'': {
                token.type = TOKEN_CHAR_CONSTANT;
                get_literal(&token, lexer, '\'');
            } break;
            
            default: {
                if (is_spacing(c)) {
                    token.type = TOKEN_SPACING;
                    
                    while (is_spacing(lexer->at[0]))
                    {
                        advance_chars(lexer, 1);
                    }
                }
                else if (is_end_of_line(c)) {
                    token.type = TOKEN_END_OF_LINE;
                    
                    if (((c == '\r') && (lexer->at[0] == '\n')) ||
                        ((c == '\n') && (lexer->at[0] == '\r'))) {
                        advance_chars(lexer, 1);
                    }
                    
                    lexer->column_number = 1;
                    ++lexer->line_number;
                }
                else if (is_numeric(c)) {
                    f32 number = (f32)(c - '0');
                    
                    while (is_numeric(lexer->at[0])) {
                        f32 digit = (f32)(lexer->at[0] - '0');
                        number = 10.0f * number + digit;
                        advance_chars(lexer, 1);
                    }
                    
                    if (lexer->at[0] == '.') {
                        advance_chars(lexer, 1);
                        f32 coefficient = 0.1f;
                        
                        while (is_numeric(lexer->at[0])) {
                            f32 digit = (f32)(lexer->at[0] - '0');
                            number += digit * coefficient;
                            coefficient *= 0.1f;
                            advance_chars(lexer, 1);
                        }
                    }
                    
                    token.type = TOKEN_NUMBER;
                    token.value_f32 = number;
                    token.value_s32 = round_f32_to_s32(number);
                } else {
                    token.type = TOKEN_UNKNOWN;
                }
            } break;
        }
    }
    
    token.text.count = (lexer->input.data - token.text.data);
    
    return token;
}

internal Token
get_token(Lexer* lexer) {
    Token token;
    
    // TODO(yuval): Maybe move getting & resetting the next token to get_token_raw?
    if (is_null_token(lexer->next_token)) {
        // NOTE(yuval): The next token was not peeked already,
        // so we need to get a new token
        
        for (;;) {
            token = get_token_raw(lexer);
            
            if ((token.type == TOKEN_SPACING) ||
                (token.type == TOKEN_END_OF_LINE) ||
                (token.type == TOKEN_COMMENT)) {
                // NOTE(yuval): These tokens are ignored
            } else {
                if (token.type == TOKEN_STRING_LITERAL) {
                    if ((token.text.count != 0) &&
                        (token.text.data[0] == '"')) {
                        ++token.text.data;
                        --token.text.count;
                    }
                    
                    if ((token.text.count != 0) &&
                        (token.text.data[token.text.count - 1] == '"')) {
                        --token.text.count;
                    }
                }
                
                if (token.type == TOKEN_CHAR_CONSTANT) {
                    if ((token.text.count != 0) &&
                        (token.text.data[0] == '\'')) {
                        ++token.text.data;
                        --token.text.count;
                    }
                    
                    if ((token.text.count != 0) &&
                        (token.text.data[token.text.count - 1] == '\'')) {
                        --token.text.count;
                    }
                }
                
                break;
            }
        }
    } else {
        // NOTE(yuval): The next token has already been peeked at,
        // so we don't need to get a new tokent
        token = lexer->next_token;
        lexer->next_token = NULL_TOKEN;
    }
    
    return token;
}

internal b32
get_token_check_type(Lexer* lexer, Token_Type desired_type, Token* out_token) {
    *out_token = get_token(lexer);
    b32 result = (out_token->type == desired_type);
    return result;
}

internal Token
require_token(Lexer* lexer, Token_Type desired_type) {
    Token token = get_token(lexer);
    
    if (token.type != desired_type) {
        report_error(&token, "unexpected token type: %S (expected %S)",
                     get_token_type_name(token.type), get_token_type_name(desired_type));
    }
    
    return token;
}

internal Token
peek_token(Lexer* lexer) {
    Token result;
    
    if (is_null_token(lexer->next_token)) {
        Lexer temp = *lexer;
        result = get_token(&temp);
        lexer->next_token = result;
    } else {
        result = lexer->next_token;
    }
    
    return result;
}

internal b32
optional_token(Lexer* lexer, Token_Type desired_type) {
    Token token = peek_token(lexer);
    b32 result = (token.type == desired_type);
    
    if (result) {
        get_token(lexer);
    }
    
    return result;
}

internal Lexer
lex(Code_File file) {
    Lexer result = {};
    
    result.file = file;
    result.line_number = 1;
    result.column_number = 1;
    result.input = file.contents;
    refill(&result);
    
    return result;
}

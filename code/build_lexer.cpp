// TODO(yuval): Maybe add support for LINE, FILE, etc. preprocessor tokens
// TODO(yuval): Add C++ support (class, new, delete, try, catch...)
global Token_Name_And_Kind global_keywords[] = {
#define KEYWORD_TOKEN_KIND(kind, name) {name, JOIN2(Token::, kind)},
    KEYWORD_TOKEN_KINDS
#undef KEYWORD_TOKEN_KIND
    
        // NOTE(yuval): BoolConstant can be true as well as false
    {"false", Token::BOOL_CONSTANT}
};

global Token_Name_And_Kind global_pp_keywords[] = {
#define PP_KEYWORD_TOKEN_KIND(kind, name) {name, JOIN2(Token::, kind)},
    PP_KEYWORD_TOKEN_KINDS
        PP_KEYWORD_TOKEN_KINDS_UPPER
#undef PP_KEYWORD_TOKEN_KIND
};

internal b32
is_null_token(Token token) {
    b32 result = (token.kind == Token::NULL);
    return result;
}

internal b32
token_equals(Token token, const char* match) {
    b32 result = strings_match(token.text, match);
    return result;
}

internal String
get_token_kind_name(Token::Kind type) {
    switch (type) {
#define TOKEN_KIND(kind) case JOIN2(Token::, kind): { return BUNDLE_LITERAL(#kind); }
        TOKEN_KINDS
#undef TOKEN_KIND
        
#define KEYWORD_TOKEN_KIND(kind, ...) case JOIN2(Token::, kind): { return BUNDLE_LITERAL(#kind); }
            KEYWORD_TOKEN_KINDS
#undef KEYWORD_TOKEN_KIND
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
                token.kind = it.kind;
                is_keyword = true;
                array_break;
            }
        }
        
        if (!is_keyword) {
            token.kind = Token::IDENTIFIER;
            
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
            case '\0': { token.kind = Token::END_OF_STREAM; } break;
            
            case '?': { token.kind = Token::TERNARY; } break;
            case '[': { token.kind = Token::OPEN_BRACKET; } break;
            case ']': { token.kind = Token::CLOSE_BRACKET; } break;
            case '(': { token.kind = Token::OPEN_PAREN; } break;
            case ')': { token.kind = Token::CLOSE_PAREN; } break;
            case '{': { token.kind = Token::OPEN_BRACE; } break;
            case '}': { token.kind = Token::CLOSE_BRACE; } break;
            case '.': { token.kind = Token::DOT; } break;
            case '~': { token.kind = Token::TILDE; } break;
            case ';': { token.kind = Token::SEMI; } break;
            case ',': { token.kind = Token::COMMA; } break;
            case '@': { token.kind = Token::AT; } break;
            
            case '&': {
                if (lexer->at[0] == '&') {
                    token.kind = Token::AMP_AMP;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.kind = Token::AMP_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::AMP;
                }
            } break;
            
            case '*': {
                if (lexer->at[0] == '=') {
                    token.kind = Token::STAR_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::STAR;
                }
            } break;
            
            case '+': {
                if (lexer->at[0] == '+') {
                    token.kind = Token::PLUS_PLUS;
                    advance_chars(lexer, 1);
                }
                else if (lexer->at[0] == '=') {
                    token.kind = Token::PLUS_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::PLUS;
                }
            } break;
            
            case '-': {
                if (lexer->at[0] == '-') {
                    token.kind = Token::MINUS_MINUS;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.kind = Token::MINUS_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::MINUS;
                }
            } break;
            
            case '!': {
                if (lexer->at[0] == '=') {
                    token.kind = Token::NOT_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::NOT;
                }
            } break;
            
            case '%': {
                if (lexer->at[0] == '=') {
                    token.kind = Token::PERCENT_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::PERCENT;
                }
            } break;
            
            case '<': {
                if (lexer->at[0] == '<') {
                    token.kind = Token::LESS_LESS;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.kind = Token::LESS_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::LESS;
                }
            } break;
            
            case '>': {
                if (lexer->at[0] == '>') {
                    token.kind = Token::GREATER_GREATER;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.kind = Token::GREATER_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::GREATER;
                }
            } break;
            
            case '^': {
                if (lexer->at[0] == '^') {
                    token.kind = Token::CARET_CARET;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.kind = Token::CARET_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::CARET;
                }
            } break;
            
            case '|': {
                if (lexer->at[0] == '|') {
                    token.kind = Token::PIPE_PIPE;
                    advance_chars(lexer, 1);
                } else if (lexer->at[0] == '=') {
                    token.kind = Token::PIPE_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::PIPE;
                }
            } break;
            
            case ':': {
                if (lexer->at[0] == ':') {
                    token.kind = Token::COLON_COLON;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::COLON;
                }
            } break;
            
            case '=': {
                if (lexer->at[0] == '=') {
                    token.kind = Token::EQUAL_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::EQUAL;
                }
            } break;
            
            case '#': {
                if (lexer->at[0] == '#') {
                    token.kind = Token::HASH_HASH;
                    advance_chars(lexer, 1);
                } else {
                    b32 is_keyword = false;
                    
                    array_foreach (global_pp_keywords) {
                        if (strings_match_part(lexer->input, it.name)) {
                            // TODO(yuval): Copy-and-paste - string_length is called twice
                            advance_chars(lexer, string_length(it.name));
                            token.kind = it.type;
                            is_keyword = true;
                            array_break;
                        }
                    }
                    
                    if (!is_keyword) {
                        token.kind = Token::HASH;
                    }
                }
            } break;
            
            case '/': {
                if (lexer->at[0] == '/') {
                    token.kind = Token::COMMENT;
                    advance_chars(lexer, 2);
                    
                    while (lexer->at[0] && !is_end_of_line(lexer->at[0])) {
                        advance_chars(lexer, 1);
                    }
                } else if (lexer->at[0] == '*') {
                    token.kind = Token::COMMENT;
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
                    token.kind = Token::SLASH_EQUAL;
                    advance_chars(lexer, 1);
                } else {
                    token.kind = Token::SLASH;
                }
            } break;
            
            case '"': {
                token.kind = Token::STRING_LITERAL;
                get_literal(&token, lexer, '"');
            } break;
            
            case '\'': {
                token.kind = Token::CHAR_CONSTANT;
                get_literal(&token, lexer, '\'');
            } break;
            
            default: {
                if (is_spacing(c)) {
                    token.kind = Token::SPACING;
                    
                    while (is_spacing(lexer->at[0]))
                    {
                        advance_chars(lexer, 1);
                    }
                }
                else if (is_end_of_line(c)) {
                    token.kind = Token::END_OF_LINE;
                    
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
                    
                    token.kind = Token::NUMBER;
                    token.value_f32 = number;
                    token.value_s32 = round_f32_to_s32(number);
                } else {
                    token.kind = Token::UNKNOWN;
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
    
    for (;;) {
        token = get_token_raw(lexer);
        
        if ((token.kind == Token::SPACING) ||
            (token.kind == Token::END_OF_LINE) ||
            (token.kind == Token::COMMENT)) {
            // NOTE(yuval): These tokens are ignored
        } else {
            if (token.kind == Token::STRING_LITERAL) {
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
            
            if (token.kind == Token::CHAR_CONSTANT) {
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
    
    return token;
}

inline void
eat_token(Lexer* lexer) {
    get_token(lexer);
}

internal b32
get_token_check_type(Lexer* lexer, Token::Kind desired_kind, Token* out_token) {
    *out_token = get_token(lexer);
    b32 result = (out_token->type == desired_kind);
    return result;
}

internal Token
require_token(Lexer* lexer, Token::Kind desired_kind) {
    Token token = get_token(lexer);
    
    if (token.kind != desired_kind) {
        report_error(&token, "unexpected token kind: %S (expected %S)",
                     get_token_kind_name(token.kind), get_token_kind_name(desired_kind));
    }
    
    return token;
}

internal Token
peek_token(Lexer* lexer) {
    Lexer temp = *lexer;
    Token result = get_token(&temp);
    lexer->next_token = result;
    
    return result;
}

internal b32
optional_token(Lexer* lexer, Token::Kind desired_kind) {
    Token token = peek_token(lexer);
    b32 result = (token.kind == desired_kind);
    
    if (result) {
        eat_token(lexer);
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

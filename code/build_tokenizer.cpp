// TODO(yuval): Maybe add support for LINE, FILE, etc. preprocessor tokens
// TODO(yuval): Add C++ support (class, new, delete, try, catch...)
global Token_Name_And_Type global_keywords[] = {
#define KEYWORD_TOKEN_TYPE(type, name) {name, JOIN2(TOKEN_, type)},
    KEYWORD_TOKEN_TYPES
#undef KEYWORD_TOKEN_TYPE
    
        // NOTE(yuval): BoolConstant can be true as well as false
    {"false", Token_BoolConstant}
};

global Token_Name_And_Type global_pp_keywords[] = {
#define PP_KEYWORD_TOKEN_TYPE(type, name) {name, JOIN2(TOKEN_, type)},
    PP_KEYWORD_TOKEN_TYPES
        PP_KEYWORD_TOKEN_TYPES_UPPER
#undef PP_KEYWORD_TOKEN_TYPE
};

internal String
get_Token_Type_name(Token_Type type) {
    switch (type) {
#define TOKEN_TYPE(type) case JOIN2(TOKEN_, type): { return MAKE_LIT_STRING(#type); }
        TOKEN_TYPES
#undef TOKEN_TYPE
        
#define KEYWORD_TOKEN_TYPE(type, ...) case JOIN2(TOKEN_, type): { return MAKE_LIT_STRING(#type); }
            KEYWORD_TOKEN_TYPES
#undef KEYWORD_TOKEN_TYPE
    }
    
    return MAKE_LIT_STRING("Unknown");
}

internal b32
token_equals(Token token, const char* match) {
    b32 result = strings_match(token.text, match);
    return result;
}

internal void
refill(Tokenizer* tokenizer) {
    if (tokenizer->input.count == 0) {
        tokenizer->at[0] = 0;
        tokenizer->at[1] = 0;
    } else if (tokenizer->input.count == 1) {
        tokenizer->at[0] = tokenizer->input.data[0];
        tokenizer->at[1] = 0;
    } else {
        tokenizer->at[0] = tokenizer->input.data[0];
        tokenizer->at[1] = tokenizer->input.data[1];
    }
}

internal void
advance_chars(Tokenizer* tokenizer, umm count) {
    tokenizer->column_number += count;
    advance_string(&tokenizer->input, count);
    refill(tokenizer);
}

internal void
get_literal(Token* token, Tokenizer* tokenizer, char enclosing) {
    while (tokenizer->at[0] && tokenizer->at[0] != enclosing)
    {
        if ((tokenizer->at[0] == '\\') &&
            tokenizer->at[1])
        {
            advance_chars(tokenizer, 1);
        }
        
        advance_chars(tokenizer, 1);
    }
    
    if (tokenizer->at[0] == enclosing)
    {
        advance_chars(tokenizer, 1);
    }
    else
    {
        bad_token(token, "unclosed literal");
    }
}

internal Token
get_token_raw(Tokenizer* tokenizer) {
    Token token = {};
    token.filename = tokenizer->filename;
    token.line_number = tokenizer->line_number;
    token.column_number = tokenizer->column_number;
    token.text = tokenizer->input;
    token.file_data = tokenizer->input_file_data;
    
    if (is_alpha(tokenizer->at[0])) {
        b32 is_keyword = false;
        
        ArrayFor (GlobalKeywords) {
            if (strings_match_part(tokenizer->Input, it.Name)) {
                // TODO(yuval): Copy-and-paste - StringLength is called twice
                advance_chars(tokenizer, StringLength(It.Name));
                token.type = it.type;
                is_keyword = true;
                ArrayBreak;
            }
        }
        
        if (!IsKeyword) {
            token.type = TOKEN_IDENTIFIER;
            
            advance_chars(tokenizer, 1);
            
            while (is_alpha(tokenizer->at[0]) ||
                   is_numeric(tokenizer->at[0]) ||
                   (tokenizer->at[0] == '_')) {
                advance_chars(tokenizer, 1);
            }
        }
    } else {
        char c = tokenizer->at[0];
        advance_chars(tokenizer, 1);
        
        switch (c) {
            case '\0': { token.type = TOKEN_END_OF_STREAM; } break;
            
            case '?': { token.type = TOKEN_TERNARY; } break;
            case '[': { token.type = TOKEN_OPEN_BRACKET; } break;
            case ']': { token.type = TOKEN_CLOSE_BRACKET; } break;
            case '(': { token.type = TOKEN_OPEN_PAREN; } break;
            case ')': { token.type = TOKEN_CLOSE_PAREN; } break;
            case '{': { token.type = TOKEN_OPEN_BRACE; } break;
            case '}': { token.type = TOKEN_CLOSE_BRACE; } break;
            case '.': { token.type = TOKEN_PERIOD; } break;
            case '~': { token.type = TOKEN_TILDE; } break;
            case ';': { token.type = TOKEN_SEMI; } break;
            case ',': { token.type = TOKEN_COMMA; } break;
            case '@': { token.type = TOKEN_AT; } break;
            
            case '&': {
                if (tokenizer->at[0] == '&') {
                    token.type = TOKEN_AMP_AMP;
                    advance_chars(tokenizer, 1);
                } else if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_AMP_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_AMP;
                }
            } break;
            
            case '*': {
                if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_STAR_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_STAR;
                }
            } break;
            
            case '+': {
                if (tokenizer->at[0] == '+') {
                    token.type = TOKEN_PLUS_PLUS;
                    advance_chars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_PLUS_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_PLUS;
                }
            } break;
            
            case '-': {
                if (tokenizer->at[0] == '-') {
                    token.type = TOKEN_MINUS_MINUS;
                    advance_chars(tokenizer, 1);
                } else if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_MINUS_EQUAL;
                    advance_chars(tokenizer, 1);
                } else if (tokenizer->at[0] == '>') {
                    token.type = TOKEN_ARROW;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_MINUS;
                }
            } break;
            
            case '!': {
                if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_NOT_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_NOT;
                }
            } break;
            
            case '%': {
                if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_PERCENT_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_PERCENT;
                }
            } break;
            
            case '<': {
                if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_LESS_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = Token_Less;
                }
            } break;
            
            case '>': {
                if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_GREATER_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = Token_Greater;
                }
            } break;
            
            case '^': {
                if (tokenizer->at[0] == '^') {
                    token.type = TOKEN_CARET_CARET;
                    advance_chars(tokenizer, 1);
                } else if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_CARET_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_CARET;
                }
            } break;
            
            case '|': {
                if (tokenizer->at[0] == '|') {
                    token.type = TOKEN_PIPE_PIPE;
                    advance_chars(tokenizer, 1);
                } else if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_PIPE_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_PIPE;
                }
            } break;
            
            case ':': {
                if (tokenizer->at[0] == ':') {
                    token.type = TOKEN_COLON_COLON;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_COLON;
                }
            } break;
            
            case '=': {
                if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_EQUAL_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_EQUAL;
                }
            } break;
            
            case '#': {
                if (tokenizer->at[0] == '#') {
                    token.type = TOKEN_HASH_HASH;
                    advance_chars(tokenizer, 1);
                } else {
                    b32 is_keyword = false;
                    
                    ArrayFor (global_pp_keywords) {
                        if (strings_match_part(tokenizer->input, it.name)) {
                            // TODO(yuval): Copy-and-paste - StringLength is called twice
                            advance_chars(tokenizer, string_length(it.name));
                            token.type = it.type;
                            is_keyword = true;
                            ArrayBreak;
                        }
                    }
                    
                    if (!is_keyword) {
                        token.type = TOKEN_HASH;
                    }
                }
            } break;
            
            case '/': {
                if (tokenizer->at[0] == '/') {
                    token.type = TOKEN_COMMENT;
                    advance_chars(tokenizer, 2);
                    
                    while (tokenizer->at[0] && !is_end_of_line(tokenizer->at[0])) {
                        advance_chars(tokenizer, 1);
                    }
                } else if (tokenizer->at[0] == '*') {
                    token.type = TOKEN_COMEMNT;
                    advance_chars(tokenizer, 2);
                    
                    while (tokenizer->at[0] &&
                           !((tokenizer->at[0] == '*') && (tokenizer->at[1]))) {
                        if (((tokenizer->at[0] == '\r') && (tokenizer->at[1] == '\n')) ||
                            ((tokenizer->at[0] == '\n') && (tokenizer->at[1] == '\r'))) {
                            advance_chars(tokenizer, 1);
                        }
                        
                        if (is_end_of_line(tokenizer->at[0])) {
                            ++tokenizer->line_number;
                        }
                        
                        advance_chars(tokenizer, 1);
                    }
                } else if (tokenizer->at[0] == '=') {
                    token.type = TOKEN_SLASH_EQUAL;
                    advance_chars(tokenizer, 1);
                } else {
                    token.type = TOKEN_SLASH;
                }
            } break;
            
            case '"': {
                token.type = TOKEN_STRING_LITERAL;
                get_literal(&token, tokenizer, '"');
            } break;
            
            case '\'': {
                token.type = TOKEN_CHAR_CONSTANT;
                get_literal(&token, tokenizer, '\'');
            } break;
            
            default: {
                if (is_spacing(C)) {
                    token.type = TOKEN_SPACING;
                    
                    while (is_spacing(tokenizer->at[0]))
                    {
                        advance_chars(tokenizer, 1);
                    }
                }
                else if (is_end_of_line(C)) {
                    token.type = TOKEN_END_OF_LINE;
                    
                    if (((c == '\r') && (tokenizer->at[0] == '\n')) ||
                        ((c == '\n') && (tokenizer->at[0] == '\r'))) {
                        advance_chars(tokenizer, 1);
                    }
                    
                    tokenizer->column_number = 1;
                    ++tokenizer->line_number;
                }
                else if (is_numeric(c)) {
                    f32 number = (f32)(c - '0');
                    
                    while (is_numeric(tokenizer->at[0])) {
                        f32 digit = (f32)(tokenizer->at[0] - '0');
                        number = 10.0f * number + digit;
                        advance_chars(tokenizer, 1);
                    }
                    
                    if (tokenizer->at[0] == '.') {
                        f32 coefficient = 0.1f;
                        
                        while (is_numeric(tokenizer->at[0])) {
                            f32 digit = (f32)(tokenizer->at[0] - '0');
                            number += digit * coefficient;
                            coefficient *= 0.1f;
                            advance_chars(tokenizer, 1);
                        }
                    }
                    
                    token.type = TOKEN_NUMBER;
                    Token.value_f32 = number;
                    Token.value_s32 = round_f32_to_s32(number);
                } else {
                    token.type = TOKEN_UNKNOWN;
                }
            } break;
        }
    }
    
    token.text.count = (tokenizer->input.data - token.text.data);
    
    return token;
}

internal token
get_token(Tokenizer* tokenizer) {
    Token token = {};
    
    for (;;) {
        token = get_token_raw(tokenizer);
        
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
    
    return token;
}

internal b32
get_token_check_type(Tokenizer* tokenizer, Token_Type desired_type, Token* out_token) {
    *out_token = get_token(tokenizer);
    b32 result = (out_token->type == desired_type);
    return Result;
}

internal Token
peek_token(Tokenizer* tokenizer) {
    Tokenizer temp = *tokenizer;
    Token result = get_token(&temp);
    return result;
}

internal token
require_token(Tokenizer* tokenizer, Token_Type desired_type) {
    Token token = get_token(tokenizer);
    
    if (token.type != desired_type) {
        bad_token(&token, "unexpected token type: %S (expected %S)",
                  get_token_type_name(token.type), get_token_type_name(desired_type));
    }
    
    return token;
}

internal b32
optional_token(Tokenizer* tokenizer, Token_Type desired_type, Token* out_token = 0) {
    Token token = peek_token(tokenizer);
    b32 result = (token.type == desired_type);
    
    if (result) {
        Token temp = get_token(tokenizer);
        
        if (out_token) {
            *out_token = temp;
        }
    }
    
    return result;
}

internal tokenizer
tokenize(String filename, String input) {
    Tokenizer result = {};
    
    result.filename = filename;
    result.line_number = 1;
    result.column_number = 1;
    result.input = input;
    result.input_file_data = input;
    refill(&result);
    
    return result;
}

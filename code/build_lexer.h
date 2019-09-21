#if !defined(BUILD_LEXER_H)

#define TOKEN_KINDS \
TOKEN_KIND(UNKNOWN) \
TOKEN_KIND(TERNARY) \
TOKEN_KIND(OPEN_BRACKET) \
TOKEN_KIND(CLOSE_BRACKET) \
TOKEN_KIND(OPEN_PAREN) \
TOKEN_KIND(CLOSE_PAREN) \
TOKEN_KIND(OPEN_BRACE) \
TOKEN_KIND(CLOSE_BRACE) \
TOKEN_KIND(DOT) \
TOKEN_KIND(TILDE) \
TOKEN_KIND(SEMI) \
TOKEN_KIND(COMMA) \
TOKEN_KIND(AT) \
TOKEN_KIND(AMP) \
TOKEN_KIND(AMP_AMP) \
TOKEN_KIND(AMP_EQUAL) \
TOKEN_KIND(STAR) \
TOKEN_KIND(STAR_EQUAL) \
TOKEN_KIND(PLUS) \
TOKEN_KIND(PLUS_PLUS) \
TOKEN_KIND(PLUS_EQUAL) \
TOKEN_KIND(MINUS) \
TOKEN_KIND(MINUS_MINUS) \
TOKEN_KIND(MINUS_EQUAL) \
TOKEN_KIND(NOT) \
TOKEN_KIND(NOT_EQUAL) \
TOKEN_KIND(PERCENT) \
TOKEN_KIND(PERCENT_EQUAL) \
TOKEN_KIND(LESS) \
TOKEN_KIND(LESS_LESS) \
TOKEN_KIND(LESS_EQUAL) \
TOKEN_KIND(GREATER) \
TOKEN_KIND(GREATER_GREATER) \
TOKEN_KIND(GREATER_EQUAL) \
TOKEN_KIND(CARET) \
TOKEN_KIND(CARET_CARET) \
TOKEN_KIND(CARET_EQUAL) \
TOKEN_KIND(PIPE) \
TOKEN_KIND(PIPE_PIPE) \
TOKEN_KIND(PIPE_EQUAL) \
TOKEN_KIND(COLON) \
TOKEN_KIND(COLON_COLON) \
TOKEN_KIND(EQUAL) \
TOKEN_KIND(EQUAL_EQUAL) \
TOKEN_KIND(HASH) \
TOKEN_KIND(HASH_HASH) \
TOKEN_KIND(SLASH) \
TOKEN_KIND(SLASH_EQUAL) \
TOKEN_KIND(COMMENT) \
TOKEN_KIND(STRING_LITERAL) \
TOKEN_KIND(CHAR_CONSTANT) \
TOKEN_KIND(NUMBER) \
TOKEN_KIND(IDENTIFIER) \
TOKEN_KIND(SPACING) \
TOKEN_KIND(END_OF_LINE) \
TOKEN_KIND(END_OF_STREAM)

// TODO(yuval): Maybe add an "export" keyword

#define KEYWORD_TOKEN_KINDS \
KEYWORD_TOKEN_KIND(BOOL_CONSTANT, "true") \
KEYWORD_TOKEN_KIND(CONST, "const") \
KEYWORD_TOKEN_KIND(VOLATILE, "volatile") \
KEYWORD_TOKEN_KIND(IF, "if") \
KEYWORD_TOKEN_KIND(SWITCH, "switch") \
KEYWORD_TOKEN_KIND(FOR, "for") \
KEYWORD_TOKEN_KIND(WHILE, "while") \
KEYWORD_TOKEN_KIND(DO, "do") \
KEYWORD_TOKEN_KIND(ELSE, "else") \
KEYWORD_TOKEN_KIND(CASE, "case") \
KEYWORD_TOKEN_KIND(DEFAULT, "default") \
KEYWORD_TOKEN_KIND(BREAK, "break") \
KEYWORD_TOKEN_KIND(CONTINUE, "continue") \
KEYWORD_TOKEN_KIND(RETURN, "return") \
KEYWORD_TOKEN_KIND(GOTO, "goto") \
KEYWORD_TOKEN_KIND(STRUCT, "struct") \
KEYWORD_TOKEN_KIND(ENUM, "enum") \
KEYWORD_TOKEN_KIND(UNION, "union") \
KEYWORD_TOKEN_KIND(TYPEDEF, "typedef") \
KEYWORD_TOKEN_KIND(SIZEOF, "sizeof") \
KEYWORD_TOKEN_KIND(ALIGNOF, "alignof") \
KEYWORD_TOKEN_KIND(STATIC, "static") \
KEYWORD_TOKEN_KIND(INLINE, "inline") \
KEYWORD_TOKEN_KIND(EXTERN, "extern")

#define PP_KEYWORD_TOKEN_KINDS \
PP_KEYWORD_TOKEN_KIND(PP_INCLUDE, "include") \
PP_KEYWORD_TOKEN_KIND(PP_DEFINE, "define") \
PP_KEYWORD_TOKEN_KIND(PP_IF, "if") \
PP_KEYWORD_TOKEN_KIND(PP_ELIF, "elif") \
PP_KEYWORD_TOKEN_KIND(PP_ELSE, "else") \
PP_KEYWORD_TOKEN_KIND(PP_IFDEF, "ifdef") \
PP_KEYWORD_TOKEN_KIND(PP_IFNDEF, "ifndef") \
PP_KEYWORD_TOKEN_KIND(PP_ENDIF, "endif") \
PP_KEYWORD_TOKEN_KIND(PP_IMPORT, "import") \
PP_KEYWORD_TOKEN_KIND(PP_PRAGMA, "pragma") \
PP_KEYWORD_TOKEN_KIND(PP_UNDEF, "undef") \
PP_KEYWORD_TOKEN_KIND(PP_ERROR, "error") \
PP_KEYWORD_TOKEN_KIND(PP_USING, "using")

#define PP_KEYWORD_TOKEN_KINDS_UPPER \
PP_KEYWORD_TOKEN_KIND(PP_INCLUDE, "INCLUDE") \
PP_KEYWORD_TOKEN_KIND(PP_DEFINE, "DEFINE") \
PP_KEYWORD_TOKEN_KIND(PP_IF, "IF") \
PP_KEYWORD_TOKEN_KIND(PP_ELIF, "ELIF") \
PP_KEYWORD_TOKEN_KIND(PP_ELSE, "ELSE") \
PP_KEYWORD_TOKEN_KIND(PP_IFDEF, "IFDEF") \
PP_KEYWORD_TOKEN_KIND(PP_IFNDEF, "IFNDEF") \
PP_KEYWORD_TOKEN_KIND(PP_ENDIF, "ENDIF") \
PP_KEYWORD_TOKEN_KIND(PP_IMPORT, "IMPORT") \
PP_KEYWORD_TOKEN_KIND(PP_PRAGMA, "PRAGMA") \
PP_KEYWORD_TOKEN_KIND(PP_UNDEF, "UNDEF") \
PP_KEYWORD_TOKEN_KIND(PP_ERROR, "ERROR") \
PP_KEYWORD_TOKEN_KIND(PP_USING, "USING")

struct Token {
    enum Kind {
        NULL,
        
#define TOKEN_KIND(type) type,
        TOKEN_KINDS
#undef TOKEN_KIND
        
#define KEYWORD_TOKEN_KIND(type, ...) type,
            KEYWORD_TOKEN_KINDS
#undef KEYWORD_TOKEN_KIND
        
#define PP_KEYWORD_TOKEN_KIND(type, ...) type,
            PP_KEYWORD_TOKEN_KINDS
#undef PP_KEYWORD_TOKEN_KIND
    } kind;
    
    Code_File file;
    s32 line_number;
    s32 column_number;
    
    String text;
    String file_data;
    
    s32 value_s32;
    // TODO(yuval): Make this an f64!
    f32 value_f32;
};

Token NULL_TOKEN = {};

struct Lexer {
    Code_File file;
    
    s32 line_number;
    s32 column_number;
    
    String input;
    char at[2];
};

// TODO(yuval): Maybe add a better data structure for faster lookup
struct Token_Name_And_Kind {
    const char* name;
    Token::Kind kind;
};

#define BUILD_LEXER_H
#endif // #if !defined(BUILD_LEXER_H)
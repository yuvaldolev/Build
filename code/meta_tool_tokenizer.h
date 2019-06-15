#if !defined(META_TOOL_TOKENIZER_H)

// TODO(yuval): Maybe create a macro for keyword tokens.
// The macro should be named KeywordTokens and have
// many calls to a KeywordToken macro that will define each keyword.
// The KeywordToken maco will be defined before the token_type enum,
// and then undefined after it end. It will also be defined differently
// Before keywords array, and undefined after it.

#define TokenTypes \
TokenType(Unknown) \
TokenType(Ternary) \
TokenType(OpenBracket) \
TokenType(CloseBracket) \
TokenType(OpenParen) \
TokenType(CloseParen) \
TokenType(OpenBrace) \
TokenType(CloseBrace) \
TokenType(Period) \
TokenType(Tilde) \
TokenType(Semi) \
TokenType(Comma) \
TokenType(Amp) \
TokenType(AmpAmp) \
TokenType(AmpEqual) \
TokenType(Star) \
TokenType(StarEqual) \
TokenType(Plus) \
TokenType(PlusPlus) \
TokenType(PlusEqual) \
TokenType(Minus) \
TokenType(MinusMinus) \
TokenType(MinusEqual) \
TokenType(Arrow) \
TokenType(Not) \
TokenType(NotEqual) \
TokenType(Percent) \
TokenType(PercentEqual) \
TokenType(Less) \
TokenType(LessEqual) \
TokenType(Greater) \
TokenType(GreaterEqual) \
TokenType(Caret) \
TokenType(CaretCaret) \
TokenType(CaretEqual) \
TokenType(Pipe) \
TokenType(PipePipe) \
TokenType(PipeEqual) \
TokenType(Colon) \
TokenType(ColonColon) \
TokenType(Equal) \
TokenType(EqualEqual) \
TokenType(Hash) \
TokenType(HashHash) \
TokenType(Slash) \
TokenType(SlashEqual) \
TokenType(Comment) \
TokenType(StringLiteral) \
TokenType(CharConstant) \
TokenType(Number) \
TokenType(Identifier) \
TokenType(Spacing) \
TokenType(EndOfLine) \
TokenType(EndOfStream)

// TODO(yuval): Maybe add an "export" keyword

#define KeywordTokenTypes \
KeywordTokenType(BoolConstant, "true") \
KeywordTokenType(Void, "void") \
KeywordTokenType(Bool, "bool") \
KeywordTokenType(Char, "char") \
KeywordTokenType(Int, "int") \
KeywordTokenType(Float, "float") \
KeywordTokenType(Double, "double") \
KeywordTokenType(Long, "long") \
KeywordTokenType(Short, "short") \
KeywordTokenType(Unsigned, "unsigned") \
KeywordTokenType(Const, "const") \
KeywordTokenType(Volatile, "volatile") \
KeywordTokenType(If, "if") \
KeywordTokenType(Switch, "switch") \
KeywordTokenType(For, "for") \
KeywordTokenType(While, "while") \
KeywordTokenType(Do, "do") \
KeywordTokenType(Else, "else") \
KeywordTokenType(Case, "case") \
KeywordTokenType(Default, "default") \
KeywordTokenType(Break, "break") \
KeywordTokenType(Continue, "continue") \
KeywordTokenType(Return, "return") \
KeywordTokenType(Goto, "goto") \
KeywordTokenType(Struct, "struct") \
KeywordTokenType(Enum, "enum") \
KeywordTokenType(Union, "union") \
KeywordTokenType(Typedef, "typedef") \
KeywordTokenType(Static, "static") \
KeywordTokenType(Inline, "inline") \
KeywordTokenType(Extern, "extern") \
KeywordTokenType(PP_Include, "include") \
KeywordTokenType(PP_Define, "define") \
KeywordTokenType(PP_If, "if") \
KeywordTokenType(PP_Elif, "elif") \
KeywordTokenType(PP_Else, "else") \
KeywordTokenType(PP_Ifdef, "ifdef") \
KeywordTokenType(PP_Ifndef, "ifndef") \
KeywordTokenType(PP_Endif, "endif") \
KeywordTokenType(PP_Import, "import") \
KeywordTokenType(PP_Pragma, "pragma") \
KeywordTokenType(PP_Undef, "undef") \
KeywordTokenType(PP_Error, "error") \
KeywordTokenType(PP_Using, "using")

enum token_type
{
#define TokenType(Type) MetaJoin2(Token_, Type),
    TokenTypes
#undef TokenType
    
#define KeywordTokenType(Type, ...) MetaJoin2(Token_, Type),
        KeywordTokenTypes
#undef KeywordTokenType
};

struct token
{
    string FileName;
    
    s32 LineNumber;
    s32 ColumnNumber;
    
    token_type Type;
    string Text;
    string FileData;
    
    f32 F32;
    s32 S32;
};

struct tokenizer
{
    string FileName;
    
    s32 LineNumber;
    s32 ColumnNumber;
    
    string Input;
    string InputFileData;
    char At[2];
    
    b32 Error;
};

#define META_TOOL_TOKENIZER_H
#endif
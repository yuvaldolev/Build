#if !defined(BUILD_TOKENIZER_H)

/* TODO(yuval): Create Default Ast Types For These Keywords:
KeywordTokenType(Void, "void")
KeywordTokenType(Bool, "bool")
KeywordTokenType(Char, "char")
KeywordTokenType(Int, "int")
KeywordTokenType(Float, "float")
KeywordTokenType(Double, "double")
KeywordTokenType(Long, "long")
KeywordTokenType(Short, "short")
KeywordTokenType(Unsigned, "unsigned")
*/

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
TokenType(At) \
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

#define PPKeywordTokenTypes \
PPKeywordTokenType(PP_Include, "include") \
PPKeywordTokenType(PP_Define, "define") \
PPKeywordTokenType(PP_If, "if") \
PPKeywordTokenType(PP_Elif, "elif") \
PPKeywordTokenType(PP_Else, "else") \
PPKeywordTokenType(PP_Ifdef, "ifdef") \
PPKeywordTokenType(PP_Ifndef, "ifndef") \
PPKeywordTokenType(PP_Endif, "endif") \
PPKeywordTokenType(PP_Import, "import") \
PPKeywordTokenType(PP_Pragma, "pragma") \
PPKeywordTokenType(PP_Undef, "undef") \
PPKeywordTokenType(PP_Error, "error") \
PPKeywordTokenType(PP_Using, "using")

#define PPKeywordTokenTypesUpper \
PPKeywordTokenType(PP_Include, "INCLUDE") \
PPKeywordTokenType(PP_Define, "DEFINE") \
PPKeywordTokenType(PP_If, "IF") \
PPKeywordTokenType(PP_Elif, "ELIF") \
PPKeywordTokenType(PP_Else, "ELSE") \
PPKeywordTokenType(PP_Ifdef, "IFDEF") \
PPKeywordTokenType(PP_Ifndef, "IFNDEF") \
PPKeywordTokenType(PP_Endif, "ENDIF") \
PPKeywordTokenType(PP_Import, "IMPORT") \
PPKeywordTokenType(PP_Pragma, "PRAGMA") \
PPKeywordTokenType(PP_Undef, "UNDEF") \
PPKeywordTokenType(PP_Error, "ERROR") \
PPKeywordTokenType(PP_Using, "USING")


enum token_type
{
#define TokenType(Type) MetaJoin2(Token_, Type),
    TokenTypes
#undef TokenType
    
#define KeywordTokenType(Type, ...) MetaJoin2(Token_, Type),
        KeywordTokenTypes
#undef KeywordTokenType
    
#define PPKeywordTokenType(Type, ...) MetaJoin2(Token_, Type),
        PPKeywordTokenTypes
#undef PPKeywordTokenType
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

#define BUILD_TOKENIZER_H
#endif
#if !defined(META_TOOL_TOKENIZER_H)

enum token_type
{
    Token_Unknown = 0,
    
    Token_Ternary,
    Token_OpenBracket,
    Token_CloseBracket,
    Token_OpenParen,
    Token_CloseParen,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_Period,
    Token_Tilde,
    Token_Semi,
    Token_Comma,
    
    Token_Amp,
    Token_AmpAmp,
    Token_AmpEqual,
    
    Token_Star,
    Token_StarEqual,
    
    Token_Plus,
    Token_PlusPlus,
    Token_PlusEqual,
    
    Token_Minus,
    Token_MinusMinus,
    Token_MinusEqual,
    Token_Arrow,
    
    Token_Not,
    Token_NotEqual,
    
    Token_Percent,
    Token_PercentEqual,
    
    Token_Less,
    Token_LessEqual,
    
    Token_Greater,
    Token_GreaterEqual,
    
    Token_Caret,
    Token_CaretCaret,
    Token_CaretEqual,
    
    Token_Pipe,
    Token_PipePipe,
    Token_PipeEqual,
    
    Token_Colon,
    Token_ColonColon,
    
    Token_Equal,
    Token_EqualEqual,
    
    Token_Hash,
    Token_HashHash,
    
    Token_Slash,
    Token_SlashEqual,
    Token_Comment,
    
    Token_StringLiteral,
    Token_CharConstant,
    Token_BoolConstant,
    Token_Number,
    
    Token_Void,
    Token_Bool,
    Toekn_Char,
    Token_Int,
    Token_Float,
    Token_Double,
    
    Token_Long,
    Token_Short,
    Token_Unsigned,
    Token_Const,
    // TODO(yuval): Maybe @Add a "volatile" keyword
    
    Token_If,
    Token_Switch,
    Token_For,
    Token_While,
    Token_Do,
    Token_Else,
    Token_Case,
    Token_Default,
    Token_Break,
    Token_Continue,
    Token_Return,
    Token_Goto,
    
    Token_Struct,
    Token_Enum,
    Token_Union,
    Token_Typedef,
    
    Token_Static,
    Token_Inline,
    Token_Extern,
    
    Token_PP_Include,
    Token_PP_Define,
    Token_PP_If,
    Token_PP_Elif,
    Token_PP_Else,
    Token_PP_Ifdef,
    Token_PP_Ifndef,
    Token_PP_Endif,
    Token_PP_Import,
    Token_PP_Pragma,
    Token_PP_Undef,
    Token_PP_Error,
    Token_PP_Using,
    
    Token_Identifier,
    
    Token_Spacing,
    Token_EndOfLine,
    
    Token_EndOfStream
};

struct token
{
    string FileName;
    
    s32 ColumnNumber;
    s32 LineNumber;
    
    token_type Type;
    string Text;
    string FileData;
    
    f32 F32;
    s32 S32;
};

struct tokenizer
{
    string FileName;
    
    s32 ColumnNumber;
    s32 LineNumber;
    
    string Input;
    string InputFileData;
    char At[2];
    
    b32 Error;
};

#define META_TOOL_TOKENIZER_H
#endif
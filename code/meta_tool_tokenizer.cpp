// TODO(yuval): Maybe @Add a better data structure for faster lookup
struct token_name_and_type
{
    const char* Name;
    token_type Type;
};

// TODO(yuval): Separate the keyword array into two arrays: One for the "regular" keywords
// and one for the preprocessor keywords. This must be done to avoid duplicates like if / else.
// TODO(yuval): Maybe @Add support for LINE, FILE, etc. preprocessor tokens
// TODO(yuval): @Add C++ support (class, new, delete, try, catch...)
global_variable token_name_and_type GlobalKeywords[] = {
#define KeywordTokenType(Type, Name) {Name, MetaJoin2(Token_, Type)},
    KeywordTokenTypes
#undef KeywordTokenType
    
        // NOTE(yuval): BoolConstant can be true as well as false
    {"false", Token_BoolConstant}
};

internal string
GetTokenTypeName(token_type Type)
{
    switch (Type)
    {
#define TokenType(Type) case MetaJoin2(Token_, Type): { return BundleZ(#Type); }
        TokenTypes
#undef TokenType
        
#define KeywordTokenType(Type, ...) case MetaJoin2(Token_, Type): { return BundleZ(#Type); }
            KeywordTokenTypes
#undef KeywordTokenType
    }
    
    return BundleZ("Unknown");
}

internal b32
TokenEquals(token Token, const char* Match)
{
    b32 Result = StringsAreEqual(Token.Text, Match);
    return Result;
}

internal void
Refill(tokenizer* Tokenizer)
{
    if (Tokenizer->Input.Count == 0)
    {
        Tokenizer->At[0] = 0;
        Tokenizer->At[1] = 0;
    }
    else if (Tokenizer->Input.Count == 1)
    {
        Tokenizer->At[0] = Tokenizer->Input.Data[0];
        Tokenizer->At[1] = 0;
    }
    else
    {
        Tokenizer->At[0] = Tokenizer->Input.Data[0];
        Tokenizer->At[1] = Tokenizer->Input.Data[1];
    }
}

internal void
AdvanceChars(tokenizer* Tokenizer, u32 Count)
{
    Tokenizer->ColumnNumber += Count;
    AdvanceString(&Tokenizer->Input, Count);
    Refill(Tokenizer);
}

internal void
GetLiteral(token* Token, tokenizer* Tokenizer, char Enclosing)
{
    while (Tokenizer->At[0] &&
           Tokenizer->At[0] != Enclosing)
    {
        if ((Tokenizer->At[0] == '\\') &&
            Tokenizer->At[1])
        {
            AdvanceChars(Tokenizer, 1);
        }
        
        AdvanceChars(Tokenizer, 1);
    }
    
    if (Tokenizer->At[0] == Enclosing)
    {
        AdvanceChars(Tokenizer, 1);
    }
    else
    {
        BadToken(Token, "unclosed literal");
    }
}

internal token
GetTokenRaw(tokenizer* Tokenizer)
{
    token Token = {};
    Token.FileName = Tokenizer->FileName;
    Token.LineNumber = Tokenizer->LineNumber;
    Token.ColumnNumber = Tokenizer->ColumnNumber;
    Token.Text = Tokenizer->Input;
    Token.FileData = Tokenizer->InputFileData;
    
    if (IsAlpha(Tokenizer->At[0]))
    {
        b32 IsKeyword = false;
        
        For (GlobalKeywords)
        {
            ToLowercase(Tokenizer->Input);
            
            if (StringsAreEqual(Tokenizer->Input, It->Name,
                                StringLength(It->Name)))
            {
                // TODO(yuval): @Copy-and-paste - StringLength is called twice
                AdvanceChars(Tokenizer, StringLength(It->Name));
                Token.Type = It->Type;
                IsKeyword = true;
            }
        }
        
        if (!IsKeyword)
        {
            Token.Type = Token_Identifier;
            
            AdvanceChars(Tokenizer, 1);
            
            while (IsAlpha(Tokenizer->At[0]) ||
                   IsNumber(Tokenizer->At[0]) ||
                   (Tokenizer->At[0] == '_'))
            {
                AdvanceChars(Tokenizer, 1);
            }
        }
    }
    else
    {
        char C = Tokenizer->At[0];
        AdvanceChars(Tokenizer, 1);
        
        switch (C)
        {
            case '\0': { Token.Type = Token_EndOfStream; } break;
            
            case '?': { Token.Type = Token_Ternary; } break;
            case '[': { Token.Type = Token_OpenBracket; } break;
            case ']': { Token.Type = Token_CloseBracket; } break;
            case '(': { Token.Type = Token_OpenParen; } break;
            case ')': { Token.Type = Token_CloseParen; } break;
            case '{': { Token.Type = Token_OpenBrace; } break;
            case '}': { Token.Type = Token_CloseBrace; } break;
            case '.': { Token.Type = Token_Period; } break;
            case '~': { Token.Type = Token_Tilde; } break;
            case ';': { Token.Type = Token_Semi; } break;
            case ',': { Token.Type = Token_Comma; } break;
            
            case '&':
            {
                if (Tokenizer->At[0] == '&')
                {
                    Token.Type = Token_AmpAmp;
                    AdvanceChars(Tokenizer, 1);
                }
                else if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_AmpEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Amp;
                }
            } break;
            
            case '*':
            {
                if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_StarEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Star;
                }
            } break;
            
            case '+':
            {
                if (Tokenizer->At[0] == '+')
                {
                    Token.Type = Token_PlusPlus;
                    AdvanceChars(Tokenizer, 1);
                }
                else if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_PlusEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Plus;
                }
            } break;
            
            case '-':
            {
                if (Tokenizer->At[0] == '-')
                {
                    Token.Type = Token_MinusMinus;
                    AdvanceChars(Tokenizer, 1);
                }
                else if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_MinusEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else if (Tokenizer->At[0] == '>')
                {
                    Token.Type = Token_Arrow;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Minus;
                }
            } break;
            
            case '!':
            {
                if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_NotEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Not;
                }
            } break;
            
            case '%':
            {
                if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_PercentEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Percent;
                }
            } break;
            
            case '<':
            {
                if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_LessEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Less;
                }
            } break;
            
            case '>':
            {
                if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_GreaterEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Greater;
                }
            } break;
            
            case '^':
            {
                if (Tokenizer->At[0] == '^')
                {
                    Token.Type = Token_CaretCaret;
                    AdvanceChars(Tokenizer, 1);
                }
                else if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_CaretEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Caret;
                }
            } break;
            
            case '|':
            {
                if (Tokenizer->At[0] == '|')
                {
                    Token.Type = Token_PipePipe;
                    AdvanceChars(Tokenizer, 1);
                }
                else if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_PipeEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Pipe;
                }
            } break;
            
            case ':':
            {
                if (Tokenizer->At[0] == ':')
                {
                    Token.Type = Token_ColonColon;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Colon;
                }
            } break;
            
            case '=':
            {
                if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_EqualEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Equal;
                }
            } break;
            
            case '#':
            {
                if (Tokenizer->At[0] == '#')
                {
                    Token.Type = Token_HashHash;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Hash;
                }
            } break;
            
            case '/':
            {
                if (Tokenizer->At[0] == '/')
                {
                    Token.Type = Token_Comment;
                    AdvanceChars(Tokenizer, 2);
                    
                    while (Tokenizer->At[0] && !IsEndOfLine(Tokenizer->At[0]))
                    {
                        AdvanceChars(Tokenizer, 1);
                    }
                }
                else if (Tokenizer->At[0] == '*')
                {
                    Token.Type = Token_Comment;
                    AdvanceChars(Tokenizer, 2);
                    
                    while (Tokenizer->At[0] &&
                           !((Tokenizer->At[0] == '*') && (Tokenizer->At[1])))
                    {
                        if (((Tokenizer->At[0] == '\r') && (Tokenizer->At[1] == '\n')) ||
                            ((Tokenizer->At[0] == '\n') && (Tokenizer->At[1] == '\r')))
                        {
                            AdvanceChars(Tokenizer, 1);
                        }
                        
                        if (IsEndOfLine(Tokenizer->At[0]))
                        {
                            ++Tokenizer->LineNumber;
                        }
                        
                        AdvanceChars(Tokenizer, 1);
                    }
                }
                else if (Tokenizer->At[0] == '=')
                {
                    Token.Type = Token_SlashEqual;
                    AdvanceChars(Tokenizer, 1);
                }
                else
                {
                    Token.Type = Token_Slash;
                }
            } break;
            
            case '"':
            {
                Token.Type = Token_StringLiteral;
                GetLiteral(&Token, Tokenizer, '"');
            } break;
            
            case '\'':
            {
                Token.Type = Token_CharConstant;
                GetLiteral(&Token, Tokenizer, '\'');
            } break;
            
            default:
            {
                if (IsSpacing(C))
                {
                    Token.Type = Token_Spacing;
                    
                    while (IsSpacing(Tokenizer->At[0]))
                    {
                        AdvanceChars(Tokenizer, 1);
                    }
                }
                else if (IsEndOfLine(C))
                {
                    Token.Type = Token_EndOfLine;
                    
                    if (((C == '\r') && (Tokenizer->At[0] == '\n')) ||
                        ((C == '\n') && (Tokenizer->At[0] == '\r')))
                    {
                        AdvanceChars(Tokenizer, 1);
                    }
                    
                    Tokenizer->ColumnNumber = 1;
                    ++Tokenizer->LineNumber;
                }
                else if (IsNumber(C))
                {
                    f32 Number = (f32)(C - '0');
                    
                    while (IsNumber(Tokenizer->At[0]))
                    {
                        f32 Digit = (f32)(Tokenizer->At[0] - '0');
                        Number = 10.0f * Number + Digit;
                        AdvanceChars(Tokenizer, 1);
                    }
                    
                    if (Tokenizer->At[0] == '.')
                    {
                        f32 Coefficient = 0.1f;
                        
                        while (IsNumber(Tokenizer->At[0]))
                        {
                            f32 Digit = (f32)(Tokenizer->At[0] - '0');
                            Number += Digit * Coefficient;
                            Coefficient *= 0.1f;
                            AdvanceChars(Tokenizer, 1);
                        }
                    }
                    
                    Token.Type = Token_Number;
                    Token.F32 = Number;
                    Token.S32 = RoundF32ToS32(Number);
                }
                else
                {
                    Token.Type = Token_Unknown;
                }
            } break;
        }
    }
    
    Token.Text.Count = (Tokenizer->Input.Data - Token.Text.Data);
    
    return Token;
}

internal token
GetToken(tokenizer* Tokenizer)
{
    token Token = {};
    
    while (true)
    {
        Token = GetTokenRaw(Tokenizer);
        
        if ((Token.Type == Token_Spacing) ||
            (Token.Type == Token_EndOfLine) ||
            (Token.Type == Token_Comment))
        {
            // NOTE(yuval): These tokens are ignored
        }
        else
        {
            if (Token.Type == Token_StringLiteral)
            {
                if (Token.Text.Count &&
                    (Token.Text.Data[0] == '"'))
                {
                    ++Token.Text.Data;
                    --Token.Text.Count;
                }
                
                if (Token.Text.Count &&
                    Token.Text.Data[Token.Text.Count - 1] == '"')
                {
                    --Token.Text.Count;
                }
            }
            
            if (Token.Type == Token_CharConstant)
            {
                if (Token.Text.Count &&
                    (Token.Text.Data[0] == '\''))
                {
                    ++Token.Text.Data;
                    --Token.Text.Count;
                }
                
                if (Token.Text.Count &&
                    Token.Text.Data[Token.Text.Count - 1] == '\'')
                {
                    --Token.Text.Count;
                }
            }
            
            break;
        }
    }
    
    return Token;
}

internal token
PeekToken(tokenizer* Tokenizer)
{
    tokenizer Temp = *Tokenizer;
    token Result = GetToken(&Temp);
    return Result;
}

internal token
RequireToken(tokenizer* Tokenizer, token_type DesiredType)
{
    token Token = GetToken(Tokenizer);
    
    if (Token.Type != DesiredType)
    {
        // TODO(yuval): @Add the problematic token type and the desired type
        BadToken(&Token, "unexpected token type: %S (expected %S)",
                 GetTokenTypeName(Token.Type), GetTokenTypeName(DesiredType));
    }
    
    return Token;
}

internal b32
OptionalToken(tokenizer* Tokenizer, token_type DesiredType, token* OutToken = 0)
{
    token Token = PeekToken(Tokenizer);
    b32 Result = (Token.Type == DesiredType);
    
    if (Result)
    {
        token Temp = GetToken(Tokenizer);
        
        if (OutToken)
        {
            *OutToken = Temp;
        }
    }
    
    return Result;
}

internal tokenizer
Tokenize(string FileName, string Input)
{
    tokenizer Result = { };
    
    Result.FileName = FileName;
    Result.LineNumber = 1;
    Result.ColumnNumber = 1;
    Result.Input = Input;
    Result.InputFileData = Input;
    Refill(&Result);
    
    return Result;
}

// TODO(yuval): Maybe @Add a better data structure for faster lookup
struct KeywordNameAndType
{
    const char* name;
    TokenType type;
};

global_variable KeywordNameAndType globalKeywords[] = {
    { "true", Token_BoolConstant },
    { "false", Token_BoolConstant },
    
    { "void", Token_Void },
    { "bool", Token_Bool },
    { "char", Toekn_Char },
    { "int", Token_Int },
    { "float", Token_Float },
    { "double", Token_Double },
    
    { "long", Token_Long },
    { "short", Token_Short },
    { "unsigned", Token_Unsigned },
    { "const", Token_Const },
    // TODO(yuval): Maybe @Add a "volatile" keyword
    
    { "if", Token_If },
    { "switch", Token_Switch },
    { "for", Token_For },
    { "while", Token_While },
    { "do", Token_Do },
    { "else", Token_Else },
    { "case", Token_Case },
    { "default", Token_Default },
    { "break", Token_Break },
    { "continue", Token_Continue },
    { "return", Token_Return },
    { "goto", Token_Goto },
    
    { "struct", Token_Struct },
    { "enum", Token_Enum },
    { "union", Token_Union },
    { "typedef", Token_Typedef },
    
    { "static", Token_Static },
    { "inline", Token_Inline },
    { "extern", Token_Extern },
    // TODO(yuval): Maybe @Add an "export" keyword
    
    { "include", Token_PP_Include },
    { "INCLUDE", Token_PP_Include },
    { "define", Token_PP_Define },
    { "DEFINE", Token_PP_Define },
    { "if", Token_PP_If },
    { "IF", Token_PP_If },
    { "elif", Token_PP_Elif },
    { "ELIF", Token_PP_Elif },
    { "else", Token_PP_Else },
    { "ELSE", Token_PP_Else },
    { "ifdef", Token_PP_Ifdef },
    { "IFDEF", Token_PP_Ifdef },
    { "ifndef", Token_PP_Ifndef },
    { "IFNDEF", Token_PP_Ifndef },
    { "endif", Token_PP_Endif },
    { "ENDIF", Token_PP_Endif },
    { "import", Token_PP_Import },
    { "IMPORT", Token_PP_Import },
    { "pragma", Token_PP_Pragma },
    { "PRAGMA", Token_PP_Pragma },
    { "undef", Token_PP_Undef },
    { "UNDEF", Token_PP_Undef },
    { "error", Token_PP_Error },
    { "ERROR", Token_PP_Error },
    { "using", Token_PP_Using },
    { "USING", Token_PP_Using }
    // TODO(yuval): Maybe @Add support for LINE, FILE, etc. preprocessor tokens
    
    // TODO(yuval): @Add C++ support (class, new, delete, try, catch...)
};

b32
TokenEquals(Token token, const char* match)
{
    b32 result = StringsAreEqual(token.text, match);
    return result;
}

internal void
Refill(Tokenizer* tokenizer)
{
    if (tokenizer->input.count == 0)
    {
        tokenizer->at[0] = 0;
        tokenizer->at[1] = 0;
    }
    else if (tokenizer->input.count == 1)
    {
        tokenizer->at[0] = tokenizer->input.data[0];
        tokenizer->at[1] = 0;
    }
    else
    {
        tokenizer->at[0] = tokenizer->input.data[0];
        tokenizer->at[1] = tokenizer->input.data[1];
    }
}

internal void
AdvanceChars(Tokenizer* tokenizer, u32 count)
{
    tokenizer->columnNumber += count;
    AdvanceString(&tokenizer->input, count);
    Refill(tokenizer);
}

internal void
GetLiteral(Token* token, Tokenizer* tokenizer, char enclosing)
{
    while (tokenizer->at[0] &&
           tokenizer->at[0] != enclosing)
    {
        if ((tokenizer->at[0] == '\\') &&
            tokenizer->at[1])
        {
            AdvanceChars(tokenizer, 1);
        }
        
        AdvanceChars(tokenizer, 1);
    }
    
    if (tokenizer->at[0] == enclosing)
    {
        AdvanceChars(tokenizer, 1);
    }
    else
    {
        BadToken(token, "unclosed literal");
    }
}

internal Token
GetTokenRaw(Tokenizer* tokenizer)
{
    Token token = { };
    token.filename = tokenizer->filename;
    token.columnNumber = tokenizer->columnNumber;
    token.lineNumber = tokenizer->lineNumber;
    token.text = tokenizer->input;
    token.fileData = tokenizer->inputFileData;
    
    if (IsAlpha(tokenizer->at[0]))
    {
        b32 isKeyword = false;
        
        ArrayForeachRef(KeywordNameAndType* keyword, globalKeywords)
        {
            if (StringsAreEqual(tokenizer->input, keyword->name,
                                StringLength(keyword->name)))
            {
                // TODO(yuval): @Copy-and-paste - StringLength is called twice
                AdvanceChars(tokenizer, StringLength(keyword->name));
                token.type = keyword->type;
                isKeyword = true;
            }
        }
        
        if (!isKeyword)
        {
            token.type = Token_Identifier;
            
            AdvanceChars(tokenizer, 1);
            
            while (IsAlpha(tokenizer->at[0]) ||
                   IsNumber(tokenizer->at[0]) ||
                   (tokenizer->at[0] == '_'))
            {
                AdvanceChars(tokenizer, 1);
            }
        }
    }
    else
    {
        
        char c = tokenizer->at[0];
        AdvanceChars(tokenizer, 1);
        
        switch (c)
        {
            case '\0': { token.type = Token_EndOfStream; } break;
            
            case '?': { token.type = Token_Ternary; } break;
            case '[': { token.type = Token_OpenBracket; } break;
            case ']': { token.type = Token_CloseBracket; } break;
            case '(': { token.type = Token_OpenParen; } break;
            case ')': { token.type = Token_CloseParen; } break;
            case '{': { token.type = Token_OpenBrace; } break;
            case '}': { token.type = Token_CloseBrace; } break;
            case '.': { token.type = Token_Period; } break;
            case '~': { token.type = Token_Tilde; } break;
            case ';': { token.type = Token_Semi; } break;
            case ',': { token.type = Token_Comma; } break;
            
            case '&':
            {
                if (tokenizer->at[0] == '&')
                {
                    token.type = Token_AmpAmp;
                    AdvanceChars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '=')
                {
                    token.type = Token_AmpEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Amp;
                }
            } break;
            
            case '*':
            {
                if (tokenizer->at[0] == '=')
                {
                    token.type = Token_StarEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Star;
                }
            } break;
            
            case '+':
            {
                if (tokenizer->at[0] == '+')
                {
                    token.type = Token_PlusPlus;
                    AdvanceChars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '=')
                {
                    token.type = Token_PlusEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Plus;
                }
            } break;
            
            case '-':
            {
                if (tokenizer->at[0] == '-')
                {
                    token.type = Token_MinusMinus;
                    AdvanceChars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '=')
                {
                    token.type = Token_MinusEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '>')
                {
                    token.type = Token_Arrow;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Minus;
                }
            } break;
            
            case '!':
            {
                if (tokenizer->at[0] == '=')
                {
                    token.type = Token_NotEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Not;
                }
            } break;
            
            case '%':
            {
                if (tokenizer->at[0] == '=')
                {
                    token.type = Token_PercentEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Percent;
                }
            } break;
            
            case '<':
            {
                if (tokenizer->at[0] == '=')
                {
                    token.type = Token_LessEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Less;
                }
            } break;
            
            case '>':
            {
                if (tokenizer->at[0] == '=')
                {
                    token.type = Token_GreaterEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Greater;
                }
            } break;
            
            case '^':
            {
                if (tokenizer->at[0] == '^')
                {
                    token.type = Token_CaretCaret;
                    AdvanceChars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '=')
                {
                    token.type = Token_CaretEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Caret;
                }
            } break;
            
            case '|':
            {
                if (tokenizer->at[0] == '|')
                {
                    token.type = Token_PipePipe;
                    AdvanceChars(tokenizer, 1);
                }
                else if (tokenizer->at[0] == '=')
                {
                    token.type = Token_PipeEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Pipe;
                }
            } break;
            
            case ':':
            {
                if (tokenizer->at[0] == ':')
                {
                    token.type = Token_ColonColon;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Colon;
                }
            } break;
            
            case '=':
            {
                if (tokenizer->at[0] == '=')
                {
                    token.type = Token_EqualEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Equal;
                }
            } break;
            
            case '#':
            {
                if (tokenizer->at[0] == '#')
                {
                    token.type = Token_HashHash;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Hash;
                }
            } break;
            
            case '/':
            {
                if (tokenizer->at[0] == '/')
                {
                    token.type = Token_Comment;
                    AdvanceChars(tokenizer, 2);
                    
                    while (tokenizer->at[0] && !IsEndOfLine(tokenizer->at[0]))
                    {
                        AdvanceChars(tokenizer, 1);
                    }
                }
                else if (tokenizer->at[0] == '*')
                {
                    token.type = Token_Comment;
                    AdvanceChars(tokenizer, 2);
                    
                    while (tokenizer->at[0] &&
                           !((tokenizer->at[0] == '*') && (tokenizer->at[1])))
                    {
                        if (((tokenizer->at[0] == '\r') && (tokenizer->at[1] == '\n')) ||
                            ((tokenizer->at[0] == '\n') && (tokenizer->at[1] == '\r')))
                        {
                            AdvanceChars(tokenizer, 1);
                        }
                        
                        if (IsEndOfLine(tokenizer->at[0]))
                        {
                            ++tokenizer->lineNumber;
                        }
                        
                        AdvanceChars(tokenizer, 1);
                    }
                }
                else if (tokenizer->at[0] == '=')
                {
                    token.type = Token_SlashEqual;
                    AdvanceChars(tokenizer, 1);
                }
                else
                {
                    token.type = Token_Slash;
                }
            } break;
            
            case '"':
            {
                token.type = Token_StringLiteral;
                GetLiteral(&token, tokenizer, '"');
            } break;
            
            case '\'':
            {
                token.type = Token_CharConstant;
                GetLiteral(&token, tokenizer, '\'');
            } break;
            
            default:
            {
                if (IsSpacing(c))
                {
                    token.type = Token_Spacing;
                    
                    while (IsSpacing(tokenizer->at[0]))
                    {
                        AdvanceChars(tokenizer, 1);
                    }
                }
                else if (IsEndOfLine(c))
                {
                    token.type = Token_EndOfLine;
                    
                    if (((c == '\r') && (tokenizer->at[0] == '\n')) ||
                        ((c == '\n') && (tokenizer->at[0] == '\r')))
                    {
                        AdvanceChars(tokenizer, 1);
                    }
                    
                    tokenizer->columnNumber = 1;
                    ++tokenizer->lineNumber;
                }
                else if (IsNumber(c))
                {
                    f32 number = (f32)(c - '0');
                    
                    while (IsNumber(tokenizer->at[0]))
                    {
                        f32 digit = (f32)(tokenizer->at[0] - '0');
                        number = 10.0f * number + digit;
                        AdvanceChars(tokenizer, 1);
                    }
                    
                    if (tokenizer->at[0] == '.')
                    {
                        f32 coefficient = 0.1f;
                        
                        while (IsNumber(tokenizer->at[0]))
                        {
                            f32 digit = (f32)(tokenizer->at[0] - '0');
                            number += digit * coefficient;
                            coefficient *= 0.1f;
                            AdvanceChars(tokenizer, 1);
                        }
                    }
                    
                    token.type = Token_Number;
                    token.f32 = number;
                    token.s32 = RoundReal32ToInt32(number);
                }
                else
                {
                    token.type = Token_Unknown;
                }
            } break;
        }
    }
    
    token.text.count = (tokenizer->input.data - token.text.data);
    
    return token;
}

Token
GetToken(Tokenizer* tokenizer)
{
    Token token = { };
    
    while (true)
    {
        token = GetTokenRaw(tokenizer);
        
        if ((token.type == Token_Spacing) ||
            (token.type == Token_EndOfLine) ||
            (token.type == Token_Comment))
        {
            // NOTE(yuval): These tokens are ignored
        }
        else
        {
            if (token.type == Token_StringLiteral)
            {
                if (token.text.count &&
                    (token.text.data[0] == '"'))
                {
                    ++token.text.data;
                    --token.text.count;
                }
                
                if (token.text.count &&
                    token.text.data[token.text.count - 1] == '"')
                {
                    --token.text.count;
                }
            }
            
            if (token.type == Token_CharConstant)
            {
                if (token.text.count &&
                    (token.text.data[0] == '\''))
                {
                    ++token.text.data;
                    --token.text.count;
                }
                
                if (token.text.count &&
                    token.text.data[token.text.count - 1] == '\'')
                {
                    --token.text.count;
                }
            }
            
            break;
        }
    }
    
    return token;
}

Token
RequireToken(Tokenizer* tokenizer, TokenType desiredType)
{
    Token token = GetToken(tokenizer);
    
    if (token.type != desiredType)
    {
        // TODO(yuval): @Replace with desired token name
        BadToken(&token, "Unexpected token type");
    }
}

Tokenizer
Tokenize(String filename, String input)
{
    Tokenizer result = { };
    
    result.filename = filename;
    result.columnNumber = 1;
    result.lineNumber = 1;
    result.input = input;
    result.inputFileData = input;
    Refill(&result);
    
    return result;
}

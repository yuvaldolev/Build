internal void
ParseFunction(Token returnType, Token name)
{
    RequireToken(tokenizer, Token_OpenParen);
}

internal void
ParseTopLevelDecl(ASTFile* file, Tokenizer* tokenizer)
{
    Token declSpecifier = GetToken(); // TODO(yuval): @Incomplete what about pointers
    Token name = RequireToken(tokenizer, Token_Identifier);
    
    switch (declSpecifier.type)
    {
        case Token_Struct:
        {
            // TODO(yuval): @Add struct parsing
            // ParseStruct();
        } break;
        
        case Token_Enum:
        {
            // TODO(yuval): @Add enum parsing
            // ParseEnum();
        } break;
        
        case Token_Union:
        {
            // TODO(yuval): @Add union parsing
        } break;
        
        case Token_Typedef:
        {
            // TODO(yuval): @Add typedef parsing
        } break;
        
        default:
        {
            ParseFunction();
        }
    }
    
    
}

#if 0
case Token_Void:
{
    
} break;

case Token_Bool:
{
} break;

case Token_Char:
{
} break;

case Token_Int:
{
} break;

case Token_Float:
{
} break;

case Token_Double:
{
} break;
#endif

ASTFile
ParseFile(Tokenizer* tokenizer)
{
    
}
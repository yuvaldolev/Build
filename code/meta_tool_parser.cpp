#if 0
internal void
ParseFunction(token ReturnType, token Name)
{
    RequireToken(Tokenizer, Token_OpenParen);
}

internal void
ParseTopLevelDecl(ast_file* File, tokenizer* Tokenizer)
{
    token DeclSpecifier = GetToken(); // TODO(yuval): @Incomplete what about pointers
    token Name = RequireToken(Tokenizer, Token_Identifier);
    
    switch (DeclSpecifier.Type)
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

ASTFile
ParseFile(tokenizer* Tokenizer)
{
    
}
#endif
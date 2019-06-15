#define AstGetToken(File) (File)->Token = GetToken(&(File)->Tokenizer)
#define AstRequireToken(File, Type) (File)->Token = RequireToken(&(File)->Tokenizer, Type)
#define AstOptionalToken(File, Type) OptionalToken(&(File)->Tokenizer, Type, &(File)->Token)

internal ast*
AstNew(ast_type Type, ast_file* File, token* Token = 0)
{
    ast* Result = Alloc(ast);
    
    Result->MyType = Type;
    Result->MyFile = File;
    
    if (!Token)
    {
        Token = &File->Token;
    }
    
    Result->MyLine = Token->LineNumber;
    Result->MyColumn = Token->ColumnNumber;
    
    return Result;
}

internal ast*
AstNewDecl(ast_declaration_type Type, ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_Declaration, File, Token);
    Result->Decl.Type = Type;
    return Result;
}

internal ast*
AstNewTypeDef(ast_type_definition_type Type, ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_TypeDefinition, File, Token);
    Result->TypeDef.Type = Type;
    return Result;
}

internal ast*
AstNewStmt(ast_statement_type Type, ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_Statement, File, Token);
    Result->Stmt.Type = Type;
    return Result;
}

internal ast*
AstNewExpr(ast_expression_type Type, ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_Expression, File, Token);
    Result->Expr.Type = Type;
    return Result;
}

internal ast*
FindType(ast* Scope, string TypeName)
{
    ast* CurrScope = Scope;
    
    while (CurrScope)
    {
        For (CurrScope->Block.Decls)
        {
            if (*It)
            {
                string* Name = &(*It)->Decl.Identifier->MyName;
                
                if (StringsAreEqual(*Name, TypeName))
                {
                    ast* Result = (*It)->Decl.TypeDef;
                    return Result;
                }
            }
        }
        
        CurrScope = CurrScope->Block.Parent;
    }
    
    return 0;
}

// NOTE(yuval): Forward declaring ParseDeclaration because
// ParseTypeDeclaration and ParseDeclaration use each other
internal ast*
ParseDeclaration(ast_file* File, ast* Scope);

internal ast*
ParseTypeDeclaration(ast_file* File, ast* ParentScope, ast_type_definition_type Type)
{
    ast* Result = AstNewDecl(AstDecl_Type, File);
    ast_declaration* Decl = &Result->Decl;
    
    AstRequireToken(File, Token_Identifier);
    Decl->Identifier = Alloc(ast_identifier);
    Decl->Identifier->MyName = File->Token.Text;
    
    if (AstOptionalToken(File, Token_OpenBrace))
    {
        Decl->MyScope = AstNew(Ast_Block, File);
        Decl->MyScope->Block.Parent = ParentScope;
        Decl->MyScope->Block.OwningDecl = Result;
        
        Decl->TypeDef = AstNewTypeDef(Type, File);
        ast_type_definition* TypeDef = &Decl->TypeDef->TypeDef;
        
        TypeDef->MyScope = Decl->MyScope;
        
        // NOTE(yuval): Temporary
        ast** Decls = 0;
        u32* DeclIndex = 0;
        
        switch (Type)
        {
            case AstTypeDef_Struct:
            {
                Decls = (ast**)&TypeDef->Struct.Members;
                DeclIndex = &TypeDef->Struct.MemberIndex;
            } break;
            
            case AstTypeDef_Enum:
            {
                Decls = (ast**)&TypeDef->Enum;
                DeclIndex = &TypeDef->Enum.DeclIndex;
            } break;
            
            case AstTypeDef_Union:
            {
                Decls = (ast**)&TypeDef->Union;
                DeclIndex = &TypeDef->Union.DeclIndex;
            } break;
        }
        
        // NOTE(yuval): Member Declarations Parsing
        b32 ParsingDecls = true;
        while (File->Token.Type != Token_CloseBrace)
        {
            AstGetToken(File);
            
            if (File->Token.Type == Token_CloseBrace)
            {
                ParsingDecls = false;
            }
            else if (Decls)
            {
                Decls[*DeclIndex] =
                    ParseDeclaration(File, TypeDef->MyScope);
                ++(*DeclIndex);
            }
        }
    }
    else
    {
        // TODO(yuval): Think about forward declarations
    }
    
    AstRequireToken(File, Token_Semi);
    ParentScope->Block.Decls[ParentScope->Block.DeclIndex++] = Result;
    
    return Result;
}

internal ast*
ParseDeclaration(ast_file* File, ast* Scope)
{
    ast* Result = 0;
    
    switch (File->Token.Type)
    {
        case Token_Typedef:
        {
        } break;
        
        case Token_Identifier:
        {
            ast* Type = FindType(Scope, File->Token.Text);
            if (true)
            {
                int i = 0;
            }
        } break;
        
        case Token_Void:
        case Token_Bool:
        case Token_Char:
        case Token_Int:
        case Token_Float:
        case Token_Double:
        case Token_Long:
        case Token_Short:
        case Token_Unsigned:
        case Token_Const:
        {
            
        } break;
        
        case Token_Struct:
        {
            Result = ParseTypeDeclaration(File, Scope, AstTypeDef_Struct);
        } break;
        
        case Token_Enum:
        {
            Result = ParseTypeDeclaration(File, Scope, AstTypeDef_Enum);
        } break;
        
        case Token_Union:
        {
            Result = ParseTypeDeclaration(File, Scope, AstTypeDef_Union);
        } break;
    }
    
    return Result;
};

internal void
ParseTopLevel(ast_file* File)
{
    ast* Decl = ParseDeclaration(File, &File->GlobalScope);
};

internal ast_file*
ParseFile(string FileName, string FileContents)
{
    ast_file* File = Alloc(ast_file);
    File->Tokenizer = Tokenize(FileName, FileContents);
    
    b32 Parsing = true;
    while (Parsing)
    {
        AstGetToken(File);
        
        switch (File->Token.Type)
        {
            case Token_EndOfStream:
            {
                Parsing = false;
            } break;
            
            default:
            {
                ParseTopLevel(File);
            };
        }
    }
    
    return File;
};
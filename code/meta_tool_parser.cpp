#define AstGetToken(File) (File)->Token = GetToken(&(File)->Tokenizer)
#define AstGetTokenOfType(File, Type) GetTokenOfType(&(File)->Tokenizer, Type, &(File)->Token)
#define AstRequireToken(File, Type) (File)->Token = RequireToken(&(File)->Tokenizer, Type)
#define AstOptionalToken(File, Type) OptionalToken(&(File)->Tokenizer, Type, &(File)->Token)

#define DefaultTypes \
DefaultType(Void, BundleZ("void")) \
DefaultType(Bool, BundleZ("bool")) \
DefaultType(Char, BundleZ("char")) \
DefaultType(Int, BundleZ("int")) \
DefaultType(Float, BundleZ("float")) \
DefaultType(Double, BundleZ("double")) \
DefaultType(Long, BundleZ("long")) \
DefaultType(Short, BundleZ("short")) \
DefaultType(Unsigned, BundleZ("Unsigned"))

#define DefaultType(Type, ...) global_variable ast* MetaJoin2(TypeDef, Type);
DefaultTypes
#undef DefaultType

#define BeginAstDumpBlock ++GlobalIndentation;
#define EndAstDumpBlock --GlobalIndentation;

global_variable u32 GlobalIndentation = 0;

internal ast*
AstNew(ast_type Type, ast_file* File, token* Token = 0)
{
    ast* Result = Alloc(ast);
    u32 UnsignedInt = 0;
    
    Result->Type = Type;
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
AstNewDecl(ast_declaration_type Type,
           ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_Declaration, File, Token);
    Result->Decl.Type = Type;
    return Result;
}

internal ast*
AstNewTypeDef(ast_type_definition_type Type,
              ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_TypeDefinition, File, Token);
    Result->TypeDef.Type = Type;
    return Result;
}

internal int*
Test()
{
}

internal ast*
AstNewStmt(ast_statement_type Type,
           ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_Statement, File, Token);
    Result->Stmt.Type = Type;
    return Result;
}

internal ast*
AstNewExpr(ast_expression_type Type,
           ast_file* File, token* Token = 0)
{
    ast* Result = AstNew(Ast_Expression, File, Token);
    Result->Expr.Type = Type;
    return Result;
}

internal void
InitDefaultTypes()
{
#define DefaultType(TypeName, Name) MetaJoin2(TypeDef, TypeName) = Alloc(ast); \
    MetaJoin2(TypeDef, TypeName)->Type = Ast_TypeDefinition; \
    MetaJoin2(TypeDef, TypeName)->TypeDef.Type = AstTypeDef_Default; \
    MetaJoin2(TypeDef, TypeName)->TypeDef.DefaultTypeName = Name;
    
    DefaultTypes
    
#undef DefaultType
}

internal ast*
GetDefaultType(string TypeName)
{
#define DefaultType(Type, Name) \
    if (StringsAreEqual(TypeName, Name)) \
    { \
        return MetaJoin2(TypeDef, Type); \
    }
    
    DefaultTypes
    
#undef DefaultType
    
        return 0;
}

internal void
IndentLine(u32 nSpaces)
{
    while (nSpaces--)
    {
        printf(" ");
    }
}

internal void
DumpAstDetails(ast* Ast)
{
    string* FileName = &Ast->MyFile->FileName;
    
    printf("%p <%.*s:%d:%d> ",
           (void*)Ast,
           (s32)FileName->Count, FileName->Data,
           Ast->MyLine, Ast->MyColumn);
}

internal void
DumpAstDeclaration(ast* DeclAst)
{
    IndentLine(GlobalIndentation);
    
    ast_declaration* Decl = &DeclAst->Decl;
    ast_identifier* Ident = Decl->Identifier;
    
    switch (Decl->Type)
    {
        case AstDecl_Type:
        {
            printf("TypeDecl ");
            DumpAstDetails(DeclAst);
            printf("%.*s ",
                   (s32)Ident->MyName.Count,
                   Ident->MyName.Data);
            
            ast* TypeDefAst = Decl->MyType;
            ast_type_definition* TypeDef = &TypeDefAst->TypeDef;
            
            ast** Decls = 0;
            u32 DeclsCount = 0;
            
            switch (TypeDef->Type)
            {
                case AstTypeDef_Pointer:
                {
                } break;
                
                case AstTypeDef_Struct:
                {
                    printf("'struct'");
                    
                    Decls = (ast**)TypeDef->Struct.Members;
                    DeclsCount = TypeDef->Struct.MemberIndex;
                } break;
                
                case AstTypeDef_Enum:
                {
                    printf("'enum'");
                    
                    Decls = (ast**)TypeDef->Enum.Decls;
                    DeclsCount = TypeDef->Enum.DeclIndex;
                } break;
                
                case AstTypeDef_Union:
                {
                    printf("'union'");
                    
                    Decls = (ast**)TypeDef->Union.Decls;
                    DeclsCount = TypeDef->Union.DeclIndex;
                } break;
            }
            
            printf("\n");
            
            if (Decls)
            {
                BeginAstDumpBlock;
                
                For (DeclIndex, Range(DeclsCount))
                {
                    DumpAstDeclaration(Decls[DeclIndex]);
                }
                
                EndAstDumpBlock;
            }
            
        } break;
        
        case AstDecl_Func:
        {
        } break;
        
        case AstDecl_Var:
        {
            printf("VarDecl ");
            DumpAstDetails(DeclAst);
            printf("%.*s ",
                   (s32)Ident->MyName.Count, Ident->MyName.Data);
            
            
            ast* TypeDefAst = Decl->MyType;
            ast_type_definition* TypeDef = &TypeDefAst->TypeDef;
            
            string* TypeName;
            
            switch (TypeDef->Type)
            {
                case AstTypeDef_Default:
                {
                    TypeName = &TypeDef->DefaultTypeName;
                } break;
                
                case AstTypeDef_Pointer:
                {
                    // TODO(yuval): Handle Pointers
                    TypeName = 0;
                } break;
                
                default:
                {
                    TypeName = &TypeDef->MyDecl->Decl.Identifier->MyName;
                } break;
            }
            
            if (TypeName)
            {
                printf("'%.*s'",
                       (s32)TypeName->Count, TypeName->Data);
            }
            
            printf("\n");
        } break;
    }
}

internal void
DumpAstFile(ast_file* File)
{
    ast_block* GlobalScope = &File->GlobalScope.Block;
    
    // NOTE(yuval): AST Dumping
    For (DeclIndex, Range(GlobalScope->DeclIndex))
    {
        DumpAstDeclaration(GlobalScope->Decls[DeclIndex]);
    }
}

internal ast*
FindType(ast* Scope, string TypeName)
{
    ast* Result = GetDefaultType(TypeName);
    
    if (!Result)
    {
        ast* CurrScope = Scope;
        
        b32 Searching = true;
        while (Searching && CurrScope)
        {
            For (CurrScope->Block.Decls)
            {
                if (It)
                {
                    string* Name = &It->Decl.Identifier->MyName;
                    
                    if ((It->Decl.Type == AstDecl_Type) &&
                        (StringsAreEqual(*Name, TypeName)))
                    {
                        Result = It->Decl.MyType;
                        Searching = false;
                        break;
                    }
                }
            }
            
            CurrScope = CurrScope->Block.Parent;
        }
    }
    
    return Result;
}

// NOTE(yuval): Forward declaring ParseDeclaration because
// ParseTypeDeclaration and ParseDeclaration use each other
internal ast*
ParseDeclaration(ast_file* File, ast* Scope);

internal ast*
ParseStatement(ast_file* File, ast* Scope)
{
    
}

internal ast*
ParseCompoundStatement(ast_file* File, ast* Scope)
{
    AstRequireToken(File, Token_OpenBrace);
    
    ast* FirstStmt = ParseStatement(File, Scope);
    ast* PrevStmt = FirstStmt;
    
    while (!AstGetTokenOfType(File, Token_CloseBrace))
    {
        ast* Stmt = ParseStatement(File, Scope);
        Stmt->Left = PrevStmt;
        PrevStmt->Right = Stmt;
        PrevStmt = Stmt;
    }
    
    return FirstStmt;
}

internal ast*
ParseTypeDeclaration(ast_file* File, ast* ParentScope,
                     ast_type_definition_type Type)
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
        
        Decl->MyType = AstNewTypeDef(Type, File);
        ast_type_definition* TypeDef = &Decl->MyType->TypeDef;
        TypeDef->MyDecl = Result;
        
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
                Decls = (ast**)&TypeDef->Enum.Decls;
                DeclIndex = &TypeDef->Enum.DeclIndex;
            } break;
            
            case AstTypeDef_Union:
            {
                Decls = (ast**)&TypeDef->Union.Decls;
                DeclIndex = &TypeDef->Union.DeclIndex;
            } break;
        }
        
        // NOTE(yuval): Member Declarations Parsing
        while (!AstGetTokenOfType(File, Token_CloseBrace))
        {
            Decls[*DeclIndex] = ParseDeclaration(File, Decl->MyScope);
            ++(*DeclIndex);
            
            AstRequireToken(File, Token_Semi);
        }
    }
    else
    {
        // TODO(yuval): Think about forward declarations
    }
    
    return Result;
}

internal ast*
ParseDeclaration(ast_file* File, ast* Scope)
{
    ast* Result = 0;
    
    // NOTE(yuval): Tag Parsing
    ast_tag* Tags[16] = {};
    u32 TagIndex = 0;
    
    while (File->Token.Type == Token_At)
    {
        AstRequireToken(File, Token_Identifier);
        ast_tag* Tag = Alloc(ast_tag);
        Tag->Tag = File->Token.Text;
        Tags[TagIndex++] = Tag;
        AstGetToken(File);
    }
    
    switch (File->Token.Type)
    {
        case Token_Typedef:
        {
        } break;
        
        case Token_Const:
        {
        } break;
        
        case Token_Identifier:
        {
            ast* Type = FindType(Scope, File->Token.Text);
            
            if (!Type)
            {
                BadToken(&File->Token, "use of undeclared identifier '%S'",
                         File->Token.Text);
            }
            
            AstRequireToken(File, Token_Identifier);
            ast_identifier* Identifier = Alloc(ast_identifier);
            Identifier->MyName = File->Token.Text;
            
            if (AstOptionalToken(File, Token_OpenParen))
            {
                Result = AstNewDecl(AstDecl_Func, File);
                
                // TODO(yuval): Maybe unite this code with the variable declaration code below?
                ast_declaration* Decl = &Result->Decl;
                Decl->Identifier = Identifier;
                Decl->MyScope = AstNew(Ast_Block, File);
                Decl->MyScope->Block.Parent = Scope;
                
                ast_function* Func = &Decl->Func;
                Func->ReturnType = Type;
                
                // NOTE(yuval): Function Declaration Parsing
                while (!AstGetTokenOfType(File, Token_CloseParen))
                {
                    Func->Params[Func->ParamIndex++] =
                        ParseDeclaration(File, Scope);
                }
                
                // NOTE(yuval): Function Body Parsing
                Func->MyBody = ParseCompoundStatement(File, Decl->MyScope);
            }
            else
            {
                // NOTE(yuval): Variable Declaration Parsing
                // TODO(yuval): Handle default value assignment
                Result = AstNewDecl(AstDecl_Var, File);
                
                ast_declaration* Decl = &Result->Decl;
                Decl->Identifier = Identifier;
                Decl->MyScope = Scope;
                Decl->MyType = Type;
            }
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
    
    if (Result)
    {
        Copy(Result->Decl.MyTags, Tags, sizeof(Tags));
        Scope->Block.Decls[Scope->Block.DeclIndex++] = Result;
    }
    
    return Result;
}

internal void
ParseTopLevel(ast_file* File)
{
    ast* Decl = ParseDeclaration(File, &File->GlobalScope);
    
    if (Decl->Decl.Type == AstDecl_Type ||
        Decl->Decl.Type == AstDecl_Var)
    {
        AstRequireToken(File, Token_Semi);
    }
}

internal ast_file*
ParseFile(string FileName, string FileContents)
{
    ast_file* File = Alloc(ast_file);
    File->FileName = FileName;
    File->FileData = FileContents;
    File->Tokenizer = Tokenize(FileName, FileContents);
    
    while (!AstGetTokenOfType(File, Token_EndOfStream))
    {
        ParseTopLevel(File);
    }
    
    return File;
};

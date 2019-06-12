#if !defined(META_TOOL_AST_H)

// TODO(yuval): @Replace all of fixed size arrays with resizable arrays

struct ast_block;
struct ast_statement;

struct ast_expression
{
    
};

struct ast_if
{
    ast_expression* Condition;
    ast_statement* Then;
    ast_statement* Else;
};

struct ast_case
{
    ast_expression* Value; // TODO(yuvak): Should this be a statement???
    ast_statement* Body;
};

struct ast_switch
{
    ast_expression* Condition;
    ast_case* FirstCase;
};

struct ast_for
{
    ast_statement* Init;
    ast_expression* Condition;
    ast_expression* Inc;
};

struct ast_while
{
    ast_expression* Condition;
    ast_statement* Body;
};

struct ast_statement
{
    // TODO(yuval): Handle expression statements (like variable referencing)
    // Maybe create a DeclRefExpr struct?
    
    ast_block* MyParentScope;
    ast_block* MyScope;
    
    ast_statement* Left;
    ast_statement* Right;
    
    // NOTE(yuval): Declaration Statement
    struct ast_declaration* decl;
    
    // NOTE(yuval): If Statement
    ast_if* IfStmt;
    
    // NOTE(yuval): Switch Statement
    ast_switch* SwitchStmt;
    
    // NOTE(yuval): For Statement
    ast_for* ForStmt;
    
    // NOTE(yuval): While Statement
    ast_while* WhileStmt;
};

struct ast_tag
{
};

struct ast_identifier
{
    
};

struct ast_declaration
{
    ast_identifier* Identifier;
    ast_block* MyParentScope;
    ast_block* MyScope;
    struct ast_type_definition* MyType;
    
    // NOTE(yuval): Tags
    ast_tag MyTags[16];
    
    // NOTE(yuval): Functions
    struct ast_function* Function;
    
    // NOTE(yuval): Variables
    ast_expression Expression;
};

struct ast_union
{
    ast_block* Scope;
    
    // NOTE(yuval): Declarations
    ast_declaration Declarations[512];
};

struct ast_enum
{
    ast_block* Scope;
    
    // NOTE(yuval): Declarations
    ast_declaration Declarations[512];
};

struct ast_struct
{
    ast_block* Scope;
    
    // NOTE(yuval): Members
    ast_declaration Members[512];
};

struct ast_type_definition
{
    // NOTE(yuval): Struct
    ast_struct* StructDesc;
    
    // NOTE(yuval): Enum
    ast_enum* EnumDecs;
    
    // NOTE(yuval): Union
    ast_union* UnionDesc;
    
    // NOTE(yuval): Pointer
    ast_type_definition* PointerTo;
    s32 PointerLevel;
};

struct ast_function
{
    // NOTE(yuval): If the function is both declared and defined
    b32 IsFunctionDefinition;
    
    ast_type_definition* ReturnType;
    ast_declaration Params[512];
    ast_statement* MyBody;
};

struct ast_block
{
    struct ast_file* MyFile;
    ast_block* Parent;
    ast_declaration* BelongsToDecl;
    
    ast_declaration Decls[512];
    
    // TODO(yuval): @Add array for child blocks
};

struct ast_file
{
    string FileName;
    ast_block* GlobalScope;
};

#define META_TOOL_AST_H
#endif
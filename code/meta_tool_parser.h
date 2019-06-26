#if !defined(META_TOOL_PARSER_H)

struct ast;
struct ast_file;

// TODO(yuval): @Replace all of fixed size arrays with resizable arrays

///////////////////////////////
//        Expressions        //
///////////////////////////////
enum ast_expression_type
{
    AstExpr_Operator,
    AstExpr_DeclRef,
    AstExpr_Assignment,
    AstExpr_Binary,
    AstExpr_Cast,
    AstExpr_Constant
};

enum ast_operator
{
    AstOp_None,
    
    AstOp_Amp,
    AstOp_AmpAmp,
    AstOp_AmpEqual,
    
    AstOp_Star,
    AstOp_StarEqual,
    
    AstOp_Plus,
    AstOp_PlusPlus,
    AstOp_PlusEqual,
    
    AstOp_Minus,
    AstOp_MinusMinus,
    AstOp_MinusEqual,
    AstOp_Arrow,
    
    AstOp_Not,
    AstOp_NotEqual,
    
    AstOp_Percent,
    AstOp_PercentEqual,
    
    AstOp_Less,
    AstOp_LessEqual,
    
    AstOp_Greater,
    AstOp_GreaterEqual,
    
    AstOp_Caret,
    AstOp_CaretCaret,
    AstOp_CaretEqual,
    
    AstOp_Pipe,
    AstOp_PipePipe,
    AstOp_PipeEqual,
    
    AstOp_Colon,
    AstOp_ColonColon,
    
    AstOp_Equal,
    AstOp_EqualEqual,
    
    AstOp_Hash,
    AstOp_HashHash
};

struct ast_constant
{
    union
    {
        u64 IntConst;
        f64 FloatConst;
        string StringLiteral;
        char CharConstant;
        b32 BoolConstant;
    };
};

struct ast_cast
{
    ast* CastedExpr; // Expression
    ast* CastType; // Expression
};

struct ast_assignment
{
    ast* Decl; // Declaration
    ast* Expression; // Expression
};

struct ast_decl_ref
{
    ast* DeclRef; // Declaration
};

struct ast_expression
{
    ast_expression_type Type;
    
    union
    {
        ast_operator Operator;
        ast_decl_ref DeclRef;
        ast_assignment Assignment;
        ast_cast Cast;
        ast_constant Const;
    };
};

//////////////////////////////
//        Statements        //
//////////////////////////////
enum ast_statement_type
{
    AstStmt_Decl,
    AstStmt_ExprStmt,
    AstStmt_If,
    AstStmt_Switch,
    AstStmt_Case,
    AstStmt_Default,
    AstStmt_For,
    AstStmt_While,
    AstStmt_DoWhile,
    AstStmt_Break,
    AstStmt_Continue,
    AstStmt_Return
};

struct ast_return
{
    ast* Expression;
};

struct ast_while
{
    ast* Condition; // Expression
    ast* Body; // Statement
};

struct ast_for
{
    ast* Init; // Statement
    ast* Condition; // Expression
    ast* Inc; // Expression
    
    ast* Body;
};

struct ast_default
{
    ast* Body; // Statement
};

struct ast_case
{
    ast* Value; // Expression
    ast* Body; // Statement
};

struct ast_switch
{
    ast* Condition; // Expression
    ast* FirstCase; // Case
};

struct ast_if
{
    ast* Condition; // Expression
    ast* Then; // Statement
    ast* Else; // Statement
};

struct ast_expr_statement
{
    
};

struct ast_decl_statement
{
    ast* Decl; // Declaration
    ast* Expression; // Expression
};

struct ast_statement
{
    ast_statement_type Type;
    ast* MyScope; // Block
    
    union
    {
        ast_decl_statement DeclStmt;
        ast_assignment Assignment;
        ast_expr_statement ExprStmt;
        ast_if If;
        ast_switch Switch;
        ast_case Case;
        ast_default Default;
        ast_for ForStmt;
        ast_while While;
        ast_return Return;
    };
};

//////////////////////////////////
//       Type Definitions       //
//////////////////////////////////
enum ast_type_definition_type
{
    AstTypeDef_Default,
    AstTypeDef_Pointer,
    AstTypeDef_Struct,
    AstTypeDef_Enum,
    AstTypeDef_Union,
};

struct ast_union
{
    // NOTE(yuval): Declarations
    ast* Decls[512]; // Declaration
    u32 DeclIndex;
};

struct ast_enum
{
    // NOTE(yuval): Declarations
    ast* Decls[512]; // Declaration
    u32 DeclIndex;
};

struct ast_struct
{
    // NOTE(yuval): Members
    ast* Members[512]; // Declaration
    u32 MemberIndex;
};

struct ast_type_definition
{
    ast_type_definition_type Type;
    
    ast* MyDecl; // Declaration
    
    // NOTE(yuval): Pointer
    ast* PointerTo; // Type Definition
    s32 PointerLevel;
    
    union
    {
        string DefaultTypeName;
        ast_struct Struct;
        ast_enum Enum;
        ast_union Union;
    };
};

////////////////////////////////
//        Declarations        //
////////////////////////////////
enum ast_declaration_type
{
    AstDecl_Type,
    AstDecl_Func,
    AstDecl_Var
};

struct ast_tag
{
    string Tag;
};

struct ast_identifier
{
    string MyName;
};

struct ast_function
{
    // NOTE(yuval): If the function is both declared and defined
    b32 IsFunctionDefinition;
    
    ast* ReturnType; // Type Definition
    
    ast* Params[512]; // Declaration
    u32 ParamIndex;
    
    ast* MyBody; // Statement
};

struct ast_declaration
{
    ast_declaration_type Type;
    
    ast_identifier* Identifier;
    ast* MyScope; // Block
    
    // NOTE(yuval): Tags
    ast_tag* MyTags[16];
    
    union
    {
        ast* MyType; // Type Definition
        ast_function Func;
    };
};

///////////////////////////////
//           Block           //
///////////////////////////////
struct ast_block
{
    ast* Parent; // Block
    ast* OwningDecl; // Declaration
    
    ast* Decls[512]; // Declaration
    u32 DeclIndex;
    
    // TODO(yuval): @Add array for child blocks
};

/////////////////////////////
//           AST           //
/////////////////////////////
enum ast_type
{
    Ast_Block,
    Ast_Declaration,
    Ast_TypeDefinition,
    Ast_Statement,
    Ast_Expression
};

struct ast
{
    ast_type Type;
    
    ast_file* MyFile;
    s32 MyLine;
    s32 MyColumn;
    
    ast* Left;
    ast* Right;
    
    union
    {
        ast_block Block;
        ast_declaration Decl;
        ast_type_definition TypeDef;
        ast_statement Stmt;
        ast_expression Expr;
    };
};

/////////////////////////////
//        AST File         //
/////////////////////////////
struct ast_file
{
    string FileName;
    string FileData;
    
    ast GlobalScope; // Block
    
    tokenizer Tokenizer;
    token Token;
};

#define META_TOOL_PARSER_H
#endif
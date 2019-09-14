#if !defined(BUILD_PARSER_H)

struct Ast;
struct Ast_Translation_Unit;

// TODO(yuval): @Replace all of fixed size arrays with resizable arrays

///////////////////////////////
//        Expressions        //
///////////////////////////////
enum Ast_Expression_Type {
    AST_EXPR_DECL_REF,
    AST_EXPR_ASSIGNMENT,
    AST_EXPR_CONDITIONAL,
    AST_EXPR_BINARY,
    AST_EXPR_CAST,
    AST_EXPR_CONSTANT,
    AST_EXPR_TERNARY
};

enum Ast_Operator {
    AST_OP_NONE,
    
    AST_OP_PLUS,
    AST_OP_PLUS_PLUS,
    AST_OP_PLUS_EQUAL,
    
    AST_OP_MINUS,
    AST_OP_MINUS_MINUS,
    AST_OP_MINUS_EQUAL,
    
    AST_OP_MUL,
    AST_OP_MUL_EQUAL,
    
    AST_OP_DIV,
    AST_OP_DIV_EQUAL,
    
    AST_OP_MOD,
    AST_OP_MOD_EQUAL,
    
    AST_OP_AND,
    AST_OP_AND_AND,
    AST_OP_AND_EQUAL,
    
    AST_OP_OR,
    AST_OP_OR_OR,
    AST_OP_OR_EQUAL,
    
    AST_OP_XOR,
    AST_OP_XOR_XOR,
    AST_OP_XOR_EQUAL,
    
    AST_OP_EQUAL,
    AST_OP_EQUAL_EQUAL,
    
    AST_OP_NOT,
    AST_OP_NOT_EQUAL,
    
    AST_OP_LESS,
    AST_OP_LESS_EQUAL,
    
    AST_OP_GREATER,
    AST_OP_GREATER_EQUAL,
    
    AST_OP_ARROW,
    
    AST_OP_HASH,
    AST_OP_HASH_HASH
};

struct Ast_Ternary {
    Ast* condition_expr; // Expression
    Ast* then_expr; // Expression
    Ast* else_expr; // Expression
};

struct Ast_Constant {
    union {
        u64 int_constant;
        f64 float_constant;
        String string_literal;
        char char_constant;
        b32 bool_constant;
    };
};

struct Ast_Cast {
    Ast* casted_expr; // Expression
    Ast* cast_type; // Expression
};

# if 0
struct Ast_Assignment {
    Ast* decl; // Declaration
    Ast_Operator op;
    Ast* expr; // Expression
};
#endif

struct Ast_Decl_Ref {
    Ast* decl_ref; // Declaration
};

struct Ast_Expression {
    Ast_Expression_Type type;
    
    union {
        Ast_Operator op;
        Ast_Decl_Ref decl_ref;
        Ast_Assignment assignment;
        Ast_Cast cast;
        Ast_Constant constant;
        Ast_Ternary ternary;
    };
};

//////////////////////////////
//        Statements        //
//////////////////////////////
enum Ast_Statement_Type {
    AST_STMT_DECL,
    AST_STMT_EXPR_STMT,
    AST_STMT_IF,
    AST_STMT_SWITCH,
    AST_STMT_CASE,
    AST_STMT_DEFAULT,
    AST_STMT_FOR,
    AST_STMT_WHILE,
    AST_STMT_DO_WHILE,
    AST_STMT_BREAK,
    AST_STMT_CONTINUE,
    AST_STMT_RETURN
};

struct Ast_Return {
    Ast* expr;
};

struct Ast_While {
    Ast* condition; // Expression
    Ast* body; // Statement
};

struct Ast_For {
    Ast* init; // Statement
    Ast* condition; // Expression
    Ast* inc; // Expression
    
    Ast* body;
};

struct Ast_Default {
    Ast* body; // Statement
};

struct Ast_Case {
    Ast* value; // Expression
    Ast* body; // Statement
};

struct Ast_Switch {
    Ast* condition_expr; // Expression
    Ast* first_case; // Case
};

struct Ast_If {
    Ast* condition_expr; // Expression
    Ast* then_stmt; // Statement
    Ast* else_stmt; // Statement
};

struct Ast_Expr_Statement {
    
};

struct Ast_Decl_Statement {
    Ast* decl; // Declaration
    Ast* expression; // Expression
};

struct Ast_Statement {
    Ast_Statement_Type type;
    Ast* my_scope; // Block
    
    union {
        Ast_Decl_Statement decl_stmt;
        Ast_Assignment assignment;
        Ast_Expr_Statement expr_stmt;
        Ast_If if_stmt;
        Ast_Switch switch_stmt;
        Ast_Case case_stmt;
        Ast_Default default_stmt;
        Ast_For for_stmt;
        Ast_While while_stmt;
        Ast_Return return_stmt;
    };
};

//////////////////////////////////
//       Type Definitions       //
//////////////////////////////////
enum Ast_Type_Definition_Type {
    AST_TYPE_DEF_DEFAULT,
    AST_TYPE_DEF_POINTER,
    AST_TYPE_DEF_STRUCT,
    AST_TYPE_DEF_ENUM,
    AST_TYPE_DEF_UNION
};

struct Ast_Union {
    // NOTE(yuval): Declarations
    Ast* decls[512]; // Declaration
    umm decl_count;
};

struct Ast_Enum {
    // NOTE(yuval): Declarations
    Ast* decls[512]; // Declaration
    umm decl_count;
};

struct Ast_Struct {
    // NOTE(yuval): Members
    Ast* members[512]; // Declaration
    umm member_count;
};

struct Ast_Type_Definition {
    Ast_Type_Definition_Type type;
    
    Ast* my_decl; // Declaration
    
    // NOTE(yuval): Pointer
    Ast* pointer_to; // Type Definition
    s32 pointer_level;
    
    union {
        String default_type_name;
        Ast_Struct struct_type_def;
        Ast_Enum enum_type_def;
        Ast_Union union_type_def;
    };
};

////////////////////////////////
//        Declarations        //
////////////////////////////////
enum Ast_Declaration_Type {
    AST_DECL_TYPE,
    AST_DECL_FUNC,
    AST_DECL_VAR
};

struct Ast_Tag {
    String tag;
};

struct Ast_Identifier {
    String name;
};

struct Ast_Function {
    // NOTE(yuval): If the function is both declared and defined
    b32 is_function_definition;
    
    Ast* return_type; // Type Definition
    
    Ast* params[512]; // Declaration
    umm param_count;
    
    Ast* my_body; // Statement
};

struct Ast_Declaration {
    Ast_Declaration_Type type;
    
    Ast_Identifier* my_identifier;
    Ast* my_scope; // Block
    
    // NOTE(yuval): Tags
    Ast_Tag* my_tags[16];
    
    union {
        Ast* my_type; // Type Definition
        Ast_Function func;
    };
};

///////////////////////////////
//           Block           //
///////////////////////////////
struct Ast_Block {
    Ast* parent; // Block
    Ast* owning_decl; // Declaration
    
    Ast* decls[512]; // Declaration
    umm decl_count;
    
    // TODO(yuval): @Add array for child blocks
};

/////////////////////////////
//           AST           //
/////////////////////////////
enum Ast_Type {
    AST_BLOCK,
    AST_DECLARATION,
    AST_TYPE_DEFINITION,
    AST_STATEMENT,
    AST_EXPRESSION
};

struct Ast {
    Ast_Type type;
    
    Ast_Translation_Unit* my_translation_unit;
    
    Code_File my_file;
    s32 my_line;
    s32 my_column;
    
    Ast* lhs;
    Ast* rhs;
    
    union {
        Ast_Block block;
        Ast_Declaration decl;
        Ast_Type_Definition type_def;
        Ast_Statement stmt;
        Ast_Expression expr;
    };
};

////////////////////////////////////
//      AST Translation Unit      //
////////////////////////////////////
struct Ast_Translation_Unit {
    Ast global_scope;
};

////////////////////////////
//         Parser         //
////////////////////////////
struct Parser {
    Memory_Arena arena;
    
    Ast_Translation_Unit* translation_unit;
    
    Lexer lexer;
    Token token;
};

#define BUILD_PARSER_H
#endif // #if !defined(BUILD_PARSER_H)
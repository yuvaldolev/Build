#if !defined(BUILD_PARSER_H)

struct Ast;
struct Ast_Translation_Unit;

// TODO(yuval): @Replace all of fixed size arrays with resizable arrays

///////////////////////////////
//        Expressions        //
///////////////////////////////
namespace Ast_Expression_Kind {
    enum Type {
        COMMA,
        ASSIGNMENT,
        CONDITIONAL,
        TERNARY,
        ARITHMETIC,
        BIT_OPERATION,
        BOOL_OPERATION,
        MEMORY_OPERATION,
        DOT,
        SUBSCRIPT,
        CAST,
        DECL_REF,
        FUNC_CALL,
        CONSTANT
    };
}

namespace Ast_Operator {
    enum Type {
        NONE,
        
        COMMA,
        
        PLUS,
        PLUS_PLUS,
        PLUS_EQUAL,
        
        MINUS,
        MINUS_MINUS,
        MINUS_EQUAL,
        
        MUL,
        MUL_EQUAL,
        
        DIV,
        EQUAL,
        
        MOD,
        MOD_EQUAL,
        
        AND,
        AND_AND,
        AND_EQUAL,
        
        OR,
        OR_OR,
        OR_EQUAL,
        
        XOR,
        XOR_EQUAL,
        
        SHL,
        SHR,
        
        EQUAL,
        EQUAL_EQUAL,
        
        NOT,
        NOT_EQUAL,
        
        LESS,
        LESS_EQUAL,
        
        GREATER,
        GREATER_EQUAL,
        
        ADDR,
        DEREF,
        
        DOT,
        
        SIZEOF,
        ALIGNOF,
        
        HASH,
        HASH_HASH
    };
}

namespace Ast_Constant_Kind {
    enum Type {
        NUMBER,
        CHAR,
        BOOL,
        STRING_LITERAL
    };
}

struct Ast_Constant {
    Ast_Constant_Kind::Type kind;
    
    union {
        struct {
            u64 int_constant;
            f64 float_constant;
        };
        char char_constant;
        b32 bool_constant;
        String string_literal;
    };
};

struct Ast_Decl_Ref {
    Ast* decl; // Declaration
};

struct Ast_Function_Call {
    Ast* func;
    Ast* first_arg;
};

struct Ast_Cast {
    Ast* cast_type; // Type Definition
    Ast* casted_expr; // Expression
};

struct Ast_Subscript {
    Ast* subscript; // Expression
};

struct Ast_Dot {
    String field_name;
};

struct Ast_Ternary {
    Ast* condition_expr; // Expression
    Ast* then_expr; // Expression
    Ast* else_expr; // Expression
};

struct Ast_Expression {
    Ast_Expression_Kind::Type kind;
    
    union {
        Ast_Operator::Type op;
        Ast_Ternary ternary;
        Ast_Dot dot;
        Ast_Subscript subscript;
        Ast_Cast cast;
        Ast_Function_Call func_call;
        Ast_Decl_Ref decl_ref;
        Ast_Constant constant;
    };
};

//////////////////////////////
//        Statements        //
//////////////////////////////
namespace Ast_Statement_Kind {
    enum Type {
        DECL,
        EXPR,
        IF,
        SWITCH,
        CASE,
        DEFAULT,
        FOR,
        WHILE,
        DO_WHILE,
        BREAK,
        CONTINUE,
        RETURN
    };
}

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
    Ast* body; // Statement
};

struct Ast_If {
    Ast* condition_expr; // Expression
    Ast* then_stmt; // Statement
    Ast* else_stmt; // Statement
};

struct Ast_Expression_Statement {
    Ast* expr; // Expression
};

struct Ast_Declaration_Statement {
    Ast* decl; // Declaration
};

struct Ast_Statement {
    Ast_Statement_Kind::Type kind;
    
    Ast* my_scope; // Block
    
    union {
        Ast_Declaration_Statement decl_stmt;
        Ast_Expression_Statement expr_stmt;
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
namespace Ast_Type_Definition_Kind {
    enum Type {
        DEFAULT,
        POINTER,
        STRUCT,
        ENUM,
        UNION
    };
}

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
    Ast_Type_Definition_Kind::Type kind;
    
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
namespace Ast_Declaration_Kind {
    enum Type {
        UNDEFINED,
        
        TYPE,
        FUNC,
        VAR
    };
}

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
    Ast_Declaration_Kind::Type kind;
    
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
namespace Ast_Kind {
    enum Type {
        BLOCK,
        DECLARATION,
        TYPE_DEFINITION,
        STATEMENT,
        EXPRESSION
    };
}

struct Ast {
    Ast_Kind::Type kind;
    
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
};

#define BUILD_PARSER_H
#endif // #if !defined(BUILD_PARSER_H)
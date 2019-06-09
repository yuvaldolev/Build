#if !defined(META_TOOL_AST_H)

struct AstFile
{
    
};

enum AstType
{
    
};

struct AstTypeDefinition
{
    String name;
    
    // TODO(yuval): @Refactor this into a union
    AstStruct* structDesc; // If it's a struct
    AstEnum* enumDesc; // If it's an enum
    Ast
};

struct AstFunctionImplementation
{
    
};

struct AstExpression
{
    AstType type;
    
    AstExpression* left;
    AstExpression* right;
    
    // TODO(yuval): @Refactor this into a union
    AstTypeDefinition* typeDfn// If it's a type definition
        AstTypeInstantiation* typeInst; // If it's a type instantiation
    AstFunctionImplementation* funcImpl; // If it's a function implementation
    AstFunctionCall* functionCall; // If it's a function call
    AstIfStatement* ifStmt; // If it's an if statement
    AstSwitchStatement* switchStmt; // If it's a switch statement
};

AstFile
ParseFile(Tokenizer* tokenizer);

#define META_TOOL_AST_H
#endif
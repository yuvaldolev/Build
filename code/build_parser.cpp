#define DEFAULT_TYPES \
DEFAULT_TYPE(void, BUNDLE_LITERAL("void")) \
DEFAULT_TYPE(bool, BUNDLE_LITERAL("bool")) \
DEFAULT_TYPE(char, BUNDLE_LITERAL("char")) \
DEFAULT_TYPE(int, BUNDLE_LITERAL("int")) \
DEFAULT_TYPE(float, BUNDLE_LITERAL("float")) \
DEFAULT_TYPE(double, BUNDLE_LITERAL("double")) \
DEFAULT_TYPE(long, BUNDLE_LITERAL("long")) \
DEFAULT_TYPE(short, BUNDLE_LITERAL("short")) \
DEFAULT_TYPE(unsigned, BUNDLE_LITERAL("unsigned"))

#define DEFAULT_TYPE(type, ...) global Ast* JOIN2(global_type_def_, type);
DEFAULT_TYPES
#undef DEFAULT_TYPE

#define BEGIN_AST_DUMP_BLOCK(...) ++global_indentation;
#define END_AST_DUMP_BLOCK(...) --global_indentation;

//
// NOTE(yuval): AST Dumping
//

global u32 global_indentation = 0;

internal void
indent_line(u32 n_spaces) {
    while (n_spaces--) {
        printf(" ");
    }
}

internal void
dump_ast_details(Ast* ast) {
    String* filename = &ast->my_file.name;
    
    printf("%p <%.*s:%d:%d> ",
           (void*)ast, PRINTABLE_STRING(*filename),
           ast->my_line, ast->my_column);
}

internal void
dump_ast_declaration(Ast* decl_ast) {
    indent_line(global_indentation);
    
    Ast_Declaration* decl = &decl_ast->decl;
    Ast_Identifier* ident = decl->my_identifier;
    
    switch (decl->kind) {
        case Ast_Declaration_Kind::TYPE: {
            printf("TypeDecl ");
            dump_ast_details(decl_ast);
            printf("%.*s ", PRINTABLE_STRING(ident->name));
            
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &type_def_ast->type_def;
            
            Ast** decls = 0;
            umm decl_count = 0;
            
            switch (type_def->type) {
                case Ast_Type_Definition_Kind::POINTER: {
                } break;
                
                case Ast_Type_Definition_Kind::STRUCT: {
                    printf("'struct'");
                    
                    decls = (Ast**)type_def->struct_type_def.members;
                    decl_count = type_def->struct_type_def.member_count;
                } break;
                
                case Ast_Type_Definition_Kind::ENUM: {
                    printf("'enum'");
                    
                    decls = (Ast**)type_def->enum_type_def.decls;
                    decl_count = type_def->enum_type_def.decl_count;
                } break;
                
                case Ast_Type_Definition_Kind::UNION: {
                    printf("'union'");
                    
                    decls = (Ast**)type_def->union_type_def.decls;
                    decl_count = type_def->union_type_def.decl_count;
                } break;
            }
            
            printf("\n");
            
            if (decls) {
                BEGIN_AST_DUMP_BLOCK();
                
                for (umm decl_index = 0; decl_index < decl_count; ++decl_index) {
                    dump_ast_declaration(decls[decl_index]);
                }
                
                END_AST_DUMP_BLOCK();
            }
            
        } break;
        
        case Ast_Declaration_Kind::FUNC: {
        } break;
        
        case Ast_Declaration_Kind::VAR: {
            printf("VarDecl ");
            dump_ast_details(decl_ast);
            printf("%.*s ", PRINTABLE_STRING(ident->name));
            
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &type_def_ast->type_def;
            
            String* type_name;
            
            switch (type_def->type) {
                case Ast_Type_Definition_Kind::DEFAULT: {
                    type_name = &type_def->default_type_name;
                } break;
                
                case Ast_Type_Definition_Kind::POINTER: {
                    // TODO(yuval): Handle Pointers
                    type_name = 0;
                } break;
                
                default: {
                    type_name = &type_def->my_decl->decl.my_identifier->name;
                } break;
            }
            
            if (type_name) {
                printf("'%.*s'", PRINTABLE_STRING(*type_name));
            }
            
            printf("\n");
        } break;
    }
}

internal void
dump_translation_unit_ast(Ast_Translation_Unit* translation_unit) {
    Ast_Block* global_scope = &translation_unit->global_scope.block;
    
    // NOTE(yuval): AST Dumping
    for (umm decl_index = 0; decl_index < global_scope->decl_count; ++decl_index) {
        dump_ast_declaration(global_scope->decls[decl_index]);
    }
}

//
// NOTE(yuval): AST Parsing
//

internal void
init_default_types(Memory_Arena* arena) {
#define DEFAULT_TYPE(type_name, name) JOIN2(global_type_def_, type_name) = PUSH_STRUCT(arena, Ast); \
    JOIN2(global_type_def_, type_name)->type = AST_TYPE_DEFINITION; \
    JOIN2(global_type_def_, type_name)->type_def.type = Ast_Type_Definition_Kind::DEFAULT; \
    JOIN2(global_type_def_, type_name)->type_def.default_type_name = name;
    
    DEFAULT_TYPES
    
#undef DEFAULT_TYPE
}

internal Ast*
get_default_type(String type_name) {
#define DEFAULT_TYPE(type, name) \
    if (strings_match(type_name, name)) { \
        return JOIN2(global_type_def_, type); \
    }
    
    DEFAULT_TYPES
    
#undef DEFAULT_TYPE
    
        return 0;
}

internal Ast*
ast_new(Ast_Type type, Parser* parser) {
    Ast* result = PUSH_STRUCT(&parser->arena, Ast);
    
    result->type = type;
    result->my_file = parser->lexer.file;
    
    result->my_line = parser->lexer.line_number;
    result->my_column = parser->lexer.column_number;
    
    return result;
}

internal Ast*
ast_new_decl(Ast_Declaration_Type type, Parser* parser) {
    Ast* result = ast_new(AST_DECLARATION, parser);
    result->decl.type = type;
    return result;
}

internal Ast*
ast_new_type_def(Ast_Type_Definition_Type type, Parser* parser) {
    Ast* result = ast_new(AST_TYPE_DEFINITION, parser);
    result->type_def.type = type;
    return result;
}

internal Ast*
ast_new_stmt(Ast_Statement_Type type, Parser* parser) {
    Ast* result = ast_new(AST_STATEMENT, parser);
    result->stmt.type = type;
    return result;
}

internal Ast*
ast_new_expr(Ast_Expression_Type type, Parser* parser) {
    Ast* result = ast_new(AST_EXPRESSION, parser);
    result->expr.type = type;
    return result;
}

internal Ast*
find_decl(Ast* scope, String name, Ast_Declaration_Type type = Ast_Declaration_Kind::UNDEFINED) {
    // TODO(yuval): Use this for find_type!
    Ast* result = 0;
    Ast* curr_scope = scope;
    
    while (curr_scope) {
        for (umm decl_index = 0;
             decl_index < curr_scope->block.decl_count;
             ++decl_index) {
            Ast* it = curr_scope->block.decls[decl_index];
            
            if (it) {
                Ast_Declaration_Type it_type = it->decl.type;
                String* it_name = &it->decl.my_identifier->name;
                
                if (((type == Ast_Declaration_Kind::UNDEFINED) || (it_type == type)) &&
                    strings_match(*it_name, name)) {
                    result = it;
                    goto doublebreak;
                }
            }
        }
        
        curr_scope = curr_scope->block.parent;
    }
    
    doublebreak:;
    
    return result;
}

internal Ast*
find_type(Ast* scope, String type_name) {
    Ast* result = get_default_type(type_name);
    
    if (!result) {
        Ast* type_decl = find_decl(scope, type_name, Ast_Declaration_Kind::TYPE);
        result = type_decl->decl.my_type;
    }
    
    return result;
}

internal Ast*
ast_new_binop_expr(Ast_Expression_Type type,
                   Ast* lhs, Ast_Operator op, Ast* rhs,
                   Parser* parser) {
    Ast* result = ast_new_expr(type, parser);
    result->lhs = lhs;
    result->rhs = rhs;
    
    Ast_Expression* expr = &result->expr;
    expr->op = op;
    
    return result;
}

internal Ast*
ast_new_unary_expr(Ast_Expression_Type type,
                   Ast_Operator op, Ast* rhs,
                   Parser* parser) {
    Ast* result = ast_new_binop_expr(type, 0, op, rhs, parser);
    return result;
}

internal Ast*
ast_new_postfix_expr(Ast_Expression_Type type,
                     Ast* lhs, Ast_Operator op,
                     Parser* parser) {
    Ast* result = ast_new_binop_expr(type, lhs, op, 0, parser);
    return result;
}

internal Ast*
parse_declaration(Parser* parser, Ast* scope);

internal Ast*
parse_compound_statement(Parser* parser, Ast* scope);

internal Ast*
parse_expression(Parser* parser, Ast* scope);

internal Ast*
parse_assignment_expression(Parser* parser, Ast* scope);

internal Ast*
parse_primary_expression(Parser* parser, Ast* scope) {
    Ast* result = 0;
    Token token = peek_token(&parser->lexer);
    
    switch (token.kind) {
        case Token_Kind::OPEN_PAREN: {
            eat_token(&parser->lexer);
            token = peek_token(&parser->lexer);
            
            b32 is_cast_expr = false;
            if (token.kind == Token_Kind::IDENTIFIER) {
                Ast* cast_type = find_type(scope, token.text);
                if (cast_type) {
                    result = ast_new_expr(Ast_Expression_Kind::CAST, parser);
                    
                    eat_token(&parser->lexer);
                    require_token(&parser->lexer, Token_Kind::CLOSE_PAREN);
                    
                    Ast_Cast* cast = &result->expr.cast;
                    cast->cast_type = cast_type;
                    cast->casted_expr = parse_expression(parser, scope);
                    
                    is_cast_expr = true;
                }
            }
            
            if (!is_cast_expr) {
                eat_token(&parser->lexer);
                result = parse_expression(parser, scope);
                AST_REQUIRE_TOKEN(parser, Token_Kind::CLOSE_PAREN);
            }
        } break;
        
        case Token_Kind::IDENTIFIER: {
            eat_token(&parser->lexer);
            
            Token next_token = peek_token(&parser->lexer);
            if (next_token.kind == Token_Kind::OPEN_PAREN) {
                // NOTE(yuval): Function Call
                Ast* function_decl = find_decl(scope, token.text,
                                               Ast_Declaration_Kind::FUNC);
                
                if (function_decl) {
                    result = ast_new_expr(Ast_Expression_Kind::FUNC_CALL, parser);
                    
                    eat_token(&parser->lexer);
                    
                    Ast_Function_Call* func_call = &result->expr.func_call;
                    func_call->func = function_decl;
                    
                    Ast* prev_arg = 0;
                    while (!optional_token(&parser->lexer, Token_Kind::CLOSE_PAREN)) {
                        Ast* arg = parse_assignment_expression(parser, scope);
                        
                        if (arg) {
                            if (prev_arg) {
                                arg->lhs = prev_arg;
                                prev_arg->rhs = arg;
                                prev_arg = arg;
                            } else {
                                func_call->first_arg = arg;
                                prev_arg = arg;
                            }
                            
                            require_token(&parser->lexer, Token_Kind::COMMA);
                        } else {
                            report_error(parser, "expected ')'");
                        }
                    }
                } else {
                    report_error(&token, "\"%S\" is not a function", token.text);
                }
            } else {
                // NOTE(yuval): Decl Ref
                Ast* decl = find_decl(scope, token.text);
                
                if (decl) {
                    result = ast_new_expr(Ast_Expression_Kind::DECL_REF, parser);
                    Ast_Decl_Ref* decl_ref = &result->expr.decl_ref;
                    decl_ref->decl = decl;
                } else {
                    report_error(&token, "undefined declaration %S", token.text);
                }
            }
        } break;
        
        case Token_Kind::NUMBER: {
            result = ast_new_expr(Ast_Expression_Kind::CONSTANT, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Constant* constant = &result->expr.constant;
            constant->int_constant = token.value_s32;
            constant->float_constant = token.value_f32;
        } break;
        
        case Token_Kind::CHAR_CONSTANT: {
            result = ast_new_expr(Ast_Expression_Kind::CONSTANT, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Constant* constant = &result->expr.constant;
            if (token.text.count == 1) {
                constant->char_constant = token.text.data[0];
            } else {
                report_error(&parser->token, "char literal can only contain a single character");
            }
        } break;
        
        case Token_Kind::BOOL_CONSTANT: {
            result = ast_new_expr(Ast_Expression_Kind::CONSTANT, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Constant* constant = &result->expr.constant;
            // TODO(yuval): Temporary! Determine the bool constant value in the lexer!!!!!
            if (strings_match(token.text, "true")) {
                constant->bool_constant = true;
            } else {
                constant->bool_constant = false;
            }
        } break;
        
        case Token_Kind::STRING_LITERAL: {
            result = ast_new_expr(Ast_Expression_Kind::CONSTANT, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Constant* constant = &result->expr.constant;
            constant->string_literal = parser->token.text;
        } break;
        
        default: {
            report_error(&parser->token, "primary expression expected");
        } break;
    }
    
    return result;
}

internal Ast*
parse_postfix_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_primary_expression(parser, scope);
    
    b32 parsing = true;
    while (parsing) {
        if (AST_OPTIONAL_TOKEN(parser, Token_Kind::PLUS_PLUS)) {
            result = ast_new_postfix_expr(Ast_Expression_Kind::ARITHMETIC,
                                          result, AST_OP_PLUS_PLUS,
                                          parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::MINUS_MINUS)) {
            result = ast_new_postfix_expr(Ast_Expression_Kind::ARITHMETIC,
                                          result, AST_OP_MINUS_MINUS,
                                          parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::DOT)) {
            // NOTE(yuval): Dot operator is used for both pointers
            // and regular structs (no arrow operator)
            Ast* dot_ast = ast_new_expr(Ast_Expression_Kind::DOT, parser);
            dot_ast->lhs = result;
            
            Ast_Dot* dot = &dot_ast->expr.dot;
            AST_REQUIRE_TOKEN(parser, Token_Kind::IDENTIFIER);
            dot->field_name = parser->token.text;
            
            result = dot_ast;
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::OPEN_BRACKET)) {
            Ast* subscript_ast = ast_new_expr(Ast_Expression_Kind::SUBSCRIPT, parser);
            subscript_ast->lhs = result;
            
            Ast_Subscript* subscript = &subscript_ast->expr.subscript;
            subscript->subscript = parse_assignment_expression(parser, scope);
            AST_REQUIRE_TOKEN(parser, Token_Kind::CLOSE_BRACKET);
            
            result = subscript_ast;
        } else {
            parsing = false;
        }
    }
    
    return result;
}

internal Ast*
parse_unary_expression(Parser* parser, Ast* scope) {
    Ast* result;
    Token token = AST_PEEK_TOKEN(parser);
    
    switch (token.kind) {
        case Token_Kind::MINUS: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::ARITHMETIC,
                                        AST_OP_MINUS,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::STAR: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::MEMORY_OPERATION,
                                        AST_OP_DEREF,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::AMP: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::MEMORY_OPERATION,
                                        AST_OP_ADDR,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::NOT: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::BOOL_OPERATION,
                                        AST_OP_NOT,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::TILDE: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::BIT_OPERATION,
                                        AST_OP_NOT,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::SIZEOF: {
            AST_GET_TOKEN(parser);
            AST_REQUIRE_TOKEN(parser, Token_Kind::OPEN_PAREN);
            result = ast_new_unary_expr(Ast_Expression_Kind::MEMORY_OPERATION,
                                        AST_OP_SIZEOF,
                                        parse_unary_expression(parser, scope),
                                        parser);
            AST_REQUIRE_TOKEN(parser, Token_Kind::CLOSE_PAREN);
        } break;
        
        case Token_Kind::ALIGNOF: {
            AST_GET_TOKEN(parser);
            AST_REQUIRE_TOKEN(parser, Token_Kind::OPEN_PAREN);
            result = ast_new_unary_expr(Ast_Expression_Kind::MEMORY_OPERATION,
                                        AST_OP_ALIGNOF,
                                        parse_unary_expression(parser, scope),
                                        parser);
            AST_REQUIRE_TOKEN(parser, Token_Kind::CLOSE_PAREN);
        } break;
        
        case Token_Kind::PLUS_PLUS: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::ARITHMETIC,
                                        AST_OP_PLUS_PLUS,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::MINUS_MINUS: {
            AST_GET_TOKEN(parser);
            result = ast_new_unary_expr(Ast_Expression_Kind::ARITHMETIC,
                                        AST_OP_MINUS_MINUS,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } break;
        
        default: {
            result = parse_postfix_expression(parser, scope);
        } break;
    }
    
    return result;
}

internal Ast*
parse_mul_arithmetic_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_unary_expression(parser, scope);
    
    b32 parsing = true;
    while (parsing) {
        if (AST_OPTIONAL_TOKEN(parser, Token_Kind::STAR)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::ARITHMETIC,
                                        result, AST_OP_MUL,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::SLASH)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::ARITHMETIC,
                                        result, AST_OP_DIV,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::PERCENT)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::ARITHMETIC,
                                        result, AST_OP_MOD,
                                        parse_unary_expression(parser, scope),
                                        parser);
        } else {
            parsing = false;
        }
    }
    
    return result;
}

internal Ast*
parse_add_arithmetic_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_mul_arithmetic_expression(parser, scope);
    
    b32 parsing = true;
    while (parsing) {
        if (AST_OPTIONAL_TOKEN(parser, Token_Kind::PLUS)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::ARITHMETIC,
                                        result, AST_OP_PLUS,
                                        parse_mul_arithmetic_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::MINUS)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::ARITHMETIC,
                                        result, AST_OP_MINUS,
                                        parse_mul_arithmetic_expression(parser, scope),
                                        parser);
        } else {
            parsing = false;
        }
    }
    
    return result;
}

internal Ast*
parse_bit_shift_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_add_arithmetic_expression(parser, scope);
    
    b32 parsing = true;
    while (parsing) {
        if (AST_OPTIONAL_TOKEN(parser, Token_Kind::LESS_LESS)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::BIT_OPERATION,
                                        result, AST_OP_SHL,
                                        parse_add_arithmetic_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::GREATER_GREATER)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::BIT_OPERATION,
                                        result, AST_OP_SHR,
                                        parse_add_arithmetic_expression(parser, scope),
                                        parser);
        } else {
            parsing = false;
        }
    }
    
    return result;
}

internal Ast*
parse_relational_conditional_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_bit_shift_expression(parser, scope);
    
    b32 parsing = true;
    while (parsing) {
        if (AST_OPTIONAL_TOKEN(parser, Token_Kind::LESS)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                        result, AST_OP_LESS,
                                        parse_bit_shift_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::GREATER)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                        result, AST_OP_GREATER,
                                        parse_bit_shift_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::LESS_EQUAL)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                        result, AST_OP_LESS_EQUAL,
                                        parse_bit_shift_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::GREATER_EQUAL)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                        result, AST_OP_GREATER_EQUAL,
                                        parse_bit_shift_expression(parser, scope),
                                        parser);
        } else {
            parsing = false;
        }
    }
    
    return result;
}

internal Ast*
parse_equality_conditional_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_relational_conditional_expression(parser, scope);
    
    b32 parsing = true;
    while (parsing) {
        if (AST_OPTIONAL_TOKEN(parser, Token_Kind::EQUAL_EQUAL)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                        result, AST_OP_EQUAL_EQUAL,
                                        parse_relational_conditional_expression(parser, scope),
                                        parser);
        } else if (AST_OPTIONAL_TOKEN(parser, Token_Kind::NOT_EQUAL)) {
            result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                        result, AST_OP_NOT_EQUAL,
                                        parse_relational_conditional_expression(parser, scope),
                                        parser);
        } else {
            parsing = false;
        }
    }
    
    return result;
}

internal Ast*
parse_bit_and_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_equality_conditional_expression(parser, scope);
    
    while (AST_OPTIONAL_TOKEN(parser, Token_Kind::AMP)) {
        result = ast_new_binop_expr(Ast_Expression_Kind::BIT_OPERATION,
                                    result, AST_OP_AND,
                                    parse_equality_conditional_expression(parser, scope),
                                    parser);
    }
    
    return result;
}

internal Ast*
parse_bit_xor_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_bit_and_expression(parser, scope);
    
    while (AST_OPTIONAL_TOKEN(parser, Token_Kind::CARET)) {
        result = ast_new_binop_expr(Ast_Expression_Kind::BIT_OPERATION,
                                    result, AST_OP_XOR,
                                    parse_bit_and_expression(parser, scope),
                                    parser);
    }
    
    return result;
}

internal Ast*
parse_bit_or_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_bit_xor_expression(parser, scope);
    
    while (AST_OPTIONAL_TOKEN(parser, Token_Kind::PIPE)) {
        result = ast_new_binop_expr(Ast_Expression_Kind::BIT_OPERATION,
                                    result, AST_OP_OR,
                                    parse_bit_xor_expression(parser, scope),
                                    parser);
    }
    
    return result;
}

internal Ast*
parse_and_conditional_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_bit_or_expression(parser, scope);
    
    while (AST_OPTIONAL_TOKEN(parser, Token_Kind::AMP_AMP)) {
        result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                    result, AST_OP_AND_AND,
                                    parse_bit_or_expression(parser, scope),
                                    parser);
    }
    
    return result;
}

internal Ast*
parse_or_conditional_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_and_conditional_expression(parser, scope);
    
    while (AST_OPTIONAL_TOKEN(parser, Token_Kind::PIPE_PIPE)) {
        result = ast_new_binop_expr(Ast_Expression_Kind::CONDITIONAL,
                                    result, AST_OP_OR_OR,
                                    parse_and_conditional_expression(parser, scope),
                                    parser);
    }
    
    return result;
}

internal Ast*
parse_conditional_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_or_conditional_expression(parser, scope);
    
    if (AST_OPTIONAL_TOKEN(parser, Token_Kind::TERNARY)) {
        AST_GET_TOKEN(parser);
        
        Ast* ternary_ast = ast_new_expr(Ast_Expression_Kind::TERNARY, parser);
        Ast_Expression* expr = &result->expr;
        
        Ast_Ternary* ternary = &expr->ternary;
        ternary->condition_expr = result;
        ternary->then_expr = parse_expression(parser, scope);
        
        AST_REQUIRE_TOKEN(parser, Token_Kind::COLON);
        
        ternary->else_expr = parse_conditional_expression(parser, scope);
        
        result = ternary_ast;
    }
    
    return result;
}


internal Ast*
parse_assignment_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_conditional_expression(parser, scope);
    
    AST_GET_TOKEN(parser);
    
    switch (parser->token.kind) {
        case Token_Kind::EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::PLUS_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_PLUS_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::MINUS_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_MINUS_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::STAR_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_MUL_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::SLASH_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_DIV_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::PERCENT_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_MOD_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::AMP_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_AND_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::PIPE_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        AST_OP_OR_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
        
        case Token_Kind::CARET_EQUAL: {
            result = ast_new_binop_expr(Ast_Expression_Kind::ASSIGNMENT, result,
                                        
                                        AST_OP_XOR_EQUAL,
                                        parse_assignment_expression(parser, scope),
                                        parser);
        } break;
    }
    
    return result;
}

internal Ast*
parse_expression(Parser* parser, Ast* scope) {
    Ast* result = parse_assignment_expression(parser, scope);
    
    if (AST_OPTIONAL_TOKEN(parser, Token_Kind::COMMA)) {
        result = ast_new_binop_expr(Ast_Expression_Kind::COMMA,
                                    result, AST_OP_COMMA,
                                    parse_expression(parser, scope),
                                    parser);
    }
    
    return result;
}

internal Ast*
parse_statement(Parser* parser, Ast* parent_scope) {
    Ast* result = 0;
    
    Token token = peek_token(&parser->lexer);
    
    switch (token.kind) {
        case Token_Kind::IF: {
            result = ast_new_stmt(Ast_Statement_Kind::IF, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = ast_new(Ast_Kind::BLOCK, parser);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_If* if_stmt = &stmt->if_stmt;
            
            // NOTE(yuval): If Condition
            require_token(&parser->lexer, Token_Kind::OPEN_PAREN);
            if_stmt->condition_expr = parse_expression(parser, parent_scope);
            require_token(&parser->lexer, Token_Kind::CLOSE_PAREN);
            
            // NOTE(yuval): If Body
            if_stmt->then_stmt = parse_statement(parser, stmt->my_scope);
            
            // NOTE(yuval): Else
            if (optional_token(&parser->lexer, Token_Kind::ELSE)) {
                if_stmt->else_stmt = parse_statement(parser, parent_scope);
            }
        } break;
        
        case Token_Kind::SWITCH: {
            result = ast_new_stmt(Ast_Statement_Kind::SWITCH, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = ast_new(Ast_Kind::BLOCK, parser);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_Switch* switch_stmt = &stmt->switch_stmt;
            
            // NOTE(yuval): Switch Condition
            require_token(&parser->lexer, Token_Kind::OPEN_PAREN);
            switch_stmt->condition_expr = parse_expression(parser, parent_scope);
            require_token(&parser->lexer, Token_Kind::CLOSE_PAREN);
            
            // NOTE(yuval): Switch Body
            switch_stmt->body = parse_statement(parser, stmt->my_scope);
        } break;
        
        case Token_Kind::CASE: {
            result = ast_new_stmt(Ast_Statement_Kind::CASE, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            Ast_Case* case_stmt = &stmt->case_stmt;
            
            // NOTE(yuval): Case Expression
            case_stmt->value = parse_expression(parser, parent_scope);
            
            // TODO(yuval): Check for colon
            
            Ast* scope = parent_scope;
            token = peek_token(&parser->lexer);
            if (token.kind == Token_Kind::OPEN_BRACE) {
                scope = ast_new(Ast_Kind::BLOCK, parser);
                scope->block.parent = parent_scope;
            }
            
            stmt->my_scope = scope;
            
            // NOTE(yuval): Case Body
            case_stmt->body = parse_statement(parser, stmt->my_scope);
            
            /* TODO(yuval):
Instead of always parsing compound statement:
             1. Parse compound statement only when there is an open brace.
             2. If there isn't an open brace statement parse until a break statement is found.
        */
        } break;
        
        case Token_Kind::DEFAULT: {
            result = ast_new_stmt(Ast_Statement_Kind::DEFAULT, parser);
            
            eat_token(&parser->lexer);
            
            Ast* scope = parent_scope;
            token = peek_token(&parser->lexer);
            if (token.kind == Token_Kind::OPEN_BRACE) {
                scope = ast_new(Ast_Kind::BLOCK, parser);
                scope->block.parent = parent_scope;
            }
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = scope;
            
            Ast_Default* default_stmt = &stmt->default_stmt;
            
            // TODO(yuval): Check for colon
            
            // NOTE(yuval): Default Body
            default_stmt->body = parse_statement(parser, stmt->my_scope);
        } break;
        
        case Token_Kind::FOR: {
            result = ast_new_stmt(Ast_Statement_Kind::FOR, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = ast_new(Ast_Kind::BLOCK, parser);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_For* for_stmt = &stmt->for_stmt;
            
            require_token(&parser->lexer, Token_Kind::OPEN_PAREN);
            
            // NOTE(yuval): For Initialization Statement
            if (!optional_token(&parser->lexer, Token_Kind::SEMI)) {
                for_stmt->init = parse_statement(parser, stmt->my_scope);
                require_token(&parser->lexer, Token_Kind::SEMI);
            }
            
            // NOTE(yuval): For Condition Expression
            if (!optional_token(&parser->lexer, Token_Kind::SEMI)) {
                for_stmt->condition = parse_expression(parser, parent_scope);
                require_token(&parser->lexer, Token_Kind::SEMI);
            }
            
            // NOTE(yuval): For Incrementation Expression
            if (!optional_token(&parser->lexer, Token_Kind::CLOSE_PAREN)) {
                for_stmt->inc = parse_expression(parser, parent_scope);
                require_token(&parser->lexer, Token_Kind::CLOSE_PAREN);
            }
            
            // NOTE(yuval): For Body Statement
            for_stmt->body = parse_statement(parser, stmt->my_scope);
        } break;
        
        case Token_Kind::WHILE: {
            result = ast_new_stmt(Ast_Statement_Kind::WHILE, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = ast_new(Ast_Kind::BLOCK, parser);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_While* while_stmt = &stmt->while_stmt;
            
            // NOTE(yuval): While Condition
            require_token(&parser->lexer, Token_Kind::OPEN_PAREN);
            while_stmt->condition = parse_expression(parser, parent_scope);
            require_token(&parser->lexer, Token_Kind::CLOSE_PAREN);
            
            // NOTE(yuval): While Body
            while_stmt->body = parse_statement(parser, stmt->my_scope);
        } break;
        
        case Token_Kind::DO: {
            result = ast_new_stmt(Ast_Statement_Kind::DO_WHILE, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = ast_new(Ast_Kind::BLOCK, parser);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_While* while_stmt = &stmt->while_stmt;
            
            // NOTE(yuval): Do While Body
            while_stmt->body = parse_statement(parser, stmt->my_scope);
            
            // NOTE(yuval): Do While Condition
            require_token(&parser->lexer, Token_Kind::WHILE);
            
            require_token(&parser->lexer, Token_Kind::OPEN_PAREN);
            while_stmt->condition = parse_expression(parser, parent_scope);
            require_token(&parser->lexer, Token_Kind::CLOSE_PAREN);
            
            require_token(&parser->lexer, Token_Kind::SEMI);
        } break;
        
        case Token_Kind::BREAK: {
            result = ast_new_stmt(Ast_Statement_Kind::BREAK, parser);
            eat_token(&parser->lexer);
            require_token(&parser->lexer, Token_Kind::SEMI);
        } break;
        
        case Token_Kind::CONTINUE: {
            result = ast_new_stmt(Ast_Statement_Kind::CONTINUE, parser);
            eat_token(&parser->lexer);
            AST_REQUIRE_TOKEN(parser, Token_Kind::SEMI);
        } break;
        
        case Token_Kind::RETURN: {
            result = ast_new_stmt(Ast_Statement_Kind::RETURN, parser);
            
            eat_token(&parser->lexer);
            
            Ast_Statement* stmt = &result->stmt;
            stmt->my_scope = parent_scope;
            
            Ast_Return* return_stmt = &stmt->return_stmt;
            return_stmt->expr = parse_expression(parser, parent_scope);
            require_token(&parser->lexer, Token_Kind::SEMI);
        } break;
        
        case Token_Kind::OPEN_BRACE: {
            eat_token(&parser->lexer);
            result = parse_compound_statement(parser, parent_scope);
        } break;
        
        default: {
            Ast* type_def = 0;
            
            if (token.kind == Token_Kind::IDENTIFIER) {
                type_def = find_type(parent_scope, token.text);
            }
            
            if (type_def) {
                // NOTE(yuval: Declaration Statement
                result = ast_new_stmt(Ast_Statement_Kind::DECL, parser);
                
                Ast_Statement* stmt = &result->stmt;
                stmt->my_scope = parent_scope;
                
                Ast_Declaration_Statement* decl_stmt = &stmt->decl_stmt;
                decl_stmt->decl = parse_declaration(parser, parent_scope);
            } else {
                // NOTE(yuval): Expression Statement
                result = ast_new_stmt(Ast_Statement_Kind::EXPR, parser);
                
                Ast_Statement* stmt = &result->stmt;
                stmt->my_scope = parent_scope;
                
                Ast_Expression_Statement* expr_stmt = &stmt->expr_stmt;
                expr_stmt->expr = parse_expression(parser, parent_scope);
            }
        }
    }
    
    return result;
}

internal Ast*
parse_compound_statement(Parser* parser, Ast* scope) {
    Ast* first_stmt = parse_statement(parser, scope);
    Ast* prev_stmt = first_stmt;
    
    while (!optional_token(&parser->lexer, Token_Kind::CLOSE_BRACE)) {
        Ast* stmt = parse_statement(parser, scope);
        
        if (stmt) {
            stmt->lhs = prev_stmt;
            prev_stmt->rhs = stmt;
            prev_stmt = stmt;
        } else {
            report_error(&parser->token, "expected '}'");
        }
    }
    
    return first_stmt;
}

internal Ast*
parse_type_declaration(Parser* parser, Ast* parent_scope, Ast_Type_Definition::Kind kind) {
    Ast* result = ast_new_decl(Ast_Declaration::TYPE, parser);
    
    Token token = require_token(&parser->lexer, Token_Kind::IDENTIFIER);
    
    Ast_Declaration* decl = &result->decl;
    decl->my_identifier = PUSH_STRUCT(&parser->arena, Ast_Identifier);
    decl->my_identifier->name = token.text;
    
    if (optional_token(&parser->lexer, Token_Kind::OPEN_BRACE)) {
        decl->my_scope = ast_new(Ast::BLOCK, parser);
        decl->my_scope->block.parent = parent_scope;
        decl->my_scope->block.owning_decl = result;
        
        decl->my_type = ast_new_type_def(kind, parser);
        Ast_Type_Definition* type_def = &decl->my_type->type_def;
        type_def->my_decl = result;
        
        // NOTE(yuval): Temporary
        Ast** decls = 0;
        umm* decl_count = 0;
        
        switch (type) {
            case Ast_Type_Definition_Kind::STRUCT: {
                decls = (Ast**)&type_def->struct_type_def.members;
                decl_count = &type_def->struct_type_def.member_count;
            } break;
            
            case Ast_Type_Definition_Kind::ENUM: {
                decls = (Ast**)&type_def->enum_type_def.decls;
                decl_count= &type_def->enum_type_def.decl_count;
            } break;
            
            case Ast_Type_Definition_Kind::UNION: {
                decls = (Ast**)&type_def->union_type_def.decls;
                decl_count= &type_def->union_type_def.decl_count;
            } break;
        }
        
        // NOTE(yuval): Member Declarations Parsing
        while (!optional_token(&parser->lexer, Token_Kind::CLOSE_BRACE)) {
            decls[*decl_count] = parse_declaration(parser, decl->my_scope);
            ++(*decl_count);
            
            require_token(&parser->lexer, Token_Kind::SEMI);
        }
    } else {
        // TODO(yuval): Think about forward declarations
    }
    
    return result;
}

internal Ast*
parse_declaration(Parser* parser, Ast* scope) {
    Ast* result = 0;
    
    Token token = peek_token(&parser->lexer);
    
    // NOTE(yuval): Tag Parsing
    Ast_Tag* tags[16] = {};
    u32 tag_count = 0;
    
    while (token.kind == Token_Kind::AT) {
        eat_token(&parser->lexer);
        token = require_token(&parser->lexer, Token_Kind::IDENTIFIER);
        Ast_Tag* tag = PUSH_STRUCT(&parser->arena, Ast_Tag);
        tag->tag = token.text;
        tags[tag_count++] = tag;
        token = peek_token(&parser->lexer);
    }
    
    switch (token.kind) {
        case Token_Kind::TYPEDEF: {
        } break;
        
        case Token_Kind::CONST: {
        } break;
        
        case Token_Kind::IDENTIFIER: {
            Ast* type = find_type(scope, parser->token.text);
            
            if (!type) {
                report_error(&token, "use of undeclared identifier '%S'",
                             token.text);
            }
            
            eat_token(&parser->lexer);
            
            token = require_token(&parser->lexer, Token_Kind::IDENTIFIER);
            Ast_Identifier* identifier = PUSH_STRUCT(&parser->arena, Ast_Identifier);
            identifier->name = token.text;
            
            if (optional_token(&parser->lexer, Token_Kind::OPEN_PAREN)) {
                result = ast_new_decl(Ast_Declaration_Kind::FUNC, parser);
                
                // TODO(yuval): Maybe unite this code with the variable declaration code below?
                Ast_Declaration* decl = &result->decl;
                decl->my_identifier = identifier;
                decl->my_scope = ast_new(AST_BLOCK, parser);
                decl->my_scope->block.parent = scope;
                
                Ast_Function* func = &decl->func;
                func->return_type = type;
                
                // NOTE(yuval): Function Parameters Parsing
                do {
                    func->params[func->param_count++] = parse_declaration(parser, scope);
                } while (optional_token(&parser->lexer, Token_Kind::COMMA));
                
                require_token(parser, Token_Kind::CLOSE_PAREN);
                
                if (optional_token(&parser->lexer, Token_Kind::OPEN_BRACE)) {
                    // NOTE(yuval): Function Definition
                    func->is_function_definition = true;
                    func->my_body = parse_compound_statement(parser, decl->my_scope);
                } else {
                    // NOTE(yuval): Function Declaration
                    func->is_function_definition = false;
                    require_token(&parser->lexer, Token_Kind::SEMI);
                }
            } else {
                // NOTE(yuval): Variable Declaration Parsing
                // TODO(yuval): Handle default value assignment
                result = ast_new_decl(Ast_Declaration_Kind::VAR, parser);
                
                Ast_Declaration* decl = &result->decl;
                decl->my_identifier = identifier;
                decl->my_scope = scope;
                decl->my_type = type;
            }
        } break;
        
        case Token_Kind::STRUCT: {
            eat_token(&parser->lexer);
            result = parse_type_declaration(parser, scope,
                                            Ast_Type_Definition_Kind::STRUCT);
        } break;
        
        case Token_Kind::ENUM: {
            eat_token(&parser->lexer);
            result = parse_type_declaration(parser, scope,
                                            Ast_Type_Definition_Kind::ENUM);
        } break;
        
        case Token_Kind::UNION: {
            eat_token(&parser->lexer);
            result = parse_type_declaration(parser, scope,
                                            Ast_Type_Definition_Kind::UNION);
        } break;
    }
    
    if (result) {
        copy(result->decl.my_tags, tags, sizeof(tags));
        scope->block.decls[scope->block.decl_count++] = result;
    }
    
    return result;
}

internal void
parse_top_level(Parser* parser) {
    Ast* decl = parse_declaration(parser, &parser->translation_unit->global_scope);
    
    if (decl->decl.kind == Ast_Declaration_Kind::TYPE ||
        decl->decl.kind == Ast_Declaration_Kind::VAR) {
        require_token(&parser->lexer, Token_Kind::SEMI);
    }
}

internal Ast_Translation_Unit*
parse_translation_unit(Parser* parser, Code_File file) {
    Ast_Translation_Unit* translation_unit =
        PUSH_STRUCT(&parser->arena, Ast_Translation_Unit);
    
    parser->translation_unit = translation_unit;
    parser->lexer = lex(file);
    
    while (!optional_token(&parser->lexer, Token_Kind::END_OF_STREAM)) {
        parse_top_level(parser);
    }
    
    return translation_unit;
};

global u32 global_cpp_converter_indentation = 0;

#define BEGIN_CPP_CONVERTER_BLOCK(...) (++global_cpp_converter_indentation)
#define END_CPP_CONVERTER_BLOCK(...) (--global_cpp_converter_indentation)

internal void
cpp_converter_indent_line(u32 indentation) {
    while (indentation--) {
        printf("    ");
    }
}

internal String
get_type_name(Ast* type_def_ast) {
    String type_name = {};
    
    if (type_def_ast && (type_def_ast->kind == Ast_Kind::TYPE_DEFINITION)) {
        Ast_Type_Definition* type_def = &type_def_ast->type_def;
        
        switch (type_def->kind) {
            case Ast_Type_Definition_Kind::DEFAULT: {
                type_name = type_def->default_type_name;
            } break;
            
            case Ast_Type_Definition_Kind::POINTER: {
                // TODO(yuval): Handle Pointers
            } break;
            
            default: {
                // TODO(yuval): What if the type doesn't have a name?
                type_name = type_def->my_decl->decl.my_identifier->name;
            } break;
        }
    }
    
    return type_name;
    
}

internal void
cpp_convert_declaration(Ast* decl_ast);

internal void
cpp_convert_statement(Ast* stmt_ast) {
    Ast* stmt_at = stmt_ast;
    
    while (stmt_at) {
        ASSERT(stmt_at->kind == Ast_Kind::STATEMENT);
        
        Ast_Statement* stmt = &stmt_at->stmt;
        ASSERT(stmt);
        
        // TODO(yuval): Maybe clean this up so that stmt's kind will not be checked twice
        if (stmt->kind != Ast_Statement_Kind::DECL) {
            cpp_converter_indent_line(global_cpp_converter_indentation);
        }
        
        switch (stmt->kind) {
            case Ast_Statement_Kind::DECL: {
                cpp_convert_declaration(stmt->decl_stmt.decl);
                printf(";\n");
            } break;
            
            case Ast_Statement_Kind::EXPR: {
                
            } break;
            
            case Ast_Statement_Kind::IF: {
                Ast_If* if_stmt = &stmt->if_stmt;
                
                printf("if (");
                // TODO(yuval): Convert Expressions
                printf(") {\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                cpp_convert_statement(if_stmt->then_stmt);
                END_CPP_CONVERTER_BLOCK();
                
                cpp_converter_indent_line(global_cpp_converter_indentation);
                printf("}");
                
                if (if_stmt->else_stmt) {
                    printf(" else ");
                    cpp_convert_statement(if_stmt->else_stmt);
                } else {
                    printf("\n");
                }
            } break;
            
            case Ast_Statement_Kind::SWITCH: {
                Ast_Switch* switch_stmt = &stmt->switch_stmt;
                
                printf("switch (");
                // TODO(yuval): Convert the condition expression
                printf(") {\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                cpp_convert_statement(switch_stmt->body);
                END_CPP_CONVERTER_BLOCK();
                
                cpp_converter_indent_line(global_cpp_converter_indentation);
                printf("}\n");
            } break;
            
            case Ast_Statement_Kind::CASE: {
                Ast_Case* case_stmt = &stmt->case_stmt;
                
                printf("case ");
                // TODO(yuval): Convert case value
                printf(": {\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                cpp_convert_statement(case_stmt->body);
                END_CPP_CONVERTER_BLOCK();
                
                cpp_converter_indent_line(global_cpp_converter_indentation);
                printf("}");
                
                Ast* rhs = stmt_at->rhs;
                if (rhs &&
                    (rhs->stmt.kind == Ast_Statement_Kind::BREAK)) {
                    printf(" break;");
                    
                    // NOTE(yuval): Eating the rhs statement
                    stmt_at = stmt_at->rhs;
                }
                
                printf("\n");
            } break;
            
            case Ast_Statement_Kind::DEFAULT: {
                Ast_Default* default_stmt = &stmt->default_stmt;
                
                printf("default: {\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                cpp_convert_statement(default_stmt->body);
                END_CPP_CONVERTER_BLOCK();
                
                cpp_converter_indent_line(global_cpp_converter_indentation);
                printf("}");
                
                Ast* rhs = stmt_at->rhs;
                if (rhs &&
                    (rhs->stmt.kind == Ast_Statement_Kind::BREAK)) {
                    printf(" break;");
                    
                    // NOTE(yuval): Eating the rhs statement
                    stmt_at = stmt_at->rhs;
                }
                
                printf("\n");
            } break;
            
            case Ast_Statement_Kind::FOR: {
                Ast_For* for_stmt = &stmt->for_stmt;
                
                printf("for (");
                
                // NOTE(yuval): Initialization Statement
                cpp_convert_statement(for_stmt->init);
                // TODO(yuval): Check if a semicolon needs to be printed
                
                // NOTE(yuval): Condition Expression
                // TODO(yuval): Convert condition expression
                printf(";");
                
                // NOTE(yuval): Incrementation Expression
                // TODO(yuval): Convert inc expression
                
                printf(") {\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                cpp_convert_statement(for_stmt->body);
                END_CPP_CONVERTER_BLOCK();
                
                cpp_converter_indent_line(global_cpp_converter_indentation);
                printf("}");
            } break;
            
            case Ast_Statement_Kind::WHILE: {
                
            } break;
            
            case Ast_Statement_Kind::DO_WHILE: {
                
            } break;
            
            case Ast_Statement_Kind::BREAK: {
                printf("break;\n");
            } break;
            
            case Ast_Statement_Kind::CONTINUE: {
                
            } break;
            
            case Ast_Statement_Kind::RETURN: {
                
            } break;
        }
        
        stmt_at = stmt_at->rhs;
    }
}

internal void
cpp_convert_declaration(Ast* decl_ast) {
    cpp_converter_indent_line(global_cpp_converter_indentation);
    
    Ast_Declaration* decl = &decl_ast->decl;
    ASSERT(decl);
    
    Ast_Identifier* ident = decl->my_identifier;
    
    switch (decl->kind) {
        case Ast_Declaration_Kind::TYPE: {
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &type_def_ast->type_def;
            
            Ast** decls = 0;
            umm decl_count = 0;
            
            switch (type_def->kind) {
                case Ast_Type_Definition_Kind::POINTER: {
                    
                } break;
                
                case Ast_Type_Definition_Kind::STRUCT: {
                    printf("struct ");
                    
                    decls = (Ast**)type_def->struct_type_def.members;
                    decl_count = type_def->struct_type_def.member_count;
                } break;
                
                case Ast_Type_Definition_Kind::ENUM: {
                    printf("enum ");
                    
                    decls = (Ast**)type_def->enum_type_def.decls;
                    decl_count = type_def->enum_type_def.decl_count;
                } break;
                
                case Ast_Type_Definition_Kind::UNION: {
                    printf("union ");
                    
                    decls = (Ast**)type_def->union_type_def.decls;
                    decl_count = type_def->union_type_def.decl_count;
                } break;
            }
            
            printf("%.*s", PRINTABLE_STRING(ident->name));
            
            if (decls) {
                printf(" {\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                for (umm decl_index = 0; decl_index < decl_count; ++decl_index) {
                    cpp_convert_declaration(decls[decl_index]);
                    printf(";\n");
                }
                END_CPP_CONVERTER_BLOCK();
                
                printf("}");
            }
            
            printf(";\n");
        } break;
        
        case Ast_Declaration_Kind::FUNC: {
            Ast_Function* func = &decl->func;
            ASSERT(func);
            
            String return_type_name = get_type_name(func->return_type);
            if (!is_null_string(return_type_name)) {
                // NOTE(yuval): Function Return Type And Name
                printf("%.*s ", PRINTABLE_STRING(return_type_name));
                printf("%.*s", PRINTABLE_STRING(ident->name));
                
                // NOTE(yuval): Function Parameters
                printf("(");
                for (umm param_index = 0; param_index < func->param_count; ++param_index) {
                    cpp_convert_declaration(func->params[param_index]);
                    
                    if (param_index != (func->param_count - 1)) {
                        printf(", ");
                    }
                }
                printf(")");
                
                if (func->is_function_definition) {
                    printf(" {\n");
                    
                    BEGIN_CPP_CONVERTER_BLOCK();
                    cpp_convert_statement(func->my_body);
                    END_CPP_CONVERTER_BLOCK();
                    
                    printf("}");
                } else {
                    printf(";");
                }
                
                printf("\n");
            } else {
                // TODO(yuval): Report Error
            }
        } break;
        
        case Ast_Declaration_Kind::VAR: {
            String type_name = get_type_name(decl->my_type);
            if (!is_null_string(type_name)) {
                printf("%.*s ", PRINTABLE_STRING(type_name));
                printf("%.*s", PRINTABLE_STRING(ident->name));
                // TODO(yuval): Handle variable initialization
            } else {
                report_error(decl_ast, "variable declarations should start with a type");
            }
        } break;
        
        default: {
            report_error(decl_ast, "cannot convert declaration with an undefined kind to cpp code");
        } break;
    }
}

internal void
cpp_convert_translation_unit(Ast_Translation_Unit* translation_unit) {
    Ast_Block* global_scope = &translation_unit->global_scope.block;
    
    for (umm decl_index = 0;
         decl_index < global_scope->decl_count;
         ++decl_index) {
        Ast* decl = global_scope->decls[decl_index];
        cpp_convert_declaration(decl);
        
        if (decl->decl.kind == Ast_Declaration_Kind::VAR) {
            printf(";\n");
        }
        
        printf("\n");
    }
}
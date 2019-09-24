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
cpp_convert_declaration(Ast* decl_ast) {
    cpp_converter_indent_line(global_cpp_converter_indentation);
    
    Ast_Declaration* decl = &decl_ast->decl;
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
                }
                END_CPP_CONVERTER_BLOCK();
                
                printf("}");
            }
            
            printf(";\n\n");
        } break;
        
        case Ast_Declaration_Kind::FUNC: {
            Ast_Function* func = &decl->func;
            
            String return_type_name = get_type_name(func->return_type);
            if (!is_null_string(return_type_name)) {
                // NOTE(yuval): Function Return Type And Name
                printf("%.*s ", PRINTABLE_STRING(return_type_name));
                printf("%.*s", PRINTABLE_STRING(ident->name));
                
                // NOTE(yuval): Function Parameters
                printf("(");
                for (umm param_index = 0; param_index < func->param_count; ++param_index) {
                    cpp_convert_declaration(func->params[param_index]);
                }
                printf(")");
                
                printf("\n");
            } else {
                // TODO(yuval): Report Error
            }
        } break;
        
        case Ast_Declaration_Kind::VAR: {
            String type_name = get_type_name(decl->my_type);
            if (!is_null_string(type_name)) {
                printf("%.*s ", PRINTABLE_STRING(type_name));
                printf("%.*s\n", PRINTABLE_STRING(ident->name));
                // TODO(yuval): Handle variable initialization
            } else {
                report_error(decl_ast, "variable declarations should start with a type");
            }
        } break;
        
        default: {
            report_error(decl_ast, "cannot convert declaration with an undefined type to cpp code");
        } break;
    }
}

internal void
cpp_convert_translation_unit(Ast_Translation_Unit* translation_unit) {
    Ast_Block* global_scope = &translation_unit->global_scope.block;
    
    for (umm decl_index = 0;
         decl_index < global_scope->decl_count;
         ++decl_index) {
        cpp_convert_declaration(global_scope->decls[decl_index]);
    }
}
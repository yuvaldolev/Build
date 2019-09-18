global u32 global_cpp_converter_indentation = 0;

#define BEGIN_CPP_CONVERTER_BLOCK(...) (++global_cpp_converter_indentation)
#define END_CPP_CONVERTER_BLOCK(...) (--global_cpp_converter_indentation)

internal void
cpp_converter_indent_line(u32 indentation) {
    while (indentation--) {
        printf("    ");
    }
}

internal void
cpp_convert_declaration(Ast* decl_ast) {
    cpp_converter_indent_line(global_cpp_converter_indentation);
    
    Ast_Declaration* decl = &decl_ast->decl;
    Ast_Identifier* ident = decl->my_identifier;
    
    switch (decl->type) {
        case AST_DECL_TYPE: {
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &type_def_ast->type_def;
            
            Ast** decls = 0;
            umm decl_count = 0;
            
            switch (type_def->type) {
                case AST_TYPE_DEF_POINTER: {
                    
                } break;
                
                case AST_TYPE_DEF_STRUCT: {
                    printf("struct ");
                    
                    decls = (Ast**)type_def->struct_type_def.members;
                    decl_count = type_def->struct_type_def.member_count;
                } break;
                
                case AST_TYPE_DEF_ENUM: {
                    printf("enum ");
                    
                    
                    decls = (Ast**)type_def->enum_type_def.decls;
                    decl_count = type_def->enum_type_def.decl_count;
                } break;
                
                case AST_TYPE_DEF_UNION: {
                    printf("union ");
                    
                    decls = (Ast**)type_def->union_type_def.decls;
                    decl_count = type_def->union_type_def.decl_count;
                } break;
            }
            
            printf("%.*s ", PRINTABLE_STRING(ident->name));
            
            if (decls) {
                printf("{\n");
                
                BEGIN_CPP_CONVERTER_BLOCK();
                for (umm decl_index = 0; decl_index < decl_count; ++decl_index) {
                    cpp_convert_declaration(decls[decl_index]);
                }
                END_CPP_CONVERTER_BLOCK();
                
                printf("}");
            }
            
            printf(";\n");
        } break;
        
        case AST_DECL_FUNC: {
            
        } break;
        
        case AST_DECL_VAR: {
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &type_def_ast->type_def;
            
            String* type_name = 0;
            
            switch (type_def->type) {
                case AST_TYPE_DEF_DEFAULT: {
                    type_name = &type_def->default_type_name;
                } break;
                
                case AST_TYPE_DEF_POINTER: {
                    // TODO(yuval): Handle Pointers
                } break;
                
                default: {
                    // TODO(yuval): What if the type doesn't have a name?
                    type_name = &type_def->my_decl->decl.my_identifier->name;
                } break;
            }
            
            if (type_name) {
                printf("%.*s ", PRINTABLE_STRING(*type_name));
                printf("%.*s;\n", PRINTABLE_STRING(ident->name));
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
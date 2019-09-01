#define AST_GET_TOKEN(parser) (parser)->token = get_token(&(parser)->tokenizer)
#define AST_PEEK_TOKEN(parser) peek_token(&(parser)->tokenizer)
#define AST_GET_TOKEN_CHECK_TYPE(parser, type) get_token_check_type(&(parser)->tokenizer, type, &(parser)->token)
#define AST_REQUIRE_TOKEN(parser, type) (file)->token = require_token(&(parser)->tokenizer, type)
#define AST_OPTIONAL_TOKEN(parser, type) optional_token(&(parser)->Tokenizer, type, &(parser)->token)

#define DEFAULT_TYPES \
DEFAULT_TYPE(Void, BundleZ("void")) \
DEFAULT_TYPE(Bool, BundleZ("bool")) \
DEFAULT_TYPE(Char, BundleZ("char")) \
DEFAULT_TYPE(Int, BundleZ("int")) \
DEFAULT_TYPE(Float, BundleZ("float")) \
DEFAULT_TYPE(Double, BundleZ("double")) \
DEFAULT_TYPE(Long, BundleZ("long")) \
DEFAULT_TYPE(Short, BundleZ("short")) \
DEFAULT_TYPE(Unsigned, BundleZ("unsigned"))

#define DEFAULT_TYPE(Type, ...) global Ast* Join2(type_def, Type);
DEFAULT_TYPES
#undef DEFAULT_TYPE

#define BEGIN_AST_DUMP_BLOCK(...) ++global_indentation;
#define END_AST_DUMP_BLOCK(...) --global_indentation;

global u32 global_indentation = 0;

internal Ast*
ast_new(Ast_Type type, Ast_Translation_Unit* file, Token* token = 0) {
    Ast* result = PUSH_STRUCT(ParserArena, ast);
    
    result->type = type;
    result->my_file = file;
    
    if (!token) {
        token = &file->token;
    }
    
    result->my_line = token->line_number;
    result->my_column = token->column_number;
    
    return result;
}

internal Ast*
ast_new_decl(Ast_Declaration_Type type,
             Ast_Translation_Unit* file, Token* token = 0) {
    Ast* result = ast_new(Ast_Declaration, file, token);
    result->decl.type = type;
    return result;
}

internal Ast*
ast_new_type_def(Ast_Type_Definition_Type type,
                 Ast_Translation_Unit* file, Token* token = 0) {
    Ast* result = ast_new(Ast_TypeDefinition, file, token);
    result->type_def.type = type;
    return result;
}

internal Ast*
ast_new_stmt(Ast_Statement_Type type,
             Ast_Translation_Unit* file, Token* token = 0) {
    Ast* result = ast_new(Ast_Statement, file, token);
    result->stmt.type = type;
    return result;
}

internal Ast*
ast_new_expr(Ast_Expression_Type type,
             Ast_Translation_Unit* file, Token* token = 0) {
    Ast* result = ast_new(Ast_Expression, file, token);
    result->expr.type = type;
    return result;
}

internal void
init_default_types() {
#define DEFAULT_TYPE(type_name, name) JOIN2(type_def, type_name) = PUSH_STRUCT(, ast); \
    JOIN2(type_def, type_name)->type = Ast_Type_Definition; \
    JOIN2(type_def, type_name)->type_def.type = AST_TYPE_DEF_DEFAULT; \
    JOIN2(TypeDef, type_name)->type_def.default_type_name = name;
    
    DEFAULT_TYPES
    
#undef DEFAULT_TYPE
}

internal Ast*
get_default_type(String type_name) {
#define DEFAULT_TYPE(type, name) \
    if (strings_match(type_name, name)) { \
        return JOIN2(type_def, type); \
    }
    
    DEFAULT_TYPES
    
#undef DEFAULT_TYPE
    
        return 0;
}

internal void
indent_line(u32 n_spaces) {
    while (n_spaces--) {
        printf(" ");
    }
}

internal void
dump_ast_details(Ast* ast) {
    String* file_name = &ast->my_file->file_name;
    
    printf("%p <%.*s:%d:%d> ",
           (void*)ast,
           (s32)file_name->count, file_name->Data,
           ast->my_line, ast->my_column);
}

internal void
dump_ast_declaration(Ast* decl_ast) {
    indent_line(global_indentation);
    
    Ast_Declaration* decl = &decl_ast->decl;
    Ast_Identifier* ident = decl->identifier;
    
    switch (decl->type) {
        case AST_DECL_TYPE: {
            printf("TypeDecl ");
            dump_ast_details(decl_ast);
            printf("%.*s ",
                   (s32)ident->my_name.count,
                   ident->my_name.data);
            
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &type_def_ast->type_def;
            
            Ast** decls = 0;
            u32 decls_count = 0;
            
            switch (type_def->Type) {
                case AST_TYPE_DEF_POINTER: {
                } break;
                
                case AST_TYPE_DEF_STRUCT: {
                    printf("'struct'");
                    
                    decls = (Ast**)type_def->struct_type_def.members;
                    decls_count = type_def->struct_type_def.member_index;
                } break;
                
                case AST_TYPE_DEF_ENUM: {
                    printf("'enum'");
                    
                    decls = (Ast**)type_def->Enum.decls;
                    decls_count = type_def->Enum.decl_index;
                } break;
                
                case AST_TYPE_DEF_UNION: {
                    printf("'union'");
                    
                    decls = (Ast**)type_def->Union.decls;
                    decls_count = type_def->Union.decl_index;
                } break;
            }
            
            printf("\n");
            
            if (decls) {
                BEGIN_AST_DUMP_BLOCK();
                
                for (int decl_index = 0; decl_index < decls_count; ++decl_index) {
                    dump_ast_declaration(decls[decl_index]);
                }
                
                END_AST_DUMP_BLOCK();
            }
            
        } break;
        
        case AST_DECL_FUNC: {
        } break;
        
        case AST_DECL_VAR: {
            printf("VarDecl ");
            dump_ast_details(decl_ast);
            printf("%.*s ",
                   (s32)ident->my_name.count, ident->my_name.data);
            
            Ast* type_def_ast = decl->my_type;
            Ast_Type_Definition* type_def = &TypeDefAst->TypeDef;
            
            String* type_name;
            
            switch (type_def->type) {
                case AST_TYPE_DEF_DEFAULT: {
                    type_name = &type_def->DEFAULT_TYPEName;
                } break;
                
                case AST_TYPE_DEF_POINTER: {
                    // TODO(yuval): Handle Pointers
                    type_name = 0;
                } break;
                
                default: {
                    type_name = &type_def->my_decl->decl.identifier->my_name;
                } break;
            }
            
            if (type_name) {
                printf("'%.*s'",
                       (s32)type_name->Count, type_name->Data);
            }
            
            printf("\n");
        } break;
    }
}

internal void
dump_ast_file(Ast_Translation_Unit* file) {
    Ast_Block* global_scope = &file->global_scope.block;
    
    // NOTE(yuval): AST Dumping
    for (int decl_index = 0; decl_index < global_scope->decl_count) {
        dump_ast_declaration(global_scope->decls[decl_index]);
    }
}

internal Ast*
find_type(Ast* scope, String type_name) {
    Ast* result = get_default_type(type_name);
    
    if (!result) {
        Ast* curr_scope = scope;
        
        while (curr_scope) {
            for (int decl_index = 0; i < curr_scope->block.decl_count; ++decl_index) {
                Ast* it = curr_scope->block.decls[decl_index];
                
                if (it) {
                    String* name = &it->decl.identifier->my_name;
                    
                    if ((it->decl.type == AST_DECL_TYPE) &&
                        (strings_match(name, type_name))) {
                        result = it->decl.my_type;
                        goto doublebreak;
                    }
                }
            }
            
            curr_scope = curr_scope->block.parent;
        }
        
        doublebreak:;
    }
    
    return result;
}

internal Ast*
parse_declaration(Parser* parser, Ast* scope);

internal Ast*
parse_compound_statement(Parser* parser, Ast* Scope);

internal Ast*
parse_statement(Parser* parser, Ast* parent_scope) {
    Ast* result = 0;
    
    AST_GET_TOKEN(file);
    
    switch (file->token.type) {
        case TOKEN_IDENTIFIER: {
            Ast* type_def = find_type(parent_scope, file->token.text);
            
            if (type_def) {
                // NOTE(yuval): Declaration Statement
            } else {
                // NOTE(yuval): Assignment Statement
            }
        } break;
        
        case TOKEN_IF: {
            result = ast_new_stmt(AST_STMT_IF, file);
            Ast_Statement* stmt = &result->stmt;
            
            stmt->my_scope = ast_new(Ast_Block, file);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_If* if_stmt = &stmt->if_stmt;
            
            // NOTE(yuval): If Condition
            AST_REQUIRE_TOKEN(file, TOKEN_OPEN_PAREN);
            if_stmt->condition_expr = parse_expression();
            AST_REQUIRE_TOKEN(file, TOKEN_CLOSE_PAREN);
            
            // NOTE(yuval): If Body
            if_stmt->then_stmt = parse_statement(file, stmt->my_scope);
            
            // NOTE(yuval): Else
            if (AST_OPTIONAL_TOKEN(file, token_else)) {
                if_stmt->else_stmt = parse_statement(file, parent_scope);
            }
        } break;
        
        case TOKEN_SWITCH: {
            result = ast_new_stmt(AST_STMT_SWITCH, file);
            Ast_Statement* stmt = &result->stmt;
            
            stmt->my_scope = ast_new(Ast_Block, file);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_Switch* switch_stmt = &stmt->switch_stmt;
            
            // NOTE(yuval): Switch Condition
            AST_REQUIRE_TOKEN(file, TOKEN_OPEN_PAREN);
            switch_stmt->condition = parse_expression();
            AST_REQUIRE_TOKEN(File, TOKEN_CLOSE_PAREN);
            
            // NOTE(yuval): Switch Body
            switch_stmt->first_case = parse_statement(File, Stmt->my_scope);
        } break;
        
        case TOKEN_CASE: {
            result = ast_new_stmt(AST_STMT_CASE, file);
            Ast_Statement* stmt = &result->Stmt;
            
            Ast* scope = parent_scope;
            
            Token next_token = AST_PEEK_TOKEN(file);
            if (next_token.type == TOKEN_OPEN_BRACE) {
                scope = ast_new(Ast_Block, File);
                scope->block.parent = parent_scope;
            }
            
            stmt->my_scope = scope;
            
            Ast_Case* case_stmt = &stmt->case_stmt;
            
            // NOTE(yuval): Case Expression
            case_stmt->value = parse_expression();
            
            // NOTE(yuval): Case Body
            case_stmt->body = parse_statement(file, stmt->my_scope);
        } break;
        
        case TOKEN_DEFAULT: {
            result = ast_new_stmt(AST_STMT_DEFAULT, file);
            Ast_Statement* stmt = &result->stmt;
            
            Ast* scope = parent_scope;
            
            token next_token = AST_PEEK_TOKEN(file);
            if (next_token.type == TOKEN_OPEN_BRACE) {
                scope = ast_new(Ast_Block, file);
                scope->block.parent = parent_scope;
            }
            
            stmt->my_scope = scope;
            
            Ast_Default* default_stmt = &stmt->default_stmt;
            
            // NOTE(yuval): Default Body
            default_stmt->body = parse_statement(file, stmt->my_scope);
        } break;
        
        case TOKEN_FOR: {
            result = ast_new_stmt(AST_STMT_FOR, file);
            Ast_Statement* stmt = &result->stmt;
            
            stmt->my_scope = ast_new(Ast_Block, file);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_For* for_stmt = &Stmt->for_stmt;
            
            AST_REQUIRE_TOKEN(file, TOKEN_OPEN_PAREN);
            
            // NOTE(yuval): For Initialization Statement
            if (!AST_OPTIONAL_TOKEN(file, TOKEN_SEMI)) {
                for_stmt->init = parse_statement(file, stmt->my_scope);
                AST_REQUIRE_TOKEN(file, TOKEN_SEMI);
            }
            
            // NOTE(yuval): For Condition Expression
            if (!AST_OPTIONAL_TOKEN(file, TOKEN_SEMI)) {
                ForStmt->Condition = parse_expression();
                AST_REQUIRE_TOKEN(file, TOKEN_SEMI);
            }
            
            // NOTE(yuval): For Incrementation Expression
            if (!AST_OPTIONAL_TOKEN(file, TOKEN_CLOSE_PAREN)) {
                for_stmt->inc = parse_expression();
                AST_REQUIRE_TOKEN(file, TOKEN_CLOSE_PAREN);
            }
            
            // NOTE(yuval): For Body Statement
            for_stmt->body = parse_statement(file, stmt->my_scope);
        } break;
        
        case TOKEN_WHILE: {
            result = ast_new_stmt(AST_STMT_WHILE, file);
            Ast_Statement* stmt = &result->stmt;
            
            stmt->my_scope = ast_new(Ast_Block, file);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_While* while_stmt = &stmt->while_stmt;
            
            // NOTE(yuval): While Condition
            AST_REQUIRE_TOKEN(File, TOKEN_OPEN_PAREN);
            while_stmt->condition = parse_expression();
            AST_REQUIRE_TOKEN(File, TOKEN_CLOSE_PAREN);
            
            // NOTE(yuval): While Body
            while_stmt->body = parse_statement(file, stmt->my_scope);
        } break;
        
        case TOKEN_DO: {
            result = ast_new_stmt(AST_STMT_DO_WHILE, file);
            Ast_Statement* stmt = &result->stmt;
            
            stmt->my_scope = ast_new(Ast_Block, File);
            stmt->my_scope->block.parent = parent_scope;
            
            Ast_While* while_stmt = &stmt->while_stmt;
            
            // NOTE(yuval): Do While Body
            while_stmt->body = parse_statement(file, stmt->my_scope);
            
            // NOTE(yuval): Do While Condition
            AST_REQUIRE_TOKEN(file, TOKEN_WHILE);
            
            AST_REQUIRE_TOKEN(file, TOKEN_OPEN_PAREN);
            while_stmt->condition = parse_expression();
            AST_REQUIRE_TOKEN(file, TOKEN_CLOSE_PAREN);
            
            AST_REQUIRE_TOKEN(file, TOKEN_SEMI);
        } break;
        
        case TOKEN_BREAK: {
            result = ast_new_stmt(AST_STMT_BREAK, File);
            AST_REQUIRE_TOKEN(File, TOKEN_SEMI);
        } break;
        
        case TOKEN_CONTINUE: {
            result = ast_new_stmt(AST_STMT_CONTINUE, File);
            AST_REQUIRE_TOKEN(File, TOKEN_SEMI);
        } break;
        
        case TOKEN_RETURN: {
            result = ast_new_stmt(AstStmt_Return, File);
            Ast_Statement* stmt = &result->stmt;
            
            stmt->my_scope = parent_scope;
            
            Ast_Return* return_stmt = &stmt->return_stmt;
            return_stmt->expr = parse_expression();
            AST_REQUIRE_TOKEN(file, TOKEN_SEMI);
        } break;
        
        case TOKEN_OPEN_BRACE: {
            result = parse_compound_statement(file, parent_scope);
        } break;
    }
    
    return result;
}

internal Ast*
parse_compound_statement(Parser* parser, Ast* scope) {
    Ast* first_stmt = ParseStatement(file, scope);
    Ast* prev_stmt = FirstStmt;
    
    while (!AST_OPTIONAL_TOKEN(file, TOKEN_CLOSE_BRACE)) {
        Ast* stmt = parse_statement(file, scope);
        
        if (stmt) {
            stmt->left = prev_stmt;
            prev_stmt->right = stmt;
            prev_stmt = stmt;
        } else {
            bad_token(&file->token, "expected '}'");
        }
    }
    
    return first_stmt;
}

internal Ast*
parse_type_declaration(Parser* parser, Ast* parent_scope,
                       Ast_Type_Definition_Type type) {
    Ast* result = ast_new_decl(AST_DECL_TYPE, file);
    Ast_Declaration* decl = &result->decl;
    
    AST_REQUIRE_TOKEN(file, TOKEN_IDENTIFIER);
    decl->identifier = PUSH_STRUCT(, Ast_Identifier);
    decl->identifier->my_name = file->token.text;
    
    if (AST_OPTIONAL_TOKEN(file, TOKEN_OPEN_BRACE)) {
        decl->my_scope = ast_new(Ast_Block, file);
        decl->my_scope->block.parent = parent_scope;
        decl->my_scope->block.owning_decl = result;
        
        decl->my_type = ast_new_type_def(type, file);
        Ast_Type_Definition* type_def = &decl->my_type->type_def;
        type_def->myDecl = result;
        
        // NOTE(yuval): Temporary
        Ast** decls = 0;
        u32* decl_index = 0;
        
        switch (type) {
            case AST_TYPE_DEF_STRUCT: {
                decls = (Ast**)&type_def->Struct.Members;
                decl_index = &type_def->Struct.MemberIndex;
            } break;
            
            case AST_TYPE_DEF_ENUM: {
                decls = (Ast**)&type_def->Enum.decls;
                decl_index = &type_def->Enum.decl_index;
            } break;
            
            case AST_TYPE_DEF_UNION: {
                decls = (Ast**)&type_def->Union.decls;
                decl_index = &type_def->Union.decl_index;
            } break;
        }
        
        // NOTE(yuval): Member Declarations Parsing
        while (!AST_GET_TOKEN_CHECK_TYPE(File, TOKEN_CLOSE_BRACE)) {
            decls[*decl_index] = parse_declaration(file, decl->myScope);
            ++(*decl_index);
            
            AST_REQUIRE_TOKEN(file, TOKEN_SEMI);
        }
    } else {
        // TODO(yuval): Think about forward declarations
    }
    
    return result;
}

internal Ast*
parse_declaration(Parser* parser, Ast* scope) {
    Ast* result = 0;
    
    // NOTE(yuval): Tag Parsing
    Ast_Tag* tags[16] = {};
    u32 tag_index = 0;
    
    while (file->token.type == TOKEN_AT) {
        AST_REQUIRE_TOKEN(file, TOKEN_IDENTIFIER);
        Ast_Tag* tag = PUSH_STRUCT(, Ast_Tag);
        tag->tag = file->token.text;
        tags[TagIndex++] = tag;
        AST_GET_TOKEN(file);
    }
    
    switch (file->token.type) {
        case TOKEN_TYPEDEF: {
        } break;
        
        case TOKEN_CONST: {
        } break;
        
        case TOKEN_IDENTIFIER: {
            Ast* type = find_type(scope, file->token.text);
            
            if (!type) {
                bad_token(&file->token, "use of undeclared identifier '%S'",
                          file->token.text);
            }
            
            AST_REQUIRE_TOKEN(file, TOKEN_IDENTIFIER);
            ast_identifier* Identifier = PUSH_STRUCT(, Ast_Identifier);
            identifier->my_name = file->token.text;
            
            if (AST_OPTIONAL_TOKEN(file, TOKEN_OPEN_PAREN)) {
                result = ast_new_decl(AstDecl_Func, File);
                
                // TODO(yuval): Maybe unite this code with the variable declaration code below?
                Ast_Declaration* decl = &result->decl;
                decl->identifier = identifier;
                decl->my_scope = ast_new(Ast_Block, file);
                decl->my_scope->block.parent = scope;
                
                Ast_Function* func = &decl->func;
                func->return_type = type;
                
                // NOTE(yuval): Function Declaration Parsing
                while (!AST_GET_TOKEN_CHECK_TYPE(file, TOKEN_CLOSE_BRACE)) {
                    func->params[func->param_count++] = parse_declaration(file, scope);
                }
                
                // NOTE(yuval): Function Body Parsing
                AST_REQUIRE_TOKEN(file, TOKEN_OPEN_BRACE);
                func->my_body = parse_compound_statement(file, decl->my_scope);
            } else {
                // NOTE(yuval): Variable Declaration Parsing
                // TODO(yuval): Handle default value assignment
                result = ast_new_decl(AstDecl_Var, file);
                
                Ast_Declaration* decl = &result->decl;
                decl->identifier = identifier;
                decl->my_scope = scope;
                decl->my_type = type;
            }
        } break;
        
        case TOKEN_STRCUT: {
            result = parse_type_declaration(file, Scope, AST_TYPE_DEF_STRUCT);
        } break;
        
        case TOKEN_ENUM: {
            result = parse_type_declaration(File, Scope, AST_TYPE_DEF_ENUM);
        } break;
        
        case TOKEN_UNION: {
            result = parse_type_declaration(file, scope, AST_TYPE_DEF_UNION);
        } break;
    }
    
    if (result) {
        copy(result->decl.myTags, tags, sizeof(tags));
        scope->block.decls[scope->block.decl_index++] = result;
    }
    
    return result;
}

internal void
parse_top_level(Parser* parser) {
    Ast* decl = parse_declaration(file, &file->global_scope);
    
    if (decl->decl.type == AST_DECL_TYPE ||
        decl->decl.type == AST_DECL_VAR) {
        AST_REQUIRE_TOKEN(file, TOKEN_SEMI);
    }
}

internal Ast*
parse_translation_unit(Parser* parser, String filename, String file_contents) {
    Ast_Translation_Unit* translation_unit = PUSH_STRUCT(Ast_Translation_Unit,
                                                         &parser->parser_arena);
    
    parser->translation_unit = translation_unit;
    parser->tokenizer = tokenize(filename, file_contents);
    
    while (!AST_GET_TOKEN_CHECK_TYPE(file, TOKEN_END_OF_STREAM)) {
        parse_top_level(parser);
    }
    
    return translation_unit;
};

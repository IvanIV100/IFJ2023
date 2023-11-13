#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"


// typedef struct node_struct {
//     struct Token *current;
//     struct node_struct* left;
//     struct node_struct* right;
    
// } node_t;



void parser();
Token* handle_statement_list(Token* token);
Token* handle_param(Token* token);
Token* handle_param_list(Token* token);
Token* handle_type(Token* token);
Token* handle_func_def(Token* token);
Token* handle_in_param(Token* token);
Token* handle_in_param_list(Token* token);
Token* handle_assign_ops(Token* token);
Token* handle_var_def(Token* token);
Token* handle_funcall_ops(Token* token);
Token* handle_cond_ops(Token* token);
Token* handle_if(Token* token);
Token* handle_while(Token* token);
Token* handle_statement(Token* token);
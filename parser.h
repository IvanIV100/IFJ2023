#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"


typedef struct node_struct {
    Token *current;
    struct node_struct* left;
    struct node_struct* right;
    
} *node_t;



void parser();
void handle_statement_list(node_t *node);
void handle_param(node_t *node);
void handle_param_list(node_t *node);
void handle_type(node_t *node);
void handle_func_def(node_t *node);
void handle_in_param(node_t *node);
void handle_in_param_list(node_t *node);
void handle_assign_ops(node_t *node);
void handle_var_def(node_t *node);
void handle_funcall_ops(node_t *node);
void handle_cond_ops(node_t *node);
void handle_if(node_t *node);
void handle_while(node_t *node);
void handle_statement(node_t *node);
void node_init(node_t *node);
void node_insert(node_t *node);
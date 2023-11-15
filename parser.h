#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "symtable.h"


typedef struct node {
    struct Token *current;
    struct node* left;
    struct node* right;
    
} node_t;


typedef struct myInfo {
    bool inFun;
    struct symTabLVL *currentLVL;
} myInfo;

typedef struct symTabLVL {
    SymTable *currentTab;
    struct symTabLVL *parantLVL;
    
} symTabLVL;


void parser();
node_t* handle_statement_list(node_t* node);
node_t* handle_param(node_t* node);
node_t* handle_param_list(node_t* node);
node_t* handle_type(node_t* node);
node_t* handle_func_def(node_t* node);
node_t* handle_in_param(node_t* node);
node_t* handle_in_param_list(node_t* node);
node_t* handle_assign_ops(node_t* node);
node_t* handle_var_def(node_t* node);
node_t* handle_funcall_ops(node_t* node);
node_t* handle_cond_ops(node_t* node);
node_t* handle_if(node_t* node);
node_t* handle_while(node_t* node);
node_t* handle_statement(node_t* node);
node_t* create_node();
node_t* get_next(node_t* node);
void copy_to_child(SymTable *parent, SymTable *child);
void pop_level();
void create_level();
void init_myInfo();
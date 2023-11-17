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

typedef struct runTimeInfo {
    SymTable *globalFrame;
    SymTable *builtInFunctions;
    struct symTabLVL *currentLVL;

} runTimeInfo;

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
void fill_builtin_symtab(SymTable *builtIn);
void define_var_ST(node_t* node);
void assign_varType_ST(node_t* node, int type, int nillable);
void check_type();
void define_func_ST();
void pop_level();
void create_level();
void init_myInfo();
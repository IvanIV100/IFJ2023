#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>






#include "expressions.h"


void parser();
node_t* handle_statement_list(node_t* node);
node_t* handle_param(node_t* node);
node_t* handle_param_list(node_t* node);
int handle_type(node_t* node);
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
node_t* handle_return(node_t* node);
node_t* create_node();
node_t* get_next(node_t* node);
node_t* expression_token_count(node_t* node,int* count);

void free_node_list(node_t* node);

void start_generator(node_t* node);
//void start_generating(node_t* node, runTimeInfo* runInfo);
void fill_builtin_symtab(SymTable *builtIn);
void define_var_ST(node_t* node);
void assign_varType_ST(node_t* node, int type, int nillable);
//void check_type(char* id, int retT);

void pop_level();
void create_level();
void init_myInfo();

#endif
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
DataType handle_type(node_t* node);
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

void parse_for_fdef();
void load_all_tokens();
node_t* load_next_token(node_t* current);

void start_generator(node_t* node);
//void start_generating(node_t* node, runTimeInfo* runInfo);
void fill_builtin_symtab(SymTable *builtIn);
void define_var_ST(node_t* node);
void assign_varType_ST(char* key, int type);
//void check_type(char* id, int retT);

void add_funcall(char* FID);

void pop_level();
void create_level();
void init_runInfo();

#endif
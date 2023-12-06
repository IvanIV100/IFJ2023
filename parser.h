/**
 * Project: Implementace překladače IFJ23
 *
 * @brief  Parser .h file.
 *
 * @author Ivan Chodák xchoda00
 */



#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "expressions.h"

/// @brief Parser function entry point.
void parser();
/// @brief Entry point for the recursive descent of statement handle
/// @param node 
/// @return 
node_t* handle_statement_list(node_t* node);
/// @brief Handle validity function call param
/// @param node 
/// @return 
node_t* handle_param(node_t* node);
/// @brief Entry for recursive descent of function call params
/// @param node 
/// @return 
node_t* handle_param_list(node_t* node);
/// @brief Check and return type
/// @param node
/// @return
DataType handle_type(node_t* node);
/// @brief Check function defintion
/// @param node 
/// @return 
node_t* handle_func_def(node_t* node);
/// @brief handle function call params
/// @param node 
/// @return 
node_t* handle_in_param(node_t* node);
/// @brief entry point for recursive descent of function call params
/// @param node 
/// @return 
node_t* handle_in_param_list(node_t* node);
/// @brief Handle assign or call ops
/// @param node 
/// @return 
node_t* handle_assign_ops(node_t* node);
/// @brief handle variable definition
/// @param node 
/// @return 
node_t* handle_var_def(node_t* node);
/// @brief handle function calls
/// @param node 
/// @return 
node_t* handle_funcall_ops(node_t* node);
/// @brief handle conditions
/// @param node 
/// @return 
node_t* handle_cond_ops(node_t* node);
/// @brief If entry pooint
/// @param node 
/// @return 
node_t* handle_if(node_t* node);
/// @brief while entry point
/// @param node 
/// @return 
node_t* handle_while(node_t* node);
/// @brief handle given statement
/// @param node 
/// @return 
node_t* handle_statement(node_t* node);
/// @brief handle return statement
/// @param node 
/// @return 
node_t* handle_return(node_t* node);
/// @brief create new node for linked list
/// @return 
node_t* create_node();
/// @brief shifts to next in linked list
/// @param node 
/// @return 
node_t* get_next(node_t* node);\
/// @brief counts the lenght of expression
/// @param node 
/// @param count 
/// @return 
node_t* expression_token_count(node_t* node,int* count);
/// @brief checks for FID validity
/// @param id 
void  check_funcall_id(char* id);


/// @brief free linked list
/// @param node 
void free_node_list(node_t* node);

/// @brief fill in functions before syntax/semantic check
void parse_for_fdef();
/// @brief create linked list with all tokens
void load_all_tokens();
/// @brief loads another token from scanner
/// @param current 
/// @return 
node_t* load_next_token(node_t* current);
/// @brief starts generation
/// @param node 
void start_generator(node_t* node);
/// @brief fill in built in funcs
/// @param builtIn 
void fill_builtin_symtab(SymTable *builtIn);
/// @brief fill var to symtab
/// @param node 
void define_var_ST(node_t* node);
/// @brief assign type to variable
/// @param key 
/// @param type 
void assign_varType_ST(char* key, int type);
/// @brief  create new symtab level
void create_level();
/// @brief destroy symtab level
void pop_level();
/// @brief init run time info struct
void init_runInfo();

#endif
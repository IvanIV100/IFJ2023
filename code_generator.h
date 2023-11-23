/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Code generator interface.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "expressions.h"


#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

const char* built_in_functions[] = {
    "$Int2Double",
    "$Double2Int",
    "$readString",
    "$readInt",
    "$readDouble",
    "$length",
    "$ord",
    "$chr",
    "$substring",
    "$write",
    NULL // End of the list marker
};

bool is_builtin_function(const char* identifier);

void generate_builtin_function_call(node_t* node);

void generate_expression_code(node_t* node);

void generate_binary_arithmetic_code(node_t* node);

char* get_temp_variable();

void generate_assignment_code(node_t* node);

void generate_variable_definition_code(node_t* node);

void generate_function_call_code(node_t* node);

void generate_in_param_list_code(node_t* node);

void generate_in_param_code(node_t* node);

void generate_if_statement_code(node_t* node);

void generate_cond_ops_code(node_t* node);

int generate_label();

void generate_while_loop_code(node_t* node);

void generate_return_statement_code(node_t* node);

void generate_statement_list_code(node_t* node);

void generate_statement_code(node_t* node);

//BUILT IN funcs

void generateInt2Double();

void generateDouble2Int();

void generatereadString();

void generatereadInt();

void generatereadDouble();

void generateLength();

void generateSubstring();

void generateord();

void generatechr();

void generateWrite();

#endif

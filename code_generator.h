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
    "Int2Double",
    "Double2Int",
    "readString",
    "readInt",
    "readDouble",
    "length",
    "ord",
    "chr",
    "substring",
    "write",
    NULL // End of the list marker
};



/**
 * @brief Checks if an identifier corresponds to a built-in function.
 *
 * @param identifier The identifier to check.
 * @return true if the identifier is a built-in function, false otherwise.
 */
bool is_builtin_function(const char* identifier);

/**
 * @brief Generates code for a built-in function call.
 *
 * @param node The AST node representing the function call.
 */
void generate_builtin_function_call(node_t* node);

/**
 * @brief Generates code for an expression.
 *
 * @param node The AST node representing the expression.
 */
void generate_expression_code(node_t* node);

/**
 * @brief Generates code for binary arithmetic operations.
 *
 * @param node The AST node representing the binary operation.
 */
void generate_binary_arithmetic_code(node_t* node);

/**
 * @brief Generates code for assignment statements.
 *
 * @param node The AST node representing the assignment statement.
 */
void generate_assignment_code(node_t* node);

/**
 * @brief Generates code for variable definition and assignment.
 *
 * @param node The AST node representing the variable definition.
 */
void generate_variable_definition_code(node_t* node);

/**
 * @brief Generates code for a function call.
 *
 * @param node The AST node representing the function call.
 */
void generate_function_call_code(node_t* node);

/**
 * @brief Generates code for the list of input parameters in a function call.
 *
 * @param node The AST node representing the list of input parameters.
 */
void generate_in_param_list_code(node_t* node);

/**
 * @brief Generates code for a single input parameter in a function call.
 *
 * @param node The AST node representing the input parameter.
 */
void generate_in_param_code(node_t* node);

/**
 * @brief Generates code for an if statement.
 *
 * @param node The AST node representing the if statement.
 */
void generate_if_statement_code(node_t* node);

/**
 * @brief Generates code for conditional operations in an if statement.
 *
 * @param node The AST node representing the conditional expression.
 */
void generate_cond_ops_code(node_t* node);

/**
 * @brief Generates a unique label for control flow statements.
 *
 * @return The generated label.
 */
int generate_label();

/**
 * @brief Generates code for a while loop.
 *
 * @param node The AST node representing the while loop.
 */
void generate_while_loop_code(node_t* node);

/**
 * @brief Generates code for a return statement.
 *
 * @param node The AST node representing the return statement.
 */
void generate_return_statement_code(node_t* node);

/**
 * @brief Generates code for a list of statements.
 *
 * @param node The AST node representing the list of statements.
 */
void generate_statement_list_code(node_t* node);

/**
 * @brief Generates code for a single statement.
 *
 * @param node The AST node representing the statement.
 */
void generate_statement_code(node_t* node);

// BUILT-IN functions

/**
 * @brief Generates code for the built-in function Int2Double.
 */
void generateInt2Double();

/**
 * @brief Generates code for the built-in function Double2Int.
 */
void generateDouble2Int();

/**
 * @brief Generates code for the built-in function readString.
 */
void generatereadString();

/**
 * @brief Generates code for the built-in function readInt.
 */
void generatereadInt();

/**
 * @brief Generates code for the built-in function readDouble.
 */
void generatereadDouble();

/**
 * @brief Generates code for the built-in function length.
 */
void generateLength();

/**
 * @brief Generates code for the built-in function substring.
 */
void generateSubstring();

/**
 * @brief Generates code for the built-in function ord.
 */
void generateord();

/**
 * @brief Generates code for the built-in function chr.
 */
void generatechr();

/**
 * @brief Generates code for the built-in function write.
 *
 * @param node The AST node representing the write statement.
 */
void generateWrite(node_t* node);

#endif
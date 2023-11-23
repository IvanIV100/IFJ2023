/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Code generator interface.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */

#include "code_generator.h"

//TODO
//free temp_var


// Function to check if an identifier corresponds to a built-in function
bool is_builtin_function(const char* identifier) {
    for (int i = 0; built_in_functions[i] != NULL; ++i) {
        if (strcmp(identifier, built_in_functions[i]) == 0) {
            return true;
        }
    }
    return false;
}

void generate_builtin_function_call(node_t* node) {
    if (strcmp(node->current->value.ID_name, "Int2Double") == 0) {
        generateInt2Double();
    } else if (strcmp(node->current->value.ID_name, "Double2Int") == 0) {
        generateDouble2Int();
    } else if (strcmp(node->current->value.ID_name, "readString") == 0) {
        generatereadString();
    } else if (strcmp(node->current->value.ID_name, "readInt") == 0) {
        generatereadInt();
    } else if (strcmp(node->current->value.ID_name, "readDouble") == 0) {
        generatereadDouble();
    } else if (strcmp(node->current->value.ID_name, "length") == 0) {
        generateLength();
    } else if (strcmp(node->current->value.ID_name, "substr") == 0) {
        generateSubstring();
    } else if (strcmp(node->current->value.ID_name, "ord") == 0) {
        generateord();
    } else if (strcmp(node->current->value.ID_name, "chr") == 0) {
        generatechr();
    } else if (strcmp(node->current->value.ID_name, "write") == 0) {
        generateWrite();
    }
}


void generate_expression_code(node_t* node) {
    switch (node->current->type) {
        case T_PLUS:
        case T_MINUS:
        case T_MUL:
        case T_DIV:
            // Binary arithmetic operations
            generate_binary_arithmetic_code(node);
            break;
        case T_STRING:
            // Generate code for a string literal
            printf("PUSHS string@%s\n", node->current->value.stringVal);
            break;

        case T_MULTILINE_STRING:
            // Generate code for a multiline string literal
            printf("PUSHS string@%s\n", node->current->value.stringVal);
            break;
        case T_IDENTIFIER:
            // Variable reference
            printf("MOVE LF@%s TF@%s\n", node->current->value.ID_name, node->current->value.ID_name);
            break;

        case T_INT:
            // Integer literal
            printf("MOVE LF@t%d int@%d\n", get_temp_variable(), node->current->value.integer);
            break;

        case T_DOUBLE:
            // Double literal
            printf("MOVE LF@t%f float@%f\n", get_temp_variable(), node->current->value.decimal);
            break;
        

        default:
            // Handle other expression types if needed
            break;
    }
}

void generate_binary_arithmetic_code(node_t* node) {
    //node has a left and right child representing operands
    generate_expression_code(node->left);
    generate_expression_code(node->right);

    if (node->left->current->type == T_INT) {
        printf("INT2FLOAT LF@%s LF@%s\n", node->left->current->value, node->left->current->value);
    }

    if (node->right->current->type == T_INT) {
        printf("INT2FLOAT LF@%s LF@%s\n", node->right->current->value, node->right->current->value);
    }

    // Generate code for the binary operation
    switch (node->current->type) {
        case T_PLUS:
            printf("ADD LF@%fLF@%f LF@%f\n", get_temp_variable(), node->left->current->value.decimal, node->right->current->value.decimal);
            break;
                    case T_MINUS:
                        printf("SUB LF@%fLF@%f LF@%f\n", get_temp_variable(), node->left->current->value.decimal, node->right->current->value.decimal);
            break;
                    case T_MUL:
                        printf("MUL LF@%fLF@%f LF@%f\n", get_temp_variable(), node->left->current->value.decimal, node->right->current->value.decimal);
            break;
                    case T_DIV:
                        printf("DIV LF@%f LF@%f LF@%f\n", get_temp_variable(), node->left->current->value.decimal, node->right->current->value.decimal);
            break;

        default:
            // Handle other cases if needed
            break;
    }
}

// Helper function to get a temporary variable name
char* get_temp_variable() {
    static int temp_variable_count = 0;
    char* temp_variable_name = malloc(10);
    sprintf(temp_variable_name, "temp%d", temp_variable_count++);
    return temp_variable_name;
}

void generate_assignment_code(node_t* node) {
    // Generate code for the expression on the right-hand side
    generate_expression_code(node->right);

    //Print the left side of assigment
    if (node->left->current->type == T_IDENTIFIER) {
        printf("POP VAR, %s\n", node->left->current->value.ID_name);
    } else {
        //error handling
        return 0;
    }
}

void generate_variable_definition_code(node_t* node) {
    if (node->right != NULL) {
        generate_assignment_code(node->right);
    }

    if (node->left->current->type == T_IDENTIFIER) {
        printf("DECLARE VAR, %s\n", node->left->current->value.ID_name);
    } else {
        //error
        return 0;
    }
}

void generate_function_call_code(node_t* node) {
    //generates all params
    generate_in_param_list_code(node->right);

    if (node->left->current->type == T_IDENTIFIER) {
        printf("CALL %s\n", node->left->current->value.ID_name);
    } else {
        //error
        return 0;
    }
}

void generate_in_param_list_code(node_t* node) {
    // Generate code for each parameter in the list
    while (node->left != NULL) {
        generate_in_param_code(node->left);
        node = node->left;
    }
}

void generate_in_param_code(node_t* node) {
 
    generate_expression_code(node->right);

    if (node->left->current->type == T_IDENTIFIER) {
        printf("PUSH VAR, %s\n", node->left->current->value.ID_name);
    } else {
        //error
        return 0;
    }
}

void generate_if_statement_code(node_t* node) {
    generate_cond_ops_code(node->left);

    if (node->right != NULL && node->right->current->type == T_LEFT_BRACE) {
        printf("JUMP_IF_TRUE L%d\n", generate_label());
        generate_statement_list_code(node->right->right);
        printf("LABEL L%d\n", generate_label());
    } else {
        //error 
        return 0;
    }

    if (node->right != NULL && node->right->right != NULL &&
        node->right->right->current->type == T_ELSE &&
        node->right->right->right != NULL &&
        node->right->right->right->current->type == T_LEFT_BRACE) {
        printf("JUMP L%d\n", generate_label());
        printf("LABEL L%d\n", generate_label());
        generate_statement_list_code(node->right->right->right->right);
    }
}

void generate_cond_ops_code(node_t* node) {
    generate_expression_code(node);
    printf("JUMP_IF_FALSE L%d\n", generate_label());
}

int generate_label() {
    static int label_counter = 0;
    return label_counter++;
}


void generate_while_loop_code(node_t* node) {
    int loopStartLabel = generate_label();
    printf("LABEL L%d\n", loopStartLabel);

    generate_cond_ops_code(node->left);

    int loopEndLabel = generate_label();
    printf("JUMP_IF_FALSE L%d\n", loopEndLabel);

    generate_statement_list_code(node->right);

    printf("JUMP L%d\n", loopStartLabel);
    printf("LABEL L%d\n", loopEndLabel);
}

void generate_return_statement_code(node_t* node) {
    if (node->left != NULL) {
        generate_expression_code(node->left);
    }

    printf("RETURN\n");
}

void generate_statement_list_code(node_t* node) {
    while (node != NULL) {
        generate_statement_code(node->left);
        node = node->right;
    }
}

void generate_statement_code(node_t* node) {
    switch (node->current->type) {
        case T_LET:
        case T_VAR:
            generate_variable_definition_code(node->right);
            break;
        case T_IDENTIFIER:
            //checking if func is built-in
            if (is_builtin_function(node->current->value.ID_name)) {
                generate_builtin_function_call(node);
            } else {
                generate_function_call_code(node);
            }
            break;
        case T_IF:
            generate_if_statement_code(node->right);
            break;
        case T_WHILE:
            generate_while_loop_code(node->right);
            break;
        case T_RETURN:
            generate_return_statement_code(node->right);
            break;
        default:
            // Handle other statement types if necessary
            break;
    }

    if (node->left != NULL) {
        generate_expression_code(node->left);
    }
}


void generateInt2Double() {
    printf(
        "\n LABEL $Int2Double" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n INT2FLOAT LF@%retval LF@%0" \
        "\n MOVE LF@%retval float@0.0" \ 
        "\n ADD LF@%retval LF@%retval LF@%0" \ 
        "\n POPFRAME" \
        "\n RETURN");
}


void generateDouble2Int()
{
    printf(
    "\n LABEL $Double2Int" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n FLOAT2INT LF@%retval LF@%0" \
    "\n MOVE LF @%retval int@0"\
    "\n ADD LF@%retval LF@%retval LF@%0"
	"\n POPFRAME" \
	"\n RETURN");
}

void generatereadString()
{
    printf(
    "\n LABEL $readString" \
	"\n PUSHFRAME" \
    "\n DEFVAR LF @%retval"\
	"\n READ LF@%retval string" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generatereadInt()
{
    printf(
	"\n LABEL $readInt" \
	"\n PUSHFRAME" \
    "\n DEFVAR LF@%retval" \
	"\n READ LF@%retval int" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generatereadDouble() {
    printf(
        "\n LABEL $readDouble" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n READ LF@%retval double" \
        "\n POPFRAME" \
        "\n RETURN");
}

void generateLength()
{
    printf(
	"\n LABEL $length" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n STRLEN LF@%retval LF@%0" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generateSubstring() {
    printf(
        "\n LABEL $substring" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n MOVE LF@%retval string@" \
        "\n DEFVAR LF@length_str" \
        "\n CREATEFRAME" \
        "\n DEFVAR TF@%0" \
        "\n MOVE TF@%0 LF@%0" \
        "\n CALL $length" \
        "\n MOVE LF@length_str TF@%retval" \
        "\n DEFVAR LF@ret_cond" \
        "\n LT LF@ret_cond LF@length_str int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n EQ LF@ret_cond LF@length_str int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n LT LF@ret_cond LF@%1 int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n EQ LF@ret_cond LF@%1 int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n GT LF@ret_cond LF@%1 LF@length_str"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n EQ LF@ret_cond LF@%2 int@0" \
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n DEFVAR LF@max_n" \
        "\n MOVE LF@max_n LF@length_str" \
        "\n SUB LF@max_n LF@max_n LF@%1" \
        "\n ADD LF@max_n LF@max_n int@1" \
        "\n DEFVAR LF@edit_n_cond" \
        "\n LT LF@edit_n_cond LF@%2 int@0" \
        "\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" \
        "\n GT LF@edit_n_cond LF@%2 LF@max_n" \
        "\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" \
        "\n JUMP $substr$process" \
        "\n LABEL $substr$edit_n" \
        "\n MOVE LF@%2 LF@max_n" \
        "\n LABEL $substr$process" \
        "\n DEFVAR LF@index" \
        "\n MOVE LF@index LF@%1" \
        "\n SUB LF@index LF@index int@1" \
        "\n DEFVAR LF@char"	\
        "\n DEFVAR LF@process_loop_cond" \
        "\n LABEL $substr$process_loop"	\
        "\n GETCHAR LF@char LF@%0 LF@index"	\
        "\n CONCAT LF@%retval LF@%retval LF@char" \
        "\n ADD LF@index LF@index int@1" \
        "\n SUB LF@%2 LF@%2 int@1" \
        "\n GT LF@process_loop_cond LF@%2 int@0" \
        "\n JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true" \
        "\n LABEL $substr$return" \
        "\n POPFRAME" \
        "\n RETURN");
}


void generateord() {
    printf(
        "\n LABEL $ord" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n MOVE LF@%retval int@0" \
        "\n DEFVAR LF@cond_range" \
        "\n LT LF@cond_range LF@%0 int@0" \
        "\n JUMPIFEQ $ord$return LF@cond_range bool@true" \
        "\n GT LF@cond_range LF@%0 int@255" \
        "\n JUMPIFEQ $ord$return LF@cond_range bool@true" \
        "\n STR2INT LF@%retval LF@%0" \
        "\n LABEL $ord$return" \
        "\n POPFRAME" \
        "\n RETURN");
}


void generatechr()
{
    printf(
	"\n LABEL $chr"	\
	"\n PUSHFRAME"	\
	"\n DEFVAR LF@%retval" \
	"\n MOVE LF@%retval string@" \
	"\n DEFVAR LF@cond_range" \
	"\n LT LF@cond_range LF@%0 int@0" \
	"\n JUMPIFEQ $chr$return LF@cond_range bool@true" \
	"\n GT LF@cond_range LF@%0 int@255" \
	"\n JUMPIFEQ $chr$return LF@cond_range bool@true" \
	"\n INT2CHAR LF@%retval LF@%0" \
	"\n LABEL $chr$return" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generateWrite()
{
    printf(
	"\n LABEL $write" \
	"\n PUSHFRAME" \
	"\n WRITE %%0" \
	"\n POPFRAME" \
	"\n RETURN");
}
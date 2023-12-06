/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Code generator interface.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */

#include "code_generator.h"

// Function to check if an identifier corresponds to a built-in function

void start_code_generation(node_t* node) {
    if (node != NULL) {
        generate_statement_list_code(node);
    }
}

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
        generateWrite(node);
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
            printf("WRITE string@%s\n", node->current->value.stringVal);
            break;

        case T_MULTILINE_STRING:
            // Generate code for a multiline string literal
            printf("WRITE string@%s\n", node->current->value.stringVal);
            break;

        case T_IDENTIFIER:
            // Variable reference
            printf("MOVE LF@%s TF@%s\n", node->current->value.ID_name, node->current->value.ID_name);
            break;
        case T_INT:
        case T_KW_INT:
            // Integer literal
            if(node->left->left->current->type == T_KW_INT){
            printf("MOVE LF@%s int@%d\n", node->left->left->left->left->current->value.ID_name, node->current->value.integer);
            node->left->left->left->left->current->value.integer = node->current->value.integer;
            }else{
            printf("MOVE LF@%s int@%d\n", node->left->left->current->value.ID_name, node->current->value.integer);
            node->left->left->current->value.integer = node->current->value.integer;
            }
            break;
        case T_DOUBLE:
        case T_KW_DOUBLE:
            // Double literal
            if(node->left->left->current->type == T_KW_DOUBLE){
            printf("MOVE LF@%s float@%f\n", node->left->left->left->left->current->value.ID_name, node->current->value.decimal);
            node->left->left->left->left->current->value.decimal = node->current->value.decimal;
            }else{
            printf("MOVE LF@%s float@%f\n", node->left->left->current->value.ID_name, node->current->value.decimal);
            node->left->left->current->value.decimal = node->current->value.decimal;
            }
            break;
        default:
            break;
    }
}

void generate_binary_arithmetic_code(node_t* node) {
    if (node->left->current == NULL || node->right->current == NULL) {
        return;
    }
    // Generate code for the binary operation
    switch (node->current->type) {
        case T_PLUS:
        if(node->left->current->value.integer == 0 && node->right->current->value.integer == 0){
            if(node->left->left->left->current->type == T_KW_DOUBLE){
            printf("ADD LF@%s LF@%f LF@%f\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }else{
            printf("ADD LF@%s LF@%f LF@%f\n", node->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }
            break;

        }else{
            if(node->left->left->left->current->type == T_KW_INT){
            printf("ADD LF@%s LF@%d LF@%d\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.integer, node->right->current->value.integer);
            }else{
            printf("ADD LF@%s LF@%d LF@%d\n", node->left->left->left->current->value.ID_name, node->left->current->value.integer, node->right->current->value.integer);
            }
            break;
        }
        case T_MINUS:
        if(node->left->current->value.integer == 0 && node->right->current->value.integer == 0){
            if(node->left->left->left->current->type == T_KW_DOUBLE){
            printf("SUB LF@%s LF@%f LF@%f\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }else{
            printf("SUB LF@%s LF@%f LF@%f\n", node->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }
            break;
        }else{
            if(node->left->left->left->current->type == T_KW_INT){
            printf("SUB LF@%s LF@%d LF@%d\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.integer, node->right->current->value.integer);
            }else{
            printf("SUB LF@%s LF@%d LF@%d\n", node->left->left->left->current->value.ID_name, node->left->current->value.integer, node->right->current->value.integer);
            }
            break;
        }
        case T_MUL:
        if(node->left->current->value.integer == 0 && node->right->current->value.integer == 0){
            if(node->left->left->left->current->type == T_KW_DOUBLE){
            printf("MUL LF@%s LF@%f LF@%f\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }else{
            printf("MUL LF@%s LF@%f LF@%f\n", node->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }
            break;
        }else{
            if(node->left->left->left->current->type == T_KW_INT){
            printf("MUL LF@%s LF@%d LF@%d\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.integer, node->right->current->value.integer);
            }else{
            printf("MUL LF@%s LF@%d LF@%d\n", node->left->left->left->current->value.ID_name, node->left->current->value.integer, node->right->current->value.integer);
            }
            break;
        }
        case T_DIV:
            if(node->right->current->value.decimal == 0){
                exit(57);
            }
            if(node->left->left->left->current->type == T_KW_DOUBLE){
            printf("DIV LF@%s LF@%f LF@%f\n", node->left->left->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }else{
            printf("DIV LF@%s LF@%f LF@%f\n", node->left->left->left->current->value.ID_name, node->left->current->value.decimal, node->right->current->value.decimal);
            }
            break;
        default:
            break;
    }
}

void generate_assignment_code(node_t* node) {
    // Generate code for the expression on the right-hand side
    if(node->right->right->current->type == T_PLUS || node->right->right->current->type == T_MINUS || node->right->right->current->type == T_DIV || node->right->right->current->type == T_MUL){
        generate_expression_code(node->right->right);
    }else{
    generate_expression_code(node->right);
    }
    // Generate the left side of the assignment
    if(node->left->current->Category == TC_TYPE){
        printf("PUSHS LF@%s\n", node->left->left->left->current->value.ID_name);
        printf("POPS LF@%s\n", node->left->left->left->current->value.ID_name);
    }
    else{
        printf("PUSHS LF@%s\n", node->left->current->value.ID_name);
        printf("POPS LF@%s\n", node->left->current->value.ID_name);  
        }
}
void generate_variable_definition_code(node_t* node) {
    if(node->right->right->current->type == T_ASSIGN){
        generate_assignment_code(node->right->right);
    }
    if(node->right->right->current->type == T_KW_STRING){
        generate_expression_code(node->right->right);
    }
    //checking for Token Assign
    if (node->right->right->right->right->current->type == T_ASSIGN){
        generate_assignment_code(node->right->right->right->right);
    }
    if (node->left != NULL && node->left->current->type == T_IDENTIFIER) {
        printf("DEFVAR %s\n", node->current->value.ID_name);
    }
}

void generate_function_call_code(node_t* node) {
    // generates all params
    if (node->right != NULL && node->right->current->type != T_LEFT_BRACE) {
        generate_in_param_list_code(node->right);
    }
}

void generate_in_param_list_code(node_t* node) {
    // Generate code for each parameter in the list

    while (node->right != NULL) {
        generate_in_param_code(node);
        node = node->right;
        if(node->current->type == T_RIGHT_PAREN){
            break;
        }
    }

}

void generate_in_param_code(node_t* node) {
    if (node->left->current->type == T_IDENTIFIER) {
        printf("DEFVAR %s\n", node->left->current->value.ID_name);
    } 
    generate_expression_code(node);
}

void generate_if_statement_code(node_t* node) {
    generate_cond_ops_code(node);
    node_t * tempn = node;
    while(node->right != NULL){
        node = node->right;
            if (node->right->current->type == T_LEFT_BRACE) {
                printf("JUMP_IF_TRUE L%d\n", generate_label());
                printf("LABEL L%d\n", generate_label()-1);
                generate_statement_list_code(node);
            } 
            if (node->right->right->current->type == T_ELSE &&
            node->right->right->right->current->type == T_LEFT_BRACE) {
            printf("JUMP_IF_FALSE L%d\n", generate_label());
            printf("LABEL L%d\n", generate_label()-1);
            generate_statement_list_code(node);
            break;
        }
    }
    node = tempn;
}

void generate_cond_ops_code(node_t* node) {
    generate_expression_code(node);
}

//Helper function for if_statement_code 
int generate_label() {
    static int label_counter = 0;
    label_counter++;
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
        if(node->right->current->type == T_LEFT_BRACE && (node->current->type = T_ELSE || node->current->type == T_IF))
        {
            while(node->current->type != T_RIGHT_BRACE){
                generate_statement_code(node);
                node = node->right;
            }
            return;
        }
        
        generate_statement_code(node);

        if (node->right != NULL) {
        node = node->right;
        }else{
            break;
        }
        if(node->left->current->type == T_IF && node->current->type == T_LEFT_PAREN){
            while(node->current->type != T_RIGHT_PAREN){
                node = node->right;
            }
            return;
        }


        if(node->left->current->type == T_VAR && node->current->type == T_IDENTIFIER){
            node = node->right;
        }
        if(node->left->current->type == T_FUNC){
            while(node->current->type != T_LEFT_BRACE){
            node = node->right;
            }
        }
    }
}

void generate_statement_code(node_t* node) {
    switch (node->current->type) {
        case T_IDENTIFIER:
            if (is_builtin_function(node->current->value.ID_name)) {
                generate_builtin_function_call(node);
            }else{
            generate_expression_code(node);
            }
            break;
        case T_LET:
        case T_VAR:
            generate_variable_definition_code(node);
            break;
        case T_FUNC:
            generate_function_call_code(node);
            break;
        case T_IF:
            generate_if_statement_code(node);
            break;
        case T_WHILE:
            generate_while_loop_code(node->right);
            break;
        case T_RETURN:
            generate_return_statement_code(node->right);
            break;
        default:
            break;
    }
}



void generateInt2Double() {
    printf(
        " LABEL $Int2Double" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n INT2FLOAT LF@%retval LF@%%0" \
        "\n MOVE LF@%retval float@0.0" \ 
        "\n ADD LF@%retval LF@%retval LF@%%0" \ 
        "\n POPFRAME" \
        "\n RETURN \n");
}


void generateDouble2Int()
{
    printf(
    " LABEL $Double2Int" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n FLOAT2INT LF@%retval LF@%%0" \
    "\n MOVE LF @%retval int@0"\
    "\n ADD LF@%retval LF@%retval LF@%%0"
	"\n POPFRAME" \
	"\n RETURN \n");
}

void generatereadString()
{
    printf(
    " LABEL $readString" \
	"\n PUSHFRAME" \
    "\n DEFVAR LF @%retval"\
	"\n READ LF@%retval string" \
	"\n POPFRAME" \
	"\n RETURN \n");
}

void generatereadInt()
{
    printf(
	" LABEL $readInt" \
	"\n PUSHFRAME" \
    "\n DEFVAR LF@%retval" \
	"\n READ LF@%retval int" \
	"\n POPFRAME" \
	"\n RETURN \n");
}

void generatereadDouble() {
    printf(
        " LABEL $readDouble" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n READ LF@%retval double" \
        "\n POPFRAME" \
        "\n RETURN \n");
}

void generateLength()
{
    printf(
	" LABEL $length" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n STRLEN LF@%retval LF@%%0" \
	"\n POPFRAME" \
	"\n RETURN \n");
}

void generateSubstring() {
    printf(
        " LABEL $substring" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n MOVE LF@%retval string@" \
        "\n DEFVAR LF@length_str" \
        "\n CREATEFRAME" \
        "\n DEFVAR TF@%%0" \
        "\n MOVE TF@%%0 LF@%%0" \
        "\n CALL $length" \
        "\n MOVE LF@length_str TF@%retval" \
        "\n DEFVAR LF@ret_cond" \
        "\n LT LF@ret_cond LF@length_str int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n EQ LF@ret_cond LF@length_str int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n LT LF@ret_cond LF@%%1 int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n EQ LF@ret_cond LF@%%1 int@0"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n GT LF@ret_cond LF@%%1 LF@length_str"	\
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n EQ LF@ret_cond LF@%%2 int@0" \
        "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
        "\n DEFVAR LF@max_n" \
        "\n MOVE LF@max_n LF@length_str" \
        "\n SUB LF@max_n LF@max_n LF@%%1" \
        "\n ADD LF@max_n LF@max_n int@1" \
        "\n DEFVAR LF@edit_n_cond" \
        "\n LT LF@edit_n_cond LF@%%2 int@0" \
        "\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" \
        "\n GT LF@edit_n_cond LF@%%2 LF@max_n" \
        "\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" \
        "\n JUMP $substr$process" \
        "\n LABEL $substr$edit_n" \
        "\n MOVE LF@%%2 LF@max_n" \
        "\n LABEL $substr$process" \
        "\n DEFVAR LF@index" \
        "\n MOVE LF@index LF@%%1" \
        "\n SUB LF@index LF@index int@1" \
        "\n DEFVAR LF@char"	\
        "\n DEFVAR LF@process_loop_cond" \
        "\n LABEL $substr$process_loop"	\
        "\n GETCHAR LF@char LF@%0 LF@index"	\
        "\n CONCAT LF@%retval LF@%retval LF@char" \
        "\n ADD LF@index LF@index int@1" \
        "\n SUB LF@%%2 LF@%%2 int@1" \
        "\n GT LF@process_loop_cond LF@%%2 int@0" \
        "\n JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true" \
        "\n LABEL $substr$return" \
        "\n POPFRAME" \
        "\n RETURN \n");
}


void generateord() {
    printf(
        " LABEL $ord" \
        "\n PUSHFRAME" \
        "\n DEFVAR LF@%retval" \
        "\n MOVE LF@%retval int@0" \
        "\n DEFVAR LF@cond_range" \
        "\n LT LF@cond_range LF@%%0 int@0" \
        "\n JUMPIFEQ $ord$return LF@cond_range bool@true" \
        "\n GT LF@cond_range LF@%%0 int@255" \
        "\n JUMPIFEQ $ord$return LF@cond_range bool@true" \
        "\n STR2INT LF@%retval LF@%%0" \
        "\n LABEL $ord$return" \
        "\n POPFRAME" \
        "\n RETURN \n");
}


void generatechr()
{
    printf(
	" LABEL $chr"	\
	"\n PUSHFRAME"	\
	"\n DEFVAR LF@%retval" \
	"\n MOVE LF@%retval string@" \
	"\n DEFVAR LF@cond_range" \
	"\n LT LF@cond_range LF@%%0 int@0" \
	"\n JUMPIFEQ $chr$return LF@cond_range bool@true" \
	"\n GT LF@cond_range LF@%%0 int@255" \
	"\n JUMPIFEQ $chr$return LF@cond_range bool@true" \
	"\n INT2CHAR LF@%retval LF@%%0" \
	"\n LABEL $chr$return" \
	"\n POPFRAME" \
	"\n RETURN \n");
}

void generateWrite(node_t* node)
{
    if(node->right->current->type == T_LEFT_PAREN){
        node->right = node->right->right;
    }
    printf(
	" LABEL write" \
	"\n PUSHFRAME");

    while(node->current->type != T_RIGHT_PAREN){
    printf("\n WRITE LF@%%0 %s", node->right->current->value.ID_name);
    node = node->right->right;
    }
    printf(	"\n POPFRAME" \
	"\n RETURN \n");
}

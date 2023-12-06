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
    //node_t * tempn = node;
    while(node->right != NULL){
        node = node->right;
            if (node->right != NULL && node->right->current->type == T_LEFT_BRACE) {
                printf("JUMP_IF_TRUE L%d\n", generate_label());
                printf("LABEL L%d\n", generate_label()-1);
                generate_statement_list_code(node);
            } 
            if (node->right != NULL && node->right->right != NULL &&node->right->right->current->type == T_ELSE &&
            node->right->right->right != NULL && node->right->right->right->right != NULL &&
             node->right->right->right->current->type == T_LEFT_BRACE) {
            printf("JUMP_IF_FALSE L%d\n", generate_label());
            printf("LABEL L%d\n", generate_label()-1);
            generate_statement_list_code(node);
            break;
        }
    }
    //node = tempn;
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
        "LABEL $Int2Double" 
        "\nPUSHFRAME" 
        "\nDEFVAR LF@%retval" 
        "\nINT2FLOAT LF@%retval LF@%%0"
        "\nMOVE LF@%retval float@0.0" 
        "\nADD LF@%retval LF@%retval LF@%%0" 
        "\nPOPFRAME"
        "\nRETURN \n");
}


void generateDouble2Int()
{
    printf(
    "LABEL $Double2Int"
	"\nPUSHFRAME" 
	"\nDEFVAR LF@%retval"
	"\nFLOAT2INT LF@%retval LF@%%0"
    "\nMOVE LF @%retval int@0"
    "\nADD LF@%retval LF@%retval LF@%%0"
	"\nPOPFRAME" 
	"\nRETURN \n");
}

void generatereadString()
{
    printf(
    "LABEL $readString" 
	"\nPUSHFRAME" 
    "\nDEFVAR LF @%retval"
	"\nREAD LF@%retval string"
	"\nPOPFRAME"
	"\nRETURN \n");
}

void generatereadInt()
{
    printf(
	"LABEL $readInt" 
	"\nPUSHFRAME" 
    "\nDEFVAR LF@%retval" 
	"\nREAD LF@%retval int" 
	"\nPOPFRAME"
	"\nRETURN \n");
}

void generatereadDouble() {
    printf(
        "LABEL $readDouble"
        "\nPUSHFRAME" 
        "\nDEFVAR LF@%retval" 
        "\nREAD LF@%retval double"
        "\nPOPFRAME" 
        "\nRETURN \n");
}

void generateLength()
{
    printf(
	"LABEL $length" 
	"\nPUSHFRAME" 
	"\nDEFVAR LF@%retval" 
	"\nSTRLEN LF@%retval LF@%%0" 
	"\nPOPFRAME" 
	"\nRETURN \n");
}

void generateSubstring() {
    printf(
        "LABEL $substring" 
        "\nPUSHFRAME" 
        "\nDEFVAR LF@%retval" 
        "\nMOVE LF@%retval string@" 
        "\nDEFVAR LF@length_str" 
        "\nCREATEFRAME" 
        "\nDEFVAR TF@%%0" 
        "\nMOVE TF@%%0 LF@%%0" 
        "\nCALL $length" 
        "\nMOVE LF@length_str TF@%retval" 
        "\nDEFVAR LF@ret_cond" 
        "\nLT LF@ret_cond LF@length_str int@0"	
        "\nJUMPIFEQ $substr$return LF@ret_cond bool@true" 
        "\nEQ LF@ret_cond LF@length_str int@0"	
        "\nJUMPIFEQ $substr$return LF@ret_cond bool@true" 
        "\nLT LF@ret_cond LF@%%1 int@0"	
        "\nJUMPIFEQ $substr$return LF@ret_cond bool@true" 
        "\nEQ LF@ret_cond LF@%%1 int@0"	
        "\nJUMPIFEQ $substr$return LF@ret_cond bool@true" 
        "\nGT LF@ret_cond LF@%%1 LF@length_str"	
        "\nJUMPIFEQ $substr$return LF@ret_cond bool@true" 
        "\nEQ LF@ret_cond LF@%%2 int@0" 
        "\nJUMPIFEQ $substr$return LF@ret_cond bool@true" 
        "\nDEFVAR LF@max_n" 
        "\nMOVE LF@max_n LF@length_str" 
        "\nSUB LF@max_n LF@max_n LF@%%1" 
        "\nADD LF@max_n LF@max_n int@1" 
        "\nDEFVAR LF@edit_n_cond" 
        "\nLT LF@edit_n_cond LF@%%2 int@0" 
        "\nJUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" 
        "\nGT LF@edit_n_cond LF@%%2 LF@max_n" 
        "\nJUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" 
        "\nJUMP $substr$process" 
        "\nLABEL $substr$edit_n" 
        "\nMOVE LF@%%2 LF@max_n" 
        "\nLABEL $substr$process" 
        "\nDEFVAR LF@index" 
        "\nMOVE LF@index LF@%%1" 
        "\nSUB LF@index LF@index int@1" 
        "\nDEFVAR LF@char"	
        "\nDEFVAR LF@process_loop_cond" 
        "\nLABEL $substr$process_loop"	
        "\nGETCHAR LF@char LF@%0 LF@index"	
        "\nCONCAT LF@%retval LF@%retval LF@char" 
        "\nADD LF@index LF@index int@1" 
        "\nSUB LF@%%2 LF@%%2 int@1" 
        "\nGT LF@process_loop_cond LF@%%2 int@0" 
        "\nJUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true" 
        "\nLABEL $substr$return"
        "\nPOPFRAME" 
        "\nRETURN \n");
}


void generateord() {
    printf(
        "LABEL $ord" 
        "\nPUSHFRAME" 
        "\nDEFVAR LF@%retval" 
        "\nMOVE LF@%retval int@0" 
        "\nDEFVAR LF@cond_range" 
        "\nLT LF@cond_range LF@%%0 int@0" 
        "\nJUMPIFEQ $ord$return LF@cond_range bool@true" 
        "\nGT LF@cond_range LF@%%0 int@255" 
        "\nJUMPIFEQ $ord$return LF@cond_range bool@true" 
        "\nSTR2INT LF@%retval LF@%%0" 
        "\nLABEL $ord$return" 
        "\nPOPFRAME" 
        "\nRETURN \n");
}


void generatechr()
{
    printf(
	"LABEL $chr"	
	"\nPUSHFRAME"	
	"\nDEFVAR LF@%retval" 
	"\nMOVE LF@%retval string@" 
	"\nDEFVAR LF@cond_range" 
	"\nLT LF@cond_range LF@%%0 int@0" 
	"\nJUMPIFEQ $chr$return LF@cond_range bool@true" 
	"\nGT LF@cond_range LF@%%0 int@255" 
	"\nJUMPIFEQ $chr$return LF@cond_range bool@true" 
	"\nINT2CHAR LF@%retval LF@%%0" 
	"\nLABEL $chr$return" 
	"\nPOPFRAME" 
	"\nRETURN \n");
}

void generateWrite(node_t* node)
{
    if(node->right->current->type == T_LEFT_PAREN){
        node->right = node->right->right;
    }
    printf(
	"LABEL write"
	"\nPUSHFRAME");

    while(node->current->type != T_RIGHT_PAREN){
    printf("\nWRITE LF@%%0 %s", node->right->current->value.ID_name);
    node = node->right->right;
    }
    printf(	"\nPOPFRAME"
	"\nRETURN \n");
}

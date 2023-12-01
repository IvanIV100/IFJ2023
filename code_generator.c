/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Code generator interface.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */

<<<<<<< HEAD
//TODO
//ExpressionVariable generation (look)

=======
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e
#include "code_generator.h"

<<<<<<< HEAD
char *llvlVarForm = "%s@&_%s$%d_&";
char *globVar1 = "GF@$%otempvar%$";
char *globVar2 = "GF@$%otempvar2%$";
=======
//TODO
//free temp_var
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e


<<<<<<< HEAD
    // Initialize frame and variables
    initializeFrameAndVariables();

    // Initialize stack parameter counter
    printf("PUSHS int@0\n");

    generateReadInt();
    generateWrite();
    //generatechr();
    //generateDouble2Int();
    //generateInt2Double();
    //generateLength();
    //generateord();
    //generatereadDouble();
    //generatereadSrting();
    //generateSubstring();    
=======
// Function to check if an identifier corresponds to a built-in function
bool is_builtin_function(const char* identifier) {
    for (int i = 0; built_in_functions[i] != NULL; ++i) {
        if (strcmp(identifier, built_in_functions[i]) == 0) {
            return true;
        }
    }
    return false;
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e
}

void start_code_generation(node_t* parse_tree) {
    generate_statement_list_code(parse_tree);
}

<<<<<<< HEAD
void initializeFrame() {
    printf("CREATEFRAME\n");
}

void defineAndMoveVariable(char *variable, char *value) {
    defineVariable(variable);
    move(variable, value);
}

void defineVariable(char *name) {
    printf("DEFVAR %s\n", name);
}

void move(char *var, char *symb) {
    printf("MOVE %s %s\n", var, symb);
}

void jump(char *lbl) {
    printf("JUMP %s\n", lbl);
}

void address3Operator(char *op, char *res, char *s1, char *s2) {
    if (strcmp(op, "CONCAT") == 0) {
        printf("CONCAT %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "OR") == 0) {
        printf("OR %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "AND") == 0) {
        printf("AND %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "NOT") == 0) {
        printf("NOT %s %s\n", res, s1);
    } else if (strcmp(op, "PLUS") == 0) {
        printf("ADD %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "MINUS") == 0) {
        printf("SUB %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "MUL") == 0) {
        printf("MUL %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "DIV") == 0) {
        printf("DIV %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "NEQ") == 0) {
        printf("EQ %s %s %s\n", s1, s1, s2);
        printf("NOT %s %s\n", res, s1);
    }  else if (strcmp(op, "EQ") == 0) {
        printf("EQ %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "LT") == 0) {
        printf("LT %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "GT") == 0) {
        printf("GT %s %s %s\n", res, s1, s2);
    } else if (strcmp(op, "LTE") == 0) {
        printf("LT %s %s %s\n", globVar1, s1, s2);
        printf("EQ %s %s %s\n", globVar2, s1, s2);
        printf("OR %s %s %s\n", res, globVar1, globVar2);
    } else if (strcmp(op, "GTE") == 0) {
        printf("GT %s %s %s\n", globVar1, s1, s2);
        printf("EQ %s %s %s\n", globVar2, s1, s2);
        printf("OR %s %s %s\n", res, globVar1, globVar2);
    } else if (strcmp(op, "QQ") == 0) {
        char *lbl1 = generateLabel();
        char *lbl2 = generateLabel();
        char *lbl3 = generateLabel();

        printf("JUMPIFEQ %s %s %s\n", lbl1, s1, "nil@nil");
        printf("JUMP %s\n", lbl2);
        printf("LABEL %s\n", lbl1);
        printf("MOVE %s %s\n", res, s2);
        printf("JUMP %s\n", lbl3);
        printf("LABEL %s\n", lbl2);
        printf("MOVE %s %s\n", res, s1);
        printf("LABEL %s\n", lbl3);

        free(lbl1);
        free(lbl2);
        free(lbl3);
    }
}

char *generateExpressionVariable() {
    static int cnt = 0;
    char mx[MAX_VAR];
    sprintf(mx, "TF@$generated_on_stack_%d$", cnt);
    printf("DEFVAR %s\n", mx);
    char *res = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(res, mx);
    cnt++;
    return res;
}

char *getVarName(char *var) {
    Symbol *Cur;
    SymTable *tbl;
    Searching(&tbl,var);

    char mx[MAX_VAR];
    char *frame;
    int lvlNameAdd = -1;
    if (runInfo->currentLVL == 0) {
        frame = "GF";
        //TOTO
    } else if (runInfo->currentLVL->currentTab == runInfo->currentLVL) {
        frame = "TF";
    } else {
        frame = "GF";
        lvlNameAdd = runInfo->currentLVL;
    }

    if (lvlNameAdd == -1) {
        sprintf(mx, "%s@%s", frame, var);
    } else {
        sprintf(mx, llvlVarForm, frame, var, lvlNameAdd);
    }

    char *res = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(res, mx);

    return res;
}

char *convStr(const char *inputStr) {
    size_t inputLength = strlen(inputStr);
    char *resStr = (char *)malloc(sizeof(char) * inputLength * 5);
    size_t i = 0, j = 0;
    for (; i < inputLength; i++) {
        char current = inputStr[i];

        if (((int)current >= 0 && (int)current <= 32) || (int)current == 35 || (int)current == 92) {
            sprintf(resStr + j, "\\%03d", (int)current);
            j += 4;
        } else {
            resStr[j++] = current;
        }
    }
    resStr[j] = '\0';
    return resStr;
}

char *getLitName(Token liter) {
    char mx[MAX_LIT];
    switch (liter.Category) {
        case NIL: {
            sprintf(mx, "nil@nil");
            break;
        }
        case T_KW_STRING: {
            char *tmp = convStr(liter.value.stringVal);
            sprintf(mx, "string@%s", tmp);
            free(tmp);
=======
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
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e
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
            printf("MOVE LF@t%s int@%d\n", get_temp_variable, node->current->value.integer);
            break;

        case T_DOUBLE:
            // Double literal
            printf("MOVE LF@t%s float@%f\n", get_temp_variable, node->current->value.decimal);
            break;
        

        default:
            // Handle other expression types if needed
            break;
    }
<<<<<<< HEAD
    char *res = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(res, mx);
    return res;
}

char *getIntLitName(int c) {
    char mx[MAX_LIT];
    sprintf(mx, "int@%d", c);
    char *res = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(res, mx);
    return res;
}

char *generateNewlbl() {
    static int cnt = 0;
    char mx[MAX_VAR];
    sprintf(mx, "$help_label_%d$", cnt);
    char *res = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(res,mx);
    cnt++;
    return res;
}

void moveVariableToGlobal(char *globalVar, char *localVar) {
    move(globalVar, localVar);
}
void handleGlobalVariable(char *globalVar, char *localVar) {
    Symbol *Cur;

    // Check if the variable exists in the global table
    if (getFromGlobaltbl(globalVar, &Cur)) {
        moveVariableToGlobal(globalVar, localVar);
    } else {
        // If not, define the variable and handle recursion
        char *defSkipLabel = generateLabel();
        printf("JUMPIFEQ %s GF@___$recursion$___ bool@true\n", defSkipLabel);
        defineVariable(globalVar);
        printf("LABEL %s\n", defSkipLabel);
        free(defSkipLabel);

        moveVariableToGlobal(globalVar, localVar);

        // Insert the variable into the global symbol table
        Symbol defined;
        SymtblInit(&defined);
        symtb_insert(runInfo->globalFrame, globalVar, defined);
        RemoveSymbol(runInfo->currentLVL->currentTab,&defined);
    }
}
void passVariablesToGlobal() {
    int localLevel = runInfo->currentLVL;
    for (int i = 0; i < sizeof(runInfo->currentLVL); i++) {
        char globalVar[MAX_VAR];
        sprintf(globalVar, llvlVarForm, "GF", runInfo->currentLVL->currentTab, localLevel);
        char *localVar = getVarName(runInfo->currentLVL->currentTab);
=======
}

void generate_binary_arithmetic_code(node_t* node) {
    //node has a left and right child representing operands
    generate_expression_code(node->left);
    generate_expression_code(node->right);

    if (node->left->current->type == T_INT) {
        printf("INT2FLOAT LF@%d LF@%f\n", node->left->current->value.integer, node->left->current->value.decimal);
    }

    if (node->right->current->type == T_INT) {
        printf("INT2FLOAT LF@%d LF@%f\n", node->right->current->value.integer, node->right->current->value.decimal);
    }

    // Generate code for the binary operation
    switch (node->current->type) {
        case T_PLUS:
            printf("ADD LF@%sLF@%f LF@%f\n", get_temp_variable, node->left->current->value.decimal, node->right->current->value.decimal);
            break;
        case T_MINUS:
            printf("SUB LF@%sLF@%f LF@%f\n", get_temp_variable, node->left->current->value.decimal, node->right->current->value.decimal);
            break;
        case T_MUL:
            printf("MUL LF@%sLF@%f LF@%f\n", get_temp_variable, node->left->current->value.decimal, node->right->current->value.decimal);
            break;
        case T_DIV:
            printf("DIV LF@%s LF@%f LF@%f\n", get_temp_variable, node->left->current->value.decimal, node->right->current->value.decimal);
            break;
        default:
            // Handle other cases if needed
            break;
    }
}
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e

// Helper function to get a temporary variable name
char* get_temp_variable() {
    static int temp_variable_count = 0;
    char* temp_variable_name = malloc(10);
    sprintf(temp_variable_name, "temp%f", temp_variable_count++);
    return temp_variable_name;
}

void free_temp_variable(char * temp_variable_name){
    free(temp_variable_name);
}

void generate_assignment_code(node_t* node) {
    // Generate code for the expression on the right-hand side
    generate_expression_code(node->right);

    //Print the left side of assigment
    if (node->left->current->type == T_IDENTIFIER) {
        printf("POP VAR, %s\n", node->left->current->value.ID_name);
    }
}

void generate_variable_definition_code(node_t* node) {
    if (node->right != NULL) {
        generate_assignment_code(node->right);
    }

    if (node->left->current->type == T_IDENTIFIER) {
        printf("DECLARE VAR, %s\n", node->left->current->value.ID_name);
    }
}

void generate_function_call_code(node_t* node) {
    //generates all params
    generate_in_param_list_code(node->right);

    if (node->left->current->type == T_IDENTIFIER) {
        printf("CALL %s\n", node->left->current->value.ID_name);
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
    } 
}

void generate_if_statement_code(node_t* node) {
    generate_cond_ops_code(node->left);

    if (node->right != NULL && node->right->current->type == T_LEFT_BRACE) {
        printf("JUMP_IF_TRUE L%d\n", generate_label());
        generate_statement_list_code(node->right->right);
        printf("LABEL L%d\n", generate_label());
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
        generate_statement_code(node);
        if (node->right != NULL) {
            node = node->right;
        } else {
            break;  // Break out of the loop if node->right is NULL
        }
    }
}


<<<<<<< HEAD
void moveGlobalToVariable(char *variable, char *globalVar) {
    move(variable, globalVar);
}

void processGlobalVariable(char *variable, char *globalVar) {
    Symbol *Cur;

    // Check if the variable exists in the global tbl
    if (getFromGlobaltbl(globalVar, &Cur)) {
        moveGlobalToVariable(variable, globalVar);
    }
}
void returnPassedVariables() {
    int localLevel = runInfo->currentLVL;
    for (int i = 0; i < sizeof(runInfo->currentLVL); i++) {
        char globalVar[MAX_VAR];
        sprintf(globalVar, llvlVarForm, "GF", runInfo->currentLVL->currentTab, localLevel);
        char *variable = getVarName(runInfo->currentLVL->currentTab);
=======
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
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e

    if (node->left != NULL) {
        generate_expression_code(node->left);
    }
}


<<<<<<< HEAD
void writeCallPutprmtr(Token prmtr, Symbol CFunc) {
    printf("PUSHS %s\n", globVar1);
    printf("ADD %s %s int@1\n", globVar1, globVar1);
    if (prmtr.Category == TC_ID) {
        char *varN = getVarName(prmtr.value.stringVal);
        printf("PUSHS %s\n", varN);
        free(varN);
    } else {
        char *litN = getLitName(prmtr);
        printf("PUSHS %s\n", litN);
        free(litN);
    }
    printf("PUSHS %s\n", globVar1);
}

void functionCallPutprmtr(Token prmtr, Symbol CFunc) {
    if (strcmp(CFunc.id, "write") == 0) {
        writeCallPutprmtr(prmtr, CFunc);
        return;
    }

    if (prmtr.Category == TC_ID) {
        char *varN = getVarName(prmtr.value.stringVal);
        printf("PUSHS %s\n", varN);
        free(varN);
    } else {
        char *litN = getLitName(prmtr);
        printf("PUSHS %s\n", litN);
        free(litN);
    }
}


void generateInt2Double() {
    printf(
        "\n LABEL $int2Double" \
=======
void generateInt2Double() {
    printf(
        "\n LABEL $Int2Double" \
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e
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
<<<<<<< HEAD
        "\n LABEL $substr" \
=======
        "\n LABEL $substring" \
>>>>>>> d27f9842d0298b81609793bc523c57c6732cd08e
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
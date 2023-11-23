/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Code generator interface.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */

//TODO
//ExpressionVariable generation (look)

#include "code_generator.h"
#include "expressions.h"
//#include "parser.c"

char *llvlVarForm = "%s@&_%s$%d_&";
char *globVar1 = "GF@$%otempvar%$";
char *globVar2 = "GF@$%otempvar2%$";

void initializeEnvironment() {
    printf(".ifjcode23\n");

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
}

void initializeFrameAndVariables() {
    initializeFrame();
    defineAndMoveVariable("GF@___$expr_result$___", "bool@false");
    defineVariable(globVar1);
    defineVariable(globVar2);
    defineVariable("GF@%retval");
}

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
            break;
        }
        case T_KW_INT: {
            sprintf(mx, "int@%d", liter.value.integer);
            break;
        }
        case T_KW_DOUBLE: {
            sprintf(mx, "float@%a", liter.value.decimal);
            break;
        }
        default:
            break;
    }
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

        handleGlobalVariable(globalVar, localVar);

        free(localVar);
    }
}


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

        processGlobalVariable(variable, globalVar);

        free(variable);
    }
}


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
        "\n LABEL $substr" \
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
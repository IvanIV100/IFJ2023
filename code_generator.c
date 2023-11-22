/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Code generator interface.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */

#include "code_generator.h"
#include "symtable.h"
#include "parser.h"
#include "parser.c"

char *lolVarForm = "%s@&_%s$%d_&";
char *globVar1 = "GF@$%operatortempvar%$";
char *globVar2 = "GF@$%operatortempvar2%$";

void initializeEnvironment() {
    printf(".ifjcode23\n");

    // Initialize frame and variables
    initializeFrameAndVariables();

    // Initialize stack parameter counter
    printf("PUSHS int@0\n");

    // Generate code to read integer
    generateReadInt();
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

void address3Operator(char *operator, char *result, char *s1, char *s2) {
    if (strcmp(operator, "CONCAT") == 0) {
        printf("CONCAT %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "OR") == 0) {
        printf("OR %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "AND") == 0) {
        printf("AND %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "NOT") == 0) {
        printf("NOT %s %s\n", result, s1);
    } else if (strcmp(operator, "PLUS") == 0) {
        printf("ADD %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "MINUS") == 0) {
        printf("SUB %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "MUL") == 0) {
        printf("MUL %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "DIV") == 0) {
        printf("DIV %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "NEQ") == 0) {
        printf("EQ %s %s %s\n", s1, s1, s2);
        printf("NOT %s %s\n", result, s1);
    }  else if (strcmp(operator, "EQ") == 0) {
        printf("EQ %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "LT") == 0) {
        printf("LT %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "GT") == 0) {
        printf("GT %s %s %s\n", result, s1, s2);
    } else if (strcmp(operator, "LTE") == 0) {
        printf("LT %s %s %s\n", globVar1, s1, s2);
        printf("EQ %s %s %s\n", globVar2, s1, s2);
        printf("OR %s %s %s\n", result, globVar1, globVar2);
    } else if (strcmp(operator, "GTE") == 0) {
        printf("GT %s %s %s\n", globVar1, s1, s2);
        printf("EQ %s %s %s\n", globVar2, s1, s2);
        printf("OR %s %s %s\n", result, globVar1, globVar2);
    } else if (strcmp(operator, "QQ") == 0) {
        char *lbl1 = generateLabel();
        char *lbl2 = generateLabel();
        char *lbl3 = generateLabel();

        printf("JUMPIFEQ %s %s %s\n", lbl1, s1, "nil@nil");
        printf("JUMP %s\n", lbl2);
        printf("LABEL %s\n", lbl1);
        printf("MOVE %s %s\n", result, s2);
        printf("JUMP %s\n", lbl3);
        printf("LABEL %s\n", lbl2);
        printf("MOVE %s %s\n", result, s1);
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
    char *result = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(result, mx);
    cnt++;
    return result;
}

char *getVariableName(char *var) {
    Symbol *found;
    SymTable *table;
    getFromEverywhere(var, &found, &table);

    char mx[MAX_VAR];
    char *frame;
    int lvlNameAdd = -1;
    if (table->local_level == 0) {
        frame = "GF";
    } else if (table->local_level == runInfo->currentLVL) {
        frame = "TF";
    } else {
        frame = "GF";
        lvlNameAdd = table->local_level;
    }

    if (lvlNameAdd == -1) {
        sprintf(mx, "%s@%s", frame, var);
    } else {
        sprintf(mx, lolVarForm, frame, var, lvlNameAdd);
    }

    char *result = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(result, mx);

    return result;
}

char *convertString(const char *inputStr) {
    size_t inputLength = strlen(inputStr);
    char *resultStr = (char *)malloc(sizeof(char) * inputLength * 5);
    size_t i = 0, j = 0;
    for (; i < inputLength; i++) {
        char current = inputStr[i];

        if (((int)current >= 0 && (int)current <= 32) || (int)current == 35 || (int)current == 92) {
            sprintf(resultStr + j, "\\%03d", (int)current);
            j += 4;
        } else {
            resultStr[j++] = current;
        }
    }
    resultStr[j] = '\0';
    return resultStr;
}

char *getLiteralName(Token liter) {
    char mx[MAX_LIT];
    switch (liter.Category) {
        case NIL: {
            sprintf(mx, "nil@nil");
            break;
        }
        case T_KW_STRING: {
            char *tmp = convertString(liter.value.stringVal);
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
    char *result = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(result, mx);
    return result;
}

char *getIntLiteralName(int c) {
    char mx[MAX_LIT];
    sprintf(mx, "int@%d", c);
    char *result = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(result, mx);
    return result;
}

char *generateNewLabel() {
    static int cnt = 0;
    char mx[MAX_VAR];
    sprintf(mx, "$help_label_%d$", cnt);
    char *result = malloc(sizeof(char) * (strlen(mx) + 1));
    strcpy(result, mx);
    cnt++;
    return result;
}

void moveVariableToGlobal(char *globalVar, char *localVar) {
    move(globalVar, localVar);
}

void handleGlobalVariable(char *globalVar, char *localVar) {
    Symbol *found;

    // Check if the variable exists in the global table
    if (getFromGlobalTable(globalVar, &found)) {
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
        SymTableInit(&defined);
        symtb_insert(&global_symtb, globalVar, defined);
        clearSymtbToken(&defined);
    }
}

void passVariablesToGlobal() {
    int localLevel = temp_symtb.local_level;
    for (int i = 0; i < temp_symtb.capacity; i++) {
        if (temp_symtb.symtb_arr[i].deleted)
            continue;

        char globalVar[MAX_VAR];
        sprintf(globalVar, lolVarForm, "GF", temp_symtb.symtb_arr[i].token.id_name, localLevel);
        char *localVar = getVariableName(temp_symtb.symtb_arr[i].token.id_name);

        handleGlobalVariable(globalVar, localVar);

        free(localVar);
    }
}


void moveGlobalToVariable(char *variable, char *globalVar) {
    move(variable, globalVar);
}

void processGlobalVariable(char *variable, char *globalVar) {
    Symbol *found;

    // Check if the variable exists in the global table
    if (getFromGlobalTable(globalVar, &found)) {
        moveGlobalToVariable(variable, globalVar);
    }
}

void returnPassedVariables() {
    int localLevel = temp_symtb.local_level;
    for (int i = 0; i < temp_symtb.capacity; i++) {
        if (temp_symtb.symtb_arr[i].deleted)
            continue;

        char globalVar[MAX_VAR];
        sprintf(globalVar, lolVarForm, "GF", temp_symtb.symtb_arr[i].token.id_name, localLevel);
        char *variable = getVariableName(temp_symtb.symtb_arr[i].token.id_name);

        processGlobalVariable(variable, globalVar);

        free(variable);
    }
}


void writeCallPutprmtr(Token prmtr, Symbol calledFun) {
    printf("PUSHS %s\n", globVar1);
    printf("ADD %s %s int@1\n", globVar1, globVar1);
    if (prmtr.Category == TC_ID) {
        char *varName = getVariableName(prmtr.value.stringVal);
        printf("PUSHS %s\n", varName);
        free(varName);
    } else {
        char *litName = getLiteralName(prmtr);
        printf("PUSHS %s\n", litName);
        free(litName);
    }
    printf("PUSHS %s\n", globVar1);
}

void functionCallPutprmtr(Token prmtr, Symbol calledFun) {
    if (strcmp(calledFun.id, "write") == 0) {
        writeCallPutprmtr(prmtr, calledFun);
        return;
    }

    if (prmtr.Category == TC_ID) {
        char *varName = getVariableName(prmtr.value.stringVal);
        printf("PUSHS %s\n", varName);
        free(varName);
    } else {
        char *litName = getLiteralName(prmtr);
        printf("PUSHS %s\n", litName);
        free(litName);
    }
}


void generateInt2Double()
{
    printf(
    "\n LABEL $Int2Double" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%%retval" \
	"\n INT2FLOAT LF@%%retval LF@%%0" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generateDouble2Int()
{
    printf(
    "\n LABEL $Double2Int" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%%retval" \
	"\n FLOAT2INT LF@%%retval LF@%%0" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generatereadString()
{
    printf(
    "\n LABEL $readString" \
	"\n PUSHFRAME" \
	"\n READ LF@%%retval string" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generatereadInt()
{
    printf(
	"\n LABEL $readInt" \
	"\n PUSHFRAME" \
	"\n READ LF@%%retval int" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generatereadDouble()
{
    printf(
	"\n LABEL $readDouble" \
	"\n PUSHFRAME" \
	"\n READ LF@%%retval float" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generateLength()
{
    printf(
	"\n LABEL $length" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%%retval" \
	"\n STRLEN LF@%%retval LF@%%0" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generateSubstring()
{
    printf(
	"\n LABEL $substring" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%%retval" \
	"\n MOVE LF@%%retval string@" \
	"\n DEFVAR LF@index" \
	"\n MOVE LF@index int@0" \
	"\n DEFVAR LF@length" \
	"\n MOVE LF@length int@0" \
	"\n STRLEN LF@length LF@%%0" \
	"\n DEFVAR LF@char" \
	"\n DEFVAR LF@process_loop_cond" \
	"\n LABEL $substring_loop" \
	"\n GETCHAR LF@char LF@%%0 LF@index" \
	"\n CONCAT LF@%%retval LF@%%retval LF@char" \
	"\n ADD LF@index LF@index int@1" \
	"\n ADD LF@length LF@length int@1" \
	"\n LT LF@process_loop_cond LF@index LF@%1" \
	"\n JUMPIFEQ $substring_loop LF@process_loop_cond bool@true" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generateord()
{
    printf(
	"\n LABEL $ord" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%%retval" \
	"\n STRI2INT LF@%%retval LF@%%0 LF@%%1" \
	"\n POPFRAME" \
	"\n RETURN");
}

void generatechr()
{
    printf(
	"\n LABEL $chr" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%%retval" \
	"\n INT2CHAR LF@%%retval LF@%%0" \
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
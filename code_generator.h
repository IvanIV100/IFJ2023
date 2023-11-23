#include "symtable.h"
#include "scanner.h"
#include "parser.c"
#include "parser.h"

// code_generator.h

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "symtable.h"
#include "parser.h"

#define MAX_VAR 256
<<<<<<< HEAD
#define MAX_LIT 8192

extern char *llvlVarForm;
extern char *globVar1;
extern char *globVar2;

void initializeEnvironment();
void initializeFrameAndVariables();
void initializeFrame();
void defineAndMoveVariable(char *variable, char *value);
void defineVariable(char *name);
void move(char *var, char *symb);
void jump(char *lbl);
void address3Operator(char *op, char *res, char *s1, char *s2);
char *generateExpressionVariable();
char *getVariableName(char *var);
char *convertString(const char *inputStr);
char *getLiteralName(Token liter);
char *getIntLiteralName(int c);
char *generateNewLabel();
void moveVariableToGlobal(char *globalVar, char *localVar);
void handleGlobalVariable(char *globalVar, char *localVar);
void passVariablesToGlobal();
void moveGlobalToVariable(char *variable, char *globalVar);
void processGlobalVariable(char *variable, char *globalVar);
void returnPassedVariables();
void writeCallPutprmtr(Token prmtr, Symbol CFunc);
void functionCallPutprmtr(Token prmtr, Symbol CFunc);
//BUILT-IN functions 
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

#endif CODE_GENERATOR_H
=======
#define MAX_LIT 8192
>>>>>>> 9b1427c4e147c41b9f453655f2c63677e48b99ef

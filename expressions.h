/**
 * Project: Implementace překladače IFJ23
 *
 * @brief  Syntax and semantic analysis for expressions .h file.
 *
 * @author Ivan Chodák xchoda00
 */


#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H


#include <stdlib.h>


#include "error.h"
#include "symtable.h"
#include "scanner.h"

typedef struct node {
    struct Token *current;
    struct node* left;
    struct node* right;
    
} node_t;

typedef struct runTimeInfo {
    SymTable* globalFrame;
    SymTable* builtInFunctions;
    struct symTabLVL *currentLVL;
    node_t* firstNode;

    char* FID;
    char* leftID;
    char* rightID;
    int vol;
    int inDef;

} runTimeInfo;

typedef struct symTabLVL {
    SymTable *currentTab;
    struct symTabLVL *parantLVL;
    
} symTabLVL;








typedef enum relation_type{
    R_SHIFT,
    R_REDUCE,
    R_EQUAL,
    R_ERROR
} RelationType;

typedef enum term_type{
    T_UNKNOWN, 
    T_ADDSUB,
    T_MULDIV,
    T_LTGT,
    T_EQ,
    T_LITERAL,
    T_DQ,
    T_LPAREN,
    T_RPAREN,
    T_VARIABLE,
    T_$,
} TermType;

typedef enum stack_item_type{
    SHIFT,
    TERMINAL,
    NONTERMINAL,
} StackItemType;

typedef enum exprType{
    E_UNKNOWN,
    E_INT,
    E_DOUBLE,
    E_STRING,
    E_BOOL,
    E_BUILTIN,
    E_FUN,
    E_NIL,
} ExprType;


typedef struct StackItem{
    StackItemType type;
    Token *term;
    ExprType exprType;
} *stackItem;

typedef struct Stack{
    int top;
    int size;
    stackItem *items;
} *stack;

void start_code_generation(node_t* parse_tree);
Symbol *search_upwards_ST(char* ID);
void stack_init(stack stack);
void stack_dispose(stack stack);
void stack_push(stack stack, stackItem item);
stackItem stack_pop(stack stack);
TermType stack_top(stack stack);
void stack_shift(stack stack, int index);
int stack_get_index(stack stack, TermType type);
void print_stack(stack stack);
RelationType precedence_table(TermType stackTerm, TermType newTerm);
TermType token_to_term(Token *token);
DataType expression_parser(node_t* node, runTimeInfo *rti, int countDown);
int expression_reduce(stack stack, runTimeInfo *rti);

#endif
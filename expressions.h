#include <stdlib.h>
#include "scanner.h"
#include "parser.h"





typedef enum{
    R_SHIFT,
    R_REDUCE,
    R_EQUAL,
    R_ERROR
} RelationType;

typedef enum{
    T_UNKNOWN,
    T_ADDSUB,
    T_MULDIV,
    T_LTGT,
    T_EQ,
    T_LPAREN,
    T_RPAREN,
    T_VARIABLE,
    T_FUNC
} TermType;

typedef enum{
    SHIFT,
    TERMINAL,
    NONTERMINAL
} StackItemType;

typedef enum{
    E_UNKNOWN,
    E_INT,
    E_DOUBLE,
    E_STRING,
    E_BOOL,
    E_BUILTIN,
    E_FUN
} ExprType;


typedef struct StackItem{
    StackItemType type;
    Token *token;
    ExprType exprType;
} *stackItem;

typedef struct Stack{
    int top;
    int size;
    stackItem *items;
} *stack;

void stack_init(stack stack);
void stack_dispose(stack stack);
void stack_push(stack stack, stackItem item);
stackItem stack_pop(stack stack);
TermType stack_GetTopTerm(stack stack);
void stack_shift(stack stack, int index);
int stack_get_index(stack stack, TermType type);
RelationType precedence_table(TermType stackTerm, TermType newTerm);
TermType get_term(Token *token);
ExprType expression_parser(node_t *node, runTimeInfo *rti);
int expression_reduce(stack stack, runTimeInfo *rti);
node_t *expression_generate(stack stack, runTimeInfo *rti);
int generate_expression_reduce(stack stack, runTimeInfo *rti);
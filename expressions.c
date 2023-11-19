
#include <string.h>
#include <stdbool.h>

#include "expressions.h"

//#include "codegen.h"


void stack_init(stack stack){
    stack->top = -1;
    stack->size = 100;
    stack->items = malloc(sizeof(stackItem) * stack->size);
}

void stack_dispose(stack stack){
    if (stack == NULL){
        return;
    }
    for (int i = stack->top; i >= 0; i--){
        free(stack->items[i]);
    }
    free(stack->items);
    free(stack);
    stack = NULL;
}
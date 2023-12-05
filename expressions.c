#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#include "expressions.h"


DataType overrideType = VOID;
//#include "codegen.h"

int countDown = 0;
DataType returnTerm = VOID;

void stack_init(stack stack){
    
    stack->size = 100;
    stack->items = malloc(sizeof(stackItem) * stack->size);
    stack->top = -1;
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
}

void stack_push(stack stack, stackItem item){
    stack->top++;
    if (stack->top >= stack->size){
        stack->size += 100;
        stack->items = realloc(stack->items, sizeof(stackItem) * stack->size);
    }
    stack->items[stack->top] = item;
}

stackItem stack_pop(stack stack){
    if (stack->top < 0){
        ThrowError(99);
    }
    stackItem item = stack->items[stack->top];
    stack->top--;
    return item;
}

TermType stack_top(stack stack){
    int index = stack->top;
    while(index >= 0){
        if (stack->items[index]->type == TERMINAL){
            return token_to_term(stack->items[index]->term); // return type of current terminal;
        }
        index--;
    } 
    return T_$;
}

void stack_shift(stack stack, int index){
    stack_push(stack, NULL);
    for (int i = stack->top-1; i > index; i--){
        stack->items[i+1] = stack->items[i];
    }
    stackItem item = malloc(sizeof(struct StackItem));
    item->type = SHIFT;
    stack->items[index + 1] = item;
    
}

int stack_get_index(stack stack, TermType type){
    for (int i = stack->top; i >= 0; i--){
        if (token_to_term(stack->items[i]->term) == type){
            return i;
        }
    }
    return -1;
}

void print_stack(stack stack){
    printf("new round: \n");
    for (int i = 0; i <= stack->top; i++){
        if (stack->items[i]->type == TERMINAL){
            printf("Pterminal, ");
            printf("type %d \n", token_to_term(stack->items[i]->term));   
        } else if (stack->items[i]->type == NONTERMINAL){
            printf("Pnonterminal, ");
            printf("type %d \n", stack->items[i]->exprType);
        } else {
            printf("Pshift, ");
        }
    }
    printf("\n");
}


RelationType precedence_table(TermType stackTerm, TermType newTerm){
    switch (stackTerm){
        case T_ADDSUB:
            if (newTerm == T_MULDIV || newTerm == T_LPAREN || newTerm == T_VARIABLE || newTerm == T_LITERAL ){
                return R_SHIFT;
            } else {
                return R_REDUCE;
            }
        
        case T_MULDIV:
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE || newTerm == T_LITERAL){
                return R_SHIFT;
            } else {
                return R_REDUCE;
            }
        
        case T_LTGT: 
            if (newTerm == T_LTGT){
                return R_ERROR;
            } else if (newTerm == T_EQ || newTerm == T_DQ || newTerm == T_RPAREN || newTerm == T_$){
                return R_REDUCE;
            } else {
                return R_SHIFT;
            }

        case T_EQ:
            if(newTerm == T_EQ){
                return R_ERROR;
            } else if (newTerm == T_RPAREN || newTerm == T_$){
                return R_REDUCE;
            } else {
                return R_SHIFT;
            }
            
        
        case T_DQ:
            if (newTerm == T_DQ || newTerm == T_RPAREN || newTerm == T_$){
                return R_REDUCE;
            } else {
                return R_SHIFT;
            }
        
        case T_LPAREN:
            if (newTerm == T_$){
                return R_ERROR;
            } else if (newTerm == T_RPAREN){
                return R_EQUAL;
            } else {
                return R_SHIFT;
            }
        
        case T_RPAREN:
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE || newTerm == T_LITERAL){
                return R_ERROR;
            } else {
                return R_REDUCE;
            }
        
        case T_VARIABLE:
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE || newTerm == T_LITERAL){
                return R_ERROR;
            } else {
                return R_REDUCE;
            }
        
        case T_LITERAL:
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE || newTerm == T_LITERAL){
                return R_ERROR;
            } else {
                return R_REDUCE;
            }

        case T_$:
            if (newTerm == T_$ || newTerm == T_RPAREN){
                return R_ERROR;
            } else {
                return R_SHIFT;
            }

        case T_UNKNOWN:
            break;
    }
    return R_ERROR;

}

TermType token_to_term(Token *token){
    if (token == NULL){
        return T_UNKNOWN;
    }

    switch (token->type){
        case T_PLUS:
        case T_MINUS:
            return T_ADDSUB;
        
        case T_MUL:
        case T_DIV:
            return T_MULDIV;
        
        case T_GREATER:
        case T_GREATER_EQUAL:
        case T_LESS_THAN:
        case T_LESS_EQUAL:
            overrideType = BOOL;
            return T_LTGT;

        case T_EQUALS:
        case T_NOT_EQUALS:
            overrideType = BOOL;
            return T_EQ;

        case T_LEFT_PAREN:
            return T_LPAREN;
        
        case T_RIGHT_PAREN:
            return T_RPAREN;

        
        case T_INT:
        case T_DOUBLE:
        case T_STRING:
        case T_NIL:
            return T_LITERAL;

        case T_IDENTIFIER:
            return T_VARIABLE;

        case T_DOUBLE_QUESTION_MARK:
            return T_DQ;
        default:
            return T_$;
    }
}

DataType expression_parser(node_t *node, runTimeInfo *rti, int length){
    countDown = length;
    printf("length: %d\n", length);
    stack stack = malloc(sizeof(struct Stack));
    stack_init(stack);

    if(node->current->type >= 23 && node->current->type <= 33){
        if (node->current->type != T_NIL){
            printf("error7\n");
            ThrowError(2);
        }
    }
    TermType newTerm, stackTerm = T_UNKNOWN;
    int index;
    int EQcount = 0;

    if (length == 1){ //*add nillable options*
        if (node->current->type == T_IDENTIFIER){
            Symbol *checkType;
            if (rti->currentLVL != NULL){
                if (GetSymbol(rti->currentLVL->currentTab, node->current->value.ID_name) == NULL){
                    ThrowError(5);
                    } else {
                    checkType = GetSymbol(rti->currentLVL->currentTab, node->current->value.ID_name);
                    }
            } else {
                if (GetSymbol(rti->globalFrame, node->current->value.ID_name) == NULL){
                    ThrowError(5);
                } else {
                    checkType = GetSymbol(rti->globalFrame, node->current->value.ID_name);
                }

                if (checkType->variable.datatype == INT){
                    returnTerm = INT;
                } else if (checkType->variable.datatype == FLOAT){
                    returnTerm = FLOAT;
                } else if (checkType->variable.datatype == STR){
                    returnTerm = STR;
                } else if (checkType->variable.datatype == NIL){
                    returnTerm = NIL;
                }
            }
        } else if (node->current->type == T_INT){
            returnTerm = INT;
        } else if (node->current->type == T_DOUBLE){
            returnTerm = FLOAT;
        } else if (node->current->type == T_STRING){
            returnTerm = STR;
        } else if (node->current->type == T_NIL){
            returnTerm = NIL;
        } else {
            printf("error8\n");
            ThrowError(2);
        }
        printf("overrideType: %d\n", overrideType);
        if (overrideType != VOID){
            returnTerm = overrideType;
        }
        return returnTerm;
    }

    while((newTerm != T_$ || stackTerm != T_$) && countDown > 0){

        stackTerm = stack_top(stack);
        newTerm = token_to_term(node->current);
        
        stackItem item = malloc(sizeof(struct StackItem));
        if (item == NULL){
            ThrowError(99);
        }
        printf("newTerm: %d, stackTerm: %d\n", newTerm, stackTerm);
        if (newTerm == T_LTGT || newTerm == T_DQ){
            EQcount++;
            if (EQcount > 2){
                printf("error99\n");
                ThrowError(2);
            }
        }

        switch (precedence_table(stackTerm, newTerm)){
            case R_SHIFT:
                index = stack_get_index(stack, stackTerm);
                stack_shift(stack, index);
                item->type = TERMINAL;
                item->term = node->current;
                stack_push(stack, item);
                node = node->right;
                countDown--;
                break;

            case R_REDUCE:
                expression_reduce(stack, rti);
                break;

            case R_EQUAL:
                item->type = TERMINAL;
                item->term = node->current;
                stack_push(stack, item);
                node = node->right;
                countDown--;
                break;

            case R_ERROR:
                printf("331 newTerm: %d, stackTerm: %d\n", newTerm, stackTerm);
                if ((newTerm == T_$ || newTerm == T_RPAREN) && stackTerm == T_$){
                    if (node->current->type == T_RIGHT_PAREN && stack->top >= 0){
                        return stack_pop(stack)->exprType;
                    } else {
                        printf("error1\n");
                        ThrowError(2);
                    }
                } else {
                    printf("error\n");
                    ThrowError(2);
                }
                break;
        }
    

    }
    stack_dispose(stack);
    return returnTerm;
}

int expression_reduce(stack stack, runTimeInfo *rti){

    SymTable *currentST;
    if (rti->currentLVL == NULL){
        currentST = rti->globalFrame;
    } else {
        currentST = rti->currentLVL->currentTab;
    }
    stackItem item;
    item =stack_pop(stack);

    if (item->type == NONTERMINAL){
        stackItem E2 = item;
        stackItem op = stack_pop(stack);
        if (op->type == TERMINAL && (token_to_term(op->term) != T_VARIABLE || 
            token_to_term(op->term) != T_LPAREN ||
            token_to_term(op->term) != T_RPAREN || token_to_term(op->term) != T_$)){
        
            stackItem E1 = stack_pop(stack);

            //check if null is viable and add it
            if (E1->type == NONTERMINAL){
                if (E1->exprType == E2->exprType){
                    item->exprType = E1->exprType;
                } else if (E1->exprType == INT && E2->exprType == FLOAT){
                    item->exprType = FLOAT;
                } else if (E1->exprType == FLOAT && E2->exprType == INT){
                    item->exprType = FLOAT; 
                } else if (E1->exprType == NIL && E2->exprType == INT){
                    item->exprType = INT;
                } else if (E1->exprType == INT && E2->exprType == NIL){
                    item->exprType = INT;
                } else if (E1->exprType == NIL && E2->exprType == FLOAT){
                    item->exprType = FLOAT;
                } else if (E1->exprType == FLOAT && E2->exprType == NIL){
                    item->exprType = FLOAT;
                } else if (E1->exprType == NIL && E2->exprType == BOOL){
                    item->exprType = BOOL;
                } else if (E1->exprType == BOOL && E2->exprType == NIL){
                    item->exprType = BOOL;
                } else if (E1->exprType == NIL && E2->exprType == STR){
                    item->exprType = STR;
                } else if (E1->exprType == STR && E2->exprType == NIL){
                    item->exprType = STR;
                } else if (token_to_term(op->term) == T_EQ || token_to_term(op->term) == T_LTGT){
                    item->exprType = BOOL;
                } else {
                    printf("error2\n");
                    ThrowError(7);
                }
            
                returnTerm = item->exprType;

                item->type = NONTERMINAL;
                item->term = NULL;
            }
        } else{
            printf("error3\n");
            ThrowError(2);
        }
    } else if (item->type == TERMINAL){
        if (token_to_term(item->term) == T_VARIABLE || token_to_term(item->term) == T_LITERAL){
            switch (item->term->type) {

                case T_INT:
                    item->type = NONTERMINAL;
                    item->exprType = INT;
                    break;
                
                case T_DOUBLE:
                    item->type = NONTERMINAL;
                    item->exprType = FLOAT;
                    break;

                case T_STRING:
                    item->type = NONTERMINAL;
                    item->exprType = STR;
                    break;
                
                case T_IDENTIFIER: 
                    
                    if (GetSymbol(currentST, item->term->value.ID_name) == NULL){
                        ThrowError(5);
                    } 
                    Symbol *checkType;
                    checkType = GetSymbol(currentST, item->term->value.ID_name);
                    if (checkType->variable.datatype == INT){
                        item->exprType = INT;
                    } else if (checkType->variable.datatype == FLOAT){
                        item->exprType = FLOAT;
                    } else if (checkType->variable.datatype == STR){
                        item->exprType = STR;
                    } else if (checkType->variable.datatype == NIL){
                        item->exprType = NIL;
                    }
                    
                    item->type = NONTERMINAL;
                    break;
                
                case T_NIL:
                    item->type = NONTERMINAL;
                    item->exprType = NIL;
                    break;
                
                default:
                    printf("error4\n");
                    ThrowError(2);
            }    
        } else if (token_to_term(item->term) == T_RPAREN){
            item = stack_pop(stack);
            stackItem E = item;
            if (item->type == NONTERMINAL){
                item = stack_pop(stack);
                if (item->type == TERMINAL && token_to_term(item->term) == T_LPAREN){
                    item->exprType = E->exprType;
                    item->type = NONTERMINAL;
                }
            } else { 
                printf("error5\n");
                ThrowError(2);
            }
        } else {
            printf("error6\n");
            ThrowError(2);
            }   
        }

    stackItem shift = stack_pop(stack);
    if (shift->type == SHIFT){
        free(shift);
    } 
    stack_push(stack, item);
    return 1;
    
}


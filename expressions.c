#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "expressions.h"
#include "error.h"
#include "symtable.h"

//#include "codegen.h"

int countDown = 0;
ExprType returnTerm = E_UNKNOWN;

void stack_init(stack stack){
    stack->top = -1;
    stack->size = 100;
    stack->items = malloc(sizeof(stackItem) * stack->size);
}

void stack_dispose(stack stack){
    if (stack == NULL){
        return;
    }
    printf("dispose\n");
    for (int i = stack->top; i > 0; i--){
        if (stack->items[i] != NULL){
            free(stack->items[i]);
            stack->items[i] = NULL;
        }
        
    }
    printf("dispose2\n");
    if (stack->items != NULL){
        free(stack->items);
        stack->items = NULL;
    }
    printf("dispose3\n");
    if (stack != NULL){
        free(stack);
        stack = NULL;
    }
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
        return NULL;
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
    for (int i = stack->top-1; i > 0; i--){
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
            if (newTerm == T_MULDIV || newTerm == T_LPAREN || newTerm == T_VARIABLE){
                return R_SHIFT;
            } else {
                return R_REDUCE;
            }
        
        case T_MULDIV:
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE){
                return R_SHIFT;
            } else {
                return R_REDUCE;
            }
        
        case T_REL: //check if ltgt and eq arent the same
            if (newTerm == T_REL){
                return R_ERROR;
            } else if (newTerm == T_DQ || newTerm == T_RPAREN || newTerm == T_$){
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
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE){
                return R_ERROR;
            } else {
                return R_REDUCE;
            }
        
        case T_VARIABLE:
            if (newTerm == T_LPAREN || newTerm == T_VARIABLE){
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
        case T_EQUALS:
        case T_NOT_EQUALS:
            return T_REL;

        case T_LEFT_PAREN:
            return T_LPAREN;
        
        case T_RIGHT_PAREN:
            return T_RPAREN;

        case T_IDENTIFIER:
        case T_INT:
        case T_DOUBLE:
        case T_STRING:
            return T_VARIABLE;

        case T_DOUBLE_QUESTION_MARK:
            return T_DQ;
        default:
            return T_$;
    }
}

ExprType expression_parser(node_t *node, runTimeInfo *rti, int length){
    countDown = length;
    printf("expression parser\n");
    printf("current node: %d\n", node->current->type);
    // SymTable *currentST = NULL;
    // if (rti->currentLVL == NULL){
    //     currentST = rti->globalFrame;
    // } else {
    //     currentST = rti->currentLVL->currentTab;
    // }

    stack stack = malloc(sizeof(struct Stack));
    stack_init(stack);

    //check if synced
    //node_t* node = get_next(node);
    if(node->current->type >= 23 && node->current->type <= 33){
        return E_BUILTIN;
    }
    printf("not keyword\n");
    TermType newTerm, stackTerm;
    int index;
    int EQcount = 0;

    while((newTerm != T_$ || stackTerm != T_$) && countDown > 0){
        stackTerm = stack_top(stack);
        newTerm = token_to_term(node->current);
        printf("stackTerm: %d\n", stackTerm);
        printf("newTerm: %d\n", newTerm);
        
        stackItem item = malloc(sizeof(struct StackItem));
        if (item == NULL){
            ThrowError(99);
        }

        if (newTerm == T_REL || newTerm == T_DQ){
            EQcount++;
            if (EQcount > 2){
                printf("eq\n");
                ThrowError(2);
            }
        }

        switch (precedence_table(stackTerm, newTerm)){
            case R_SHIFT:
                printf("SWshift\n");
                //print_stack(stack);
                index = stack_get_index(stack, stackTerm);
                stack_shift(stack, index);
                item->type = TERMINAL;
                item->term = node->current;
                stack_push(stack, item);
                //print_stack(stack);
                node = node->right;
                countDown--;
                printf("countDown: %d\n", countDown);
                // if (countDown == 0)
                // {
                //     return 691;
                // }
                

                break;

            case R_REDUCE:
                printf("reduce\n");
                //print_stack(stack);
                expression_reduce(stack, rti);
                //print_stack(stack);
                break;

            case R_EQUAL:
                printf("equal\n");
                //print_stack(stack);
                item->type = TERMINAL;
                item->term = node->current;
                stack_push(stack, item);
                node = node->right;
                countDown--;
                // if (countDown == 0)
                // {
                //     return 692;
                // }
                break;

            case R_ERROR:
                printf("error\n");
                if ((newTerm == T_$ || newTerm == T_RPAREN) && stackTerm == T_$){
                    if ((node->current->type == T_RIGHT_PAREN) && stack->top >= 0){
                        printf("fail\n");
                        return stack_pop(stack)->exprType;
                    } else {
                        printf("not ) and empty\n");
                        ThrowError(2);
                    }
                } else {
                    printf("not $ or )\n");
                    ThrowError(2);
                }
                break;
                
            default:
                printf("default\n");
                break;
        }

    }
    printf("end\n");
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
                    returnTerm = item->exprType;
                } else if (E1->exprType == E_INT && E2->exprType == E_DOUBLE){
                    item->exprType = E2->exprType;
                    returnTerm = item->exprType;
                } else if (E1->exprType == E_DOUBLE && E2->exprType == E_INT){
                    item->exprType = E1->exprType; 
                    returnTerm = item->exprType;
                } else if (token_to_term(op->term) == T_REL){
                    item->exprType = E_BOOL;
                    returnTerm = item->exprType;
                } else {
                    printf("not same type\n");
                    ThrowError(2);
                }
            
                
                item->type = NONTERMINAL;
                item->term = NULL;
                }
            } else{
                ThrowError(2);
            }
    } else if (item->type == TERMINAL){
        if (token_to_term(item->term) == T_VARIABLE){
            switch (item->term->type) {

                case T_INT:
                    item->type = NONTERMINAL;
                    item->exprType = E_INT;
                    returnTerm = item->exprType;
                    break;
                
                case T_DOUBLE:
                    item->type = NONTERMINAL;
                    item->exprType = E_DOUBLE;
                    returnTerm = item->exprType;
                    break;

                case T_STRING:
                    item->type = NONTERMINAL;
                    item->exprType = E_STRING;
                    returnTerm = item->exprType;
                    break;
                
                case T_IDENTIFIER: 
                    
                    if (GetSymbol(currentST, item->term->value.ID_name) == NULL){
                        ThrowError(5);
                    } 
                    Symbol *checkType;
                    checkType = GetSymbol(currentST, item->term->value.ID_name);
                    if (checkType->variable.datatype == INT){
                        item->exprType = E_INT;
                        returnTerm = item->exprType;
                    } else if (checkType->variable.datatype == FLOAT){
                        item->exprType = E_DOUBLE;
                        returnTerm = item->exprType;
                    } else if (checkType->variable.datatype == STR){
                        item->exprType = E_STRING;
                        returnTerm = item->exprType;
                    } else if (checkType->variable.datatype == NIL){
                        item->exprType = E_NIL;
                        returnTerm = item->exprType;
                    }
                    
                    item->type = NONTERMINAL;
                    break;
                
                case T_NIL:
                    item->type = NONTERMINAL;
                    item->exprType = E_NIL;
                    returnTerm = item->exprType;
                    break;
                
                default:
                    ThrowError(2);
            }    
        } else if (token_to_term(item->term) == T_RPAREN){
            item = stack_pop(stack);
            stackItem E = item;
            if (item->type == NONTERMINAL){
                item = stack_pop(stack);
                if (item->type == TERMINAL && token_to_term(item->term) == T_LPAREN){
                    item->exprType = E->exprType;
                    returnTerm = item->exprType;
                    item->type = NONTERMINAL;
                }
            } else { 
                printf("348 not viable\n");
                ThrowError(2);
            }
        } else {
            print_stack(stack);
            printf("352 not viable\n");
            ThrowError(2);
        
            }   
        }

    stackItem shift = stack_pop(stack);
    if (shift->type == SHIFT){
        free(shift);
        stack->top--;
    } 
    stack_push(stack, item);
    return 1;
    
}

// node_t* generate_expression(node_t* node, runTimeInfo *rti){
//     stack stack = malloc(sizeof(struct Stack));
//     stack_init(stack);

//     node = get_next(node);

//     if (node->current->type >= 23 && node->current->type <= 38){
//         if (node->current->type != 34){
//             return node->current;
//         }
//     }
//     TermType newTerm, stackTerm; =T_UNKNOWN;
//     int index;
//     int EQcount = 0;

//     while (newTerm != T_$ || stackTerm != T_$){
//         stackTerm = stack_top(stack);
//         newTerm = token_to_term(node->current);

//         stackItem item = malloc(sizeof(struct StackItem));
//         if (item == NULL){
//             ThrowError(99);
//         }

//         if (newTerm == T_EQ || newTerm == T_LTGT){
//             EQcount++;
//             if (EQcount > 2){
//                 ThrowError(2);
//             }
//         }

//         switch(precedence_table(stackTerm, newTerm)){
//             case R_SHIFT:
//                 index = stack_get_index(stack, stackTerm);
//                 stack_shift(stack, index);

//                 item->type = TERMINAL;
//                 item->term = node->current;
//                 stack_push(stack, item);
//                 node = get_next(node);
//                 break;

//             case R_REDUCE:
//                 generate_expression_reduce(stack, rti);
//                 break;

//             case R_EQUAL:
//                 item->type = TERMINAL;
//                 item->term = node->current;
//                 stack_push(stack, item);
//                 node = get_next(node);
//                 break;

//             case R_ERROR
//                 if ((newTerm == T_$ || newTerm == T_RPAREN) && stackTerm == T_$){
//                     if ((node->current->type == T_RIGHT_PAREN) && stack->top >= 0){
//                         printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
//                         return node; 
//                     } else {
//                         ThrowError(2);
//                     }
//                 } else {
//                     ThrowError(2);
//                 }
//                 break;
//         }
//     }
//     stack_dispose(stack);
//     return node;
// }

// int generate_expression_reduce(stack stack, runTimeInfo *rti){
//     return 0;

// }
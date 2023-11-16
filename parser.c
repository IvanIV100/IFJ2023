/*
Parser for the compiler
xchoda00
2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "scanner.h" //change to scanner.h
#include "error.h"


/* TODO 
* - symtabLVL
    -clean up
    -symtab insert

* - sort out return validity in func
* - fix mem free of linked list
* - optional else?
* - handle built in funcs
* - check if err output is correct
* - check if "var name" is valid
* - expression parser

* ---TESTS---
* valid:
* 
* - var def
* - var def with assign expression
* - var def with assign  funcall
* - var def with assign  funcall and in params
* - id with assign
* - id with assign and funcall
* - id with assign and funcall and in params
* - id funcall
* - if
* - if else
* - while 
* x func def 
* x func def w retT
* - func def with body
* - func def with retT and body
* x func def with params
* - func def with params and returnt
* - func def with params and returnt and body

*/

myInfo *runInfo;

node_t* handle_param(node_t* node){
    if (node->current->type == T_UNDERSCORE){
        //handle in symtab _ case
    } else if (node->current->type == T_IDENTIFIER){
        //symtab assign
    } else {
        ThrowError(2);
    }
    node = get_next(node);
    if (node->current->type != T_IDENTIFIER){
        ThrowError(2);
    }
    node = get_next(node);
    if (node->current->type != T_COLON){
        ThrowError(2);
    }
    node = get_next(node);

    node = handle_type(node);

    return  node;
}


node_t* handle_param_list(node_t* node){
    if (node->current->type == T_RIGHT_PAREN){
        return node;
    } else {
        node = handle_param(node);
    }
    node = get_next(node);
    if (node->current->type == T_COMMA){    
        node = get_next(node);
        if (node->current->type == T_RIGHT_PAREN){            // (param,) invalid

            ThrowError(2);
        }
        node = handle_param_list(node);
        return node;
    } else {
        return node;
    }
}

node_t* handle_type(node_t* node){ //what is difference between t_int a t_kw_int

    if (node->current->type == T_KW_INT){
        //assign int in symtable with ? check
    } else if (node->current->type == T_KW_DOUBLE){
        //assign double in symtable with ? check
    } else if (node->current->type == T_KW_STRING){
        //assign str in symtable with ? check
    } else {
        ThrowError(2);
    }
    return  node;
}

node_t* hadnle_func_retType(node_t* node){
    if (node->current->type == T_ARROW){
        node = get_next(node);
        node = handle_type(node);
        node = get_next(node);
    } 
    return node;

}

node_t* handle_func_def(node_t* node){
    if (node->current->type != T_IDENTIFIER) {
        ThrowError(2);
    }
    //symtable entry of new func with id
    node = get_next(node);
    if (node->current->type != T_LEFT_PAREN) {
        ThrowError(2);
     }
    node = get_next(node);
    node = handle_param_list(node);
     if (node->current->type != T_RIGHT_PAREN){
        ThrowError(2);
    } 
    //removed check for ) as its in handle_param_list
    node = get_next(node);
    node = hadnle_func_retType(node);
    if (node->current->type == T_LEFT_BRACE) {
        node = get_next(node);
        node = handle_statement_list(node);
    } else {
        ThrowError(2);
    }
    // tu je : why  
    if (node->current->type != T_RIGHT_BRACE) {
        ThrowError(2);
    }
    return node;
}

node_t* handle_in_param(node_t* node){
    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        if (node->current->type == T_COLON){
            node = get_next(node);
            if (node->current->type == T_IDENTIFIER){
                return  node;

            } else if (node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
               
                node = get_next(node);
                return  node;
            } else {
                ThrowError(2);
            }
            ThrowError(2);
        } 
        return  node;
    }
    ThrowError(2);
    return  node;
}

node_t* handle_in_param_list(node_t* node){
    if(node->current->type == T_RIGHT_PAREN){
        return  node;
    }
    node = handle_in_param(node);
    if (node->current->type == T_COMMA){
        
        node = get_next(node);
        if(node->current->type == T_RIGHT_PAREN){
            ThrowError(2);
        }
        node = handle_in_param_list(node);
    }
    if (node->current->type != T_RIGHT_PAREN){
        ThrowError(2);
    }
    return  node;
}

node_t* handle_assign_ops(node_t* node){ 
    if (node->current->type != T_ASSIGN) {
        return node;
    }
    node = get_next(node);
    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        if(node->current->type == T_LEFT_PAREN){
            node = get_next(node);
            node = handle_in_param_list(node);;
            if (node->current->type != T_RIGHT_PAREN){
                ThrowError(2);
            }
            return node;
        } else {
            return node;
        }
    } else {
        //call expression handle
        //if failed err.         
    }
    return node;
}

node_t* handle_var_def_ops(node_t* node){
    if (node->current->type == T_COLON){
        node = get_next(node);
        node = handle_type(node);
        node = get_next(node);

    }
    if (node->current->type == T_ASSIGN){
        node = get_next(node);
        node = handle_assign_ops(node);
    }

    return node;

}

node_t* handle_var_def(node_t* node){
    if (node->current->type != T_IDENTIFIER) {
        ThrowError(2);
    }
    node = get_next(node);
    node = handle_var_def_ops(node);
    return node;
}

node_t* handle_funcall_ops(node_t* node){
    if (node->current->type == T_ASSIGN){
        node = get_next(node);
         node = handle_assign_ops(node);
        return node;
    } else if (node->current->type == T_LEFT_PAREN){
        node = get_next(node);
        node = handle_in_param_list(node);
        if (node->current->type != T_RIGHT_PAREN){
            ThrowError(2);
        } 
        return  node;
    } else {
        return node;
    }
    return node;
}

node_t* handle_cond_ops(node_t* node){
    if (node->current->type == T_LET){
        node = get_next(node);
        if (node->current->type == T_IDENTIFIER){
            //write
            return node;
        }
    } else {
        //expression handle
        return node;
    }
    return node;
}

node_t* handle_if(node_t* node){
    node = handle_cond_ops(node);

    node = get_next(node);
    if (node->current->type == T_LEFT_BRACE){
        node = get_next(node);
        node = handle_statement_list(node);

        node = get_next(node);
        if (node->current->type != T_RIGHT_BRACE){
            ThrowError(2);
        } else {
            
            
            if (node->current->type != T_ELSE){       //optional else ?
          
                ThrowError(2);
            }
            node = get_next(node);
            if (node->current->type != T_LEFT_BRACE){
                ThrowError(2);
            } else {
                
                node = handle_statement_list(node);

                node = get_next(node);
                if (node->current->type != T_RIGHT_BRACE){ //maybe insufficient check for {} counter

                    ThrowError(2);
                }
            }
        }
    } else {
        ThrowError(2);
    }
    return node;
}

node_t* handle_while(node_t* node){
    node = handle_cond_ops(node);
    node = get_next(node);
    if (node->current->type != T_LEFT_BRACE){
        ThrowError(2);
    }
    node = get_next(node);
    node = handle_statement_list(node); //check for nested conditions and issues
    
    node = get_next(node);
    if (node->current->type != T_RIGHT_BRACE){
        ThrowError(2);
    }
    return node;
}

node_t* handle_statement_list(node_t* node){
    if (node->current->type == T_RIGHT_BRACE ){
        return node;
    }
    node = handle_statement(node);
    node = handle_statement_list(node);
    return node;
}

node_t* handle_statement(node_t* node){
    switch (node->current->type){
        case T_LET:
        case T_VAR:
            node = get_next(node);
            node = handle_var_def(node);
            break;

        case T_IDENTIFIER:
            node = get_next(node);
            node = handle_funcall_ops(node);
            break;
        case T_IF:
            node = get_next(node);
            node = handle_if(node);
            break;
        case T_WHILE:
            node = get_next(node);
            node = handle_while(node);
            break;
        case T_RETURN : 
            node = get_next(node);
            node = get_next(node);
            return node;
        case T_EOF:
            exit(0);
        default: 
            node = get_next(node);
            return node;
    }
    node = get_next(node);
    return node;
}


void free_node_list(node_t* node){
    while (node->left != NULL){
        node = node->left;
    }
    while (node->right != NULL){
        free_token_Values(node->current);
        node = node->right;
        free(node->left);
        node->left = NULL;
    }
}

node_t* get_next(node_t* current){
    node_t *node = create_node();
    node->current = scan();
    current->right = node;
    node->left = current;

    return node;
}


node_t* create_node(){
    node_t *node = malloc(sizeof(struct node));
    if (node == NULL) {
        ThrowError(99);
    }
    node->current = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void init_myInfo(){
    runInfo = malloc(sizeof(struct myInfo));
    if (runInfo == NULL){
        ThrowError(99);
    }
    runInfo->inFun = false;
    runInfo->currentLVL = NULL;
    create_level();
}

void create_level(){
    symTabLVL *current_level = malloc(sizeof(struct symTabLVL));
    if (current_level == NULL){
        ThrowError(99);
    }
    SymTable newSymTab;
    SymTableInit(&newSymTab);
    current_level->currentTab = &newSymTab;
    if (runInfo->currentLVL == NULL){
        current_level->parantLVL = NULL;
    } else {
        current_level->parantLVL = runInfo->currentLVL;
        //copy_to_child(current_level->parantLVL->currentTab, current_level->currentTab);
    }
    runInfo->currentLVL = current_level;
}

void pop_level(){
    
    //SymTableFree(runInfo->currentLVL->currentTab); //maybe bad mem free

    runInfo->currentLVL->currentTab = NULL;
    runInfo->currentLVL = runInfo->currentLVL->parantLVL;
}



void parser(){
   
    
    node_t *node = create_node();
    node->current = scan();


    bool run = true;
    while (run){
        switch (node->current->type) {
        case T_FUNC:
            node = get_next(node);
            node = handle_func_def(node);
            break;
        case T_EOF:
            exit(0);
        default:
            node = handle_statement_list(node);
            printf("448token type %d\n", node->current->type);
            break;
        }
        node = get_next(node);
        printf("452token type %d\n", node->current->type);
    }  

    free_node_list(node);
}

int main()
{
    parser();
    return 0;
}
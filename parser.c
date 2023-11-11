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
#include "scanner.c" //change to scanner.h
#include "scanner.h"


/* TODO
* - sort out return validity if in func
* - make error file
* - optional else?
* - free of node list
*/

void handle_statement_list(node_t *node){
    printf("current token: %d\n", (*node)->current->type);
    if ((*node)->current->type == T_RIGHT_BRACE){
        return;
    }
    handle_statement(node);
    handle_statement_list(node);
    return;
}

void handle_param(node_t *node){
    if ((*node)->current->type == T_UNDERSCORE){
        //handle in symtab _ case
    } else if ((*node)->current->type == T_IDENTIFIER){
        //symtab assign
        return;
    } else {
        //error
        return;
    }
    node_insert(node);
    if ((*node)->current->type != T_IDENTIFIER){
        //error
        return;
    }
    node_insert(node);
    if ((*node)->current->type != T_COLON){
        //error
        return;
    }
    handle_type(node);


}

void handle_param_list(node_t *node){
    if ((*node)->current->type == T_RIGHT_PAREN){
        return;
    } else {
        handle_param(node);
    }
    node_insert(node);
    if ((*node)->current->type == T_COMMA){
        node_insert(node);
        handle_param_list(node);
    } else {
        //error
        return;
    }
}

void handle_type(node_t *node){ //what is difference between t_int a t_kw_int
    if ((*node)->current->type == T_KW_INT){
        //assign int in symtable with ? check
        return;
    } else if ((*node)->current->type == T_KW_DOUBLE){
        //assign double in symtable with ? check
        return;
    } else if ((*node)->current->type == T_KW_STRING){
        //assign str in symtable with ? check
        return;
    } else {
        /* err */
        return;
    }
    return;
}

void handle_func_def(node_t *node){
    
    node_insert(node);

    if ((*node)->current->type != T_IDENTIFIER) {
        //error
        return;
    }
    //symtable entry of new func with id
    node_insert(node);
    if ((*node)->current->type != T_LEFT_PAREN) {
        //error
        return;
    }
    node_insert(node);
    handle_param_list(node);
    
    node_insert(node);
    if ((*node)->current->type != T_RIGHT_PAREN) {
        if ((*node)->current->type != T_ARROW) {
            //error
            return;
        }
        node_insert(node);
        handle_type(node);

        node_insert(node);
    } else if ((*node)->current->type == T_LEFT_BRACE) {
        node_insert(node);
        handle_statement_list(node);
    } else {
        //error
        return;
    }
    node_insert(node);
    if ((*node)->current->type != T_RIGHT_BRACE) {
        //error
        return;
    }
}

void handle_in_param(node_t *node){
    if ((*node)->current->type == T_IDENTIFIER){
        node_insert(node);
        if ( (*node)->current->type == T_COLON){
            node_insert(node);
            if ((*node)->current->type == T_IDENTIFIER){
                return;
            }
        }
    }
    //error
    return;
}

void handle_in_param_list(node_t *node){
    node_insert(node);
    if((*node)->current->type == T_RIGHT_PAREN){
        return;
    }
    handle_in_param(node);
    
    node_insert(node);
    if ((*node)->current->type == T_COMMA){
        handle_in_param_list(node);
    }
    if ((*node)->current->type != T_RIGHT_PAREN){
        //error
        return;
    }
}

void handle_assign_ops(node_t *node){
    if ((*node)->current->type == T_IDENTIFIER){
        node_insert(node);
        if((*node)->current->type == T_LEFT_PAREN){
            node_insert(node);
            handle_in_param_list(node);

            node_insert(node);
            if ((*node)->current->type != T_RIGHT_PAREN){
                //error
                return;
            }
        } else {
            //error
            return;
        }

    } else {
        //call expression handle
        //if failed err.         
    }

}

void handle_var_def(node_t *node){
    node_insert(node);
    if ((*node)->current->type != T_IDENTIFIER) {
        //error
        return;
    }
    //symtable entry of new var with id with look back if let or var. implement maybe creation
    node_insert(node);
    if ((*node)->current->type == T_COLON) {
        node_insert(node);
        //handle and assign type
        handle_type(node);
        node_insert(node);
    }
    handle_assign_ops(node);
    return;
}

void handle_funcall_ops(node_t *node){
    if ((*node)->current->type == T_ASSIGN){
        node_insert(node);
        handle_assign_ops(node);
        return;
    } else if ((*node)->current->type == T_LEFT_PAREN){
        node_insert(node);
        handle_param_list(node);

        node_insert(node);
        if ((*node)->current->type != T_RIGHT_PAREN){
            //error
            return;
        } 
        return;
    } else {
        //error
        return;
    }
}

void handle_cond_ops(node_t *node){
    if ((*node)->current->type == T_LET){
        node_insert(node);
        if ((*node)->current->type == T_IDENTIFIER){
            //write
            return;
        }
    } else {
        //expression handle
        return;
    }
}

void handle_if(node_t *node){
    node_insert(node);
    handle_cond_ops(node);

    node_insert(node);
    if ((*node)->current->type == T_LEFT_BRACE){
        node_insert(node);
        handle_statement_list(node);

        node_insert(node);
        if ((*node)->current->type != T_RIGHT_BRACE){
            //error
            return;
        } else {
            //optional else ?
            node_insert(node);
            if ((*node)->current->type != T_ELSE){
                
                //error
                return;
            }
            node_insert(node);
            if ((*node)->current->type != T_LEFT_BRACE){
                //error
                return;
            } else {
                node_insert(node);
                handle_statement_list(node);

                node_insert(node);
                if ((*node)->current->type != T_RIGHT_BRACE){ //maybe insufficient check for {} counter
                    //error
                    return;
                }
            }
        }
    } else {
        //error
        return;
    }
}

void handle_while(node_t *node){
    node_insert(node);
    handle_cond_ops(node);

    if ((*node)->current->type != T_LEFT_BRACE){
        //error
        return;
    }
    node_insert(node);
    handle_statement_list(node); //check for nested conditions and issues
    
    node_insert(node);
    if ((*node)->current->type != T_RIGHT_BRACE){
        //error
        return;
    }
}

void handle_statement(node_t *node){
    switch ((*node)->current->type){
        case T_LET:
        case T_VAR:
            handle_var_def(node);
            break;

        case T_IDENTIFIER:
            handle_funcall_ops(node);
            break;
        case T_IF:
            handle_if(node);
            break;
        case T_WHILE:
            handle_while(node);
            break;
        case T_RETURN : 
            //check if in func 
            return;
        default: 
            //error 
            return;
    }
}

void node_init(node_t *node){
    (*node) = NULL;
}

void node_insert(node_t *node){
    Token* nextToken = scan();
    node_t* new_node = malloc(sizeof(struct node_struct));
    (*new_node)->current = nextToken;
    (*new_node)->right = NULL;
    if (node == NULL){
        node = new_node;
    } else {
        node_t* tmp = node;
        while ( (*tmp)->right  != NULL){
            tmp = &(*tmp)->right;
        }
        (*new_node)->left = *tmp;
        (*tmp)->right = *new_node;
    }
}

void parser(){
    node_t *root = malloc(sizeof(node_t));
    node_init(root);
    node_insert(root);

    bool run = true;
    while (run){
        switch ((*root)->current->type)
        {
        case T_FUNC:
            node_insert(root);
            handle_func_def(root);
            break;
        
        default:
            node_insert(root);
            handle_statement(root);
            break;
        }
    }  
}

int main()
{
    parser();
    return 0;
}
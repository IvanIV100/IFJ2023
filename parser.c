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


/* TODO 
* - symtabLVL
    -clean up
    -symtab insert

* - Documentation
    -add headers to C files
    -add comments to functions

* - Random
    -fix mem free of linked list
    -optional else?
    -handle built in funcs
    -check if err output is correct
    -expression parser


*/

runTimeInfo *runInfo;


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
            printf("Error: bad end\n");
            ThrowError(2);
        }
        node = handle_param_list(node);
        return node;
    } else {
        return node;
    }
}

node_t* handle_type(node_t* node){ //what is difference between t_int a t_kw_int
    int type;
    if (node->current->type == T_KW_INT){
        type = 32;
    } else if (node->current->type == T_KW_DOUBLE){
        type = 33;
    } else if (node->current->type == T_KW_STRING){
        type = 31;
    } else {
        printf("Error: Expected type\n");
        ThrowError(2);
    }
    
    assign_varType_ST(node, type, node->current->value.nillable);
    return  node;
}

node_t* hadnle_func_retType(node_t* node){
    printf("funcRetType \n");
    printf("100 current token type: %d\n", node->current->type);
    if (node->current->type == T_ARROW){
        node = get_next(node);
        printf("103 current token type: %d\n", node->current->type);
        node = handle_type(node);
        node = get_next(node);
    } 
    return node;

}

node_t* handle_func_def(node_t* node){
    printf("funcDef \n");
    if (node->current->type != T_IDENTIFIER) {
        printf("112 Error: Expected identifier\n");
        ThrowError(2);
    }
    //symtable entry of new func with id
    node = get_next(node);
    if (node->current->type != T_LEFT_PAREN) {
        printf("119 Error: Expected left paren\n");
        ThrowError(2);
     }
    node = get_next(node);
    node = handle_param_list(node);
    printf("125 current token type: %d\n", node->current->type);
     if (node->current->type != T_RIGHT_PAREN){
        ThrowError(2);
    } 
    //removed check for ) as its in handle_param_list
    printf("137 current token type: %d\n", node->current->type); 
    node = get_next(node);
    node = hadnle_func_retType(node);
    printf("139 current token type: %d\n", node->current->type);
    if (node->current->type == T_LEFT_BRACE) {
        node = get_next(node);
        printf("142 current token type: %d\n", node->current->type);
        node = handle_statement_list(node);
    } else {
        printf("token type %d\n", node->current->type);
        printf("Error: Expected left brace\n");
        ThrowError(2);
    }
    // tu je : why  
    printf("138 current token type: %d\n", node->current->type);
    if (node->current->type != T_RIGHT_BRACE) {
        printf("Error: Expected right brace\n");
        ThrowError(2);
    }
    return node;
}

node_t* handle_in_param(node_t* node){
    printf("inParam \n");
    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        printf("150 current token type: %d\n", node->current->type);
        if (node->current->type == T_COLON){
            node = get_next(node);
            printf("153 current token type: %d\n", node->current->type);
            if (node->current->type == T_IDENTIFIER){
                printf("156 current token type: %d\n", node->current->type);
                return  node;

            } else if (node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
                printf("160 current token type: %d\n", node->current->type);
                node = get_next(node);
                printf("162 current token type: %d\n", node->current->type);
                return  node;
            } else {
                printf("Error: Param failed\n");
                ThrowError(2);
            }
            printf("Error: Param failed\n");
            ThrowError(2);
        } 
        return  node;
    }
    printf("Error: Param failed\n");
    ThrowError(2);
    return  node;
}

node_t* handle_in_param_list(node_t* node){
    printf("inParamList \n");
    printf("174 current token type: %d\n", node->current->type);
    if(node->current->type == T_RIGHT_PAREN){
        return  node;
    }
    printf("179 current token type: %d\n", node->current->type);
    node = handle_in_param(node);
     printf("180 current token type: %d\n", node->current->type);
    if (node->current->type == T_COMMA){
        
        node = get_next(node);
        printf("181 current token type: %d\n", node->current->type);
        if(node->current->type == T_RIGHT_PAREN){
            printf("Error: Not right\n");
            ThrowError(2);
        }
        node = handle_in_param_list(node);
        printf("188 current token type: %d\n", node->current->type);
        printf("190 current token type: %d\n", node->current->type);  
    }
     printf("193 current token type: %d\n", node->current->type);
    if (node->current->type != T_RIGHT_PAREN){
        printf("Error: Expected right paren\n");
        ThrowError(2);
    }
    printf("196 current token type: %d\n", node->current->type);
    return  node;
}

node_t* handle_assign_ops(node_t* node){ 
    printf("assignOps \n");
    printf("196 curr token %d \n", node->current->type);
    if (node->current->type != T_ASSIGN) {
        return node;
    }
    node = get_next(node);
    if (node->current->type == T_IDENTIFIER){
        printf("197 current token type: %d\n", node->current->type);
        node = get_next(node);
        printf("200 current token type: %d\n", node->current->type);
        if(node->current->type == T_LEFT_PAREN){
            node = get_next(node);
            printf("203 current token type: %d\n", node->current->type);
            node = handle_in_param_list(node);
            printf("205 current token type: %d\n", node->current->type);
            if (node->current->type != T_RIGHT_PAREN){
                printf("Error: Expected right paren\n");
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
    printf("varDefOps \n");
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
    printf("varDef \n");
    node = get_next(node);
    if (node->current->type != T_IDENTIFIER) {
        printf("Error: Expected identifier\n");
        ThrowError(2);
    }
    define_var_ST(node);
    node = get_next(node);
    node = handle_var_def_ops(node);
    return node;
}

node_t* handle_funcall_ops(node_t* node){
    printf("funcallOps \n");
    printf("254 token type %d\n", node->current->type);
    if (node->current->type == T_ASSIGN){
        node = get_next(node);
        printf("257 token type %d\n", node->current->type);
         node = handle_assign_ops(node);
        return node;
    } else if (node->current->type == T_LEFT_PAREN){
        node = get_next(node);
        printf("261 token type %d\n", node->current->type);
        node = handle_in_param_list(node);
        printf("265 token type %d\n", node->current->type); 
        if (node->current->type != T_RIGHT_PAREN){
            printf("Error: Expected right paren\n");
            ThrowError(2);
        } 
        return  node;
    } else {
        return node;
        // printf("261 token type %d\n", token->type);
        // printf("261 Error: Expected assign or left paren\n");
        // ThrowError(2);
    }
    return node;
}

node_t* handle_cond_ops(node_t* node){
    printf("condOps \n");
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
    printf("if \n");
    node = handle_cond_ops(node);

    node = get_next(node);
    if (node->current->type == T_LEFT_BRACE){
        node = get_next(node);
        node = handle_statement_list(node);

        node = get_next(node);
        if (node->current->type != T_RIGHT_BRACE){
            printf("Error: Expected right brace\n");
            ThrowError(2);
        } else {
            
            
            if (node->current->type != T_ELSE){       //optional else ?
                printf("Error: Expected else\n");
                ThrowError(2);
            }
            node = get_next(node);
            if (node->current->type != T_LEFT_BRACE){
                printf("Error: Expected left brace\n");
                ThrowError(2);
            } else {
                
                node = handle_statement_list(node);

                node = get_next(node);
                if (node->current->type != T_RIGHT_BRACE){ //maybe insufficient check for {} counter
                    printf("Error: Expected right brace\n");
                    ThrowError(2);
                }
            }
        }
    } else {
        printf("Error: Expected left brace\n");
        ThrowError(2);
    }
    return node;
}

node_t* handle_while(node_t* node){
    printf("while \n");
    node = handle_cond_ops(node);
    node = get_next(node);
    if (node->current->type != T_LEFT_BRACE){
        printf("Error: Expected left brace\n");
        ThrowError(2);
    }
    node = get_next(node);
    node = handle_statement_list(node); //check for nested conditions and issues
    
    node = get_next(node);
    if (node->current->type != T_RIGHT_BRACE){
        printf("Error: Expected right brace\n");
        ThrowError(2);
    }
    return node;
}

node_t* handle_statement_list(node_t* node){
    printf("stmntList \n");
    printf("355 stmtntList current token type: %d\n", node->current->type);
    if (node->current->type == T_RIGHT_BRACE ){
        printf("RB return \n");
        return node;
    }
    node = handle_statement(node);
    node = handle_statement_list(node);
    return node;
}

node_t* handle_statement(node_t* node){
    printf("stmnt \n");
    switch (node->current->type){
        case T_LET:
        case T_VAR:
            printf("356 token type %d\n", node->current->type);
            node = handle_var_def(node);
            break;

        case T_IDENTIFIER:
            printf("362 token type %d\n", node->current->type);
            node = get_next(node);
            node = handle_funcall_ops(node);
            printf("366 token type %d\n", node->current->type);
            printf("367 token type %d\n", node->current->type);
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
            printf("391 token type %d\n", node->current->type);
            return node;
    }
    printf("392 token type %d\n", node->current->type);
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
    printf("token GEN %d\n", node->current->type);
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
    runInfo = malloc(sizeof(struct runTimeInfo));
    if (runInfo == NULL){
        ThrowError(99);
    }
    
    SymTable globalFrame, builtInFunctions;
    SymTableInit(&globalFrame);
    SymTableInit(&builtInFunctions);

    fill_builtin_symtab(&builtInFunctions);

    runInfo->currentLVL = NULL;
    runInfo->globalFrame = &globalFrame;
    runInfo->builtInFunctions = &builtInFunctions;
}

void fill_builtin_symtab(SymTable *builtIn){
    return;
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
    }
    runInfo->currentLVL = current_level;
}

void pop_level(){
    //SymTableFree(runInfo->currentLVL->currentTab); //maybe bad mem free
    runInfo->currentLVL->currentTab = NULL;
    runInfo->currentLVL = runInfo->currentLVL->parantLVL;
}

void define_var_ST(node_t* node){
    SymTable* current = NULL;
    if (runInfo->currentLVL == NULL){
        current = runInfo->globalFrame;
    } else {
        current = runInfo->currentLVL->currentTab;
    }

    Symbol *result = GetSymbol(current, node->current->value.ID_name);
    if (result->variable.init){
        ThrowError(3);
    } else {
        InsertSymbol(current, node->current->value.ID_name);
        int vol;
        if (node->left->current->type == 26){
            vol = 1;
        } else {
            vol = 0;
        }
        AddVarDetails(current, node->current->value.ID_name, VOID, false, vol);

    }
    current = NULL;

}

void assign_varType_ST(node_t* node, int type, int nillable){
    SymTable* current = NULL;
    if (runInfo->currentLVL == NULL){
        current = runInfo->globalFrame;
    } else {
        current = runInfo->currentLVL->currentTab;
    }
    Symbol *updateSymbol = GetSymbol(current, node->left->left->current->value.ID_name);
    if (updateSymbol == NULL){
        //error
    }
    updateSymbol->variable.datatype = type;
    //updateSymbol. = nillable;
    
}

void define_func_ST(){

}

void check_type(){
    
}

void parser(){
    init_myInfo();
    
    
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
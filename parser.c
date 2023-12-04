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
* - Mem management
    -free all allocated memory in a dedicated destructor
    -delete in throwerror

* - symtabLVL
    -clean up
    -symtab insert

* - Documentation
    -add comments to functions
    -add comment headers everywhere

* - Implement
    -built in functions check input and ret type
    -return fix
    -add semcheck in expressions

* - Check
    -if im supposed to work with interpret return codes
    -if and while expressions conditions - check if they are correct

* - Fix
    -fix type check in expressions
    -fix ?? in expressions



*/

runTimeInfo *runInfo;

void fill_builtin_symtab(SymTable *builtIn){ // fill in to check symtab and to global
    InsertSymbol(builtIn, "write");
    AddFunctionDetails(builtIn, "write",  0, true);

    InsertSymbol(builtIn, "substring");
    AddFunctionDetails(builtIn, "substring",  6, true);
    InsertSymbol(builtIn, "length");
    AddFunctionDetails(builtIn, "length",  3, true);

    InsertSymbol(builtIn, "ord");
    AddFunctionDetails(builtIn, "ord",  3, true);

    InsertSymbol(builtIn, "chr");
    AddFunctionDetails(builtIn, "chr",  5, true);

    InsertSymbol(builtIn, "readDouble");
    AddFunctionDetails(builtIn, "readDouble",  7, true);

    InsertSymbol(builtIn, "readInt");
    AddFunctionDetails(builtIn, "readInt",  4, true);

    AddFunctionDetails(builtIn, "readString",  6, true);
    InsertSymbol(builtIn, "readString");

    InsertSymbol(builtIn, "Double2Int");
    AddFunctionDetails(builtIn, "Double2Int",  3, true);

    InsertSymbol(builtIn, "Int2Double");
    AddFunctionDetails(builtIn, "Int2Double",  8, true);

}

void start_generator(node_t* node){
    while (node->left != NULL){
        node = node->left;
    }
    // start_code_generation(node);
    // free_node_list(node);
    exit(0);

}

void init_myInfo(){
    runInfo = malloc(sizeof(struct runTimeInfo));
    if (runInfo == NULL){
        ThrowError(99);
    }
    
    SymTable *globalFrame = NULL;
    globalFrame = malloc(sizeof(SymTable));
    if (globalFrame == NULL){
        ThrowError(99);
    }
    SymTableInit(&globalFrame);

    SymTable *builtInFunctions;
    builtInFunctions = malloc(sizeof(SymTable));
    if (builtInFunctions == NULL){
        ThrowError(99);
    }
    SymTableInit(&builtInFunctions);
    
    fill_builtin_symtab(builtInFunctions);

    runInfo->currentLVL = NULL;
    runInfo->globalFrame = globalFrame;
    runInfo->builtInFunctions = builtInFunctions;

}

node_t* handle_function_param(node_t* node){
    char* name;
    if (node->current->type == T_UNDERSCORE){
        name = "_";
    } else if (node->current->type == T_IDENTIFIER){
        name = node->current->value.ID_name;
    } else {
        ThrowError(2);
    }
    
    node = get_next(node);
    
    if (node->current->type != T_IDENTIFIER){
        ThrowError(2);
    }
    char* ID = node->current->value.ID_name;
    node = get_next(node);
    if (node->current->type != T_COLON){
        ThrowError(2);
    }
    node = get_next(node);
    int type = handle_type(node);
    // AddParametr(runInfo->globalFrame, runInfo->FID, name, type);
    // AddParametrID(runInfo->globalFrame, runInfo->FID, ID);

    return  node;
}


node_t* handle_param_list(node_t* node){
    if (node->current->type == T_RIGHT_PAREN){
        return node;
    } else {
        node = handle_function_param(node);
    }
    node = get_next(node);
    if (node->current->type == T_COMMA){    
        node = get_next(node);
        if (node->current->type == T_RIGHT_PAREN){ 
            ThrowError(2);
        }
        node = handle_param_list(node);
        return node;
    } else if (node->current->type == T_RIGHT_PAREN){
        return node;
    } else {
        ThrowError(2);
    }
}

int handle_type(node_t* node){ //what is difference between t_int a t_kw_int
    int type;
    if (node->current->type == T_KW_INT){
        if (node->current->value.nillable == 1 ){
            type = INTQ;
        } else {
            type = INT;
        }
    } else if (node->current->type == T_KW_DOUBLE){
        if (node->current->value.nillable == 1 ){
            type = FLOATQ;
        } else {
            type =FLOAT;
        }
    } else if (node->current->type == T_KW_STRING){
        if (node->current->value.nillable == 1 ){
            type = STRQ;
        } else {
            type = STR;
        }
    } else {
        ThrowError(2);
    }
    
    return  type;
}

node_t* hadnle_func_retType(node_t* node){
    if (node->current->type == T_ARROW){
        node = get_next(node);
        int type = handle_type(node);
        AddFunctionDetails(runInfo->globalFrame, runInfo->FID, type , true);
        node = get_next(node);
    } else {
        AddFunctionDetails(runInfo->globalFrame, runInfo->FID, 0, true);
    }
    return node;

}

node_t* handle_func_def(node_t* node){
    if (node->current->type != T_IDENTIFIER) {
        ThrowError(2);
    }

    runInfo->FID=node->current->value.ID_name;
    if (GetSymbol(runInfo->builtInFunctions, runInfo->FID) != NULL ){
        //clean all
        ThrowError(3);
    } else if (GetSymbol(runInfo->globalFrame, runInfo->FID) != NULL ){
        ThrowError(3);
    } else {
        InsertSymbol(runInfo->globalFrame, runInfo->FID);
        
    }   
    
    node = get_next(node);
    if (node->current->type != T_LEFT_PAREN) {
        ThrowError(2);
    }
    node = get_next(node);
    node = handle_param_list(node);

    node = get_next(node);
    node = hadnle_func_retType(node);
    if (node->current->type == T_LEFT_BRACE) {  
        create_level();
        node = get_next(node);
        node = handle_statement_list(node);   // *somhow check return type if missing*
    } else {
        ThrowError(2);
    }
    if (node->current->type != T_RIGHT_BRACE) {     
        ThrowError(2);
    }
    pop_level();
    node = get_next(node);
    return node;
}

node_t* handle_in_param(node_t* node){ // *ADD SEMANTIC CHECK*
    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        if (node->current->type == T_COLON){
            node = get_next(node);
            if (node->current->type == T_IDENTIFIER){
                node = get_next(node);
                return  node;
            } else if (node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
                node = get_next(node);
                return  node;
            } else {
                ThrowError(2);
            }
        } 
        return  node;
    } else { // literal assign add semantics
        if(node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
            node = get_next(node);
            return  node;
        } else {
            ThrowError(2);
        }
        
    }
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

node_t* expression_token_count(node_t* node, int* count){
    (*count) = 0;
    int parenCount = 0;
    while ((6 <= node->current->type && node->current->type <= 16) || 
            (34 <= node->current->type && node->current->type <= 38) ||
            node->current->type == T_LEFT_PAREN || node->current->type == T_RIGHT_PAREN || 
            node->current->type == T_DOUBLE_QUESTION_MARK || node->current->type == T_NIL){
        
        if (node->current->type == T_IDENTIFIER){
            if ((node->left->current->type > 16 || node->left->current->type < 6) && 
            node->left->current->type != T_LEFT_PAREN && 
            node->left->current->type != T_DOUBLE_QUESTION_MARK){
                //(*count)--;
                if (node->left->current->type == T_IF || node->left->current->type == T_WHILE || 
                node->left->current->type == T_RETURN || node->left->current->type == T_ASSIGN){
                } else {
                    break;
                }
                
            }
        }
        if(node->current->type == T_LEFT_PAREN){
            parenCount++;
        } else if (node->current->type == T_RIGHT_PAREN){
            parenCount--;
        }
        (*count)++;
        node = get_next(node);
    }
    if(parenCount != 0){
        ThrowError(2);
    }
    printf("curent: %d\n", node->current->type);
    if (node->left->current->type >= 6 && node->left->current->type <= 16){
        ThrowError(2);
    }
    return node;
}

node_t* handle_assign_ops(node_t* node){ 

    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        if(node->current->type == T_LEFT_PAREN){
            node = get_next(node);
            node = handle_in_param_list(node);
            node = get_next(node);
            return node;
        } else {
            int count1;
            ExprType result1;
            node_t* start = node;
            node = expression_token_count(node, &count1);
            
            result1 = expression_parser(start, runInfo, count1);

        //call expression handle
        //if failed err.         
        }
    } else {
            int count2;
            ExprType result2;
            node_t* start = node;
            node = expression_token_count(node, &count2);
            printf("Current 2 node: %d\n", node->current->type);
            result2 = expression_parser(start, runInfo, count2);
            
             // add type assign 
        }

    return node;
}

node_t* handle_var_def_ops(node_t* node){
    if (node->current->type == T_COLON){
        node = get_next(node);
    
        int type = handle_type(node);
        assign_varType_ST(node, type, node->current->value.nillable);
        node = get_next(node);

        if (node->current->type == T_ASSIGN){
            node = get_next(node);
            node = handle_assign_ops(node);
            return node;
        } else {
            return node;
        }

    } else {
        if (node->current->type == T_ASSIGN){
            node = get_next(node);
            node = handle_assign_ops(node);
            
            printf("cu %d\n", node->current->type);
            return node;
        } else {
            ThrowError(2);//check error type if syntax or semantic
        }
    }
}

node_t* handle_var_def(node_t* node){
    
    if (node->current->type != T_IDENTIFIER) {
        ThrowError(2);
    }
    define_var_ST(node);
    node = get_next(node);
    node = handle_var_def_ops(node);
    return node;
}

void check_funcall_id(char* ID){
    Symbol *result = GetSymbol(runInfo->builtInFunctions, ID);
    if (result == NULL){
        result = GetSymbol(runInfo->globalFrame, ID);
        if (result == NULL){
            ThrowError(3);
        }
    }
}

node_t* handle_funcall_ops(node_t* node){

    if (node->current->type == T_ASSIGN){
        SymTable *current = NULL;
        if (runInfo->currentLVL == NULL){
            current = runInfo->globalFrame;
        } else {
            current = runInfo->currentLVL->currentTab;
        }
        Symbol *result = GetSymbol(current, node->left->current->value.ID_name);
        if (result == NULL){
            ThrowError(5);
        }
        node = get_next(node);
        node = handle_assign_ops(node);
        return node;
    } else if (node->current->type == T_LEFT_PAREN){
        check_funcall_id(node->left->current->value.ID_name); // *checking if valid funcall*
        node = get_next(node);
        node = handle_in_param_list(node);
        if (node->current->type != T_RIGHT_PAREN){
            ThrowError(2);
        } 
        node = get_next(node);
        return  node;
    } else {
        return node;
    }
    return node;
}

node_t* handle_cond_ops(node_t* node){
    //node = get_next(node);
    if (node->current->type == T_LET){  
        node = get_next(node);
        if (node->current->type == T_IDENTIFIER){  // *add variable to next symtabLVL* maybe create lvl here
            node = get_next(node);
            return node;
        } else {
            ThrowError(2);
        }
    } else {
        int count3;
        node_t* start = node; 
        node = expression_token_count(node, &count3);  // *fix expressions* add check type for bool
        printf("count: %d\n", count3);
        expression_parser(start, runInfo, count3); // *add type assign* 
        printf("cond ops\n");
        return node; // check if the node is {
    }

    

}

node_t* handle_if(node_t* node){ //check if ( is passed
    node = handle_cond_ops(node);
    //node = get_next(node);
    if (node->current->type == T_LEFT_BRACE){
        create_level();
        node = get_next(node);
        node = handle_statement_list(node);
        if (node->current->type != T_RIGHT_BRACE){ 
            ThrowError(2);
        }
        pop_level();

        node = get_next(node);
        if (node->current->type != T_ELSE){  // *add the let var to else as well?*
            ThrowError(2);
        }
        node = get_next(node);
        if (node->current->type != T_LEFT_BRACE){
            ThrowError(2);
        } else {
            create_level();
            node = get_next(node);
            node = handle_statement_list(node);

            if (node->current->type != T_RIGHT_BRACE){ 
                ThrowError(2);
            }
            pop_level();
            node = get_next(node);
        }
        return node;
    } else {
        ThrowError(2);
    }
    
}

node_t* handle_while(node_t* node){
    node = handle_cond_ops(node);
    if (node->current->type == T_LEFT_BRACE){
        create_level();
        node = get_next(node);
        node = handle_statement_list(node); 
        if (node->current->type != T_RIGHT_BRACE){

            ThrowError(2);
        }
        pop_level();
        node = get_next(node);
        return node;
    } else {
        ThrowError(2);
    }
    
}

node_t* handle_statement_list(node_t* node){ //chceck func ret here

    if(node->current->type == T_LET || node->current->type == T_VAR || 
    node->current->type == T_IDENTIFIER || node->current->type == T_IF || 
    node->current->type == T_WHILE || node->current->type == T_RETURN ){
        node = handle_statement(node);
        node = handle_statement_list(node);
    } else {
        return node;
    }
    
}

node_t* handle_return(node_t* node){
    if (runInfo->FID == NULL){
        ThrowError(2);
    }

    node = get_next(node);
    int count;
    node_t* start = node;
    node = expression_token_count(node, &count);
    ExprType retVal = expression_parser(start, runInfo, count); // add type assign
    // Symbol *result = GetSymbol(runInfo->globalFrame, runInfo->FID);
    // if (result->function.ReturnType != retVal){
    //     ThrowError(6);
    // }
    return node;
}

node_t* handle_statement(node_t* node){
    switch (node->current->type){
        case T_LET:
        case T_VAR:
            node = get_next(node);
            node = handle_var_def(node);
            return node;
            break;

        case T_IDENTIFIER:

            node = get_next(node);
            node = handle_funcall_ops(node);
            return node;
            break;
        case T_IF:
            node = get_next(node);
            node = handle_if(node);
            
            return node;
            break;
        case T_WHILE:
            node = get_next(node);
            node = handle_while(node);
            return node;
            break;
        case T_RETURN : 
            node = handle_return(node);
            return node;
        default: 
            break;
    }
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
    if (node->current->type == T_ERORR){
        int err = node->current->value.integer;
        free_node_list(node);
        ThrowError(err);
    }
    current->right = node;
    node->left = current;
    printf(" current token: %d \n", node->current->type);
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

void create_level(){
    symTabLVL *current_level = malloc(sizeof(struct symTabLVL));
    if (current_level == NULL){
        ThrowError(99);
    }
    SymTable *newSymTab = malloc(sizeof(SymTable));
    SymTableInit(&newSymTab);
    current_level->currentTab = newSymTab;
    if (runInfo->currentLVL == NULL){
        current_level->parantLVL = NULL;
    } else {
        current_level->parantLVL = runInfo->currentLVL;
    }
    runInfo->currentLVL = current_level;
}

void pop_level(){
    SymTableFree(runInfo->currentLVL->currentTab); //maybe bad mem free
    runInfo->currentLVL->currentTab = NULL;
    symTabLVL *toDelete = runInfo->currentLVL;
    runInfo->currentLVL = runInfo->currentLVL->parantLVL;
    free(toDelete);
    toDelete = NULL;

}

void define_var_ST(node_t* node){
    SymTable* current = NULL;
    if (runInfo->currentLVL == NULL){
        current = runInfo->globalFrame;
    } else {
        current = runInfo->currentLVL->currentTab;
    }

    Symbol *result = GetSymbol(current, node->current->value.ID_name);
    
    if (result != NULL){
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

void assign_varType_ST(node_t* node, int type, int nillable){ // check if nillable is correctly filled and incldues !
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
    updateSymbol->variable.nillable = nillable;
    
}


void parser(){
    init_myInfo();
    node_t *node = create_node();
    node->current = scan();

    while (true){
        switch (node->current->type) {
            case T_FUNC:
                node = get_next(node);
                node = handle_func_def(node);
                break;
            case T_EOF:
                start_generator(node); 
                break;// add clean up
            case T_LET:
            case T_VAR:
            case T_IF:
            case T_WHILE:
            case T_RETURN:
            case T_IDENTIFIER:
                node = handle_statement_list(node);
                break;

            default:
                ThrowError(2);
        }
    }  
    
}


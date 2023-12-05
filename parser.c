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
    -let check on assign
    -built in functions check input and ret type
    -add semcheck in expressions

* - Check
    -if return can be in void function
    -if im supposed to work with interpret return codes

* - Fix
    -expression return types boool etc

    -syn types
    -fix type check in expressions
    -fix ?? in expressions

*/

runTimeInfo *runInfo;



Symbol *search_upwards_ST(char* ID){
    symTabLVL *current = runInfo->currentLVL;
    while (current != NULL){
        Symbol *result = GetSymbol(current->currentTab, ID);
        if (result != NULL){
            return result;
        }
        current = current->parantLVL;
    }
    return NULL;

}



void fill_builtin_symtab(SymTable *builtIn){ // fill in to check symtab and to global
    InsertSymbol(builtIn, "readString");
    InsertSymbol(runInfo->globalFrame, "readString");
    AddFunctionDetails(builtIn, "readString",  6, true);

    InsertSymbol(builtIn, "readInt");
    InsertSymbol(runInfo->globalFrame, "readInt");
    AddFunctionDetails(builtIn, "readInt",  4, true);

    InsertSymbol(builtIn, "readDouble");
    InsertSymbol(runInfo->globalFrame, "readDouble");
    AddFunctionDetails(builtIn, "readDouble",  8, true);

    InsertSymbol(builtIn, "write");
    InsertSymbol(runInfo->globalFrame, "write");
    AddFunctionDetails(builtIn, "write",  0, true);
    AddParametr(builtIn, "write", "_", "s", STR);


    InsertSymbol(builtIn, "Int2Double");
    InsertSymbol(runInfo->globalFrame, "Int2Double");
    AddFunctionDetails(builtIn, "Int2Double",  7, true);
    AddParametr(builtIn, "Int2Double", "_", "i", INT);
        
    InsertSymbol(builtIn, "Double2Int");
    InsertSymbol(runInfo->globalFrame, "Double2Int");
    AddFunctionDetails(builtIn, "Double2Int",  3, true);
    AddParametr(builtIn, "Double2Int", "_", "d", FLOAT);
    
    InsertSymbol(builtIn, "length");
    InsertSymbol(runInfo->globalFrame, "length");
    AddFunctionDetails(builtIn, "length",  3, true);
    AddParametr(builtIn, "length", "_", "s", STR);

    InsertSymbol(builtIn, "substring");
    InsertSymbol(runInfo->globalFrame, "substring");
    AddFunctionDetails(builtIn, "substring",  6, true);
    AddParametr(builtIn, "substring", "of", "s", STR);
    AddParametr(builtIn, "substring", "startingAt", "i", INT);
    AddParametr(builtIn, "substring", "endingBefore", "j", INT);
    //*return nill when cases

    InsertSymbol(builtIn, "ord");
    InsertSymbol(runInfo->globalFrame, "ord");
    AddFunctionDetails(builtIn, "ord",  3, true);
    AddParametr(builtIn, "ord", "_", "c", STR);

    InsertSymbol(builtIn, "chr");
    InsertSymbol(runInfo->globalFrame, "chr");
    AddFunctionDetails(builtIn, "chr",  5, true);
    AddParametr(builtIn, "chr", "_", "i", INT);



}

void start_generator(node_t* node){
    while (node->left != NULL){
        node = node->left;
        printf("current: %d\n", node->current->type);
    }
    printf("fn\n");
    printf("first node: %d\n", node->current->type);
    start_code_generation(node);
    // free_node_list(node);
    exit(0);

}

void init_runInfo(){
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

    runInfo->FID = NULL;
    runInfo->CurID = NULL;
    runInfo->vol = NULL;
    runInfo->firstNode = NULL;
    char** funcalls = NULL;
    int count = 0;



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

    if(strcmp(ID, name) == 0){ // *checks if name and ID are the same(can't be)*
        ThrowError(4);
    }
    node = get_next(node);
    if (node->current->type != T_COLON){
        ThrowError(2);
    }
    node = get_next(node);
    int type = handle_type(node);
    AddParametr(runInfo->globalFrame, runInfo->FID, name , ID, type);

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

int 
handle_type(node_t* node){ //what is difference between t_int a t_kw_int
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
        printf("type: %d\n", node->current->type);
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
    int result = check_FID_list(runInfo->FID);
    if (result == 1){
        delete_FID_list(runInfo->FID);
    }
    if (GetSymbol(runInfo->builtInFunctions, runInfo->FID) != NULL ){
        ThrowError(3);
    } else if (GetSymbol(runInfo->globalFrame, runInfo->FID) != NULL ){
        ThrowError(3);
    } else {
        InsertSymbol(runInfo->globalFrame, runInfo->FID);
    }   

    AddFunctionDetails(runInfo->globalFrame, runInfo->FID, 0, true); // *add type assign*
    node = get_next(node);
    if (node->current->type != T_LEFT_PAREN) {
        ThrowError(2);
    }

    node = get_next(node);
    node = handle_param_list(node);

    node = get_next(node);
    node = hadnle_func_retType(node);
    if (node->current->type == T_LEFT_BRACE) {  // *add params to symtab*

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
    if(runInfo->FID != NULL){
        Symbol *result = GetSymbol(runInfo->globalFrame, runInfo->FID);
        if (result->function.ReturnType != VOID){
            ThrowError(6);
        } else {
            runInfo->FID = NULL;
        }
    }
    node = get_next(node);
    return node;
}

void handle_in_param_type_ST(char* ID){
    return ;
}

node_t* handle_in_param(node_t* node){ // *ADD SEMANTIC CHECK*
    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        if (node->current->type == T_COLON){
            node = get_next(node);
            if (node->current->type == T_IDENTIFIER){
                char* ID = node->left->left->current->value.ID_name;

                node = get_next(node);
                return  node;
            } else if (node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
                node = get_next(node);
                return  node;
            } else {
                ThrowError(2);
            }
        } 
        printf("curId: %s\n", runInfo->CurID);
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
        Symbol *result = GetSymbol(runInfo->globalFrame, runInfo->CurID);
        if(result->function.parametr_count != 0){
            ThrowError(2);
        } else {
            return node;
        }
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
    printf("entry %d\n", node->current->type);
    while ((6 <= node->current->type && node->current->type <= 16) || 
            (34 <= node->current->type && node->current->type <= 38) ||
            node->current->type == T_LEFT_PAREN || node->current->type == T_RIGHT_PAREN || 
            node->current->type == T_DOUBLE_QUESTION_MARK || node->current->type == T_NIL){
        
        if (node->current->type == T_IDENTIFIER){
            if ((node->left->current->type > 16 || node->left->current->type < 6) && 
            node->left->current->type != T_LEFT_PAREN && 
            node->left->current->type != T_DOUBLE_QUESTION_MARK){
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
        printf("count: %d\n", *count);
        printf("current: %d\n", node->current->type);
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
            node_t* start = node->left;
            node = expression_token_count(node, &count1);
            
            result1 = expression_parser(start, runInfo, count1);

            if (result1%2 == 0){
                assign_varType_ST(node, result1, 1); 
            } else {
                assign_varType_ST(node, result1, 0); 
            }     
        }
    } else {
            int count2;
            ExprType result2;
            node_t* start = node;
            node = expression_token_count(node, &count2);
            result2 = expression_parser(start, runInfo, count2);
            
            if (result2 % 2 == 0){
                assign_varType_ST(node, result2, 1); 
            } else {
                assign_varType_ST(node, result2, 0); 
            }
               
            
        }
    printf("cu %d\n", node->current->type);
    return node;
}

node_t* handle_var_def_ops(node_t* node){
    printf("in def\n");
    if (node->current->type == T_COLON){
        node = get_next(node);

        int type = handle_type(node);
        assign_varType_ST(node, type, runInfo->vol);

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
    runInfo->CurID = node->current->value.ID_name;
    
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
            add_funcall(runInfo->CurID);
        }
    } // else {
    //     if (strcmp(result->id, "write") == 0) {

    //     } else if (strcmp(result->id, "Int2Double") == 0) {

    //     } else if (strcmp(result->id, "Double2Int") == 0) {

    //     } else if (strcmp(result->id, "length") == 0) {

    //     } else if (strcmp(result->id, "substring") == 0) {

    //     } else if (strcmp(result->id, "ord") == 0) {

    //     } else if (strcmp(result->id, "chr") == 0) {

    //     }
    // }
}

node_t* handle_funcall_ops(node_t* node){

    if (node->current->type == T_ASSIGN){
        SymTable *current = NULL;
        if (runInfo->currentLVL == NULL){
            current = runInfo->globalFrame;
        } else {
            current = runInfo->currentLVL->currentTab;
        }
        Symbol *result = GetSymbol(current, node->left->current->value.ID_name); //* add look upwrards
        if (result == NULL){
            ThrowError(5);
        }
        node = get_next(node);
        node = handle_assign_ops(node);
        return node;
    } else if (node->current->type == T_LEFT_PAREN){
        runInfo->CurID = node->left->current->value.ID_name;
        check_funcall_id(runInfo->CurID); // *checking if valid funcall*
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
            runInfo->CurID = node->current->value.ID_name;
            node = get_next(node);
            return node;
        } else {
            ThrowError(2);
        }
    } else {
        int count3;
        node_t* start = node; 
        node = expression_token_count(node, &count3);  // *fix expressions* add check type for bool
        if(count3 == 0){
            ThrowError(2);
        }
        DataType result =  expression_parser(start, runInfo, count3); // *add type assign*
        if (result != BOOL){
            printf("result: %d\n", result);
            ThrowError(7);
        } 
        return node; // check if the node is {
    }

    

}

node_t* handle_if(node_t* node){ //check if ( is passed
    node = handle_cond_ops(node);
    //node = get_next(node);
    if (node->current->type == T_LEFT_BRACE){
        create_level();

        //search upwards
        InsertSymbol(runInfo->currentLVL->currentTab, runInfo->CurID);
        Symbol *result = search_upwards_ST(runInfo->CurID);
        AddVarDetails(runInfo->currentLVL->currentTab, runInfo->CurID, BOOL, result->variable.datatype, 0); // *add type assign*
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
    Symbol *result = GetSymbol(runInfo->globalFrame, runInfo->FID);
    printf("retVal: %d\n", retVal);
    printf("result: %d\n", result->function.ReturnType);
    if (result->function.ReturnType != retVal){
        ThrowError(4);
    } else {
        runInfo->FID = NULL;
    }
    return node;
}

node_t* handle_statement(node_t* node){
    printf("statement: %d\n", node->current->type);
    switch (node->current->type){
        case T_LET:
        case T_VAR:
            if(node->current->type == T_LET){
                runInfo->vol = 1;
            } else {
                runInfo->vol = 0;
            }                
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
        node->current = NULL;
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
        AddVarDetails(current, node->current->value.ID_name, VOID, false, runInfo->vol);

    }
    current = NULL;

}

void delete_FID_list(char* FID) {
    int index = -1;
    for (int i = 0; i < runInfo->count; i++) {
        if (strcmp(runInfo->funcalls[i], FID) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }
    free(runInfo->funcalls[index]);
    for (int i = index; i < runInfo->count - 1; i++) {
        runInfo->funcalls[i] = runInfo->funcalls[i + 1];
    }
    runInfo->count--;
    runInfo->funcalls = realloc(runInfo->funcalls, runInfo->count * sizeof(char*));
}

int check_FID_list(char* FID){
    for (int i = 0; i < runInfo->count; i++){
        if (strcmp(runInfo->funcalls[i], FID) == 0){
            return 1;
        }
    }
    return 0;
}

void add_funcall(char* FID) {
    runInfo->count++;
    runInfo->funcalls = realloc(runInfo->funcalls, runInfo->count * sizeof(char*));
    runInfo->funcalls[runInfo->count - 1] = malloc((strlen(FID) + 1) * sizeof(char));
    strcpy(runInfo->funcalls[runInfo->count - 1], FID);
}

void assign_varType_ST(node_t* node, int type, int vol){ // check if nillable is correctly filled and incldues !
    SymTable* current = NULL;
    if (runInfo->currentLVL == NULL){
        current = runInfo->globalFrame;
    } else {
        current = runInfo->currentLVL->currentTab;
    }
    Symbol *updateSymbol = GetSymbol(current,runInfo->CurID);
    if (updateSymbol == NULL){
        //err
    }
    AddVarDetails(current, runInfo->CurID, type, true, vol);
}


void parser(){
    

    init_runInfo();
    node_t *node = create_node();
    node->current = scan();
    runInfo->firstNode = node;

    while (true){
        switch (node->current->type) {
            case T_FUNC:
                node = get_next(node);
                node = handle_func_def(node);
                break;
            case T_EOF:
                if(runInfo->count != 0){
                    ThrowError(3); // *checck err type
                }
                start_generator(node); 
                break;// add clean up
            case T_LET:
            case T_VAR:
            case T_IF:
            case T_WHILE:
            case T_RETURN:
            case T_IDENTIFIER:
                printf("statementss: %d\n", node->current->type);
                node = handle_statement_list(node);
                break;

            default:
                ThrowError(2);
        }
    }  
    
}


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
    -handle built in funcs
    -return fix
    -function in param fix
    -add semcheck in expressions

* - Check
    - can there be just ID alone
    -check let declarations

* - Random
    

* - Code Gen
    -


*/

runTimeInfo *runInfo;


node_t* handle_param(node_t* node){
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
    char type = handle_type(node);
    AddParametr(runInfo->globalFrame, runInfo->ID, name, type);
    AddParametrID(runInfo->globalFrame, runInfo->ID, ID);

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
        printf("Error: Expected type\n");
        ThrowError(2);
    }
    
    return  type;
}

node_t* hadnle_func_retType(node_t* node){
    printf("funcRetType \n");
    printf("100 current token type: %d\n", node->current->type);
    if (node->current->type == T_ARROW){
        node = get_next(node);
        printf("103 current token type: %d\n", node->current->type);
        int type = handle_type(node);
        AddFunctionDetails(runInfo->globalFrame, runInfo->ID, type , true);
        node = get_next(node);
    } else {
        AddFunctionDetails(runInfo->globalFrame, runInfo->ID, 0, true);
    }
    return node;

}

node_t* handle_func_def(node_t* node){
    printf("funcDef \n");
    if (node->current->type != T_IDENTIFIER) {
        printf("112 Error: Expected identifier\n");
        ThrowError(2);
    }

    runInfo->ID=node->current->value.ID_name;
    if (GetSymbol(runInfo->builtInFunctions, runInfo->ID) != NULL ){
        //clean all
        ThrowError(3);
    } else if (GetSymbol(runInfo->globalFrame, runInfo->ID) != NULL ){
        ThrowError(3);
    } else {
        printf("insert func \n");
        InsertSymbol(runInfo->globalFrame, runInfo->ID);
        
    }   
    
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

node_t* expression_token_count(node_t* node, int* count){
    printf("exprTokenCount \n");
    *count = 0;
    printf("202 current token type: %d\n", node->current->type);
    while ((6 <= node->current->type && node->current->type <= 16) || 
            (34 <= node->current->type && node->current->type <= 38) ||
            node->current->type == T_LEFT_PAREN || node->current->type == T_RIGHT_PAREN || node->current->type == T_DOUBLE_QUESTION_MARK){
                if (node->current->type == T_IDENTIFIER){
                    if(node->left->current->type != 0  && 6 >= node->left->current->type && node->left->current->type <= 16){
                        break;
                    } 
                }
        (*count)++;
        node = get_next(node);
    }
    //printf ("in count: %d\n", *count);
    return node;
}

node_t* handle_assign_ops(node_t* node){ 
    printf("assignOps \n");
    printf("196 curr token %d \n", node->current->type);

    //node = get_next(node);  //check for correct sync
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
            printf("expr id \n");
            int count1;
            ExprType result1;
            node_t* start = node;
            node = expression_token_count(node, &count1);
            printf("count1: %d\n", count1);
            result1 = expression_parser(start, runInfo, count1);

        //call expression handle
        //if failed err.         
        }
    } else {
            printf("expr num \n");
            int count2;
            ExprType result2;
            node_t* start = node;
            node = expression_token_count(node, &count2);
            printf("count2: %d\n", count2);
            result2 = expression_parser(start, runInfo, count2); // add type assign 
            printf("expr num end \n");      
        }
    return node;
}

node_t* handle_var_def_ops(node_t* node){
    printf("varDefOps \n");
    if (node->current->type == T_COLON){
        node = get_next(node);
    
        int type = handle_type(node);
        assign_varType_ST(node, type, node->current->value.nillable);
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
        node = get_next(node);
        printf("2651 token type %d\n", node->current->type); 
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
    //node = get_next(node);

    int count3;
    node_t* start = node;
    node = expression_token_count(node, &count3);
    printf("count3: %d\n", count3);
    printf("curent token type: %d\n", node->current->type);

    if (node->current->type == T_LET){
        node = get_next(node);
        if (node->current->type == T_IDENTIFIER){
            //write
            node = get_next(node);
            node = get_next(node);
            return node;
        } else {
            ThrowError(2);
        }
    }

    expression_parser(start->right, runInfo, count3 - 2); // add type assign 
    printf("if cond end \n");      
    return node;


    if (node->current->type == T_LET){
        node = get_next(node);
        if (node->current->type == T_IDENTIFIER){
            //write
            node = get_next(node);
            node = get_next(node);
            return node;
        } else {
            ThrowError(2);
        }
    } else {
       
    }
    return node;
}

node_t* handle_if(node_t* node){ //check if ( is passed
    printf("if \n");
    node = handle_cond_ops(node);
    printf("after cond token %d\n", node->current->type);
    //node = get_next(node);
    if (node->current->type == T_LEFT_BRACE){
        node = get_next(node);
        node = handle_statement_list(node);

        //node = get_next(node);
        if (node->current->type != T_RIGHT_BRACE){
            printf("Error: Expected right brace\n");
            ThrowError(2);
        } else {
            
            node = get_next(node);
            if (node->current->type != T_ELSE){       //optional else ?
                printf("Error: Expected else\n");
                ThrowError(2);
            }
            node = get_next(node);
            if (node->current->type != T_LEFT_BRACE){
                printf("Error: Expected left brace\n");
                ThrowError(2);
            } else {
                node = get_next(node);
                node = handle_statement_list(node);

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
    printf("355 stmtntList  current token type: %d\n", node->current->type);
    if (node->current->type == T_RIGHT_BRACE || node->current->type == T_FUNC ){
        printf("RB return \n");
        printf("curr: %d \n", node->current->type);
        return node;
    }
    node = handle_statement(node);
    node = handle_statement_list(node);
    return node;
}

node_t* handle_return(node_t* node){
    printf("return\n");
    if (runInfo->currentLVL == NULL){
        ThrowError(2);
    }
    node = get_next(node);
    printf("curent node value %d \n", node->current->type);


    return node;
}

node_t* handle_statement(node_t* node){
    printf("420 token type %d\n", node->current->type);
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
            node = handle_return(node);
            return node;
        case T_EOF:
            start_generator(node);
            break; // add clean up
        default: 
            ThrowError(2);

    }
    printf("392 token type %d\n", node->current->type);
    //node = get_next(node);
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
    printf("new Current:  %d\n", node->current->type);
    if (node->current->type == T_ERORR){
        int err = node->current->value.integer;
        free_node_list(node);
        ThrowError(err);
    }
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

void define_func_ST(){

}

// void check_type(){
    
// }

void start_generator(node_t* node){
    while (node->left != NULL){
        node = node->left;
    }
    printf("gen\n");
    start_code_generation(node);
    //start_generating(node, runInfo);

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

void parser(){
    init_myInfo();
    node_t *node = create_node();
    node->current = scan();



    //node_t *start = node;
    // int count = expression_token_count(node);
    // printf("count: %d\n", count);
    // ExprType result = expression_parser(start, runInfo, count);
    // printf("result: %d\n", result);
    //exit(0);

    printf("token type %s\n", node->current->value.ID_name);

    bool run = true;
    while (run){
        switch (node->current->type) {
            case T_FUNC:
                node = get_next(node);
                printf("func t type %d\n", node->current->type);
                node = handle_func_def(node);
                break;
            case T_EOF:
                start_generator(node); 
                break;// add clean up
            default:
                printf("token type %d\n", node->current->type);
                node = handle_statement_list(node);
                if (node->current->type != T_FUNC){
                    node = get_next(node);
                }
                
                break;
        }
        printf("452token type %d\n", node->current->type);
    }  

    free_node_list(node);
    return;
}


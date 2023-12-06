/**
 * Project: Implementace překladače IFJ23
 *
 * @brief  Parser that controls the flow of the program, does syntax and sematic analysis, and starts code generation.
 *
 * @author Ivan Chodák xchoda00
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"





runTimeInfo *runInfo;
int inParamCount = 0;


Symbol *search_upwards_ST(char* ID){
    symTabLVL *current = runInfo->currentLVL;
    Symbol *result = NULL;
    while (current != NULL){
        result = GetSymbol(current->currentTab, ID);
        if (result != NULL){
            return result;
        }
        current = current->parantLVL;
        
    }
    result = GetSymbol(runInfo->globalFrame, ID);
    if (result == NULL){
            return NULL;
    } else {
        return result;
    }
    
    

}



void fill_builtin_symtab(SymTable *builtIn){ 
    Symbol *ToChange = NULL;

    InsertSymbol(builtIn, "readString");
    InsertSymbol(runInfo->globalFrame, "readString");
    AddFunctionDetails(runInfo->globalFrame, "readString",  STRQ, true);
    ToChange = GetSymbol(runInfo->globalFrame, "readString");
    ToChange->function.parametr_count = 0;


    InsertSymbol(builtIn, "readInt");
    InsertSymbol(runInfo->globalFrame, "readInt");
    AddFunctionDetails(runInfo->globalFrame, "readInt",  INTQ, true);
    ToChange = GetSymbol(runInfo->globalFrame, "readInt");
    ToChange->function.parametr_count = 0;

    InsertSymbol(builtIn, "readDouble");
    InsertSymbol(runInfo->globalFrame, "readDouble");
    AddFunctionDetails(runInfo->globalFrame, "readDouble",  FLOATQ, true);
    ToChange = GetSymbol(runInfo->globalFrame, "readDouble");
    ToChange->function.parametr_count = 0;


    InsertSymbol(builtIn, "write");
    InsertSymbol(runInfo->globalFrame, "write");
    AddFunctionDetails(runInfo->globalFrame, "write",  0, true);
    AddParametr(runInfo->globalFrame, "write", "_", "s", STR);
    ToChange = GetSymbol(runInfo->globalFrame, "write");
    ToChange->function.parametr_count = -1; 


    InsertSymbol(builtIn, "Int2Double");
    InsertSymbol(runInfo->globalFrame, "Int2Double");
    AddFunctionDetails(runInfo->globalFrame, "Int2Double",  FLOAT, true);
    AddParametr(runInfo->globalFrame, "Int2Double", "_", "i", INT);
        
    InsertSymbol(builtIn, "Double2Int");
    InsertSymbol(runInfo->globalFrame, "Double2Int");
    AddFunctionDetails(runInfo->globalFrame, "Double2Int",  INT, true);
    AddParametr(runInfo->globalFrame, "Double2Int", "_", "d", FLOAT);
    
    InsertSymbol(builtIn, "length");
    InsertSymbol(runInfo->globalFrame, "length");
    AddFunctionDetails(runInfo->globalFrame, "length",  INT, true);
    AddParametr(runInfo->globalFrame, "length", "_", "s", STR);


    InsertSymbol(builtIn, "substring");
    InsertSymbol(runInfo->globalFrame, "substring");
    AddFunctionDetails(runInfo->globalFrame, "substring",  STRQ, true);
    AddParametr(runInfo->globalFrame, "substring", "of", "s", STR);
    AddParametr(runInfo->globalFrame, "substring", "startingAt", "i", INT);
    AddParametr(runInfo->globalFrame, "substring", "endingBefore", "j", INT);
    //*return nill when cases

    InsertSymbol(builtIn, "ord");
    InsertSymbol(runInfo->globalFrame, "ord");
    AddFunctionDetails(runInfo->globalFrame, "ord",  INT, true);
    AddParametr(runInfo->globalFrame, "ord", "_", "c", STR);

    InsertSymbol(builtIn, "chr");
    InsertSymbol(runInfo->globalFrame, "chr");
    AddFunctionDetails(runInfo->globalFrame, "chr",  STR, true);
    AddParametr(runInfo->globalFrame, "chr", "_", "i", INT);



}

void start_generator(node_t* node){
    start_code_generation(runInfo->firstNode);
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

    SymTable *builtInFunctions = NULL;
    builtInFunctions = malloc(sizeof(SymTable));
    if (builtInFunctions == NULL){
        ThrowError(99);
    }
    SymTableInit(&builtInFunctions);
    runInfo->globalFrame = globalFrame;
    runInfo->builtInFunctions = builtInFunctions;
    fill_builtin_symtab(builtInFunctions);

    runInfo->currentLVL = NULL;
    

    runInfo->FID = NULL;
    runInfo->leftID = NULL;
    runInfo->rightID = NULL;
    runInfo->vol = -1;
    runInfo->firstNode = NULL;

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

    if(strcmp(ID, name) == 0){ 
        ThrowError(4);
    }
    node = get_next(node);
    if (node->current->type != T_COLON){
        ThrowError(2);
    }
    node = get_next(node);
    int type = handle_type(node);
    AddParametr(runInfo->globalFrame, runInfo->FID, name , ID, type);
    InsertSymbol(runInfo->currentLVL->currentTab, ID);
    AddVarDetails(runInfo->currentLVL->currentTab, ID, type, true, 1);
    Symbol *result = GetSymbol(runInfo->currentLVL->currentTab, ID);
    result->variable.parameter = true;
    
    return  node;
}


node_t* handle_param_list(node_t* node){
    if (node->current->type == T_RIGHT_PAREN){
        return node;
    }
    
    node = handle_function_param(node);
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

DataType handle_type(node_t* node){ 
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
   
    node = get_next(node);
    if (node->current->type != T_LEFT_PAREN) {
        ThrowError(2);
    }

    node = get_next(node);
    create_level();
    node = handle_param_list(node);

    node = get_next(node);
    node = hadnle_func_retType(node);
    if (node->current->type == T_LEFT_BRACE) {  

        

        node = get_next(node);
        node = handle_statement_list(node);   
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
            ThrowError(4);
        } else {
            runInfo->FID = NULL;
        }
    }
    node = get_next(node);
    return node;
}



node_t* handle_in_param(node_t* node){ 
    Symbol *functionCheck = NULL;
    if (runInfo->rightID == NULL){
        functionCheck = GetSymbol(runInfo->globalFrame, runInfo->leftID);
    } else {
        functionCheck = GetSymbol(runInfo->globalFrame, runInfo->rightID);
    }
    if (functionCheck == NULL){
        ThrowError(5);
    }

    Parametr *curPar = functionCheck->function.parametr;
    if (functionCheck->function.parametr_count != -1){
        for(int i = 0; i < inParamCount-1; i++){
        
            if(curPar == NULL){
                ThrowError(4);
            }
            curPar = curPar->next;
        }
    }
    

    if (node->current->type == T_IDENTIFIER){
        node = get_next(node);
        if (node->current->type == T_COLON){
            if(strcmp(node->left->current->value.ID_name, curPar->name) != 0){
                ThrowError(4); 
            }
            node = get_next(node);
            if (node->current->type == T_IDENTIFIER){

                char* ID = node->current->value.ID_name;
                Symbol *var = search_upwards_ST(ID);
                if (var == NULL){
                    ThrowError(5);
                }
                if (var->variable.datatype != curPar->type && var->variable.datatype + 1 != curPar->type && var->variable.datatype != curPar->type + 1){
                
                    ThrowError(4);
                }
                node = get_next(node);
                return  node;
            } else if (node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
                DataType currentType;
                if (node->current->type == T_INT){
                    if(node->current->value.nillable == 1){
                        currentType = INTQ;
                    } else {
                        currentType = INT;
                    }
                } else if (node->current->type == T_STRING){
                    if(node->current->value.nillable == 1){
                        currentType = STRQ;
                    } else {
                        currentType = STR;
                    }
                } else if (node->current->type == T_DOUBLE){
                    if(node->current->value.nillable == 1){
                        currentType = FLOATQ;
                    } else {
                        currentType = FLOAT;
                    }
                }
                if(curPar->type != currentType && curPar->type + 1 != currentType && curPar->type != currentType + 1){
                    ThrowError(4);
                }

                node = get_next(node);
                return  node;
            } else {
                ThrowError(2);
            }
        } 
        Symbol *var = search_upwards_ST(node->left->current->value.ID_name);
        if (var == NULL){
            ThrowError(5);
        }
        Symbol *func;
        if(runInfo->rightID == NULL){
            func = GetSymbol(runInfo->globalFrame, runInfo->leftID);
            
        }else {
            func= GetSymbol(runInfo->globalFrame, runInfo->rightID);
        }
        if (func->function.parametr_count == -1){
            
        } else {
            Parametr *param = func->function.parametr;
            for (int i = 0; i < inParamCount-1; i++){
                
                param = param->next;
                if(param == NULL){
                    ThrowError(4);
                }
                
            }
            if (param->type != var->variable.datatype && param->type + 1 != var->variable.datatype && param->type != var->variable.datatype + 1){

                    ThrowError(4);
            }
            
        }
        
        
        return  node;
    } else { 
        if(node->current->type == T_INT || node->current->type == T_STRING || node->current->type == T_DOUBLE){
            DataType currentType;
            if (node->current->type == T_INT){
                if(node->current->value.nillable == 1){
                    currentType = INTQ;
                } else {
                    currentType = INT;
                }
            } else if (node->current->type == T_STRING){
                if(node->current->value.nillable == 1){
                    currentType = STRQ;
                } else {
                    currentType = STR;
                }
            } else if (node->current->type == T_DOUBLE){
                if(node->current->value.nillable == 1){
                    currentType = FLOATQ;
                } else {
                    currentType = FLOAT;
                }
            }
            Symbol *func = NULL;
            if (runInfo->rightID == NULL){
                func = GetSymbol(runInfo->globalFrame, runInfo->leftID);
            } else {
                func = GetSymbol(runInfo->globalFrame, runInfo->rightID);
            } 
            Parametr *param = func->function.parametr;
            
            if (func->function.parametr_count == -1){
                if(param->type < 1 && param->type > 8){ 
                    ThrowError(3);
                }
                
            } else {
                for(int i = 0; i < inParamCount-1 ; i++){
                
                    param = param->next;       
                    if(param == NULL){
                        ThrowError(4);
                    }
                }
                if (param->type != currentType && param->type + 1 != currentType && param->type != currentType + 1){
                    ThrowError(4);
                }
            }
            
            node = get_next(node);
            return  node;
        } else {
            ThrowError(2);
        }
        
    }
}


node_t* handle_in_param_list(node_t* node){
    Symbol *result;
    if (runInfo->rightID == NULL){
            result = GetSymbol(runInfo->globalFrame, runInfo->leftID);
        } else {
            result = GetSymbol(runInfo->globalFrame, runInfo->rightID);
        }
    if(node->current->type == T_RIGHT_PAREN){
        if (inParamCount != result->function.parametr_count && result->function.parametr_count != -1){
            ThrowError(4);
        }
        inParamCount = 0;
        runInfo->rightID == NULL;
        return node;
    }
    inParamCount++;
    if (result->function.parametr_count == 0){
            ThrowError(4);
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
    if(result->function.parametr_count != -1){
        if(inParamCount != result->function.parametr_count){
        ThrowError(4);
    }
    }
    
    runInfo->rightID == NULL;
    if(inParamCount != result->function.parametr_count){
        if (result->function.parametr_count != -1){
            ThrowError(4);
        }
    }
    return node;
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
            node->left->current->type != T_DOUBLE_QUESTION_MARK  &&
            node->left->current->type != T_ASSIGN){
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
    if (node->left->current->type >= 6 && node->left->current->type <= 16){
        ThrowError(2);
    }
    return node;
}

void handle_type_assign(DataType type){
 
}



node_t* handle_assign_ops(node_t* node){ 

    if (node->current->type == T_IDENTIFIER){
        runInfo->rightID = node->current->value.ID_name;
        node = get_next(node);
        if(node->current->type == T_LEFT_PAREN){
            check_funcall_id(runInfo->rightID); 
            Symbol *function = GetSymbol(runInfo->globalFrame, runInfo->rightID);
            Symbol *toCheck = search_upwards_ST(runInfo->leftID);
            if (toCheck == NULL){
                ThrowError(5);
            }
            if (function->function.ReturnType != toCheck->variable.datatype){
                if(toCheck->variable.datatype == VOID){
                    toCheck->variable.datatype = function->function.ReturnType;
                } else {
                    ThrowError(7);
                }
                
            }
            node = get_next(node);
            inParamCount = 0;
            node = handle_in_param_list(node);
            node = get_next(node);
            return node;
        } else {
            
            Symbol *assign = search_upwards_ST(runInfo->leftID);
            Symbol *result = search_upwards_ST(runInfo->rightID);

            DataType exprType = VOID;
            if (result == NULL || assign == NULL){
                ThrowError(5);
            }
            int count1;
            node_t* start = node->left;
            node = expression_token_count(start, &count1);
            if(count1 == 1){
                assign_varType_ST(runInfo->leftID, result->variable.datatype);
            } else {
                exprType = expression_parser(start, runInfo, count1);
                assign_varType_ST(runInfo->leftID, exprType);
            }
            
            if(assign->variable.nillable == 1){
                ThrowError(9);
            }
        }
    } else {
            int count2;
            DataType result2;
            node_t* start = node;
            node = expression_token_count(node, &count2);
            Symbol *assign = search_upwards_ST(runInfo->leftID);
            if (assign == NULL){
                ThrowError(5);
            } else {
                result2 = expression_parser(start, runInfo, count2);
                if (assign->variable.datatype == VOID){
                    assign_varType_ST(runInfo->leftID, result2); 
                } else {
                    if (assign->variable.datatype != result2 && assign->variable.datatype + 1 != result2 &&
                    assign->variable.datatype != result2 + 1){
                    ThrowError(7);
                }
                }
                
            }
           

            assign_varType_ST(runInfo->leftID, result2); 
            
            
        }
    return node;
}

node_t* handle_var_def_ops(node_t* node){
    if (node->current->type == T_COLON){
        node = get_next(node);

        int type = handle_type(node);
        SymTable *current = NULL;
        if(runInfo->currentLVL == NULL){
            current = runInfo->globalFrame;
        } else {
            current = runInfo->currentLVL->currentTab;
        }
        Symbol *result = GetSymbol(current, runInfo->leftID);
        AddVarDetails(current, runInfo->leftID, type, false, result->variable.VoL);
        node = get_next(node);

        if (node->current->type == T_ASSIGN){
            node = get_next(node);

            node = handle_assign_ops(node);

            SymTable *current = NULL;
            if(runInfo->currentLVL == NULL){
                current = runInfo->globalFrame;
            } else {
                current = runInfo->currentLVL->currentTab;
            }
            Symbol *varToChange = GetSymbol(current, runInfo->leftID);
            varToChange->variable.init = true;
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
            ThrowError(2);
        }
    }
}

node_t* handle_var_def(node_t* node){
    if (node->current->type != T_IDENTIFIER) {
        ThrowError(2);
    }
    runInfo->leftID = node->current->value.ID_name;
    Symbol *result = search_upwards_ST(runInfo->leftID);
    if (result != NULL){
        ThrowError(3);
    }
    
    define_var_ST(node);
    Symbol *varToChange = GetSymbol(runInfo->globalFrame, runInfo->leftID);
    node = get_next(node);
    node = handle_var_def_ops(node);
    return node;
}

void check_funcall_id(char* ID){
    Symbol *result = GetSymbol(runInfo->globalFrame, ID);
    if (result == NULL){
        result = GetSymbol(runInfo->builtInFunctions, ID);
        if (result == NULL){
            ThrowError(3);
        } else{
            return;
        }
    } else {
        return;
    }
}

node_t* handle_funcall_ops(node_t* node){
    runInfo->rightID = NULL;
    
    if (node->current->type == T_ASSIGN){
        
        node = get_next(node);
        node = handle_assign_ops(node);
        return node;
    } else if (node->current->type == T_LEFT_PAREN){
        check_funcall_id(runInfo->leftID);
        node = get_next(node);
        inParamCount = 0;
        node = handle_in_param_list(node);
        if (node->current->type != T_RIGHT_PAREN){
            ThrowError(2);
        } 
        node = get_next(node);
        return  node;
    } else {
        ThrowError(2);
        return node;
    }
    return node;
}




node_t* handle_cond_ops(node_t* node){
    if (node->current->type == T_LET){  
        node = get_next(node);
        if (node->current->type == T_IDENTIFIER){
       
            Symbol *found =  search_upwards_ST(node->current->value.ID_name);
            if(found == NULL){
                ThrowError(5);
            }
            if (found){
                if (found->variable.VoL == 0){
                    ThrowError(6);
                }
            }
            
            InsertSymbol(runInfo->currentLVL->currentTab, node->current->value.ID_name);
            AddVarDetails(runInfo->currentLVL->currentTab, node->current->value.ID_name, VOID, false, 1);
            Symbol *result = GetSymbol(runInfo->currentLVL->currentTab, node->current->value.ID_name);
            result->variable.nillable = 2;
            node = get_next(node);
            if (node->current->type != T_LEFT_BRACE){
                ThrowError(2);
            }
            return node;
        } else {
            ThrowError(2);
        }
    } else {
        int count3;
        node_t* start = node; 
        node = expression_token_count(node, &count3); 
        if(count3 == 0){
            ThrowError(2);
        }
        DataType result =  expression_parser(start, runInfo, count3);
        if (result != BOOL){
            ThrowError(7);
        } else {
            return node;
        }
    }

    

}

node_t* handle_if(node_t* node){ 
    create_level();
    node = handle_cond_ops(node);
    if (node->current->type == T_LEFT_BRACE){
        

        
        Symbol *result = search_upwards_ST(runInfo->leftID); 

        
        node = get_next(node);
        node = handle_statement_list(node);
        if (node->current->type != T_RIGHT_BRACE){ 
            ThrowError(2);
        }
        pop_level();

        node = get_next(node);
        if (node->current->type != T_ELSE){  
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

node_t* handle_statement_list(node_t* node){ 

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
    node = expression_token_count(start, &count);
    DataType retVal = expression_parser(start, runInfo, count); 
    Symbol *result = GetSymbol(runInfo->globalFrame, runInfo->FID);
    if (result->function.ReturnType != retVal){
        ThrowError(4);
    } else {
        runInfo->FID = NULL;
    }
    return node;
}

node_t* handle_statement(node_t* node){
    switch (node->current->type){
        case T_LET:
        case T_VAR:
            if(node->current->type == T_LET){
                runInfo->vol = 1;
            } else {
                runInfo->vol = 0;
            }    
            runInfo->inDef = 1;            
            node = get_next(node);
            node = handle_var_def(node);
            return node;
            break;

        case T_IDENTIFIER:
            runInfo->leftID = node->current->value.ID_name;
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
    return current->right;
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
    SymTableFree(runInfo->currentLVL->currentTab); 
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

    InsertSymbol(current, runInfo->leftID);
    AddVarDetails(current, runInfo->leftID, VOID, false, runInfo->vol);
    current = NULL;

}




void assign_varType_ST(char* key, int type){ 
    SymTable* current = NULL;
    if (runInfo->currentLVL == NULL){
        current = runInfo->globalFrame;
    } else {
        current = runInfo->currentLVL->currentTab;
    }
    Symbol *updateSymbol = GetSymbol(current,key);
    if (updateSymbol == NULL){
        ThrowError(5);
    }
    int vol;
    if (runInfo->inDef == 1 ){
        vol = runInfo->vol;
    } else {
        vol = updateSymbol->variable.nillable;
    }

    AddVarDetails(current, key, type, true, vol);
    current = NULL;
    return;
    
}

node_t* load_next(node_t* current){
    node_t *node = create_node();
    node->current = scan();
    if (node->current->type == T_ERORR){
        int err = node->current->value.integer;
        free_node_list(node);
        ThrowError(err);
    }
    current->right = node;
    node->left = current;
    return node;

}


void load_all_tokens(){
    node_t *node = create_node();
    node->current = scan();
    if (node->current->type == T_ERORR){
        int err = node->current->value.integer;
        free_node_list(node);
        ThrowError(err);
    }
    runInfo->firstNode = node;
    if (node->current->type == T_EOF){
        return;
    } else {
        while (node->current->type != T_EOF){
            node = load_next(node);
        }
        node = load_next(node);
    }
}


node_t* handle_param_DEF_list(node_t* node){
    if (node->current->type == T_RIGHT_PAREN){
        return node;
    }
    
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

    if(strcmp(ID, name) == 0){ 
        ThrowError(4);
    }
    node = get_next(node);
    if (node->current->type != T_COLON){
        ThrowError(2);
    }
    node = get_next(node);
    int type = handle_type(node);
    AddParametr(runInfo->globalFrame, runInfo->FID, name , ID, type);

    node = get_next(node);
    if (node->current->type == T_COMMA){    
        node = get_next(node);
        if (node->current->type == T_RIGHT_PAREN){ 
            ThrowError(2);
        }
        node = handle_param_DEF_list(node);
        return node;
    } else if (node->current->type == T_RIGHT_PAREN){
        return node;
    } else {
        ThrowError(2);
    }
}


void parse_for_fdef(){
    node_t *node = runInfo->firstNode;
    while(node->current->type != T_EOF){
        if (node->current->type == T_FUNC){
            node = get_next(node);
            if(node->current->type == T_IDENTIFIER){
                runInfo->FID = node->current->value.ID_name;
                Symbol *builtIn = GetSymbol(runInfo->builtInFunctions, node->current->value.ID_name);
                if (builtIn != NULL ){
                        ThrowError(3);
                    } else if (GetSymbol(runInfo->globalFrame, runInfo->FID) != NULL ){
                        ThrowError(3);
                    } else {
                        runInfo->FID = node->current->value.ID_name;
                        InsertSymbol(runInfo->globalFrame, runInfo->FID);
                        Symbol *result1 = GetSymbol(runInfo->globalFrame, runInfo->FID);
                        
                    }   // add built in check
                AddFunctionDetails(runInfo->globalFrame, runInfo->FID, 0, true);
                node = get_next(node);
                if(node->current->type == T_LEFT_PAREN){
                    node = get_next(node);
                        node = handle_param_DEF_list(node);
                    if(node->current->type == T_RIGHT_PAREN){
                        
                        node = get_next(node);
                        if(node->current->type == T_ARROW){
                            
                            node = get_next(node);
                            if(node->current->type == T_KW_INT || node->current->type == T_KW_DOUBLE || node->current->type == T_KW_STRING){
        
                                DataType type = handle_type(node);
                                AddFunctionDetails(runInfo->globalFrame, runInfo->FID, type, true);
                                Symbol *result = GetSymbol(runInfo->globalFrame, runInfo->FID);
                                node = get_next(node);
                            } 
                        }
                    }
                }
            }
        }
        node = get_next(node);
    }
}


void parser(){
    
    init_runInfo();
    load_all_tokens();
    parse_for_fdef();
    node_t *node = runInfo->firstNode;


    while (true){
        switch (node->current->type){
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


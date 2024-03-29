/**
 * Project: Implementace překladače IFJ23
 *
 * @brief  Symtable impleneted as a implcitily chained hash table.
 *
 * @author Dominik Šagát xsagat01
 */



#include "symtable.h"


unsigned long HashFunction(char *key)
{
    int i = 0;
    unsigned long hash = 1;
    while(key[i] != '\0'){
        hash = 79*hash ^ (int)key[i];
        i++;
    }

    return hash % SYMTABLE_SIZE;
}

void SymTableInit(SymTable **table) {
    *table = (SymTable *)malloc(sizeof(SymTable));
    for (int i = 0; i < SYMTABLE_SIZE; i++) {
        (**table)[i] = NULL;
    }
}

int InsertSymbol(SymTable *table, char *key){
    if (table == NULL || key == NULL)
        return -1; //error

    unsigned long hash = HashFunction(key);

    while((*table)[hash] != NULL){
        if (hash == HashFunction(key)-1)
            return -1; // error, symtable is full
        
        if (!strcmp((*table)[hash]->id, key))
            return -1; // symbol is already there
        
         if (!strcmp((*table)[hash]->id, "if")){ //"removed" symbol
            free((*table)[hash]->id);          
            free((*table)[hash]);
            (*table)[hash] = NULL;
            break;
         }
        hash++;
        if (hash == SYMTABLE_SIZE) // the end is connected to the beginning
            hash = 0;

    }
    Symbol *temp = malloc(sizeof(Symbol));
        if (temp == NULL)
            return -1; //error

    temp->id = malloc(strlen(key) + 1);
    if (temp->id == NULL)
        return -1;
    temp->type = 3; // not function or var yet
    temp->variable.strVal = NULL;
    temp->function.parametr = NULL;
    temp->function.parametr_count = 0;

    strcpy(temp->id, key);
    (*table)[hash] = temp;
    //printf("vkladam prvek %s na index %li\n", key, hash);
    return 1;
}


int RemoveSymbol(SymTable *table, char *key){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //symbol doesnt exist

    if ((*table)[hash] != NULL) {
        if((*table)[hash]->id != NULL){
           (*table)[hash]->id = realloc((*table)[hash]->id, strlen("if") + 1);
            if((*table)[hash]->id == NULL)
                return -1;
            strcpy((*table)[hash]->id, "if"); // if cant be id, so it will represent removed symbol
        }
    }
    if ((*table)[hash]->type == 1){ //if it is a function
            while((*table)[hash]->function.parametr != NULL ){
                Parametr *parametr = (*table)[hash]->function.parametr;
                (*table)[hash]->function.parametr = parametr->next;  // clearing list of parameters
                clear_parametr(parametr); 
                parametr_free(parametr);
                free(parametr);
                parametr = NULL;
            }
        }
    (*table)[hash]->type = 2;// not function or var
    return 1;
}

Symbol *GetSymbol(SymTable *table, char *key){
    int hash = Searching(table, key);
    if(hash != -1)
        return (*table)[hash];
    return NULL;
}

int AddVarDetails(SymTable *table, char *key, DataType type, bool init, VarOrLet vol){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1;
    (*table)[hash]->type = 0;
    (*table)[hash]->variable.init = init;
    (*table)[hash]->variable.datatype = type;
    (*table)[hash]->variable.VoL = vol;

    return 1;
}

int insert_int_value(SymTable *table, char *key, int value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; 
    (*table)[hash]->variable.intVal = value;
    if((*table)[hash]->variable.datatype != 1)    
        (*table)[hash]->variable.datatype = 1; 
    return 1;
}

int insert_double_value(SymTable *table, char *key, double value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1;
    (*table)[hash]->variable.doubleVal = value;
    if((*table)[hash]->variable.datatype != 4)    
        (*table)[hash]->variable.datatype = 4; 
    return 1;
}

int insert_string_value(SymTable *table, char *key, char *value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; 
    (*table)[hash]->variable.strVal = malloc(sizeof(strlen(value)));
    strcpy((*table)[hash]->variable.strVal, value);
    if((*table)[hash]->variable.datatype != 2)    
        (*table)[hash]->variable.datatype = 2;  
    return 1;
}

int insert_bool_value(SymTable *table, char *key, bool value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1;
    (*table)[hash]->variable.boolVal = value;
    if((*table)[hash]->variable.datatype != 3)    
        (*table)[hash]->variable.datatype = 3;
    return 1;
}



int Searching(SymTable *table, char *key){
    if (table == NULL || key == NULL)
        return  -1; 
    unsigned long hash = HashFunction(key);

    while((*table)[hash] != NULL && strcmp((*table)[hash]->id, key)){
        if (hash == HashFunction(key)-1)
            return -1;

        hash++;
        if (hash == SYMTABLE_SIZE) 
            hash = 0;
    }

    if ((*table)[hash] == NULL)
        return -1;
    return hash;

}

int AddFunctionDetails(SymTable *table, char *key, DataType returnType, bool defined){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1;
    (*table)[hash]->type = 1;
    (*table)[hash]->function.defined = defined;
    (*table)[hash]->function.ReturnType = returnType;
    return 1;
}

int AddParametr(SymTable *table, char *key, char *name, char *id, DataType type){

    int hash = Searching(table, key);
    if(hash == -1)
        return -1;
    
    if ((*table)[hash]->type != 1)
        return -1; //it is not a function
    Parametr *new = malloc(sizeof (Parametr));
    if (parametr_init(new) == -1)
        return -1;
    if (add_parametr_name(new, name, type) == -1)
        return -1;
    if (add_parametr_id(new, id) == -1)
        return -1;
    new->next = NULL; 
    Parametr *current = (*table)[hash]->function.parametr;

    if (current == NULL) {
        (*table)[hash]->function.parametr = new;
    }
    else {
        while (current->next != NULL) {
            current = current->next;
        }
    current->next = new;
    }
    (*table)[hash]->function.parametr_count++;

return 1;
}

void SymTableFree(SymTable *table){
     for (int i = 0; i < SYMTABLE_SIZE; i++) {
     if ((*table)[i] != NULL) {
        if((*table)[i]->id != NULL)
            free((*table)[i]->id);
        if((*table)[i]->variable.strVal != NULL)
            free((*table)[i]->variable.strVal);
        if ((*table)[i]->type == 1){ //it is function, free parameter
            while((*table)[i]->function.parametr != NULL ){
                Parametr *parametr = (*table)[i]->function.parametr;
                (*table)[i]->function.parametr = parametr->next;
                clear_parametr(parametr); 
                parametr_free(parametr);
                free(parametr);
                parametr = NULL;
                (*table)[i]->function.parametr_count = 0;
            }
        }
        free((*table)[i]);
        }
     }
     free(table);
}

/*
int main(){
SymTable *table= NULL;
SymTable *table1 = NULL;
SymTable *table2= NULL;
SymTableInit(&table2);
SymTableInit(&table);
SymTableInit(&table1);
InsertSymbol(&(*table2), "symbol");

Symbol *symbol;
Symbol *symbol1;
InsertSymbol(&(*table2), "iffs");
InsertSymbol(&(*table2), "p");
RemoveSymbol(&(*table), "function");
InsertSymbol(&(*table), "pole");
InsertSymbol(&(*table), "function");
if (AddFunctionDetails(&(*table), "function", 2, 1) == -1)
    printf("Neexistuje \n");
InsertSymbol(&(*table), "pollal");
AddParametr(&(*table), "function", "jmena", "id",1);
AddParametr(&(*table), "function", "jmenaa","id" ,1);
AddParametr(&(*table), "function", "parmaetr" ,"ps", 1);
AddFunctionDetails(&(*table), "function", 2, 1);

AddVarDetails(&(*table), "pollal", 0, 1, 0);
insert_string_value(&(*table), "pollal", "ahoj");
symbol = GetSymbol(&(*table), "function");
symbol1 = GetSymbol(&(*table), "pollal");
if (symbol!=NULL){
    printf("string: %s\n", symbol->id);
    printf("init? %d\n", symbol->function.ReturnType);
    printf("datatype %d\n", symbol->function.defined);
    printf("PArametr %s\n", symbol->function.parametr->next->next->name);
    printf("PArametrId %s\n", symbol->function.parametr->id);
    printf("count %d\n", symbol->function.parametr_count);
}
printf("value: %s\n", symbol1->variable.strVal);
printf("type: %d\n", symbol1->variable.datatype);
RemoveSymbol(&(*table), "popal");
InsertSymbol(&(*table), "popal");
SymTableFree(&(*table));
SymTableFree(&(*table1));
SymTableFree(&(*table2));
return 0;
}*/
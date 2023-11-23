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
            return -1; // error, udelalo to uz kolo
        
        if (!strcmp((*table)[hash]->id, key))
            return -1; // symbol uz je v symtable
        
         if (!strcmp((*table)[hash]->id, "if")){ //"odstraneny" prvek
            free((*table)[hash]->id);
            clear_parametr(&(*table)[hash]->parametr);
            parametr_free(&(*table)[hash]->parametr);            
            free((*table)[hash]);
            (*table)[hash] = NULL;
            break;
         }
        hash++;
        if (hash == SYMTABLE_SIZE) //kruhove pole
            hash = 0;

    }
    Symbol *temp = malloc(sizeof(Symbol));
        if (temp == NULL)
            return -1; //error

    temp->id = malloc(strlen(key) + 1);
    if (temp->id == NULL)
        return -1;
    if (parametr_init(&temp->parametr) == -1)
        return -1;

    temp->type = 3; //zatim neni funkce ani var
    temp->variable.strVal = NULL;

    strcpy(temp->id, key);
    (*table)[hash] = temp;
    //printf("vkladam prvek %s na index %li\n", key, hash);

    return 1;
}


int RemoveSymbol(SymTable *table, char *key){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to

    if ((*table)[hash] != NULL) {
        if((*table)[hash]->id != NULL){
           (*table)[hash]->id = realloc((*table)[hash]->id, strlen("if") + 1);
            if((*table)[hash]->id == NULL)
                return -1;
            strcpy((*table)[hash]->id, "if"); // if nemuze byt identifikator, oznacuje odstraneny prvek
        }
    }
    (*table)[hash]->type = 2;//ani func ani var
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
        return -1; //nenašlo to
    (*table)[hash]->type = 0;
    (*table)[hash]->variable.init = init;
    (*table)[hash]->variable.datatype = type;
    (*table)[hash]->variable.VoL = vol;

    return 1;
}

int insert_int_value(SymTable *table, char *key, int value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to
    (*table)[hash]->variable.intVal = value;
    if((*table)[hash]->variable.datatype != 1)    
        (*table)[hash]->variable.datatype = 1; //pretypovani
    return 1;
}

int insert_double_value(SymTable *table, char *key, double value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to
    (*table)[hash]->variable.doubleVal = value;
    if((*table)[hash]->variable.datatype != 4)    
        (*table)[hash]->variable.datatype = 4; //pretypovani
    return 1;
}

int insert_string_value(SymTable *table, char *key, char *value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to
    (*table)[hash]->variable.strVal = malloc(sizeof(strlen(value)));
    strcpy((*table)[hash]->variable.strVal, value);
    if((*table)[hash]->variable.datatype != 2)    
        (*table)[hash]->variable.datatype = 2; //pretypovani   
    return 1;
}

int insert_bool_value(SymTable *table, char *key, bool value){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to
    (*table)[hash]->variable.boolVal = value;
    if((*table)[hash]->variable.datatype != 3)    
        (*table)[hash]->variable.datatype = 3; //pretypovani   
    return 1;
}



int Searching(SymTable *table, char *key){ // funkce vracejici hash, aby se to neopakovalo v kazde funkci
    if (table == NULL || key == NULL)
        return  -1; //error1
    unsigned long hash = HashFunction(key);

    while((*table)[hash] != NULL && strcmp((*table)[hash]->id, key)){
        if (hash == HashFunction(key)-1)
            return -1; // error, udelalo to uz kolo a nenaslo to symbol    

        hash++;
        if (hash == SYMTABLE_SIZE) //kruhove pole
            hash = 0;
    }

    if ((*table)[hash] == NULL)
        return -1; //retezec neni v tabulce
    return hash;

}

int AddFunctionDetails(SymTable *table, char *key, DataType returnType, bool defined){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to

    (*table)[hash]->type = 1;
    (*table)[hash]->function.defined = defined;
    (*table)[hash]->function.ReturnType = returnType;
    return 1;
}

int AddParametr(SymTable *table, char *key, char *name, char type){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to
    if (add_parametr_name(&(*table)[hash]->parametr, key, type) == -1)
        return -1;
    return 1;
}

int AddParametrID(SymTable *table, char *key, char *id){
    int hash = Searching(table, key);
    if(hash == -1)
        return -1; //nenašlo to
    if (add_parametr_id(&(*table)[hash]->parametr, id) == -1)
        return -1;
    return 1;
}

void SymTableFree(SymTable *table){
     for (int i = 0; i < SYMTABLE_SIZE; i++) {
     if ((*table)[i] != NULL) {
        if((*table)[i]->id != NULL)
            free((*table)[i]->id);
        if((*table)[i]->variable.strVal != NULL)
            free((*table)[i]->variable.strVal);
        clear_parametr(&(*table)[i]->parametr);
        parametr_free(&(*table)[i]->parametr);
        free((*table)[i]);
        }
     }
     free(table);
}

/* // kdyby se funkce potrebovala, tak ji musim dodelat
void copy_to_child(SymTable *parent, SymTable *current) { //jeste by to chtelo alokovat znovu pamet pro str value,
    if(parent == NULL || current == NULL)
        return;
    for (int i = 0; i < SYMTABLE_SIZE; i++) {
        if ((*parent)[i] != NULL) {
            Symbol *temp = malloc(sizeof(Symbol));
            temp->id = malloc(strlen((*parent)[i]->id) + 1);
            if (temp->id == NULL)
                return;
            strcpy(temp->id, (*parent)[i]->id);

            temp->type = (*parent)[i]->type;
            if (temp->type == 0){ //var
                temp->variable = (*parent)[i]->variable;
                if (parametr_init(&temp->parametr) == -1)
                    return;
            }
            else if (temp->type == 1){ //func
                temp->function = (*parent)[i]->function;
                temp->parametr.str = malloc((*parent)[i]->parametr.alloc_size);
                if(temp->parametr.str == NULL)
                    return;
                strcpy(temp->parametr.str, (*parent)[i]->parametr.str);
                temp->parametr.length = (*parent)[i]->parametr.length;
                temp->parametr.alloc_size = (*parent)[i]->parametr.alloc_size;
            }
            else{
                   if (parametr_init(&temp->parametr) == -1)
                    return;
            }
            temp->variable.strVal = NULL;
           (*current)[i]= temp;
        }
            }    }*/
/*
int main(){
SymTable *table= NULL;
SymTable *table1 = NULL;
SymTable *table2= NULL;
SymTableInit(&table2);
SymTableInit(&table);
SymTableInit(&table1);
InsertSymbol(&(*table2), "p");

Symbol *symbol;
Symbol *symbol1;
InsertSymbol(&(*table2), "poal");
InsertSymbol(&(*table2), "p");
RemoveSymbol(&(*table), "p");
InsertSymbol(&(*table), "pole");
InsertSymbol(&(*table), "popal");
if (AddFunctionDetails(&(*table), "popal", 2, 1) == -1)
    printf("Neexistuje \n");
InsertSymbol(&(*table), "pollal");
AddParametr(&(*table), "popal", "jmena", 'd');
AddParametr(&(*table), "popal", "jmenaa" ,'i');
AddParametr(&(*table), "popal", "x" ,'p');
AddParametrID(&(*table), "popal", "id");
AddParametrID(&(*table), "popal", "idadwdwadaw");
AddParametrID(&(*table), "popal", "iddsadadwdwada");

AddVarDetails(&(*table), "pollal", 0, 1, 0);
insert_string_value(&(*table), "pollal", "ahoj");
symbol = GetSymbol(&(*table), "popal");
symbol1 = GetSymbol(&(*table), "pollal");
if (symbol!=NULL){
    printf("string: %s\n", symbol->id);
    printf("init? %d\n", symbol->function.ReturnType);
    printf("datatype %d\n", symbol->function.defined);
    printf("PArametr %s\n", symbol->parametr.name);
    printf("PArametr %s\n", symbol->parametr.id);
}
printf("value: %s\n", symbol1->variable.strVal);
printf("type: %d\n", symbol1->variable.datatype);
SymTableFree(&(*table));
SymTableFree(&(*table1));
SymTableFree(&(*table2));
return 0;
}*/
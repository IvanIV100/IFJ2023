#include "symtable.h"

int *tombstone; //pomocne pole, pri odstraneni symbolu z tabulky

unsigned long HashFunction(char *str)
{
    int i = 0;
    unsigned long hash = 1;
    while(str[i] != '\0'){
        hash = 79*hash ^ (int)str[i];
        i++;
    }

    return hash % SYMTABLE_SIZE;
}

void SymTableInit(SymTable *table){
        tombstone = (int *)malloc(SYMTABLE_SIZE * sizeof(int));
        for (int i = 0; i < SYMTABLE_SIZE; i++){
            (*table)[i] = NULL;
            tombstone[i] = 0;

        }

}

int InsertSymbol(SymTable *table, char *str){
    if (table == NULL || str == NULL)
        return -1; //error

    unsigned long hash = HashFunction(str);

    while((*table)[hash] != NULL && !tombstone[hash]){
        if (hash == HashFunction(str)-1)
            return -1; // error, udelalo to uz kolo
        
        if (!strcmp((*table)[hash]->id, str))
            return -1; // symbol uz je v symtable
        
        hash++;
        if (hash == SYMTABLE_SIZE) //kruhove pole
            hash = 0;

    }
    Symbol *temp = malloc(sizeof(Symbol));
        if (temp == NULL)
            return -1; //error

    temp->id = malloc(strlen(str) + 1);
    if (temp->id == NULL)
        return -1;
    strcpy(temp->id, str);
    (*table)[hash] = temp;
    printf("Pridavam: %li\n", hash);
    tombstone[hash] = 0;

    return 1;
}

int RemoveSymbol(SymTable *table, char *str){
    if (table == NULL || str == NULL)
        return -1; //error

    unsigned long hash = HashFunction(str);
    if ((*table)[hash] == NULL)
            return -1; //retezec neni v tabulce

    while(strcmp((*table)[hash]->id, str)){
        if (hash == HashFunction(str)-1)
            return -1; // error, udelalo to uz kolo
        
        if ((*table)[hash] == NULL && !tombstone[hash])
            return -1; //error, symbol se nevyskytuje v symtable

        hash++;
        if (hash == SYMTABLE_SIZE) //kruhove pole
            hash = 0;
    }

    if ((*table)[hash] != NULL) {
        if((*table)[hash]->id != NULL){
            free((*table)[hash]->id);
            (*table)[hash]->id = NULL;
        }
        free((*table)[hash]);
        (*table)[hash] = NULL;
    }

    if (tombstone != NULL)
        tombstone[hash] = 1;

    else
        return -1; //chybí pomocné pole
    
     printf("odstranuju: %li\n", hash);

    return 1;
}

Symbol *Search(SymTable *table, char *str){
    if (table == NULL || str == NULL)
        return  NULL; //error

    unsigned long hash = HashFunction(str);
    if ((*table)[hash] == NULL)
            return NULL; //retezec neni v tabulce

    while(strcmp((*table)[hash]->id, str)){
        if ((*table)[hash] == NULL && !tombstone[hash])
            return NULL; //error, symbol se nevyskytuje v symtable

        if (hash == HashFunction(str)-1)
            return NULL; // error, udelalo to uz kolo a nenaslo to symbol    

        hash++;
        if (hash == SYMTABLE_SIZE) //kruhove pole
            hash = 0;
    }

    return (*table)[hash];
}

int AddVarDetails(SymTable *table, char *str, DataType type, bool init, VarOrLet vol){
    if (table == NULL || str == NULL)
        return  -1; //error
    unsigned long hash = HashFunction(str);
    if ((*table)[hash] == NULL)
            return -1; //retezec neni v tabulce

        while(strcmp((*table)[hash]->id, str)){
            if ((*table)[hash] == NULL && !tombstone[hash])
                return -1; //error, symbol se nevyskytuje v symtable

            if (hash == HashFunction(str)-1)
                return -1; // error, udelalo to uz kolo a nenaslo to symbol    

            hash++;
            if (hash == SYMTABLE_SIZE) //kruhove pole
                hash = 0;
    }
    (*table)[hash]->type = 0;
    (*table)[hash]->variable.init = init;
    (*table)[hash]->variable.datatype = type;
    (*table)[hash]->variable.VoL = vol;

    return 1;
}

int AddFunctionDetails(SymTable *table, char *str, DataType returnType, bool defined){
     if (table == NULL || str == NULL)
        return  -1; //error
    unsigned long hash = HashFunction(str);
    if ((*table)[hash] == NULL)
            return -1; //retezec neni v tabulce

        while(strcmp((*table)[hash]->id, str)){
            if ((*table)[hash] == NULL && !tombstone[hash])
                return -1; //error, symbol se nevyskytuje v symtable

            if (hash == HashFunction(str)-1)
                return -1; // error, udelalo to uz kolo a nenaslo to symbol    

            hash++;
            if (hash == SYMTABLE_SIZE) //kruhove pole
                hash = 0;
    }
    (*table)[hash]->type = 1;
    (*table)[hash]->function.defined = defined;
    (*table)[hash]->function.ReturnType = returnType;
    //parametry fce...
    return 1;
}

void SymTableFree(SymTable *table){
     if(tombstone != NULL)
        free(tombstone);
     for (int i = 0; i < SYMTABLE_SIZE; i++) {
     if ((*table)[i] != NULL) {
        if((*table)[i]->id != NULL)
            free((*table)[i]->id);
        free((*table)[i]);
        }
     }
}

int main(){
SymTable table;
SymTableInit(&table);
InsertSymbol(&table, "pole");
if (InsertSymbol(&table, "pole") == -1)
    printf("nepovedlo se\n");

Symbol *symbol;
InsertSymbol(&table, "poal");
AddVarDetails(&table, "pole", 2, false, 1);
InsertSymbol(&table, "po");
RemoveSymbol(&table, "ptroal");
InsertSymbol(&table, "popal");
InsertSymbol(&table, "pollal");
symbol = Search(&table, "pole");
if (symbol!=NULL){
    printf("string: %s\n", symbol->id);
    printf("init? %d\n", symbol->variable.init);
    printf("datatype %d\n", symbol->variable.datatype);
    printf("VAR or LET %d\n", symbol->variable.VoL);
}
SymTableFree(&table);

return 0;
}
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

    if (parametr_init(&temp->parametr) == -1)
        return -1;

    strcpy(temp->id, str);
    (*table)[hash] = temp;
    printf("Pridavam: %li\n", hash);
    tombstone[hash] = 0;

    return 1;
}


int RemoveSymbol(SymTable *table, char *str){
    int hash = Searching(table, str);
    if(hash == -1)
        return -1; //nenašlo to

    if ((*table)[hash] != NULL) {
        if((*table)[hash]->id != NULL){
            free((*table)[hash]->id);
            (*table)[hash]->id = NULL;
        }

        clear_parametr(&(*table)[hash]->parametr);
        parametr_free(&(*table)[hash]->parametr);

        free((*table)[hash]);
        (*table)[hash] = NULL;
    }

    if (tombstone != NULL)
        tombstone[hash] = 1;

    else
        return -1; //chybí pomocné pole
    
     printf("odstranuju: %d\n", hash);

    return 1;
}

Symbol *GetSymbol(SymTable *table, char *str){
    int hash = Searching(table, str);
    if(hash != -1)
        return (*table)[hash];
    return NULL;
}

int AddVarDetails(SymTable *table, char *str, DataType type, bool init, VarOrLet vol){
    int hash = Searching(table, str);
    if(hash == -1)
        return -1; //nenašlo to
    (*table)[hash]->type = 0;
    (*table)[hash]->variable.init = init;
    (*table)[hash]->variable.datatype = type;
    (*table)[hash]->variable.VoL = vol;

    return 1;
}

int Searching(SymTable *table, char *str){ // funkce vracejici hash, aby se to neopakovalo v kazde funkci
    if (table == NULL || str == NULL)
        return  -1; //error1

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
    return hash;

}

int AddFunctionDetails(SymTable *table, char *str, DataType returnType, bool defined){
    int hash = Searching(table, str);
    if(hash == -1)
        return -1; //nenašlo to

    (*table)[hash]->type = 1;
    (*table)[hash]->function.defined = defined;
    (*table)[hash]->function.ReturnType = returnType;
    //parametry fce...
    return 1;
}

int AddParametr(SymTable *table, char *str, char c){
    int hash = Searching(table, str);
    if(hash == -1)
        return -1; //nenašlo to
    if (add_parametr(&(*table)[hash]->parametr, c) == -1)
        return -1;
    return 1;
}

void SymTableFree(SymTable *table){
     if(tombstone != NULL)
        free(tombstone);
     for (int i = 0; i < SYMTABLE_SIZE; i++) {
     if ((*table)[i] != NULL) {
        if((*table)[i]->id != NULL)
            free((*table)[i]->id);
        clear_parametr(&(*table)[i]->parametr);
        parametr_free(&(*table)[i]->parametr);
        free((*table)[i]);
        }
     }
}

// int main(){
// SymTable table;
// SymTableInit(&table);
// InsertSymbol(&table, "pole");
// if (InsertSymbol(&table, "pole") == -1)
//     printf("nepovedlo se\n");

// Symbol *symbol;
// InsertSymbol(&table, "poal");
// InsertSymbol(&table, "po");
// RemoveSymbol(&table, "pole");
// InsertSymbol(&table, "popal");
// if (AddVarDetails(&table, "popal", 2, true, 1) == -1)
//     printf("Neexistuje \n");
// InsertSymbol(&table, "pollal");
// AddParametr(&table, "popal", 'd');
// AddParametr(&table, "popal", 'i');
// symbol = GetSymbol(&table, "popal");
// AddParametr(&table, "popal", 'p');
// if (symbol!=NULL){
//     printf("string: %s\n", symbol->id);
//     printf("init? %d\n", symbol->variable.init);
//     printf("datatype %d\n", symbol->variable.datatype);
//     printf("VAR or LET %d\n", symbol->variable.VoL);
//     printf("PArametr %s\n", symbol->parametr.str);
// }
// SymTableFree(&table);

// return 0;
// }
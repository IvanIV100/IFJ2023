#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parametrs.h"

#define SYMTABLE_SIZE 9973


/*
//data_types
typedef enum{
NIL,
INT,
STR,
BOOL,
FLOAT,
VOID,
}DataType;*/

typedef enum{
variable,    
function,
nothing,
} Type;

typedef enum{
var,
let,
}VarOrLet; // jiny nazev

typedef struct{
    DataType ReturnType; //enum od 1 do 5
    bool defined; // byla definována
    Parametr *parametr; // ukazatel na parametr
    /*typedef struct parametr{ v parametrs.h
        char *id; identifikator parametru
        char *name; jmeno parametru
        int length[2]; promenne na alokovani pameti
        int alloc_size[2]; promenne na alokovani pameti
        DataType type; datovy parametr
        struct parametr *next; ukazatel na dalsi parametr
} Parametr;*/
    int parametr_count;
} Function;

typedef struct{
DataType datatype; //od 0 do 4
bool init;
VarOrLet VoL; 
int intVal;       //hodnoty    
double doubleVal; //
char* strVal;     //
bool boolVal;     //
int nillable;
} Variable;

typedef struct{
    char *id;    // nazev
    Type type;   // funkce/promenna
    Function function; // struktura s informacemi o funkci
    Variable variable; // struktura s informacemi o promenné
}Symbol;

typedef Symbol* SymTable[SYMTABLE_SIZE]; // tabulka symbolu

void SymTableInit(SymTable **table);
/*      Symtable *table = NULL;
        SymTableInit(&table);*/

void SymTableFree(SymTable *table);
//SymTableFree(&(*table));

int InsertSymbol(SymTable *table, char *key);

int AddParametr(SymTable *table, char *key, char *name, char *id, DataType type);

int AddFunctionDetails(SymTable *table, char *key, DataType returnType, bool defined);

int AddVarDetails(SymTable *table, char *key, DataType type, bool init, VarOrLet vol);

Symbol *GetSymbol(SymTable *table, char *key);

int Searching(SymTable *table, char *key);

int RemoveSymbol(SymTable *table, char *key);

unsigned long HashFunction(char *key);

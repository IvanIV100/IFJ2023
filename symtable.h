#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parametrs.h"

#define SYMTABLE_SIZE 9973



//data_types
typedef enum{
VOID,
NIL,
BOOL,
INT,
INTQ,
STR,
STRQ,
FLOAT,
FLOATQ

}DataType;

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
    //parametry
    bool defined; // byla definována
    //navěstí, interní reprezentace idk
} Function;

typedef struct{
DataType datatype; //od 0 do 8
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
    Parametr parametr; // udelat pres pointery
}Symbol;

typedef Symbol* SymTable[SYMTABLE_SIZE]; // tabulka symbolu

void SymTableInit(SymTable **table);
/*      Symtable *table = NULL;
        SymTableInit(&table);*/

void SymTableFree(SymTable *table);
//SymTableFree(&(*table));

int InsertSymbol(SymTable *table, char *key);

int AddParametr(SymTable *table, char *key, char *name, char type);
int AddParametrID(SymTable *table, char *key, char *id);

int AddFunctionDetails(SymTable *table, char *key, DataType returnType, bool defined);

int AddVarDetails(SymTable *table, char *key, DataType type, bool init, VarOrLet vol);

Symbol *GetSymbol(SymTable *table, char *key);

int Searching(SymTable *table, char *key);

int RemoveSymbol(SymTable *table, char *key);

unsigned long HashFunction(char *key);

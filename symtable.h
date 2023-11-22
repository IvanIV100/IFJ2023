#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parametrs.h"

#define SYMTABLE_SIZE 9973



//data_types
typedef enum{
NIL,
INT,
STR,
BOOL,
FLOAT,
VOID,
}DataType;

typedef enum{
variable,    
function,
}Type;

typedef enum{
var,
let,
}VarOrLet; // jiny nazev

typedef struct{
    DataType ReturnType; //enum od 1 do 5
    //parametry
    bool defined; // byla definována
    //navěstí, interní reprezentace idk
}Function;

typedef struct{
DataType datatype; //od 0 do 4
bool init;
VarOrLet VoL; 
}Variable;

typedef struct{
    char *id;    // nazev
    Type type;   // funkce/promenna
    Function function; // struktura s informacemi o funkci
    Variable variable; // struktura s informacemi o promenné
    Parametr parametr;
}Symbol;

typedef struct Symbol_node_t
{
    Symbol token;
    bool deleted;
}Symbol_node;


typedef struct symtable_t
{
    Symbol_node* symtb_arr;
    int size;
    int capacity;
    int local_level;
}SymTable;

typedef Symbol* SymTable[SYMTABLE_SIZE]; // tabulka symbolu

void SymTableInit(SymTable *table);

void SymTableFree(SymTable *table);

int InsertSymbol(SymTable *table, char *str);

int AddParametr(SymTable *table, char *str, char c);

int AddFunctionDetails(SymTable *table, char *str, DataType returnType, bool defined);

int AddVarDetails(SymTable *table, char *str, DataType type, bool init, VarOrLet vol);

Symbol *GetSymbol(SymTable *table, char *str);

int Searching(SymTable *table, char *str);

int RemoveSymbol(SymTable *table, char *str);

unsigned long HashFunction(char *str);

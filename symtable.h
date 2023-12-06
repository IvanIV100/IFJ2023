#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parametrs.h"

#define SYMTABLE_SIZE 9973


/*
//data_types ted v parametrs.h
typedef enum{
VOID,
BOOL,
NIL,
INT,
INTQ,
STR,
STRQ,
FLOAT,
FLOATQ

}DataType;*/

typedef enum{
variable,    
function,
nothing,
} Type;

typedef enum{
var,
let,
}VarOrLet;

typedef struct{
    DataType ReturnType;    //enum od 1 do 5
    bool defined;           // byla definována
    Parametr *parametr;     // ukazatel na parametr

    /*typedef struct parametr{      //v parametrs.h
        char *id;                   //identifikator parametru
        char *name;                 //jmeno parametru
        int length[2];              //promenne na alokovani pameti
        int alloc_size[2];          //promenne na alokovani pameti
        DataType type;              //datovy typ parametru
        struct parametr *next;      //ukazatel na dalsi parametr
    }Parametr;*/

    int parametr_count;             //pocet parametru
} Function;

typedef struct{
DataType datatype; //od 0 do 4
bool init;
VarOrLet VoL;
bool parametr; 
int intVal;       //hodnoty    
double doubleVal; //
char* strVal;     //
bool boolVal;     //
int nillable;
} Variable;

typedef struct{
    char *id;            // nazev
    Type type;           // funkce/promenna
    Function function;   // struktura s informacemi o funkci
    Variable variable;   // struktura s informacemi o promenné
}Symbol;

typedef Symbol* SymTable[SYMTABLE_SIZE]; // tabulka symbolu

void SymTableInit(SymTable **table);
//inicializace tabulky
/*      Symtable *table = NULL; //pouiziti
        SymTableInit(&table);*/

void SymTableFree(SymTable *table);
//uvolneni tabulky z pameti
//SymTableFree(&(*table));

int InsertSymbol(SymTable *table, char *key);
//vkladani symbolu do tabulky

int AddParametr(SymTable *table, char *key, char *name, char *id, DataType type);
//pridavani parametru do funkce

int AddFunctionDetails(SymTable *table, char *key, DataType returnType, bool defined);
//co je to za funkci

int AddVarDetails(SymTable *table, char *key, DataType type, bool init, VarOrLet vol);
//co je to za proměnnou

Symbol *GetSymbol(SymTable *table, char *key);
//vraci symbol

int Searching(SymTable *table, char *key);
// vrací hash hledaneho klíče

int RemoveSymbol(SymTable *table, char *key);
//odstraneni symbolu z tabulky

unsigned long HashFunction(char *key);
//vrací hash hodnotu

int insert_int_value(SymTable *table, char *key, int value);
//vkládání hodnoty int

int insert_double_value(SymTable *table, char *key, double value);
//hodnota double

int insert_string_value(SymTable *table, char *key, char *value);
//hodnota string

int insert_bool_value(SymTable *table, char *key, bool value);
//hodnota boolu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parametrs.h"

#define SYMTABLE_SIZE 9973

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
    DataType ReturnType;
    bool defined;      
    Parametr *parametr;
    int parametr_count;
} Function;

typedef struct{
DataType datatype;
bool init;
VarOrLet VoL; 
int intVal;        
double doubleVal; 
char* strVal;     
bool boolVal;     
int nillable;
} Variable;

typedef struct{
    char *id;           
    Type type;         
    Function function;  
    Variable variable; 
}Symbol;

typedef Symbol* SymTable[SYMTABLE_SIZE];

/**
 * Function which initializes SymTable.
 * @param table - SymTable.
*/
void SymTableInit(SymTable **table);

/**
 * Function which frees SymTable.
 * @param table - SymTable. 
*/
void SymTableFree(SymTable *table);

/**
 * Function to insert a symbol to the SymTable.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @return 1 if it is succesfull, otherwise returns -1.
*/
int InsertSymbol(SymTable *table, char *key);

/**
 * Function to add parameter to a function.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param name - name of the parameter.
 * @param id - id of the parameter.
 * @return 1 on success, otherwise returns -1.
*/
int AddParametr(SymTable *table, char *key, char *name, char *id, DataType type);


/**
 * Function to add attributes to a function.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param returnType - return type of the function.
 * @param defined - bool to chech if the function is defined.
 * @return 1 on success, otherwise returns -1.
 */
int AddFunctionDetails(SymTable *table, char *key, DataType returnType, bool defined);

/**
 * Function to add attributes to a function.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param type - type of hte variable.
 * @param init - bool to chech if the variable is initialized.
 * @param vol - 0 it is var, 1 it is let.
 * @return 1 on success, otherwise returns -1.
 */
int AddVarDetails(SymTable *table, char *key, DataType type, bool init, VarOrLet vol);

 /** Function which returns pointer to the symbol
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @return Pointer to the symbol.
 */
Symbol *GetSymbol(SymTable *table, char *key);

/** Function which returns index of the symbol.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @return index of the symbol, if symbol does not exist returns -1.
 */
int Searching(SymTable *table, char *key);

/** Function which removes a symbol from the table.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @return 1 on success, otherwise returns -1.
 */
int RemoveSymbol(SymTable *table, char *key);

/** Function to create a hash to a key.
 * @param key - key of the symbol.
 * @return hash.
 */
unsigned long HashFunction(char *key);

/** Function to add int value to variable.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param value - value to be inserted
 * @return 1 on success, otherwise returns -1.
 */
int insert_int_value(SymTable *table, char *key, int value);

/** Function to add double value to variable.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param value - value to be inserted
 * @return 1 on success, otherwise returns -1.
 */
int insert_double_value(SymTable *table, char *key, double value);

/** Function to add string value to variable.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param value - value to be inserted
 * @return 1 on success, otherwise returns -1.
 */
int insert_string_value(SymTable *table, char *key, char *value);

/** Function to add bool value to variable.
 * @param table - SymTable.
 * @param key - key of the symbol.
 * @param value - value to be inserted
 * @return 1 on success, otherwise returns -1.
 */
int insert_bool_value(SymTable *table, char *key, bool value);

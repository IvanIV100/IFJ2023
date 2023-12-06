/**
 * Project: Implementace překladače IFJ23
 *
 * @brief  Parametrs functions in for symtable .h file.
 *
 * @author Dominik Šagát xsagat01
 */



#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ALLOC_SIZE 8

typedef enum datatype{
VOID,
BOOL,
NIL,
INT,
INTQ,
STR,
STRQ,
FLOAT,
FLOATQ

}DataType;


typedef struct parametr{
char *id;
char *name;
int length[2];
int alloc_size[2]; 
DataType type;
struct parametr *next;
} Parametr;

/**
 * Function which initializes parameter.
 * @param p - parameter.
 * @return 1 if it is succesfull, otherwise returns -1.
*/
int parametr_init(Parametr *p);

/**
 * Function to add a name to a parameter and data type of the parameter.
 * @param p - parameter.
 * @param name - name of the parameter.
 * @param type - Data type.
 * @return 1 if it is succesfull, otherwise returns -1.
*/
int add_parametr_name(Parametr *p, char *name, DataType type);

/**
 * Function to add a id to a parameter.
 * @param p - parameter.
 * @param id - name of the parameter.
 * @return 1 if it is succesfull, otherwise returns -1.
*/
int add_parametr_id(Parametr *p, char *id);

/**
 * Function which clears parameter.
 * @param p - parameter.
 * @return 1 if it is succesfull, otherwise returns -1.
*/
int clear_parametr(Parametr *p);

/**
 * Function which frees parameter.
 * @param p - parameter.
 * @return 1 if it is succesfull, otherwise returns -1.
*/
int parametr_free(Parametr *p);



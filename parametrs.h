#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ALLOC_SIZE 8

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


typedef struct parametr{
char *id;
char *name;
int length[2]; /*proměnné na alokovaní*/
int alloc_size[2]; 
DataType type;
struct parametr *next;
} Parametr;


int parametr_init(Parametr *p);
//inicializace parametru funkce

int add_parametr_name(Parametr *p, char *name, DataType type);
//pridavani jmena a typu parametru funkce

int add_parametr_id(Parametr *p, char *id);
//pridavani identifikatoru funkce

int clear_parametr(Parametr *p);
//smázaní hodnoty parametru

int parametr_free(Parametr *p);
//uvolneni hodnoty parametru z pameti


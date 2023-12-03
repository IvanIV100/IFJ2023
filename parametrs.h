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
int length[2];
int alloc_size[2];
DataType type;
struct parametr *next;
} Parametr;


int parametr_init(Parametr *p);

int add_parametr_name(Parametr *p, char *name, DataType type);

int add_parametr_id(Parametr *p, char *id);

int clear_parametr(Parametr *p);

int parametr_free(Parametr *p);



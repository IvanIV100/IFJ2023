#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ALLOC_SIZE 8

typedef struct{
char *id;
char *name;
int count[2]; //pocet pridanych prvku
int length[2]; //kdyztak jako pole [2]
int alloc_size[2]; //same
} Parametr;

int parametr_init(Parametr *p);

int add_parametr_name(Parametr *p, char *name, char c);

int add_parametr_id(Parametr *p, char *id);

int clear_parametr(Parametr *p);

int parametr_free(Parametr *p);



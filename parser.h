#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct node_struct {
    Token current;
    struct node *left;
    struct node *right;
    
} *node_t;




void parser();

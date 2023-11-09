#include "parametrs.h"

int parametr_init(Parametr *p){
    if (p == NULL)
        return -1;
    p->str = (char *) malloc(ALLOC_SIZE);
    if(p->str == NULL)
        return -1;
    p->length = 0;
    p->str[p->length] = '\0';
    p->alloc_size = ALLOC_SIZE;
    return 1;
}

int add_parametr(Parametr *p, char c){
    if (p == NULL)
        return -1;
    if (p->length+1 == p->alloc_size){
        unsigned int newsize= p->alloc_size + ALLOC_SIZE;
        p->str = (char *) realloc(p->str, newsize);
        if (!p->str)
            return -1;
        p->alloc_size = newsize;
    }
    p->str[p->length++] = c;
    p->str[p->length] = '\0'; 
    return 1;
}

int clear_parametr(Parametr *p){
    if (p == NULL)
        return -1;
    while (--p->length != -1)
    {
        p->str[p->length] = '\0';

    }

    return 1;
}

int parametr_free(Parametr *p){
    if (p == NULL)
        return -1;
    if(p->str != NULL){
        free(p->str);
        p->str = NULL;
    }
    return 1;
}
/*
int main(){
    Parametr p;
    parametr_init(&p);
    add_parametr(&p, 'c');
    add_parametr(&p, 'w');
    add_parametr(&p, 'd');
    add_parametr(&p, 'w');
    add_parametr(&p, 'e');

    printf("alloc %d \n", p.alloc_size);
    printf("length %d \n" ,p.length);
    printf("string %s \n" ,p.str);
    clear_parametr(&p);
    parametr_free(&p);
    return 0;
}*/
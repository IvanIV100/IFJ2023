#include "parametrs.h"

int parametr_init(Parametr *p){
    if (p == NULL)
        return -1;
    p->id = (char *) malloc(ALLOC_SIZE);
    if(p->id == NULL)
        return -1;
    p->name = (char *) malloc(ALLOC_SIZE);
    if(p->name == NULL)
        return -1;
    p->length[0] = 0; //delka id
    p->length[1] = 0;//delka name
    p->id[p->length[0]] = '\0';
    p->name[p->length[1]] = '\0';
    p->alloc_size[0] = ALLOC_SIZE;
    p->alloc_size[1] = ALLOC_SIZE;
    return 1;
}

int add_parametr_name(Parametr *p, char *name, DataType type){
    if (p == NULL)
        return -1;
    if (name == NULL)
        return -1;
    int i = 0;
    while(name[i] != '\0'){
        if (p->length[1]+1 == p->alloc_size[1]){
            unsigned int newsize= p->alloc_size[1] + ALLOC_SIZE;
            p->name = (char *) realloc(p->name, newsize);
            if (!p->name)
                return -1;
            p->alloc_size[1] = newsize;
        }

        p->name[p->length[1]++] = name[i];
        i++;
    }
    p->name[p->length[1]] = '\0';
    p->type = type;

    return 1;
}

int add_parametr_id(Parametr *p, char *id){
    if (p == NULL)
        return -1;
    if (id == NULL)
        return -1;
    int i = 0;
    while(id[i] != '\0'){
        if (p->length[0]+1 == p->alloc_size[0]){
            unsigned int newsize= p->alloc_size[0] + ALLOC_SIZE;
            p->id = (char *) realloc(p->id, newsize);
            if (!p->id)
                return -1;
            p->alloc_size[0] = newsize;
        }

        p->id[p->length[0]++] = id[i];
        i++;
    }
    p->id[p->length[0]] = '\0';
    return 1;

}

int clear_parametr(Parametr *p){
    if (p == NULL)
        return -1;
    while (--p->length[0] != -1)
    {
        p->id[p->length[0]] = '\0';
    }

    while (--p->length[1] != -1)
    {
        p->name[p->length[1]] = '\0';
    }

    return 1;
}

int parametr_free(Parametr *p){
    if (p == NULL)
        return -1;
    if(p->id != NULL){
        free(p->id);
        p->id = NULL;
    }

    if(p->name != NULL){
        free(p->name);
        p->name = NULL;
    }

    return 1;
}
/*
int main(){
    Parametr p;
    parametr_init(&p);
    add_parametr_name(&p, "jmeno",4);
    //add_parametr_name(&p, "int",4);
    add_parametr_id(&p, "jmeno");
    //add_parametr_id(&p, "var");

    printf("alloc %d \n", p.alloc_size[1]);
    printf("length %d \n" ,p.length[1]);
    printf("string %s \n" ,p.name);
    printf("string %s \n" ,p.id);
    printf("int %i \n" , p.type);
    clear_parametr(&p);
    parametr_free(&p);
    return 0;
}*/



typedef struct{
char *str;
int length;
int alloc_size;
} Parametr;

int parametr_init(Parametr *p);

int add_parametr(Parametr *p, char c);

int clear_parametr(Parametr *p);

int parametr_free(Parametr *p);



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"


/*
Things to do:
1) kdyz nedostane nic na vstup je to nekonecne zacikleny a ceka na EOF coz mu nikdy neprijde (timhle se nemysli prazdny file, ale proste spusteno ./scanner )
2) ID muzou byt delsi nez 256 znaku a Cislo delsi nez 256

4) Kdyz nastane Error tak to upravit at se stane to co se stat ma (mainly malloc)



*/


// funkce ktera zjisti jestli dalsi character je whiteSpace or nah
int isWhiteChar(char input){
    if (input == ' ' || input == '\t' || input == '\n' || (input >= 11 && input <= 13))
        return 1;
    else
        return 0;
}

// Ziska char ktery neni Whitespace
char getNotWhiteChar() {
    char temp = getchar();
    if (isWhiteChar(temp)) {
        return getNotWhiteChar();
    } 
    else 
        return temp;
}

// Skip comment
int SkipComment() {
    char next = getchar();
    if (next == '/') {      // normal comment        
        while (next != '\n' && next != EOF) {  
            next = getchar();
        }   
    } 
    else if (next == '*') {// Multi-line comment
        int nest = 1; 
        while (nest > 0) {
            char curr = getchar();

            if (curr == '*') {
                next = getchar();
                if (next == '/') {
                    nest--;
                }
            } 

            else if (curr == '/') {
                next = getchar();
                if (next == '*') {
                    nest++;
                }
            } 

            else if (curr == EOF) {
                return T_ERORR; 
            }
        }
    } 
    else{ 
        ungetc(next, stdin);
        return 1; 
    }
    return 0; 
}

char *keywords[] = {"Double", "Int","String", "else", "func", "if",  "let", "nil", "return", "var", "while"};
enum token_type keyword_types[] = {
    T_KW_DOUBLE, T_KW_INT, T_KW_STRING, T_ELSE, T_FUNC, T_IF, T_LET, T_NIL, T_RETURN, T_VAR, T_WHILE
};

// vytvoreni tokenu
Token* createToken(enum token_type type, enum token_Category category) {
    Token* token = malloc(sizeof(Token)); 
    if (token == NULL) {
        fprintf(stderr, "Malloc error for token.\n");
        exit(EXIT_FAILURE);
    }

    token->type = type;
    token->Category = category;

    return token;
}

// zjistime ci je to KeyWord nebo token
int isKeyword(char *str) {
    for(int i = 0; i < 11; i++) {
        if(strcmp(keywords[i], str) == 0) {
            return i;
        }
    }
    return -1;
}

// Divame jestli je to ID
Token* is_Id(char curr) { 
                            

    char identifier[256];                               // max delka id 256
    int i = 0;
    identifier[i] = curr;                               // prvni pismeno uz vime
    i++;

    curr = getchar();
    while (isalnum(curr) || curr == '_') {          // nacteme vse co tam patri bud a-z A-Z _ 0-9
        identifier[i] = curr;
        i++;
        curr = getchar();
    }

    identifier[i] = '\0';                           // konec seznamu

    int keywordi = isKeyword(identifier);       // pro kontrolu jestli to neni kw


    // nillable muze se dat jincimu tokenu
    if (keywordi >= 0) {
        Token* token = createToken(keyword_types[keywordi], TC_KEYWORDS);
        if (keywordi < 3) {  // If it's Int, String, or Double
            token->Category = TC_TYPE;
            token->value.nillable = 0;

            if (curr == '?' && token->Category == TC_TYPE) {
                token->value.nillable = 1;
            } else {
                ungetc(curr, stdin);
            }
        }
        return token;
    }

    else { 
        Token* token = createToken(T_IDENTIFIER, TC_ID);
        token->value.ID_name = malloc(strlen(identifier) + 1);
        if (token->value.ID_name == NULL) {
            fprintf(stderr, "Malloc error for token.\n");
            exit(EXIT_FAILURE);
        }          
        strcpy(token->value.ID_name, identifier);                    
        token->value.nillable=0;
        if (curr=='!')
            token->value.nillable=2;
        else
            ungetc(curr, stdin);
        return token;
    }
}


// nacte cislo
Token* scanNumber(char curr) {
    char numbers[256];              // Max 256 dlouhy cislo ?
    int i = 0;
    numbers[i] = curr;
    i++;

    curr = getchar();
    while (isdigit(curr)) {
        numbers[i] = curr;
        i++;
        curr = getchar();
    }

    if (curr == '.') {                  // Desetinna cast
        numbers[i] = curr;
        i++;
        curr = getchar();
        while (isdigit(curr)) {
            numbers[i] = curr;
            i++;
            curr = getchar();
        }
    }

    
    if (curr == 'e' || curr == 'E') {   // Exponentni cast Muze to byt jen kdyz je to exponent 
        numbers[i] = curr;
        i++;
        curr = getchar();
        
        if (curr == '+' || curr == '-') {
            numbers[i] = curr;
            i++;
            curr = getchar();
        }

        while (isdigit(curr)) {
            numbers[i] = curr;
            i++;
            curr = getchar();
            }
        }
    

    numbers[i] = '\0';                      // Pridani konce seznamu

    Token* token = createToken(T_DOUBLE, TC_TYPE);

    if (strchr(numbers, '.') != NULL || strchr(numbers, 'e') != NULL || strchr(numbers, 'E') != NULL) {
        token->value.decimal = atof(numbers);    // Convert string to double
        
    } 
    
    else {
        token->type=T_INT;
        token->value.integer = atoi(numbers);     // Convert string to int
    }

    ungetc(curr, stdin); // Push back the last read character

    return token;
}





// Funkce ktera nam rozsiri dynamickou pamet Stringu dvakrat tolik
int expand_String(Token *token, int *length) {
    int newLength = (*length) * 2;  // Double the length

    char *newString = realloc(token->value.stringVal, newLength);
    if (newString == NULL) {
        return 0; // Reallocation failed
    }

    token->value.stringVal = newString;
    *length = newLength;
    return 1; // Reallocation successful
}

// Prida character do retezce                                                   !! jeste overit jestli ok
int addChar(char curr,int i, Token *token){
    token->value.stringVal[i]=curr;
    return 1;
    }


// Dostali jsme \{ coz znamena Unixovy kod !! nemam presne moc tuseni jak to funguje yoinkl jsem to z StackFlow a implementoval na moje reseni 
int unicode(int i, Token *token) {          // over returny kdyz indikejtnou errory
    // ocekavany format \u{XX}
    if (getchar() != '{') {
        return 0;
    }

    // heximablabla cislo (fucking hate that word dont know how to even type that)
    unsigned int unicodeValue = 0;
    for (int j = 0; j < 2; j++) {
        char next = getchar();
        if ((next >= '0' && next <= '9') || (next >= 'A' && next <= 'F') || (next >= 'a' && next <= 'f')) {
            unicodeValue = unicodeValue * 16 + strtol(&next, NULL, 16);
        } 
        else {
            return 0;
        }
    }

    if (getchar() != '}') {     // pokud to neni ukoncene timhle err
        return 0;
    }
    
    token->value.stringVal[i] = (char) unicodeValue;
    return 1;
}

// Charactery ktere zacinaji \ v zadani jsou jen tyhle dany jako moznosti podivat jestli nejsou dalsi
int escape_Char(Token *token,int i){
    char next;
    next=getchar();
    int alright = 0;
    
    switch (next) {
		case '\"':                          // tohle "  
			addChar('\"',i,token);
			return 1;
		case 'n':                           // dalsi radek
			addChar('\n',i,token);
			return 1;
		case 'r':                           // Neco sposunutim ukazatele na zacatek radku muze to pak prepisovat veci na tom radku ale to co neprepise zustane
			addChar('\r',i,token);
			return 1;
		case 't' :                          // tabulator
			addChar('\t',i,token);
			return 1;
		case '\\' :                         /* tohle \ */ 
			addChar('\\',i,token);
			return 1;
        case 'u':                           // unixocy cod
            alright = unicode(i,token);
            if (alright == 0){
                return 0; 
            }
            return 1;
            break; 
            
        default:                        // Yoo zadal jsi neco co neni escape char escapni zivot 
            return 0; 
    }
}

Token* isString(char curr) {     // je to string jeste multi 
    Token* token = createToken(T_STRING, TC_VALUE);
    token->value.stringVal = malloc(30);
    if (token->value.stringVal == NULL) {
        fprintf(stderr, "Malloc error for token.\n");
        exit(EXIT_FAILURE);
    }

    int length=30;
    int i=0;
    int alright = 1;
    // prvni char
    addChar(curr,i , token);
    i++;

    curr = getchar();
    while (curr!='"') {                                     // pokud neni konec stringu
        if (i >= length - 8) {                      // reallocate memory 
            if (!expand_String(token, &length)) {
                free(token->value.stringVal);
                token->Category=TC_ERR;
                token->type=T_ERORR;
                return token;
            }
        }
        if (curr=='\\'){                        /* escape char curr == \ */ 
            alright = escape_Char(token,i);
            if (alright == 0){
                printf("Here%d",alright);
                free(token->value.stringVal);
                token->Category=TC_ERR;
                token->type=T_ERORR;
                return token;
            }
        }
        else{                                   // pokud to neni escape tak jen pridej do stringval
            addChar(curr,i,token);
        }
        i++;
        curr = getchar();
    }

    token->value.stringVal[i] = '\0'; // Neco ze musim ukoncit konec stringu idk rekl kamos kdyz mi pomahal s pameti
    return token;
}

Token* isMultiLineString() {
    Token* token = createToken(T_STRING, TC_VALUE);
    token->value.stringVal = malloc(100);
    if (token->value.stringVal == NULL) {
        fprintf(stderr, "Malloc error for token.\n");
        exit(EXIT_FAILURE);
    }
    int alright = 1;          
    int length = 100;
    int i = 0;

    char curr = getchar();

    while (curr != EOF) {
        if (i >= length - 8) {  
            if (!expand_String(token, &length)) {
                token->Category = TC_ERR;
                token->type = T_ERORR;
                free(token->value.stringVal);
                return token;
            }
        }
        if (curr == '\\') {
            alright = escape_Char(token,i);
            if (alright == 0){
                printf("Here");
                free(token->value.stringVal);
                token->Category=TC_ERR;
                token->type=T_ERORR;
                return token;
            }
            i++;  
        } 
        else {
            addChar(curr, i, token);
            i++;  
        }

        if (curr == '\"') {                 // musi to byt tri za sebou """
            char next = getchar();
            if (next == '\"') {
                next = getchar();
                if (next == '\"') {
                    token->value.stringVal[i - 3] = '\0';  
                    return token;
                } 
                else {
                    addChar(curr, i, token);  
                    ungetc(next, stdin);  
                }
            } 
            else {
                addChar(curr, i, token);  
                ungetc(next, stdin);  
            }
        }
        curr = getchar();  
    }

    token->Category = TC_ERR;
    token->type = T_ERORR;
    free(token->value.stringVal);
    return token;
}


void free_token_Values(Token *token){       // funkce ktera uvolni pamet kterou jsem mallocoval 
    if (token->type==T_IDENTIFIER){
        free(token->value.ID_name);
        token->value.ID_name = NULL;
    }
    else if(token->type==T_STRING){
        free(token->value.stringVal);
        token->value.stringVal = NULL;
    }

    free(token);
    token = NULL;
}

Token* scan() {                             // proste GetToken da ti dasli Token asi prejmenuji whatever
    char curr = getNotWhiteChar();         // next slouzi jako takovy idiot ktery se diva do predu
    char next = curr;

    if (curr!='/')                          // debug
        //printf("%c",curr);

    //if current >= a || <= z || >= A || <= Z -> curr = letter
    switch (curr) {
        case '/':
            if (SkipComment() == 1) {      
                return createToken(T_DIV,TC_OPERATORS);
            } 
            else {
                return scan(); // Recursively call scan() to get the next token
            }

        case '(':
            return createToken(T_LEFT_PAREN, TC_BRACKET);

        case ')':
            return createToken(T_RIGHT_PAREN,TC_BRACKET);

        case '{':
            return createToken(T_LEFT_BRACE, TC_BRACKET);

        case '}':
            return createToken(T_RIGHT_BRACE, TC_BRACKET);

        case '[':
            return createToken(T_LEFT_BRACKET, TC_BRACKET);

        case ']':
            return createToken(T_RIGHT_BRACKET, TC_BRACKET);



        case '=':
            next = getchar();
            if (next == '=') {                                          // podivam se na dalsi char jestli to nahodou neni == (equal)
                return createToken(T_EQUALS, TC_OPERATORS);
            } 
            else {                                                         // byla tam nejaka mrdka takze no equal
                ungetc(next, stdin);
                return createToken(T_ASSIGN, TC_OPERATORS);
            }

        case '+':
            return createToken(T_PLUS, TC_OPERATORS);

        case '-':                                                   // princip znas
            next = getchar();
            if (next == '>') {
                return createToken(T_ARROW, TC_Punctation);
            } 
            else {
                ungetc(next, stdin);
                return createToken(T_MINUS, TC_OPERATORS);
            }

        case '*':
            return createToken(T_MUL, TC_OPERATORS);

        case '!':                                                   // princip znasd
            next = getchar();
            if (next == '=') {
                return createToken(T_NOT_EQUALS, TC_LOGICAL);
            }
            else {
                ungetc(next, stdin);
                return createToken(T_ERORR, TC_ERR);
            }

        case '<':
            next = getchar();
            if (next == '=') {
                return createToken(T_LESS_EQUAL, TC_LOGICAL);
            } 
            else {
                ungetc(next, stdin);
                return createToken(T_LESS_THAN, TC_LOGICAL);
            }

        case '>':
            next = getchar();
            if (next == '=') {
                return createToken(T_GREATER_EQUAL, TC_LOGICAL);
            } 
            else {
                ungetc(next, stdin);
                return createToken(T_GREATER, TC_LOGICAL);
            }

        case ',':
            return createToken(T_COMMA, TC_Punctation);

        case ':':
            return createToken(T_COLON, TC_Punctation);

        case '?':
            next = getchar();
            if (next == '?') {
                return createToken(T_DOUBLE_QUESTION_MARK, TC_Punctation);
            } 
            else {
                ungetc(next, stdin);
                return createToken(T_ERORR, TC_ERR);
            }



        case '_':                                       // muze to byt bud ID a nebo to muze stat samo o sobe a neco delat uz jsem zapomnel teehee :3
            next = getchar();
            if (isalnum(next) || next=='_') {
                ungetc(next, stdin);
                return(is_Id(curr));                    // tady to muze byt jen Id ale posilam to do funkce ktera rozhoduje mezi ID a KW mala ztrata casu but whatecer
            }

            else {
                ungetc(next, stdin);
                return createToken(T_UNDERSCORE, TC_Punctation);
            }
    

        case 'A' ... 'Z':                                       
        case 'a' ... 'z':
            return(is_Id(curr));               // Identifikator nebo keyword
        


        case '"':                           // hate this String
            next = getchar();
            if (next == '"'){               // pokud dalsi tak to bude ""
                next = getchar();
                if(next =='"'){             // pokud dalsi tak to bude """ ci multi string to funkci dodelam
                    return isMultiLineString();
                }
                else{                       // je to jen "" takze prazdny retezec 
                    ungetc(next,stdin);
                    return createToken(T_STRING,TC_VALUE);                  // vloz tam prazdny string
                }                                                // takze kdyz se do ni bude chtit podivat tak error asi nejak jinak udelat
                
            }
            else{                           // je to normal string
                ungetc(next,stdin);
                return isString(curr);
            }

        case '0' ... '9':                  // budd Double nebo Inter
            return (scanNumber(curr));

        // EOF je broken
        case EOF:                           
            return createToken(T_EOF, TC_ERR);

        default:                            // neco co tam nema byt mozna errorum prirad Value at vis jaky presne ERROORORRO
            //printf("2.%c",curr);
            return createToken(T_ERORR, TC_ERR);
    }
    return NULL;
}



// int main() {                                    // best debuging ever cant change my mind
//     Token* xd;
//     xd=scan();
//     while(xd->Category!=TC_ERR){
//         free_token_Values(xd);
//         xd=scan();
//         printf("--%d\n",xd->type);
//         if(xd->type==T_STRING){
//             printf("xx %s xx\n",xd->value.stringVal);
//         }
//         else if(xd->type==T_MULTILINE_STRING){
//             printf("xx %s xx\n",xd->value.stringVal);
//         }
//     }
//     free_token_Values(xd);
    
    /*
    while(xd->Category!=TC_ERR){
        free_token_Values(xd);
        xd=scan();
        printf("--%d\n",xd->type);
        if (xd->type==T_INT)
            printf("%d\n",xd->value.integer);
        else if(xd->type==T_DOUBLE)
            printf("%f\n",xd->value.decimal);
        else if(xd->type==T_STRING){
            printf("xx %s xx\n",xd->value.stringVal);
        }
        else if(xd->type==T_MULTILINE_STRING){
            printf("xx %s xx\n",xd->value.stringVal);
        }
        else if(xd->type==T_IDENTIFIER){
            printf("%s",xd->value.ID_name);
        }
    }
    free_token_Values(xd);
    return 0;*/
//}



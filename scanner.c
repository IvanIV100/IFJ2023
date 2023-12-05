#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"


/*
Things to do:


*/


// Function that determines whether the next character is white space or not
int isWhiteChar(char input){
    if (input == ' ' || input == '\t' || input == '\n' || (input >= 11 && input <= 13))
        return 1;
    else
        return 0;
}

// Gets a character that is not a white space.
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
                return -1; 
            }
        }
    } 
    else{ 
        ungetc(next, stdin);
        return 1; 
    }
    return 0; 
}

// lists to determine if it is a keyword or not
char *keywords[] = {"Double", "Int","String", "else", "func", "if",  "let", "nil", "return", "var", "while"};
enum token_type keyword_types[] = {
    T_KW_DOUBLE, T_KW_INT, T_KW_STRING, T_ELSE, T_FUNC, T_IF, T_LET, T_NIL, T_RETURN, T_VAR, T_WHILE};

// Token creation
Token* createToken(enum token_type type, enum token_Category category) {
    Token* token = malloc(sizeof(Token)); 
    if (token == NULL) {
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.integer=99;
    }

    token->type = type;
    token->Category = category;

    return token;
}

// Check if it is a keyword
int isKeyword(char *str) {
    for(int i = 0; i < 11; i++) {
        if(strcmp(keywords[i], str) == 0) {
            return i;
        }
    }
    return -1;
}

// Check if it is an identifier
Token* is_Id(char curr) {
    Token* token = createToken(T_IDENTIFIER, TC_ID);
    int Id_Length=50;
    token->value.ID_name = malloc(sizeof(char) * Id_Length);

    if (token->value.ID_name == NULL) {
        free(token->value.ID_name);
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.integer=99;
        return token;
    }

    int i = 0;
    token->value.ID_name[i] = curr; // First character of the identifier
    i++;

    curr = getchar();
    while (isalnum(curr) || curr == '_') {
        // Check if more memory is needed
        if (i >= Id_Length - 5) { // -1 to leave room for the null terminator
            char *temp = realloc(token->value.ID_name, sizeof(char) * (Id_Length * 2));
            Id_Length = Id_Length * 2; 
            if (temp == NULL) {
                free(token->value.ID_name);
                token->Category = TC_ERR;
                token->type = T_ERORR;
                token->value.integer=99;
                return token;
            }
            token->value.ID_name = temp;
        }

        token->value.ID_name[i] = curr;
        i++;
        curr = getchar();
    }

    token->value.ID_name[i] = '\0'; // Null-terminate the identifier

    int keywordi = isKeyword(token->value.ID_name);

    if (keywordi >= 0) {
        token->type=keyword_types[keywordi];
        token->Category=TC_KEYWORDS;
        
        if (keywordi < 3) {  // If it's Int, String, or Double
            token->Category = TC_TYPE;
            token->value.nillable = 0;

            if (curr == '?' && token->Category == TC_TYPE) {
                token->value.nillable = 1;
                free(token->value.ID_name);
                return token;
            } 
        }
        ungetc(curr, stdin);
        free(token->value.ID_name);
        return token;
    } 
    
    else {
        token->value.nillable = 0;
        if (curr == '!') {
            token->value.nillable = 2;
        } else {
            ungetc(curr, stdin);
        }
        return token;
    }
}





// Expand the string value
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

// Add a character to the string value                                           
int addChar(char curr,int i, Token *token){
    token->value.stringVal[i]=curr;
    return 1;
    }


// We need to convert the unicode to a character
int unicode(int i, Token *token) {          
    int len = 0;
    if (getchar() != '{') {
        return 0;
    }

    // Hexadecimal number
    unsigned int unicodeValue = 0;
    for (int j = 0; j < 8; j++) {
        len++;
        char next = getchar();
        if ((next >= '0' && next <= '9') || (next >= 'A' && next <= 'F') || (next >= 'a' && next <= 'f')) {
            unicodeValue = unicodeValue * 16 + strtol(&next, NULL, 16); // Convert the hex to decimal
        }
        else if(next == '}' && len > 1){
            token->value.stringVal[i] = (char) unicodeValue;
            return 1;
        } 
        else {
            return 0;
        }
    }

    if (getchar() != '}') {     // if not } then error
        return 0;
    }
    
    token->value.stringVal[i] = (char) unicodeValue;
    return 1;
}

// Escape characters
int escape_Char(Token *token,int i){
    char next;
    next=getchar();
    int alright = 0;
    switch (next) {
		case '\"':                          // this "  
			addChar('\"',i,token);
			return 1;
		case 'n':                           // new line
			addChar('\n',i,token);
            return 1;
		case 'r':                           // dont know what is this
			addChar('\r',i,token);
            return 1;
		case 't' :                          // tab
            addChar('\t',i,token);
			return 1;
		case '\\' :                       
            addChar('\\',i,token);
			return 1;
        case 'u':                           // unix code
            alright = unicode(i,token);
            if (alright == 0){
                return 0; 
            }
            return 1;
            break; 
            
        default:                        
        ungetc(next,stdin);
        return 0; 
    }
}

Token* isString(char curr) {     
    Token* token = createToken(T_STRING, TC_VALUE);
    token->value.stringVal = malloc(30);

    if (token->value.stringVal == NULL) {
        free(token->value.stringVal);
        token->value.stringVal = NULL;
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.integer=99;
    }

    int length=30;
    int i=0;
    int alright = 1;
    // prvni char
    addChar(curr,i , token);
    i++;

    curr = getchar();
    while (curr!='"') {                                     // not end of string
        if (i >= length - 8) {                      // reallocate memory 
            if (!expand_String(token, &length)) {
                free(token->value.stringVal);
                token->Category=TC_ERR;
                token->type=T_ERORR;
                token->value.integer=99;
                return token;
            }
        }
        if (curr=='\\'){                       
            alright = escape_Char(token,i);
            if (alright == 0){
                free(token->value.stringVal);
                token->Category=TC_ERR;
                token->type=T_ERORR;
                token->value.integer=1;
                return token;
            }
            
        }
        else if (curr == EOF){                 
            free(token->value.stringVal);
            token->Category=TC_ERR;
            token->type=T_ERORR;
            token->value.integer=1;
            return token; }

        else{                                   // If it's not an escape, simply add it to the string value.
            addChar(curr,i,token);
        }
        i++;
        curr = getchar();
    }

    token->value.stringVal[i] = '\0'; // Null-terminate the string
    return token;
}

Token* isMultiLineString() {
    Token* token = createToken(T_STRING, TC_VALUE);
    token->value.stringVal = malloc(100);
    if (token->value.stringVal == NULL) {
        free(token->value.stringVal);
        token->value.stringVal = NULL;
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.integer=99;
    }
    int alright = 1;          
    int length = 100;
    int i = 0;
    char curr = getchar();
    if (curr != '\n'){                // Incorrectly written multiline string """ must be on a separate line.
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.integer=1;
        free(token->value.stringVal);
        return token;
    }
    curr = getchar();                   // The first \n following """ should be ignored.
    while (curr != EOF) {
        if (i >= length - 8) {  
            if (!expand_String(token, &length)) {
                token->Category = TC_ERR;
                token->type = T_ERORR;
                token->value.integer=99;
                free(token->value.stringVal);
                return token;
            }
        }
        if (curr == '\\') {
            alright = escape_Char(token,i);
            if (alright == 0){
                token->Category = TC_ERR;
                token->type = T_ERORR;
                token->value.integer=1;
                free(token->value.stringVal);
                return token;
            }
            i++;  
        } 

        else if (curr == '\"') {                 // It must be three in a row """.
            char next = getchar();
            if (next == '\"') {
                next = getchar();
                if (next == '\"') {
                    token->value.stringVal[i-1] = '\0';  
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

        else {
            addChar(curr, i, token);
            i++;  
        }
        curr = getchar();  
    }

    token->Category = TC_ERR;
    token->type = T_ERORR;
    token->value.integer=1;
    free(token->value.stringVal);
    return token;
}

// nacte cislo
Token* scanNumber(char curr) {
    char numbers[256];              
    int i = 0;
    numbers[i] = curr;
    i++;

    curr = getchar();
    while (isdigit(curr)) {
        numbers[i] = curr;
        i++;
        curr = getchar();
    }

    if (curr == '.') {                  // Decimal part 
        numbers[i] = curr;
        i++;
        curr = getchar();

        if(!isdigit(curr) && curr!='e' && curr!='E'){
            ungetc(curr, stdin);
            Token* token = createToken(T_ERORR, TC_ERR);
            token->value.integer=1;
            return token;
        } 

        while (isdigit(curr)) {
            numbers[i] = curr;
            i++;
            curr = getchar();
        }
    }

    
    if (curr == 'e' || curr == 'E') {   // Exponentn part
        numbers[i] = curr;
        i++;
        curr = getchar();
        
        if (curr == '+' || curr == '-') {
            numbers[i] = curr;
            i++;
            curr = getchar();
        }

        if(isdigit(curr)){
            while (isdigit(curr)) {
                numbers[i] = curr;
                i++;
                curr = getchar();
            }
        }
        else{
            ungetc(curr, stdin);
            Token* token = createToken(T_ERORR, TC_ERR);
            token->value.integer=1;
            return token;
        }

        if (curr == '.') {
            ungetc(curr, stdin);
            Token* token = createToken(T_ERORR, TC_ERR);
            token->value.integer=1;
            return token;
        }


    }
    

    numbers[i] = '\0';                     // Adding the end of the list.

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





void free_token_Values(Token *token){       // Function that frees the memory allocated using malloc.
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

Token* scan() {                             
    char curr = getNotWhiteChar();         // 'next' serves as a buffer that looks ahead.
    char next = curr;
    int is_comm_ok;
    Token* token;

    //printf("%c",curr);
    
    switch (curr) {
        case '/':
            is_comm_ok = SkipComment();
            if (is_comm_ok == 1) {      
                return createToken(T_DIV,TC_OPERATORS);
            }
            else if (is_comm_ok == -1) {
                token = createToken(T_ERORR, TC_ERR);
                token->value.integer=1;
                return token;
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
            if (next == '=') {                                         // I'll look at the next character to see if it might be == (equal).
                return createToken(T_EQUALS, TC_OPERATORS);
            } 
            else {                                                       
                ungetc(next, stdin);
                return createToken(T_ASSIGN, TC_OPERATORS);
            }

        case '+':
            return createToken(T_PLUS, TC_OPERATORS);

        case '-':                                                  
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

        case '!':                                                   
            next = getchar();
            if (next == '=') {
                return createToken(T_NOT_EQUALS, TC_LOGICAL);
            }
            else {
                ungetc(next, stdin);
                token = createToken(T_ERORR, TC_ERR);
                token->value.integer=1;
                return token;
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
                return createToken(T_DOUBLE_QUESTION_MARK, TC_OPERATORS);
            } 
            else {
                ungetc(next, stdin);
                token = createToken(T_ERORR, TC_ERR);
                token->value.integer=1;
                return token;
            }



        case '_':                                       // Id or _
            next = getchar();
            if (isalnum(next) || next=='_') {
                ungetc(next, stdin);
                return(is_Id(curr));                   
            }

            else {
                ungetc(next, stdin);
                return createToken(T_UNDERSCORE, TC_Punctation);
            }
    
        case 'a': case 'b': case 'c': case 'd': case 'e':case 'f': case 'g': case 'h': case 'i': case 'j':case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':case 'u': case 'v': case 'w': case 'x': case 'y':case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':case 'F': case 'G': case 'H': case 'I': case 'J':case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':case 'U': case 'V': case 'W': case 'X': case 'Y':case 'Z':
            return(is_Id(curr));              // Id or keyword


        case '"':                          
            next = getchar();
            if (next == '"'){
                next = getchar();
                if(next =='"'){               // if next is " then it is multi string            
                    return isMultiLineString();
                }
                else{                       // It's just "", so it's an empty string.
                    ungetc(next,stdin);
                    return createToken(T_STRING,TC_VALUE);                 
                }                                               
                
            }
            else{                           // normal string
                ungetc(next,stdin);
                return isString(curr);
            }

        case '0': case '1': case '2': case '3': case '4':case '5': case '6': case '7': case '8': case '9':                  // budd Double nebo Inter
            return (scanNumber(curr));

        case EOF:                           
            return createToken(T_EOF, TC_ERR);

        default:                                    // something that should not be there           
            token = createToken(T_ERORR, TC_ERR);
            token->value.integer=1;
            return token;
    }
    return NULL;
}





/*


const char* token_names[] = {
    "T_LEFT_PAREN",
    "T_RIGHT_PAREN",
    "T_LEFT_BRACE",
    "T_RIGHT_BRACE",
    "T_LEFT_BRACKET",
    "T_RIGHT_BRACKET",
    "T_DIV",
    "T_ASSIGN",
    "T_PLUS",
    "T_MINUS",
    "T_MUL",
    "T_EQUALS",
    "T_NOT_EQUALS",
    "T_LESS_THAN",
    "T_LESS_EQUAL",
    "T_GREATER",
    "T_GREATER_EQUAL",
    "T_COMMA",
    "T_COLON",
    "T_ARROW",
    "T_DOUBLE_QUESTION_MARK",
    "T_UNDERSCORE",
    "T_DQUOTE",
    "T_ELSE",
    "T_FUNC",
    "T_IF",
    "T_LET",
    "T_WHILE",
    "T_NIL",
    "T_RETURN",
    "T_VAR",
    "T_KW_STRING",
    "T_KW_INT",
    "T_KW_DOUBLE",
    "T_IDENTIFIER",
    "T_MULTILINE_STRING",
    "T_STRING",
    "T_INT",
    "T_DOUBLE",
    "T_EOF",
    "T_ERROR"
};








//  int main() {                                    // best debuging ever cant change my mind
//      Token* xd;
//      xd=scan();
//      printf("--%s\n", token_names[xd->type]);
//      while(xd->Category!=TC_ERR){
//          free_token_Values(xd);
//          xd=scan();
//          printf("--%s\n", token_names[xd->type]);
        
//         if(xd->type==T_STRING){
//             printf("xx %s xx\n",xd->value.stringVal);
//         }
//         else if(xd->type==T_IDENTIFIER){
//             printf("xx %s xx \n",xd->value.ID_name);}
//         else if(xd->type==T_DOUBLE){
//             printf("xx %lf xx \n",xd->value.decimal);}
//         else if(xd->type==T_INT){
//             printf("xx %d xx \n",xd->value.integer);}
//         else if(xd->type==T_ERORR){
//             printf("xx %d xx \n",xd->value.integer);
//             }
//      }
//      free_token_Values(xd);
//  }
 
*/
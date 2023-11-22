#ifndef SCANNER_H
#define SCANNER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



enum token_Category {
    TC_BRACKET,
    TC_OPERATORS,
    TC_LOGICAL,
    TC_Punctation,
    TC_KEYWORDS,
    TC_TYPE,
    TC_ID,
    TC_VALUE,
    TC_ERR
};

enum token_type {
    //Bracket
    T_LEFT_PAREN,           // 0 (
    T_RIGHT_PAREN,          // 1 )
    T_LEFT_BRACE,           // 2 {
    T_RIGHT_BRACE,          // 3 }
    T_LEFT_BRACKET,         // 4 [
    T_RIGHT_BRACKET,        // 5 ]

    // Operators
    T_DIV,                  // 6 /
    T_ASSIGN,               // 7 =
    T_PLUS,                 // 8 +
    T_MINUS,                // 9 -
    T_MUL,                  // 10 *

    // Logical
    T_EQUALS,               // 11 ==
    T_NOT_EQUALS,           // 12 !=
    T_LESS_THAN,            // 13 <
    T_LESS_EQUAL,           // 14 <=
    T_GREATER,              // 15 >
    T_GREATER_EQUAL,        // 16 >=

    //Punctation
    T_COMMA,                // 17 ,
    T_COLON,                // 18 :
    T_ARROW,                // 19 ->
    T_DOUBLE_QUESTION_MARK, // 20 ??
    T_UNDERSCORE,           // 21 _
    T_DQUOTE,               // 22 "

    // Keywords
    T_ELSE,                 // 23 else
    T_FUNC,                 // 24 func
    T_IF,                   // 25 if
    T_LET,                  // 26 let
    T_WHILE,                // 27 while
    T_NIL,                  // 28 nil
    T_RETURN,               // 29 return
    T_VAR,                  // 30 var

    // Typ
    T_KW_STRING,            // 31 String
    T_KW_INT,               // 32 Int
    T_KW_DOUBLE,            // 33 Double
    
    // Id 
    T_IDENTIFIER,           // 34 Id

    // Value
    T_MULTILINE_STRING,     // 35 Multiline_line_string
    T_STRING,               // 36 String
    T_INT,                  // 37 Int
    T_DOUBLE,               // 38 D ouble


    T_EOF,                  // 39 EOF
    T_ERORR                 // 40 Error
}token_type;

typedef struct tokenValue_t{
    char* ID_name;
    char* stringVal;
    int integer;
    double decimal;
    int nillable;           // 0- neni specifikovana 1- je nullable    2- zakazano
} tokenValue_t;


typedef struct Token {
    enum token_Category Category;
    enum token_type type;
    tokenValue_t value;
} Token;


/**
 * Funkce, která zjišťuje, zda je další znak bílý znak nebo ne.
 * @param input - Znak, který se má zkontrolovat.
 * @return 1, pokud je znak bílý, 0 jinak.
 */
int isWhiteChar(char input);


/**
 * Získá znak, který není bílý znak.
 * @return Znak, který není bílý znak.
 */
char getNotWhiteChar();


/**
 * Funkce pro přeskočení komentáře.
 * @return 0, pokud byl komentář úspěšně přeskočen, 1 jinak.
 */
int SkipComment();


/**
 * Funkce pro vytvoření tokenu.
 * @param type - Typ tokenu.
 * @param category - Kategorie tokenu.
 * @return Nový token.
 */
Token* createToken(enum token_type type, enum token_Category category);


/**
 * Zjistí, zda je řetězec klíčovým slovem.
 * @param str - Řetězec k ověření.
 * @return Index klíčového slova, pokud je to klíčové slovo, -1 jinak.
 */
int isKeyword(char *str);


/**
 *
 * Zjistí, zda je vstupní znak identifikátorem nebo klíčovým slovem.
 * @param curr - Aktuální znak.
 * @return Token identifikátoru nebo klíčového slova.
 */
Token* is_Id(char curr);


/**
 * Expanze dynamické paměti pro řetězec.
 * @param token - Token obsahující řetězec.
 * @param length - Aktuální délka řetězce.
 * @return 1 při úspěšné expanzi paměti, 0 jinak.
 */
int expand_String(Token *token, int *length);


/**
 * Přidá znak do řetězce v tokenu.
 * @param curr - Aktuální znak.
 * @param i - Index pro přidání znaku.
 * @param token - Token obsahující řetězec.
 * @return 1, pokud byl znak úspěšně přidán, 0 jinak.
 */
int addChar(char curr, int i, Token *token);


/**
 * Zpracuje Unicode escape sekvenci v řetězci.
 * @param i - Index pro přidání znaku.
 * @param token - Token obsahující řetězec.
 * @return 1, pokud byla escape sekvence úspěšně zpracována, 0 jinak.
 */
int unicode(int i, Token *token);


/**
 * Zpracuje escape znaky v řetězci.
 * @param token - Token obsahující řetězec.
 * @param i - Index pro přidání znaku.
 * @return 1, pokud byl escape znak úspěšně zpracován, 0 jinak.
 */
int escape_Char(Token *token, int i);


/**
 * Skenuje řetězec a vytváří odpovídající token.
 * @param curr - Aktuální znak.
 * @return Token reprezentující řetězec.
 */
Token* isString(char curr);


/**
 * Skenuje víceřádkový řetězec a vytváří odpovídající token.
 * @return Token reprezentující víceřádkový řetězec.
 */
Token* isMultiLineString();


/**
 * Skenuje číslo a vytváří odpovídající token.
 * @param curr - Aktuální znak.
 * @return Token reprezentující číslo.
 */
Token* scanNumber(char curr);


/**
 * Funkce, která uvolňuje paměť alokovanou pro hodnoty tokenu.
 * @param token - Token k uvolnění paměti.
 */
void free_token_Values(Token *token);




/**
 * Skenuje vstup a vytváří odpovídající token.
 * @return Token reprezentující část kódu.
 */
Token* scan();
#endif
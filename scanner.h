#pragma once
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
};

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



Token*scan();
void free_token_Values(Token *token);


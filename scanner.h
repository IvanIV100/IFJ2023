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
}token_Category;

extern enum token_type {
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
    T_DOUBLE,               // 38 Double


    T_EOF,                  // 39 EOF
    T_ERORR                 // 40 Error
}token_type;

typedef struct tokenValue_t{
    char* ID_name;
    char* stringVal;
    int integer;
    double decimal;
    int nillable;           // 0- not specified 1- is nullable    2- not allowed
} tokenValue_t;


typedef struct Token {
    enum token_Category Category;
    enum token_type type;
    tokenValue_t value;
} Token;


/**
 * Function that determines whether the next character is a white space or not.
 * @param input - Character to be checked.
 * @return 1 if the character is white space, 0 otherwise.
 */
int isWhiteChar(char input);


/**
 * Gets a character that is not a white space.
 * @return Character that is not a white space.
 */
char getNotWhiteChar();


/**
 * Function to skip a comment.
 * @return 0 if the comment was successfully skipped, 1 otherwise.
 */
int SkipComment();


/**
 * Function to create a token.
 * @param type - Token type.
 * @param category - Token category.
 * @return New token.
 */
Token* createToken(enum token_type type, enum token_Category category);


/**
 * Checks if the string is a keyword.
 * @param str - String to be checked.
 * @return Index of the keyword if it is a keyword, -1 otherwise.
 */
int isKeyword(char *str);


/**
 * Checks whether the input character is an identifier or a keyword.
 * @param curr - Current character.
 * @return Token of the identifier or keyword.
 */
Token* is_Id(char curr);


/**
 * Expands dynamic memory for a string.
 * @param token - Token containing the string.
 * @param length - Current length of the string.
 * @return 1 upon successful memory expansion, 0 otherwise.
 */
int expand_String(Token *token, int *length);


/**
 * Adds a character to the string in the token.
 * @param curr - Current character.
 * @param i - Index for adding the character.
 * @param token - Token containing the string.
 * @return 1 if the character was successfully added, 0 otherwise.
 */
int addChar(char curr, int i, Token *token);


/**
 * Processes Unicode escape sequence in the string.
 * @param i - Index for adding the character.
 * @param token - Token containing the string.
 * @return 1 if the escape sequence was successfully processed, 0 otherwise.
 */
int unicode(int i, Token *token);


/**
 * Processes escape characters in the string.
 * @param token - Token containing the string.
 * @param i - Index for adding the character.
 * @return 1 if the escape character was successfully processed, 0 otherwise.
 */
int escape_Char(Token *token, int i);


/**
 * Scans the string and creates the corresponding token.
 * @param curr - Current character.
 * @return Token representing the string.
 */
Token* isString(char curr);


/**
 * Scans a multiline string and creates the corresponding token.
 * @return Token representing the multiline string.
 */
Token* isMultiLineString();


/**
 * Scans a number and creates the corresponding token.
 * @param curr - Current character.
 * @return Token representing the number.
 */
Token* scanNumber(char curr);


/**
 * Function that frees the memory allocated for token values.
 * @param token - Token for memory release.
 */
void free_token_Values(Token *token);


/**
 * Scans the input and creates the corresponding token.
 * @return Token representing a code segment.
 */
Token* scan();
#endif
/**
 * Project: Implementace překladače IFJ23
 *
 * @brief Error handling.
 *
 * @author Ivan Chodák xchoda00
 */
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#ifndef ERROR_C
#define ERROR_C

void handleExit(int errorCode){
    exit(errorCode);
}

void ThrowError(int errorCode){
    fprintf(stderr, "\033[0;31m");
    fprintf(stderr, "%s", "[ERROR]");
    fprintf(stderr, "\033[0m");

    switch (errorCode) {
        case 1: //LEXICAL ANALYSIS
            fprintf(stderr, "Error during lexical analysis.\n");
            break;
        
        case 2: //SYNTAX_ANALYSIS
            fprintf(stderr, "Syntax error.\n");
            break;
        
        case 3: //UNDEFINED FUNCTION REDEF VAR
            fprintf(stderr, "Undefined function/Redefined var.\n");
            break;
        
        case 4: // INVLAID PARAMETER TYPE
            fprintf(stderr, "invalid number or type of the parameters used in the function or invalid return type.\n");
            break;

        case 5: // UNDEFINED/UNINITIALIZED VARIABLE
            fprintf(stderr, "Undefined variable.\n");
            break;
            
        case 6: // INVALID RETURN
            fprintf(stderr, "Function return error. Missing or extra return value.\n");
            break;

        case 7: // INCOMPATIBLE VARIABLE TYPES
            fprintf(stderr, "Incompatible variable types.\n");
            break;

        case 8: // SEMANTIC ERROR
            fprintf(stderr, "Semantic error. Can't deduce type.\n");
            break;
        
        case 9: // OTHER SEMNATIC ERRORS
            fprintf(stderr, "Semantic error.\n");
            break;

        case 99: // INTERNAL ERROR
            fprintf(stderr, "Internal error. Memory work failed\n");
            break;
    }

    fprintf(stderr,"\n");
    handleExit(errorCode);
}


#endif

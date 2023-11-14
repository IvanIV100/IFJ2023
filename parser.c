/*
Parser for the compiler
xchoda00
2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "scanner.h" //change to scanner.h
#include "error.h"


/* TODO 
* - sort out return validity if in func
* - make error file
* - check mem allocs and add null check
* - optional else?
* - free of token list
* - handle built in funcs
* - check if err output is correct
*/

Token* handle_statement_list(Token *token){
    if ((token)->type == T_RIGHT_BRACE){
        return token;
    }
    token = handle_statement(token);
    token = handle_statement_list(token);
    return token;
}

Token* handle_param(Token *token){
    printf("HP current token type: %d\n", token->type);
    if ((token)->type == T_UNDERSCORE){
        //handle in symtab _ case
    } else if ((token)->type == T_IDENTIFIER){
        //symtab assign
    } else {
        printf("Error: Expected identifier/_\n");
        ThrowError(2);
    }
    token = scan();
    printf("49 current token type: %d\n", token->type);
    if ((token)->type != T_IDENTIFIER){
        printf("Error: Expected identifier\n");
        ThrowError(2);
    }
    token = scan();
    printf("55 current token type: %d\n", token->type);
    if ((token)->type != T_COLON){
        printf("Error: Expected colon\n");
        ThrowError(2);
    }
    token = scan();
    printf("62 current token type: %d\n", token->type);
    token = handle_type(token);

    return  token;;
}

Token* handle_param_list(Token *token){
    if ((token)->type == T_RIGHT_PAREN){
        printf("end of param list\n");
        return token;
    } else {
        token = handle_param(token);
    }
    token = scan();
    printf("76 current token type: %d\n", token->type);
    if ((token)->type == T_COMMA){    
        token = scan();
        printf("79 current token type: %d\n", token->type);
        if ((token)->type == T_RIGHT_PAREN){            // (param,) invalid
            printf("Error: bad end\n");
            ThrowError(2);
        }
        token = handle_param_list(token);
        return token;
    } else {
        return token;
    }
}

Token* handle_type(Token *token){ //what is difference between t_int a t_kw_int
    if ((token)->type == T_KW_INT){
        //assign int in symtable with ? check
    } else if ((token)->type == T_KW_DOUBLE){
        //assign double in symtable with ? check
    } else if ((token)->type == T_KW_STRING){
        //assign str in symtable with ? check
    } else {
        printf("Error: Expected type\n");
        ThrowError(2);
    }
    return  token;
}

Token* handle_func_def(Token *token){

    if (token->type != T_IDENTIFIER) {
        printf("112 Error: Expected identifier\n");
        ThrowError(2);
    }
    //symtable entry of new func with id
    token = scan();
    if (token->type != T_LEFT_PAREN) {
        printf("119 Error: Expected left paren\n");
        ThrowError(2);
     }
    token = scan();
    token = handle_param_list(token);
    printf("125 current token type: %d\n", token->type);
    
    //removed check for ) as its in handle_param_list
    token = scan();
    if ((token)->type != T_ARROW) {  //check if arrow is there for Token* return 
        printf("133 current token type: %d\n", token->type);
        printf("Error: Expected arrow\n");
        ThrowError(2);
    }
    token = scan();
    token = handle_type(token);
    printf("137 current token type: %d\n", token->type);
    token = scan();    
    printf("139 current token type: %d\n", token->type);
    if ((token)->type == T_LEFT_BRACE) {
        token = scan();
        printf("142 current token type: %d\n", token->type);
        token = handle_statement_list(token);
    } else {
        printf("token type %d\n", token->type);
        printf("Error: Expected left brace\n");
        ThrowError(2);
    }
    // tu je : why
    printf("150 current token type: %d\n", token->type);
    if ((token)->type != T_RIGHT_BRACE) {
        printf("Error: Expected right brace\n");
        ThrowError(2);
    }
    return token;
}

Token* handle_in_param(Token *token){
    if ((token)->type == T_IDENTIFIER){
        token = scan();
        printf("150 current token type: %d\n", token->type);
        if ( (token)->type == T_COLON){
            token = scan();
            printf("153 current token type: %d\n", token->type);
            if ((token)->type == T_IDENTIFIER){
                token = scan();
                printf("156 current token type: %d\n", token->type);
                return  token;

            } 
            printf("Error: Param failed\n");
            ThrowError(2);
        } 
        return  token;
    }
    printf("Error: Param failed\n");
    ThrowError(2);
    return  token;
}

Token* handle_in_param_list(Token *token){
    printf("174 current token type: %d\n", token->type);
    if((token)->type == T_RIGHT_PAREN){
        return  token;
    }
    printf("179 current token type: %d\n", token->type);
    token = handle_in_param(token);
    
    if ((token)->type == T_COMMA){
        token = scan();
        if((token)->type == T_RIGHT_PAREN){
            printf("Error: Not right\n");
            ThrowError(2);
        }
        token = handle_in_param_list(token);
        
    }
    printf("192 current token type: %d\n", token->type);
    if ((token)->type != T_RIGHT_PAREN){
        printf("Error: Expected right paren\n");
        ThrowError(2);
    }
    return  token;
}

Token* handle_assign_ops(Token *token){ 
    if (token->type != T_ASSIGN) {
        printf("Error: Expected assign\n");
        ThrowError(2);
    }
    token = scan();
    if ((token)->type == T_IDENTIFIER){
        printf("197 current token type: %d\n", token->type);
        token = scan();
        printf("200 current token type: %d\n", token->type);
        if((token)->type == T_LEFT_PAREN){
            token = scan();
            printf("203 current token type: %d\n", token->type);
            token = handle_in_param_list(token);
            printf("205 current token type: %d\n", token->type);
            if ((token)->type != T_RIGHT_PAREN){
                printf("Error: Expected right paren\n");
                ThrowError(2);
            }
            return token;
        } else {
            printf(" 214 Error: Expected left paren\n");
            ThrowError(2);
        }
    } else {
        //call expression handle
        //if failed err.         
    }
    return  token;
}
Token* handle_var_def(Token* token){
    
    if ((token)->type != T_IDENTIFIER) {
        
        printf("Error: Expected identifier\n");
        ThrowError(2);
    }
    //symtable entry of new var with id with look back if let or var. implement maybe creation
    token = scan();
    printf("237 current token type: %d\n", token->type);
    if ((token)->type == T_COLON) {
        
        //handle and assign type
        token = scan();
        printf("242 current token type: %d\n", token->type);
        token = handle_type(token);
        token = scan();
        
    }
    printf("247 current token type: %d\n", token->type);
    token = handle_assign_ops(token);
    printf("249 current token type: %d\n", token->type);
    return token;
}

Token* handle_funcall_ops(Token *token){
    printf("254 token type %d\n", token->type);
    if ((token)->type == T_ASSIGN){
        token = scan();
        printf("257 token type %d\n", token->type);
        token = handle_assign_ops(token);
        return  token;
    } else if ((token)->type == T_LEFT_PAREN){
        token = scan();
        printf("261 token type %d\n", token->type);
        token = handle_in_param_list(token);

        token = scan();
        printf("265 token type %d\n", token->type); 
        if ((token)->type != T_RIGHT_PAREN){
            printf("Error: Expected right paren\n");
            ThrowError(2);
        } 
        return  token;
    } else {
        printf("261 token type %d\n", token->type);
        printf("261 Error: Expected assign or left paren\n");
        ThrowError(2);
    }
    return  token;
}

Token* handle_cond_ops(Token *token){
    if ((token)->type == T_LET){
        token = scan();
        if ((token)->type == T_IDENTIFIER){
            //write
            return  token;
        }
    } else {
        //expression handle
        return  token;
    }
    return  token;
}

Token* handle_if(Token *token){
    
    token = handle_cond_ops(token);

    token = scan();
    if ((token)->type == T_LEFT_BRACE){
        token = scan();
        token = handle_statement_list(token);

        token = scan();
        if ((token)->type != T_RIGHT_BRACE){
            printf("Error: Expected right brace\n");
            ThrowError(2);
        } else {
            
            
            if ((token)->type != T_ELSE){       //optional else ?
                printf("Error: Expected else\n");
                ThrowError(2);
            }
            token = scan();
            if ((token)->type != T_LEFT_BRACE){
                printf("Error: Expected left brace\n");
                ThrowError(2);
            } else {
                
                token = handle_statement_list(token);

                token = scan();
                if ((token)->type != T_RIGHT_BRACE){ //maybe insufficient check for {} counter
                    printf("Error: Expected right brace\n");
                    ThrowError(2);
                }
            }
        }
    } else {
        printf("Error: Expected left brace\n");
        ThrowError(2);
    }
    return token;
}

Token* handle_while(Token *token){
    
    token = handle_cond_ops(token);
    token = scan();
    if ((token)->type != T_LEFT_BRACE){
        printf("Error: Expected left brace\n");
        ThrowError(2);
    }
    token = scan();
    token = handle_statement_list(token); //check for nested conditions and issues
    
    token = scan();
    if ((token)->type != T_RIGHT_BRACE){
        printf("Error: Expected right brace\n");
        ThrowError(2);
    }
    return token;
}

Token* handle_statement(Token *token){
    switch ((token)->type){
        case T_LET:
        case T_VAR:
            token = scan();
            printf("356 token type %d\n", token->type);
            token = handle_var_def(token);
            token = scan();
            break;

        case T_IDENTIFIER:
            printf("362 token type %d\n", token->type);
            token = scan();
            token = handle_funcall_ops(token);

            printf("367 token type %d\n", token->type);
            break;
        case T_IF:
            token = scan();
            token = handle_if(token);
            break;
        case T_WHILE:
            token = scan();
            token = handle_while(token);
            break;
        case T_RETURN : 
            token = scan();
            token = scan();
            return token;
        case T_EOF:
            exit(0);
        default: 
            return token;
    }
    return token;
}



// Token* token_insert(Token *token){
//     token_t *new_token = malloc(sizeof(struct token_struct));
//     if (new_token == NULL){
//         ThrowError(99);
//         return;
//     }
//     new_token = scan();
//     new_right = NULL;
//     if (token == NULL){
//         new_left = NULL;
//         token = new_token;
//         return;

//     } else {
//         token_t* tmp = token;
//         while ( (tmp)->right  != NULL){
//             tmp = (tmp)->right;
//         }
//         new_left = tmp;
//         (tmp)->right = new_token;
//     }
// }

// Token* token_clear(Token *token){
//     if (token == NULL){
//         return;
//     }
//     while(left != NULL){
//         token = left;
//     }
//     while(right != NULL){
        
//         free(token);
//     }
    
    
// } 

void parser(){
    Token *token = scan();
    bool run = true;
    while (run){
        switch (token->type) {
        case T_FUNC:
            token = scan();
            token = handle_func_def(token);
            token = scan();
            break;
        case T_EOF:
            exit(0);
        default:
            token = handle_statement_list(token);
            break;
        }
    }  
}

int main()
{
    parser();
    return 0;
}
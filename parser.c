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
#include "scanner.c" //change to scanner.h


/* TODO
* - sort out return validity if
* - maybe get token as a pointer
*/

void handle_statement_list(){
    if (/* } */){
        return;
    }
    handle_statement();
    handle_statement_list;
    return;
}

void handlle_param(){
    if (/*is _*/){
        //handle in symtab _ case
    } else if (/*identifier*/){
        //symtab assign
        return;
    } else {
        //error
        return;
    }
    //next token
    if (/*not identifier*/){
        //error
        return;
    }
    //next token
    if (/*not : */){
        //error
        return;
    }
    handle_type();


}

void handle_param_list(){
    handle_param();
    if (/*not , */){
        return;
    }
    handle_param_list();
    
}

void handle_type(){
    if (/*not int/double/str*/){
        //error
        return;
    }
    //assign given type in symtable
}

void handle_func_def(){
    //next token
    if (/*next token is not id*/) {
        //error
        return;
    }
    //symtable entry of new func with id
    //next token
    if (/*next token is not (*/) {
        //error
        return;
    }
    
    //next token maybe not
    if (/*next token is not )*/) {
        //error
        return;
    } else {
        //next token
        handle_param_list(/*pass struct for func symtab*/);
        if (/*next token is not )*/) {
            //error
            return;
        }
    }
    if (/*next token is not {*/) {
        if (/*next token is not -> */)
        {
            //error
            return;
        }
        handle_type(/*pass struct for func symtab*/);
        return;
    } else if (/*next token is {*/) {
        handle_statement_list(/*pass struct for func symtab*/);

    
    } else {
        //error
        return;
    }
    if (/*next token is not }*/) {
        //error
        return;
    }
}

void handle_in_param(){
    if (/* ID */){
        //next
        if ( /* :*/){
            //next
            if (/* id*/){
                return;
            }
        }
    }
    //error
    return;
}

void handle_in_param_list(){
    if(/*)*/){
        return;
    }
    handle_in_param();
    //next
    if (/* , */){
        handle_in_param_list();
    }
    if (/*not ) */){
        //error
        return;
    }
}

void handle_assign_ops(){
    if (/* identifier */){
        if(/* ( */){
            handle_in_param_list();
            if (/* not )*/){
                //error
                return;
            }
        } else {
            //error
            return;
        }

    } else {
        //call expression handle
        //if failed err.         
    }

}

void handle_var_def(){
    //next token
    if (/*next token is not id*/) {
        //error
        return;
    }
    //symtable entry of new var with id with look back if let or var. implement maybe creation
    //next token
    if (/*next token is  :*/) {
        //next token
        //handle and assign type
        handle_type(/*pass struct for var symtab*/);
        return;
    }
    //get type from the assop
    handle_assign_ops();
    return;
}

void handle_funcall_ops(){
    if (/* = */){
        handle_assign_ops();
        return;
    } else if (/* ( */){
        handle_param_list();
        if (/* not ) */){
            //error
            return;
        } 
        return;
    } else {
        //error
        return;
    }
}

void handle_cond_ops(){
    //handle expression
    if (/* let */){
        if (/* identifier */){
            //write
            return
        }
    } else {
        //error
        return;
    }
}

void handle_if(){
    handle_cond_ops();
    if (/* { */){
        handle_statement_list();
        if (/* not } */){
            //error
            return;
        } else {
            //optional else ?
            if (/* not  else */){
                
                //error
                return;
            }
            if (/* not { */){
                //error
                return;
            } else {
                handle_statement_list();
                if (/* not }*/){ //maybe insufficient check for {} counter
                    //error
                    return;
                }
            }
        }
    } else {
        //error
        return;
    }
}

void handle_while(){
    handle_cond_ops();

    if (/* not { */){
        //error
        return;
    }
    handle_statement_list(); //check for nested conditions and issues
    
    if (/* not } */){
        //error
        return;
    }
}

void handle_statement(){
    switch (/*next token*/){
        case T_LET:
        case T_VAR:
            handle_var_def(/*token list*/);
            break;

        case T_IDENTIFIER:
            handle_funcall_ops(/*token list*/);
            break;
        case T_IF:
            handle_if(/*token list*/);
            break;
        case T_WHILE:
            handle_while(/*token list*/);
            break;
        case T_RETURN : 
            //check if in func 
           // return;
        default: 
            //error 
            return;
    }
}

void parse(){
    
    //check if eof
    bool run = true;
    while (run){
        switch (/*first token*/)
        {
        case T_FUNC:
            handle_func_def(/*token list*/);
            break;
        
        default:
            handle_statement(/*token list*/);
            break;
        }
    }  
}
//write for me a basic main function
int main()
{
    parser();
    return 0;
}
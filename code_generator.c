/**
 * Project: Implementace překladače IFJ23.
 *
 * @brief Code generator implementation.
 *
 * @author Robert Zelníček <xzelni06@stud.fit.vutbr.cz>
 */

 #include <ctype.h>

 #include "scanner.h"
 #include "code_generator.h"

 //built in functions

 #define FUNCTION_LENGTH 
	"\n # Built-in function Length" \
	"\n LABEL $length" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n STRLEN LF@%retval LF@%0" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_WRITE 
	"\n # Built-in function Write" \
	"\n LABEL $write" \
	"\n PUSHFRAME" \
	"\n WRITE %0" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_INT2DOUBLE 
	"\n # Built-in function Int2Double" \
	"\n LABEL $int2double" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n INT2FLOAT LF@%retval LF@%0" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_DOUBLE2INT 
	"\n # Built-in function Double2Int" \
	"\n LABEL $double2int" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n FLOAT2INT LF@%retval LF@%0" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_READ_STRING 
	"\n # Built-in function ReadString" \
	"\n LABEL $readString" \
	"\n PUSHFRAME" \
	"\n READ LF@%retval string" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_READ_INT 
	"\n # Built-in function ReadINT" \
	"\n LABEL $readInt" \
	"\n PUSHFRAME" \
	"\n READ LF@%retval int" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_READ_DOUBLE 
	"\n # Built-in function ReadDouble" \
	"\n LABEL $readDouble" \
	"\n PUSHFRAME" \
	"\n READ LF@%retval float" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_SUBSTRING 
	"\n # Built-in function Substring" \
	"\n LABEL $substring" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n MOVE LF@%retval string@" \
	"\n DEFVAR LF@index" \
	"\n MOVE LF@index int@0" \
	"\n DEFVAR LF@length" \
	"\n MOVE LF@length int@0" \
	"\n STRLEN LF@length LF@%0" \
	"\n DEFVAR LF@char" \
	"\n DEFVAR LF@process_loop_cond" \
	"\n LABEL $substring_loop" \
	"\n GETCHAR LF@char LF@%0 LF@index" \
	"\n CONCAT LF@%retval LF@%retval LF@char" \
	"\n ADD LF@index LF@index int@1" \
	"\n ADD LF@length LF@length int@1" \
	"\n LT LF@process_loop_cond LF@index LF@%1" \
	"\n JUMPIFEQ $substring_loop LF@process_loop_cond bool@true" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_ORD 
	"\n # Built-in function Ord" \
	"\n LABEL $ord" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n STRI2INT LF@%retval LF@%0 LF@%1" \
	"\n POPFRAME" \
	"\n RETURN"

    #define FUNCTION_CHR 
	"\n # Built-in function Chr" \
	"\n LABEL $chr" \
	"\n PUSHFRAME" \
	"\n DEFVAR LF@%retval" \
	"\n INT2CHAR LF@%retval LF@%0" \
	"\n POPFRAME" \
	"\n RETURN"
#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <conio.h>



#define Utils_Assert(expression) assert(expression);

//#define CONFIG_BIG_ENDIAN
#ifndef CONFIG_BIG_ENDIAN 
#define CONFIG_LITLE_ENDIAN 
#endif

//Defines the send function for all functions with StO in name. 
#define STD_OUT_SEND_CHAR(Char) putc(Char, stdout);
//Defines the read function for all functions with StI in name. 
//EOF indicates currently no chars available or recently sent.
//Otherwise it will return values between CHAR_MIN and CHAR_MAX, which means that the returned characters were received. 
#define STD_IN_GET_CHAR() _kbhit() ? _getch() : EOF

//currently redirects to standard out, however i might want to implement a way to handle this.
#define STD_DEBUG_SEND_CHAR(Char) STD_OUT_SEND_CHAR(Char)

#define CONSOLE_RETURN_CHAR '\r'
#define CONSOLE_DELETE_PREV_CHAR '\b'
#define CONSOLE_NEW_LINE_STR "\n:: "
#define CONSOLE_LINE_BUFFER_LEN 256

#define INDENT_1_LEVEL_STRING "    "

#define VARIADIC_ARG_STACK_LEN 8
#define G_VARIADIC_ARG_STACK G_Variadic_Arg_Stack





/***+*************************************
*										 *
*			  ERROR HANDLING  			 *
*										 *
++***************************************/



typedef enum
{
	err_nil_e,
	error_test_e,
	error_test2_e,

} error_enum;




#endif
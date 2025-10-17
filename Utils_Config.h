#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <conio.h>



#define Utils_Assert(expression) assert(expression);

//Defines the send function for all functions with StO in name. 
#define STD_OUT_SEND_CHAR(Char) putc(Char, stdout);
//Defines the read function for all functions with StI in name. 
//EOF indicates currently no chars available or recently sent.
//Otherwise it will return values between CHAR_MIN and CHAR_MAX, which means that the returned characters were received. 
#define STD_IN_GET_CHAR() _kbhit() ? _getch() : EOF

#define CONSOLE_RETURN_CHAR '\r'
#define CONSOLE_DELETE_PREV_CHAR '\b'
#define CONSOLE_NEW_LINE_STR "\n:: "
#define CONSOLE_LINE_BUFFER_LEN 256

#define INDENT_1_LEVEL_STRING "    "

#define VARIADIC_ARG_STACK_LEN 8
#define G_VARIADIC_ARG_STACK G_Variadic_Arg_Stack



#endif
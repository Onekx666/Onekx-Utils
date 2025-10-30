#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <conio.h>

//#include "Utils.h"
//#include "UART_Wrapper.h"

#define Utils_Assert(Expression) assert(Expression); //{ if (!(Expression)) { const char MACRO_LOCAL_VAR_MSG[] = "Assertion Failed: ( " #Expression " ) in file: `" __FILE__ "\n\r"; StD_Print(MACRO_LOCAL_VAR_MSG); } } 



//#define CONFIG_BIG_ENDIAN
#ifndef CONFIG_BIG_ENDIAN 
#define CONFIG_LITLE_ENDIAN 
#endif

#define EOF -1
//Defines the send function for all functions with StO in name. 
#define STD_OUT_SEND_CHAR(Char) putc(Char, stdout); //PROC_UART_Send_Single(Char)
//Defines the read function for all functions with StI in name. 
//EOF indicates currently no chars available or recently sent.
//Otherwise it will return values between CHAR_MIN and CHAR_MAX, which means that the returned characters were received. 
#define STD_IN_GET_CHAR() _kbhit() ? _getch() : EOF //PROC_UART_Read_Single()

//currently redirects to standard out, however i might want to implement a way to handle this.
#define STD_DEBUG_SEND_CHAR(Char) STD_OUT_SEND_CHAR(Char)

#define CONSOLE_RETURN_CHAR '\r'
#define CONSOLE_DELETE_PREV_CHAR '\b'
#define CONSOLE_NEW_LINE_STR "\n:: "
#define CONSOLE_LINE_BUFFER_LEN 256

#define INDENT_1_LEVEL_STRING "    "

#define VARIADIC_ARG_STACK_LEN 256
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





/***+*************************************
*										 *
*			    COMMANDS                 *
*										 *
++***************************************/


/*
Enum values are used to index into entry table (Utils_Commands.c), make sure they line up.
*/
typedef enum
{
	cmd_none_e,
	cmd_set_gpio_e,
	cmd_shutdown_e,
	cmd_reboot_e,
	cmd_e_command_cnt,
	cmd_parsing_failed_e,
} command_type_enum;



#endif
//Created 2025.01.13 1950 as Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <assert.h>

#include "Utils_Config.h"



/***+************************************
*										*
*			VARIADIC ARGUMETNS			*
*										*
++**************************************/



typedef short arg_cnt;



typedef enum
{
	no_type_err_e,
	bool_e,
	bool_array_e,
	int_e,
	int_array_e,
	char_e,
	char_array_e,
	string_e,
	size_e,
	size_array_e,
	var_size_int_array_e,
	c_type_enum_max,
} c_type_enum;



typedef struct
{
	short Element_Size;
	short Length;
	short Stride;
	short Type_Enum;
} type_descriptor;



typedef struct
{
	void* Ptr;
	char* Name;
	type_descriptor Type;
} variadic_arg;



/*
WARINING: If an argument is pushed to a full stack the argument will simply be ignored!

Top_P1: 3

stack top: []

	0		1		2		3		4		5
| arg A | arg B |[arg C]|  ...  |  ...  |  ...  |
*/
typedef struct
{
	arg_cnt Top_P1;
	variadic_arg Arguments_Array[VARIADIC_ARG_STACK_LEN];
} variadic_arg_stack;



/*
WARINING: If an argument is pushed to a full stack the argument will simply be ignored!

returns 1 on success.

to push: arg D

stack top: []

| arg A | arg B |[arg C]|  ...  |  ...  |  ...  |

| arg A | arg B | arg C |[arg D]|  ...  |  ...  |

if stack is full no arg will be pushed, zero will be returned.
*/
arg_cnt Push_Variadic_Arg(variadic_arg_stack* Stack, variadic_arg Arg);



/*
If the stack is empty an argument with type stack_err_e will be returned,
in that case the pointers Ptr and Name are NULL.

stack top: []

| arg A | arg B | arg C |[arg D]|  ...  |  ...  |

| arg A | arg B |[arg C]| arg D |  ...  |  ...  |

Popped: arg D
*/
variadic_arg Pop_Variadic_Arg(variadic_arg_stack* Stack);



/*
If the stack pointer is back at the size limit an argument with type stack_err_e will be returned,
in that case the pointers Ptr and Name are NULL.

WARNING: in general this error will not be signaled when to many 

stack top: []

| arg A | arg B |[arg C]| arg D | .... | .... |

| arg A | arg B | arg C |[arg D]| .... | .... |

Popped: arg C
*/
variadic_arg UNSAFE_Pop_Reverse_Variadic_Arg(variadic_arg_stack* Stack);



/*
returns true if stack was hih enough to walk, else returns false;

Walk_Back_Cnt: -3

stack top: []

| arg A | arg B | arg C |[arg D]| .... | .... |

|[arg A]| arg B | arg C | arg D | .... | .... |
*/
bool Pop_Go_Back_Variadic_Args(variadic_arg_stack* Stack, arg_cnt Walk_Back_Cnt);



extern variadic_arg_stack G_Variadic_Arg_Stack;



short CHECK_Is_Int(const int I);
short CHECK_Is_Int_Array(const int Final_Int, const short Len);
short CHECK_Is_Char(const char Chr);
short CHECK_Is_String(const char* String);
short CHECK_Is_Char_Array(const char* Char_Ptr, const char Final_Char, const short Len);

/*
CHECK_Is_Variable_Size_Int_Array():

struct size: -------- or ........

											 beginning of final BB
												  V
   --------........--------........--------........ : Struct_Size_Stride*(Len-1))+
  ^                                               V
--------........--------........--------........--------
|	0	|	1	|	2	|	3	|	4	|	5	|	6	|
AABBCCCCAABBCCCCAABBCCCCAABBCCCCAABBCCCCAABBCCCCAABBCCCC
  ^                                                ^
((char*)Member_Ptr)                                (((char*)Member_Ptr)+(Struct_Size_Stride*(Len-1))+sizeof(*Member_Ptr)-1)
												   / beginning of final BB + sizeof(*Member_Ptr) - 1 = 1
*/
short CHECK_Is_Variable_Size_Int_Array(const char First_Byte, const char Final_Byte, const short Len);


#define P_INT(Int) Push_Variadic_Arg(&G_VARIADIC_ARG_STACK, (variadic_arg) { .Name = #Int, .Ptr = &(int){ Int }, .Type = (type_descriptor) { .Type_Enum = int_e | CHECK_Is_Int(Int), } })

#define P_CHR(Char) Push_Variadic_Arg(&G_VARIADIC_ARG_STACK, (variadic_arg) { .Name = #Char,.Ptr = &Char, .Type = (type_descriptor) { .Type_Enum = char_e | CHECK_Is_Char(Char), } })

#define P_STR(Char_Ptr) Push_Variadic_Arg(&G_VARIADIC_ARG_STACK, (variadic_arg) { .Name = #Char_Ptr, .Ptr = (void*)Char_Ptr, .Type = (type_descriptor) { .Type_Enum = string_e | CHECK_Is_String(Char_Ptr), } })

#define P_IARR(Member_Ptr, Struct_Size_Stride, Len)\
 Push_Variadic_Arg(\
					&G_VARIADIC_ARG_STACK,\
					(variadic_arg)\
					{\
						.Name = #Member_Ptr,\
						.Ptr = Member_Ptr,\
						.Type = (type_descriptor)\
						{\
							.Type_Enum = var_size_int_array_e | CHECK_Is_Variable_Size_Int_Array(*((char*)Member_Ptr), *(((char*)Member_Ptr)+(Struct_Size_Stride*(Len-1))+sizeof(*Member_Ptr)-1), Len),\
							.Element_Size = sizeof(*Member_Ptr),\
							.Stride = Struct_Size_Stride,\
							.Length = Len,\
						}\
					}\
				)


//#define P_IARR(Int_Array, Len) Push_Variadic_Arg(&G_VARIADIC_ARG_STACK, (variadic_arg) { .Name = #Int_Array, .Ptr = Int_Array, .Type = (type_descriptor) { .Type_Enum = int_array_e | CHECK_Is_Int_Array(Int_Array[Len-1], Len), .Length = Len, } })
//#define P_CARR(Char_Array, Len) Push_Variadic_Arg(&G_VARIADIC_ARG_STACK, (variadic_arg) { .Name = #Char_Array, .Ptr = Char_Array, .Type = (type_descriptor) { .Type_Enum = char_array_e | CHECK_Is_Char_Array(Char_Array, Char_Array[Len-1], Len), .Length = Len, } })






/***+************************************
*										*
*			STRING UTILITIES			*
*										*
++**************************************/



/*
Checks if the first Sub_String_Len chars of String are equal to Sub_String. 
If Sub_String has a null terminator before Sub_Str_Len and strings are equal before then true will be returned as well.
If String is shorter then substring it is not considered equal, no undefined behavior occurs in that case

These two are equal:

String :		`This is the string it is longer then the substring.`
Sub_String :	`This is`

These are not equal:

String :		`This`
Sub_String :	`This is`
*/
bool SubStr_Str_S(const char* String, size_t String_Len, const char* Sub_String, size_t Sub_String_Len);

/*
Checks if the first strlen(Sub_String) String are equal to Sub_String.
If String is shorter then substring it is not considered equal, no undefined behavior occurs in that case

These two are equal:

String :		`This is the string, it is longer then the substring.0` <- notice the the null terminator, hence the name "..._NT"
Sub_String :	`This is0`

These are not equal:

String :		`This0`
Sub_String :	`This is0`

String :		`This is the string, it is longer then the substring.0`
Sub_String :	`That0`
*/
bool SubStr_Str_NT(char* String, char* Sub_String);

/*
Is the char contained in G_CONST_Space_Chars.
*/
bool Is_Space(char Char);

/*
Seeks end of white space as defined by the Is_Space function in this library.
This is will also work with null terminated strings.

				 vString
<console_command>             <parameter>
returned len:    -----+++++---^ end of seek

		0123456789v len : 10
_Valid: ##########.......
String: text.     ^ this idx will be returned

		012345678v effective len : 9
_Valid: #########0.......
String: text.    ^ this idx will be returned

The returned length may invalid for indexing into String if whitespace continues to the end.
*/
int Seek_Space_End_S(const char* const String, int String_Len);

/*
Scans till first null terminator. Returns length until terminator.
*/
int Str_Len_NT(char* String);

/*
Scans till first null terminator, while scanning at most Max_Len chars. Returns length until terminator.
*/
int Str_Len_S(char* String, int Max_Len);



/*
Parses a string as integer consisting of an optional sign - / + followed by whitespace followed by a continuous string of digits,
if the string does not begin with a sign it has to begin with a digit.
If the string could be successfully parsed, the number of parsed chars will be returned and *Out_Int_Value will be assigned the result.
On failure to Parse the integer, -1 will be returned and *Out_Int_Value will be untouched.
If the string corresponds to a to large integer undefined behaviour will not occur.

Str			Int

[-4]089		-4

-4[0]89		-40 (prev * 10 + 4)

-40[8]9		-408 (prev * 10 + 8)

-408[9]		-4089 (prev * 10 + 9)
*/
int Parse_Int_Save(const char* const String, int* const Out_Int_Value, int String_Len);



#define FORMAT_INT_AS_STR_OUT_BUFFER_SIZE 12

/*
The buffer needs to be at least as many digits as int max has +1, it also needs to be zero initialized.
Set FORMAT_INT_AS_STR_BUFFER_SIZE to the appropriate number.
After returning the buffer will contain a null-terminated string equivalent to the passed integer.

Integer: -5022

info / int				Out_Buffer

-502[2]					`2` (5022 % 10 = 2; Integer /= 10)

-50[2]					`22` (5022 % 10 = 2; Integer /= 10)

-5[0]					`220` (50 % 10 = 0; Integer /= 10)

-[5]					`2205` (50 % 10 = 0; Integer /= 10)

reverse	and
add sign				`-5022` (50 % 10 = 0; Integer /= 10)
*/
void Format_Int_As_Str(int Integer, char* const Out_Buffer);

typedef struct
{
	char Chars[2];
} char_array_2;

/*
.Chars = hex digits of { Byte&11110000, Byte&00001111 }
*/
char_array_2 Byte_To_Hex(unsigned char Byte);

//void StO_Print_F(size_t String_Len, char* const Format_String, ...);

//prints String to standard output until \0
void StO_Print(const char* const String);

void StD_Print(const char* const String);

//sprints string to standard output up to including [String_Len-1] or until \0 is encountered.s
void StO_Print_S(const char* const String, int String_Len);

//
void StO_Print_V(const char* const Format_String, arg_cnt N_Of_Variadic_Args);





/***+*************************************
*										 *
*			DEBUG DISPLAY FUNCS			 *
*										 *
++***************************************/



/*
Prints out a string and shows the index into the string below the relevant char.
WARNING: this will not work properly if there are non 1-width chars in the string.

--indent--<c>     <param>
--indent--		  ^ $Index_Name

*/
//void DBG_Display_Index_In_Line(FILE* const Stream, char* String, size_t Index, char* Index_Name, int Indentation_Len);


/*
Prints out a string and shows the index into the string in the string directly:

Chr: 34

["text"] = "hi there"

["text"] = ~"~hi there"


*/
void DBG_Display_Index_String_Multi_Line(char* String, size_t Str_Len, size_t Index);



/*
struct point		//offset
{
	int X,			//0
	int Y,			//sizeof(int) (4)
	int Z,			//sizeof(int)+sizeof(int) (8)
};

First_Member:	&Point_Array + sizeof(int) (4)

stride:			sizeof(point): 12

Len:			3

IDX:|			0			|			1			|			2			|			3			|			4			|
	|	X	|	Y	|	Z	|	X	|	Y	|	Z	|	X	|	Y	|	Z	|	X	|	Y	|	Z	|	X	|	Y	|	Z	|
	|	23	|	13	|	-1	|	13	|	9	|	5	|	1	|	0	|	8	|	8	|	3	|	11	|	99	|	52	|	19	|
				^						^						^						^						^
Output:

13		9		0		3		52

*/
void DBG_Display_Int_Member_Array(const void* const First_Member, int Member_Size, int Stride, int Len);



/***+*************************************
*										 *
*			  ERROR HANDLING  			 *
*										 *
++***************************************/



typedef struct
{
	short V;
	short Number;
} utl_error;



extern const char* const G_CONST_Error_Strings[];



utl_error PROC_Error(const error_enum Error_Type, arg_cnt N_Of_Variadic_Args);





/****************************
*                           *
*    CONSOLE & COMMANDS     *
*                           *
****************************/



/****************
*    console    *
****************/



/*
Everything initialized to zero = { 0 } is a valid initial value for the line buffer.
Last char of the buffer is reserved for a null terminator.

-----.....-----.....

got: h
Char_Got_Cnt
 v
h----.....-----.....

got: e
Char_Got_Cnt
  v
he---.....-----.....

got: l
Char_Got_Cnt
   v
hel--.....-----.....

got: p
Char_Got_Cnt
   v
help-.....-----.....

got: \r
Char_Got_Cnt
v
help-.....-----.....

On receiving return key parse command and reset line buffer.

*/
typedef struct
{
	int Chars_Got_Cnt;
	char Buffer[CONSOLE_LINE_BUFFER_LEN];
} console_line_buffer;



/************************
*  command definitions  *
************************/

#define COMMAND_NAME_MAX_LEN 8

/*
Enum values are used to index into entry table (Utils_Commands.c), make sure they line up.
*/
typedef enum
{
	cmd_none_e,
	cmd_help_e,
	cmd_set_gpio_e,
	cmd_e_command_cnt,
	cmd_parsing_failed_e,
} command_type_enum;



typedef struct
{
	char Command_Name[COMMAND_NAME_MAX_LEN];
} command_help;



typedef struct
{
	int GPIO_Id;
	int Value;
} command_set_gpio;




/*****************************
* other command related defs *
*****************************/



typedef struct
{
	c_type_enum Arg_Type;
	int Member_Offset;
} cmd_arg_entry;



typedef union
{
	command_help Cmd_Help;
	command_set_gpio Command_Set_GPIO;
} command_types_union;



typedef struct
{
	const char Name[COMMAND_NAME_MAX_LEN+1]; //including space for guaranteed null terminator.
	const int Name_Len;
	const char* const Help_Description;
	const cmd_arg_entry* Arguments_Entry_Array;
	const int Argument_Array_Len;
} command_entry;



typedef struct
{
	command_type_enum Type;
	command_types_union Command_Union;
} command;


/****************************
*        functions          *
****************************/



/*
Reads in chars from std io buffer until STD_IN_GET_CHAR() returns EOF.

When the return key, as defined by CONSOLE_RETURN_CHAR is received the command is parsed.

Commands must be of the from: <command name> <arg> <arg> <arg>, that is, 
a command name followed by arguments separated by an arbitrary amount of whitespace as define by Is_Space().
Between some argument zero whitespace is permitted, eg.: quoted strings.

If the command is invalid or the amount of received chars exceeds the limit of permissible number of chars,
then a command with Type = cmd_e_command_cnt will be returned.

If the return character has not yet been sent, cmd_none_e will be returned.

TODO: make it handle more then just int arguments
*/
command StI_Console_Read_Update(console_line_buffer* Line_Buffer);

#endif // !UTILS_H
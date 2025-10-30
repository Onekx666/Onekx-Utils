//Created as "String_Utils.c" on 2025.10.07 16:46

#include <stdlib.h>
#include <stdint.h>
//#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include <assert.h>

#include "Utils.h"



int Str_Len_NT(char* String)
{
	Utils_Assert(NULL != String);

	int Itr_Chr = 0;
	for (; '\0' != String[Itr_Chr]; Itr_Chr++);

	return Itr_Chr;
}



int Str_Len_S(char* String, int Max_Len)
{
	Utils_Assert(NULL != String);

	int Itr_Chr = 0;
	for (; '\0' != String[Itr_Chr] && Max_Len > Itr_Chr; Itr_Chr++);

	return Itr_Chr;
}



bool SubStr_Str_S(const char* String, size_t String_Len, const char* Sub_String, size_t Sub_String_Len)
{
	Utils_Assert(NULL != String);
	Utils_Assert(NULL != Sub_String);

	if (String_Len < Sub_String_Len) return false;

	for (size_t Itr_Chr = 0; Itr_Chr < Sub_String_Len && '\0' != Sub_String[Itr_Chr]; Itr_Chr++)
	{
		if ('\0' == String[Itr_Chr] || String[Itr_Chr] != Sub_String[Itr_Chr]) return false;
	}

	return true;
}



bool SubStr_Str_NT(char* String, char* Sub_String)
{
	Utils_Assert(NULL != String);
	Utils_Assert(NULL != Sub_String);

	for (size_t Itr_Chr = 0; '\0' != Sub_String[Itr_Chr]; Itr_Chr++)
	{
		if ('\0' == String[Itr_Chr]) return false; //String ran out before all characters in Sub_String are matched.
		if (String[Itr_Chr] != Sub_String[Itr_Chr]) return false;
	}

	return true;
}



const char G_CONST_Space_Chars_Array[] = { ' ', '\t', '\n', '\v', '\f', '\r' };

bool Is_Space(char Char)
{
	const int Len = sizeof(G_CONST_Space_Chars_Array) / sizeof(G_CONST_Space_Chars_Array[0]);

	for (int Itr_Char = 0; Itr_Char < Len; Itr_Char++)
	{
		if (Char == G_CONST_Space_Chars_Array[Itr_Char]) return true;
	}

	return false;
}



int Seek_Space_End_S(const char* const String, int String_Len)
{
	Utils_Assert(NULL != String);

	int Itr_Chr = 0;
	for (; Is_Space(String[Itr_Chr]) && Itr_Chr < String_Len; Itr_Chr++);

	return Itr_Chr;
}



bool Is_Digit(char Char)
{
	return Char >= '0' && Char <= '9' ? true : false;
}



#if !(INT_MAX <= INTPTR_MAX)
], ]][INTPTR_MAX_NEEDS_TO_BE_BIGGER_THEN_INT_MAX]
#endif

int Parse_Int_Save(const char* const String, int* const Out_Int_Value, int String_Len)
{
	Utils_Assert(NULL != String);
	Utils_Assert(NULL != Out_Int_Value);
	Utils_Assert(String_Len >= 0);

	int Crr_Chr = 0;

	int Sign = 1;

	if ('-' == String[Crr_Chr])
	{
		Sign = -1;
		Crr_Chr++;
	}
	else if ('+' == String[Crr_Chr])
	{
		Crr_Chr++;
	}
	else if (!Is_Digit(String[Crr_Chr]))
	{
		return -1;
	}

	if (Crr_Chr >= String_Len) return -1;

	// - 4089 or +4089
	//  ^         ^
	Crr_Chr += Seek_Space_End_S(&(String[Crr_Chr]), String_Len);

	if (Crr_Chr >= String_Len) return -1;

	// - 4089 or +4089
	//   ^        ^
	if (!Is_Digit(String[Crr_Chr])) return -1;
	unsigned int Number = 1;
	Number = Number * (String[Crr_Chr] - '0');
	Crr_Chr++;

	if (Crr_Chr >= String_Len)
	{
		*Out_Int_Value = Number * Sign;
		return Crr_Chr; // a single digit would still be valid.
	}

	// - 4089 or +4089
	//    ^        ^
	for (; Is_Digit(String[Crr_Chr]) && Crr_Chr < String_Len; Crr_Chr++)
	{
		Number = Number * 10 + (String[Crr_Chr] - '0');
		if ((+1 == Sign && Number > INT_MAX) || (-1 == Sign && Number > (-1 * (intptr_t)INT_MIN))) return -1;
	}

	*Out_Int_Value = Number * Sign;
	return Crr_Chr;
}



//#define INT_MAX 10000000000000000000
//  9223372036854775807 (INT64_MAX)
#if 9999999999999999999 < INT_MAX
//], ]][INT_MAX_TO_LARGE_FOR_Format_Int_As_Str]
#endif // 9999999999999999999 < LONG

void Format_Int_As_Str(int Integer, char* const Out_Buffer)
{
	//char INFO_STR[] = "9223372036854775807"; char[20] including null terminator

	Utils_Assert(NULL != Out_Buffer);

	char Internal_Buffer[19] = { 0 };

	int Number = Integer;

	int Itr_Chr = 0;
	do {
		int Digit_Value = Number % 10;
		Internal_Buffer[Itr_Chr] = '0' + (char)(Digit_Value < 0 ? -1 * Digit_Value : Digit_Value);
		Itr_Chr++;
		Number /= 10;
	} while (0 != Number);
	Utils_Assert(Itr_Chr < 11);

	int Write_Offset = 0;

	if (0 > Integer)
	{
		Write_Offset = 1;
		Out_Buffer[0] = '-';
	}

	//copy digits in reverse with offset
	for (int Itr_Chr_Rev = 0; Itr_Chr_Rev < Itr_Chr; Itr_Chr_Rev++)
	{
		Out_Buffer[Itr_Chr_Rev + Write_Offset] = Internal_Buffer[(Itr_Chr - 1) - Itr_Chr_Rev];
	}

#ifndef NDEBUG
	int DBG_Out_Int = -99;
	Parse_Int_Save(Out_Buffer, &DBG_Out_Int, Write_Offset + Itr_Chr);
	Utils_Assert(Integer == DBG_Out_Int);
#endif // !NDEBUG

	return;
}



const char G_CONST_Hex_Digits[] = { '0','1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', };

/*
.Chars = hex digits of { Byte&11110000, Byte&00001111 }
*/
char_array_2 Byte_To_Hex(unsigned char Byte)
{
	return (char_array_2)
	{
		.Chars =
		{
			[0] = G_CONST_Hex_Digits[(Byte & 0xF0) >> 4],	//11110000
			[1] = G_CONST_Hex_Digits[Byte & 0xF ],	//00001111
		}
	};
}



const char G_CONST_Escape_Sequences[2][10] =
{
	{'a',	'n',	't',	'r',	'b',	'\\',	'"',	'\'',	'%',	'0'},
	{'\a',	'\n',	'\t',	'\r',	'\\',	'\\',	'"',	'\'',   '%',	'\0'},
};

/*
Returns -1 if escape code not found.

\[%]

returned as (int)'%'
*/
int Escaped_Char_To_Char(char Char)
{
	for (size_t Itr_Chr = 0; Itr_Chr < sizeof(G_CONST_Escape_Sequences[0]); Itr_Chr++)
	{
		if (Char == G_CONST_Escape_Sequences[0][Itr_Chr])
			return (int)G_CONST_Escape_Sequences[1][Itr_Chr];
	}

	return -1;
}



void StO_Print(const char* const String)
{
	for (int Itr_Chr = 0; '\0' != String[Itr_Chr]; Itr_Chr++) STD_OUT_SEND_CHAR(String[Itr_Chr]);
}



void StD_Print(const char* const String)
{
	for (int Itr_Chr = 0; '\0' != String[Itr_Chr]; Itr_Chr++) STD_DEBUG_SEND_CHAR(String[Itr_Chr]);
}



void StO_Print_S(const char* const String, int String_Len)
{
	for (int Itr_Chr = 0; '\0' != String[Itr_Chr] && Itr_Chr < String_Len; Itr_Chr++) STD_OUT_SEND_CHAR(String[Itr_Chr]);
}



void StO_Print_V(const char* const Format_String, arg_cnt N_Of_Variadic_Args)
{
	Utils_Assert(NULL != Format_String);
	Utils_Assert(VARIADIC_ARG_STACK_LEN >= N_Of_Variadic_Args);
	Utils_Assert(VARIADIC_ARG_STACK_LEN >= G_VARIADIC_ARG_STACK.Top_P1);

	Pop_Go_Back_Variadic_Args(&G_VARIADIC_ARG_STACK, N_Of_Variadic_Args);

	arg_cnt Used_Args_Cnt = 0;
	for (int Itr_Chr = 0; '\0' != Format_String[Itr_Chr]; Itr_Chr++)
	{
		//printf("Chr: '%c'\n", Format_String[Itr_Chr]);
		//string format specifier \%s, string: `%s`\n
		//                        ^
		if ('\\' == Format_String[Itr_Chr])
		{
			Itr_Chr++;
			if ('\0' == Format_String[Itr_Chr]) break;
			STD_OUT_SEND_CHAR(Format_String[Itr_Chr]);
		}
		else if ('%' == Format_String[Itr_Chr])
		{
			Itr_Chr++;
			if ('\0' == Format_String[Itr_Chr]) break;

			if (Used_Args_Cnt == N_Of_Variadic_Args)
			{
				StO_Print("<not enough variadic args pushed>");
				continue;
			}

			variadic_arg V_Arg = UNSAFE_Pop_Reverse_Variadic_Arg(&G_VARIADIC_ARG_STACK);
			Used_Args_Cnt++;
			//StO_Print_F(SIZE_MAX, "t: %i\n", V_Arg.Type.Type_Enum);
			//%i
			// ^
			switch (Format_String[Itr_Chr])
			{

			case 'd':
			case 'i':
				if (int_e != V_Arg.Type.Type_Enum)
				{
					StO_Print("<expected int, got different type>");
				}
				else if (NULL == V_Arg.Ptr)
				{
					StO_Print("<*int null>");
				}
				else
				{
					char Str_Buffer[FORMAT_INT_AS_STR_OUT_BUFFER_SIZE] = { 0 };
					Format_Int_As_Str(*(int*)V_Arg.Ptr, Str_Buffer);
					StO_Print(Str_Buffer);
				}
				break;

			case 's':
				if (string_e != V_Arg.Type.Type_Enum)
				{
					StO_Print("<expected string, got different type>");
				}
				else if (NULL == V_Arg.Ptr)
				{
					StO_Print("<string null>");
				}
				else
				{
					StO_Print(V_Arg.Ptr);
				}
				break;

			case 'c':
				if (char_e != V_Arg.Type.Type_Enum)
				{
					StO_Print("<expected char[1], got different type>");
				}
				else if (NULL == V_Arg.Ptr)
				{
					StO_Print("<*(char[1]) null>");
				}
				else
				{
					STD_OUT_SEND_CHAR(*(char*)V_Arg.Ptr);
				}
				break;

			default:
				StO_Print("<invalid format specifier>");
			}
		}
		else
		{
			STD_OUT_SEND_CHAR(Format_String[Itr_Chr]);
		}
	}

	//since arguments are poped in revers:
	// a b [c] d e
	// a b c [d] e
	// a b c d [e]
	//we need to walk back again:
	// [] a b c d e
	Pop_Go_Back_Variadic_Args(&G_VARIADIC_ARG_STACK, N_Of_Variadic_Args);
}
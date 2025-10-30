//Created 2025.10.11 11:53 as "Utils_Variadics.c"

#include "Utils.h"



#define VARIADIC_ARG_STACK_ERR (variadic_arg) { .Ptr = NULL, .Type = { .Type_Enum = no_type_err_e, .Length = 0 }, .Name = NULL }



short CHECK_Is_Int(const int I) { return 0 & I; }
short CHECK_Is_Int_Array(const int Final_Int, const short Len) { return 0 & Final_Int & Len; }
short CHECK_Is_Variable_Size_Int_Array(const char First_Byte, const char Final_Byte, const short Len) { return 0 & First_Byte & Final_Byte & Len; }
short CHECK_Is_Char(const char Char) { return 0 & Char; };
short CHECK_Is_String(const char* String) { return 0 & (intptr_t)String; };
short CHECK_Is_Char_Array(const char* Char_Ptr, const char Final_Char, const short Len) { return 0 & (intptr_t)Char_Ptr & Final_Char & Len; };



arg_cnt Push_Variadic_Arg(variadic_arg_stack* Stack, variadic_arg Arg)
{
	if (VARIADIC_ARG_STACK_LEN == Stack->Top_P1) 
		return 0;

	Stack->Arguments_Array[Stack->Top_P1] = Arg;
	Stack->Top_P1++;

	return 1;
}



variadic_arg Pop_Variadic_Arg(variadic_arg_stack* Stack)
{
	if (0 == Stack->Top_P1) return  VARIADIC_ARG_STACK_ERR;

	Stack->Top_P1--;

	return Stack->Arguments_Array[Stack->Top_P1];
}



variadic_arg UNSAFE_Pop_Reverse_Variadic_Arg(variadic_arg_stack* Stack)
{
	if (Stack->Top_P1 == VARIADIC_ARG_STACK_LEN) return VARIADIC_ARG_STACK_ERR;

	const arg_cnt Old_Top = Stack->Top_P1;
	Stack->Top_P1++;

	return Stack->Arguments_Array[Old_Top];
}



bool Pop_Go_Back_Variadic_Args(variadic_arg_stack* Stack, arg_cnt Walk_Back_Cnt)
{
	const int New_Top_P1 = Stack->Top_P1 - Walk_Back_Cnt;

	if (New_Top_P1 < 0)
	{
		/*
		Lol, I missed that still have to reset Top_P1, caused problems when an incorrect number of
		args was passed to StO_Print_V.
		*/
		Stack->Top_P1 = 0;
		return false;
	}

	Stack->Top_P1 = New_Top_P1;

	return true;
}

//created 2025.10.18 12:17 as Utils_Errors.c
#include "Utils.h"



unsigned short G_Next_Error_Number = 0;



utl_error PROC_Error(const error_enum Error_Type, arg_cnt N_Of_Variadic_Args)
{
	Utils_Assert(0 <= N_Of_Variadic_Args);

	StD_Print("ERROR: ");
	char Formated_Int_Out_Buffer[FORMAT_INT_AS_STR_OUT_BUFFER_SIZE] = { 0 };
	Format_Int_As_Str(G_Next_Error_Number, Formated_Int_Out_Buffer);
	StD_Print(Formated_Int_Out_Buffer);
	StD_Print(" : ");
	StD_Print(G_CONST_Error_Strings[Error_Type]);
	StD_Print("\n");

	for (arg_cnt Itr_Variadic_Arg = 0; Itr_Variadic_Arg < N_Of_Variadic_Args; Itr_Variadic_Arg++)
	{
		variadic_arg This_Variadic_Argument = Pop_Variadic_Arg(&G_VARIADIC_ARG_STACK);
		StD_Print(This_Variadic_Argument.Name);
		StD_Print(" : ");
		
		switch (This_Variadic_Argument.Type.Type_Enum)
		{

		case int_e:
			if (NULL == This_Variadic_Argument.Ptr)
			{
				StD_Print("<*int null>");
			}
			else
			{
				char Str_Buffer[FORMAT_INT_AS_STR_OUT_BUFFER_SIZE] = { 0 };
				Format_Int_As_Str(*(int*)This_Variadic_Argument.Ptr, Str_Buffer);
				StD_Print(Str_Buffer);
			}
			break;

		case string_e:
			if (NULL == This_Variadic_Argument.Ptr)
			{
				StD_Print("<string null>");
			}
			else
			{
				StD_Print(This_Variadic_Argument.Ptr);
			}
			break;

		case char_e:
			if (NULL == This_Variadic_Argument.Ptr)
			{
				StD_Print("<*(char[1]) null>");
			}
			else
			{
				STD_OUT_SEND_CHAR(*(char*)(This_Variadic_Argument.Ptr));
			}
			break;

		case var_size_int_array_e:
			if (NULL == This_Variadic_Argument.Ptr)
			{
				StD_Print("<*(var_size_int_array) null>");
			}
			else
			{
				DBG_Display_Int_Member_Array(This_Variadic_Argument.Ptr, 
					This_Variadic_Argument.Type.Element_Size, 
					This_Variadic_Argument.Type.Stride, 
					This_Variadic_Argument.Type.Length);
			}
			break;

		default:
			StO_Print("<invalid format specifier>");
		}

		StD_Print("\n");
	}

	G_Next_Error_Number++;
	return (utl_error) { .Number = G_Next_Error_Number - 1, .V = Error_Type, };
}
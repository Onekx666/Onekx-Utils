//Created 2025.10.07 18:44 as "Utils_Debug_Display.c"
#include <stdio.h>

#include <assert.h>

#include "Utils.h"



void DBG_Display_Index_String_Multi_Line(char* String, size_t Str_Len, size_t Index)
{
	Utils_Assert(NULL != String);

	StO_Print_V("Chr: %i\nString:\n", P_INT(String[Index]));

	for (size_t Itr_Chr = 0; Itr_Chr < Str_Len; Itr_Chr++)
	{
		if (Itr_Chr == Index)
		{
			STD_OUT_SEND_CHAR('~');
			STD_OUT_SEND_CHAR(String[Itr_Chr]);
			STD_OUT_SEND_CHAR('~');
		}
		else
		{
			STD_OUT_SEND_CHAR(String[Itr_Chr]);
		}

	}

	STD_OUT_SEND_CHAR('\n');

}
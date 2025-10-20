//Created 2025.10.07 18:44 as "Utils_Debug_Display.c"
#include <stdio.h>
#include <limits.h>

#include <assert.h>

#include "Utils.h"
#include "Utils_Config.h"



void DBG_Display_Index_String_Multi_Line(char* String, size_t Str_Len, size_t Index)
{
	Utils_Assert(NULL != String);

	StO_Print_V("Chr: %i\nString:\n", P_INT(String[Index]));

	for (size_t Itr_Chr = 0; Itr_Chr < Str_Len; Itr_Chr++)
	{
		if (Itr_Chr == Index)
		{
			STD_DEBUG_SEND_CHAR('~');
			STD_DEBUG_SEND_CHAR(String[Itr_Chr]);
			STD_DEBUG_SEND_CHAR('~');
		}
		else
		{
			STD_DEBUG_SEND_CHAR(String[Itr_Chr]);
		}

	}

	STD_DEBUG_SEND_CHAR('\n');

}



void DBG_Display_Int_Member_Array(const void* const First_Member, int Member_Size, int Stride, int Len)
{
	Utils_Assert(0 <= Len);
	Utils_Assert(1 <= Stride);
	Utils_Assert(1 <= Member_Size);
	Utils_Assert(Member_Size <= Stride);
	Utils_Assert(!(0 < Len) || NULL != First_Member);

	for (int Itr_Struct = 0; Itr_Struct < Len; Itr_Struct++)
	{
		STD_DEBUG_SEND_CHAR('\t');

		const int This_Member_Offset = Stride * Itr_Struct;
		int Integer = 0;

		switch (Member_Size)
		{
			case sizeof(char) :
				Integer = *(char*)((char*)First_Member + This_Member_Offset);
				goto print_as_base_10_l;

			case sizeof(short) :
				Integer = *(short*)((char*)First_Member + This_Member_Offset);
				goto print_as_base_10_l;

			case sizeof(int) :
				Integer = *(int*)((char*)First_Member + This_Member_Offset);
				goto print_as_base_10_l;

			default:
				//Print as hexadecimal.
				StD_Print("0x");

#ifdef CONFIG_LITLE_ENDIAN 
				for (int Itr_Integer_Byte_Print = Member_Size-1; Itr_Integer_Byte_Print >= 0; Itr_Integer_Byte_Print--)
				{
					const char_array_2 Chars = Byte_To_Hex(*((unsigned char*)First_Member + This_Member_Offset + Itr_Integer_Byte_Print));
					STD_DEBUG_SEND_CHAR(Chars.Chars[0]);
					STD_DEBUG_SEND_CHAR(Chars.Chars[1]);
				}
#endif

#ifdef CONFIG_BIG_ENDIAN
				for (int Itr_Integer_Byte_Print = 0; Itr_Integer_Byte_Print < Member_Size; Itr_Integer_Byte_Print++)
				{
					const char_array_2 Chars = Byte_To_Hex(*((unsigned char*)First_Member + This_Member_Offset + Itr_Integer_Byte_Print));
					STD_DEBUG_SEND_CHAR(Chars.Chars[1]);
					STD_DEBUG_SEND_CHAR(Chars.Chars[0]);
				}
#endif
				continue;
		}

	print_as_base_10_l:
		(void)0;
		char Out_Buffer[FORMAT_INT_AS_STR_OUT_BUFFER_SIZE] = { 0 };
		Format_Int_As_Str(Integer, Out_Buffer);
		for (int Itr_Chr_Print = 0; '\0' != Out_Buffer[Itr_Chr_Print]; Itr_Chr_Print++)
			STD_DEBUG_SEND_CHAR(Out_Buffer[Itr_Chr_Print]);
	}

}
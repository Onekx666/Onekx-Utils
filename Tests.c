//Created before 2025.10.07 1/:12 as Tests.c

/*

______2025.10.07______

	16:45 to 19:50


______2025.10.09______

	19:11 to 20:26


______2025.10.10______

	10:40 to 11:35
	11:43 to 14:50


______2025.10.11______

	11:49 to 12:05
	12:10 to 13:27
	13:43 to 14:32
	16:10 to 16:53


______2025.10.13______

	19:44 to 19:56


______2025.10.14______

	14:23 to --:--


______2025.10.17______

	09:08 to 10:03
ca. 11:55 to 13:16
	13:21 to 15:38
	16:21 to --:--


_____2025.10.19_______

ca. 11:30 to 14:37


_____2025.10.20_______

	17:26 to 20:21

	library should be minimally operational now.

*/



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <Windows.h>

#include "Utils.h"



variadic_arg_stack G_Variadic_Arg_Stack = { 0 };



void TEST_String_Seeking_Functions(void)
{
	printf("\n\nTEST_String_Seeking_Functions():\n\n");

	char String_A[] = "This is a string";
	char Sub_String_A[] = "This";

	if (true != SubStr_Str_S(String_A, sizeof(String_A) - 1, Sub_String_A, sizeof(Sub_String_A) - 1)) printf("SubStr_Str_S() test 1 FAILED\n");

	if (false != SubStr_Str_S(Sub_String_A, sizeof(Sub_String_A) - 1, String_A, sizeof(String_A) - 1)) printf("SubStr_Str_S() test 2 FAILED\n");

	if (true != SubStr_Str_NT(String_A, Sub_String_A)) printf("SubStr_Str_NT() test 1 FAILED\n");

	if (false != SubStr_Str_NT(Sub_String_A, String_A)) printf("SubStr_Str_NT() test 2 FAILED\n");

	printf("\n");



	char String_Space_B[] = "<c>     <param>";
	int Crr_Chr = 3;
	Crr_Chr += Seek_Space_End_S(String_Space_B + Crr_Chr, INT_MAX);
	printf("Crr_Chr: %i\n", Crr_Chr);
	DBG_Display_Index_String_Multi_Line(String_Space_B, strlen(String_Space_B), Crr_Chr);

	if ('<' != String_Space_B[Crr_Chr]) printf("DBG_Display_Index_String_Multi_Line() test 1 FAILED\n");

	printf("\n");

	char String_Space_C[] = "\
[\"obj\"] =\n\
{\n\
	[1] = 66,\n\
	[2] = 41\n\
}";

	Crr_Chr = 7;
	Crr_Chr += Seek_Space_End_S(String_Space_C + Crr_Chr, INT_MAX);
	printf("Crr_Chr: %i\n", Crr_Chr);
	DBG_Display_Index_String_Multi_Line(String_Space_C, strlen(String_Space_C), Crr_Chr);

	if ('=' != String_Space_C[Crr_Chr]) printf("DBG_Display_Index_String_Multi_Line() test 2 FAILED\n");

	int Res = Str_Len_NT("4444");
	printf("\nStr_Len_NT(\"4444\"): %i", Res);

	Res = Str_Len_S("4444", 8);
	printf("\nStr_Len_S(\"4444\", 8): %i", Res);

	Res = Str_Len_S("22--", 2);
	printf("\nStr_Len_S(\"22--\", 2): %i\n", Res);
}



void TEST_Parsing_Function(void)
{
	printf("\n\nTEST_Parsing_Function():\n\n");

	int Int_V = 0;

	int Res = Parse_Int_Save("-5555", &Int_V, 5);
	printf("Parse_Int_Save(\"-5555\", &Int_V, 5): Success: %i Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("-    12346789", &Int_V, 13);
	printf("Parse_Int_Save(\"-    12346789\", &Int_V, 1): Success: %i %i\n", Res, Int_V);

	Res = Parse_Int_Save("+1", &Int_V, 2);
	printf("Parse_Int(\"+1\", &Int_V, 2): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("9", &Int_V, 1);
	printf("Parse_Int(\"9\", &Int_V, 1): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("-", &Int_V, 1);
	printf("Parse_Int(\"-\", &Int_V, 1): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("+", &Int_V, 1);
	printf("Parse_Int(\"+\", &Int_V, 1): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("   ", &Int_V, 3);
	printf("Parse_Int(\"   \", &Int_V, 3): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("   45", &Int_V, 5);
	printf("Parse_Int(\"   45\", &Int_V, 5): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("09090", &Int_V, 5);
	printf("Parse_Int(\"09090\", &Int_V, 5): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("2147483647", &Int_V, 10);
	printf("Parse_Int(\"2147483647\", &Int_V, 10): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("-2147483648", &Int_V, 11);
	printf("Parse_Int(\"-2147483648\", &Int_V, 11): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("2147483648", &Int_V, 10);
	printf("Parse_Int(\"2147483648\", &Int_V, 10): Success: %i  Number: %i\n", Res, Int_V);

	Res = Parse_Int_Save("-2147483649", &Int_V, 11);
	printf("Parse_Int(\"-2147483649\", &Int_V, 11): Success: %i  Number: %i\n", Res, Int_V);
}



void TEST_Formatting_Functions(void)
{
	printf("\n\nTEST_Formatting_Functions():\n\n");

	char Out_Buffer[FORMAT_INT_AS_STR_OUT_BUFFER_SIZE] = { 0 };
	Format_Int_As_Str(INT_MIN, Out_Buffer);
	printf("Format_Int_As_Str(INT_MIN, Out_Buffer): Res: %s\n", Out_Buffer);

	memset(Out_Buffer, 0, FORMAT_INT_AS_STR_OUT_BUFFER_SIZE);
	Format_Int_As_Str(-4, Out_Buffer);
	printf("Format_Int_As_Str(-4, Out_Buffer): Res: %s\n", Out_Buffer);

	memset(Out_Buffer, 0, FORMAT_INT_AS_STR_OUT_BUFFER_SIZE);
	Format_Int_As_Str(0, Out_Buffer);
	printf("Format_Int_As_Str(0, Out_Buffer): Res: %s\n", Out_Buffer);

	memset(Out_Buffer, 0, FORMAT_INT_AS_STR_OUT_BUFFER_SIZE);
	Format_Int_As_Str(2147483647, Out_Buffer);
	printf("Format_Int_As_Str(2147483647, Out_Buffer): Res: %s\n", Out_Buffer);

	memset(Out_Buffer, 0, FORMAT_INT_AS_STR_OUT_BUFFER_SIZE);
	Format_Int_As_Str(6, Out_Buffer);
	printf("Format_Int_As_Str(6, Out_Buffer): Res: %s\n", Out_Buffer);
}



void TEST_Variadic_Arguments(void)
{
	printf("\n\nTEST_Variadic_Arguments():\n\n");

	StO_Print_V("Name: %s section: %c Age: %i some numbers: %i %i %i %i %i\n",
		P_STR("Joe") +
		P_CHR((char) { 'Q' }) +
		P_INT((int) { -19 }) +
		P_INT((int) { -20 }) +
		P_INT((int) { -21 }) +
		P_INT((int) { -22 }) +
		P_INT((int) { -23 }) +
		P_INT((int) { -24 }) +
		P_INT((int) { -25 }));

	StO_Print_V("2*\\%i: %d %i \\%c: %c \\%s: %s \\n not available: %i\\n", P_INT((int) { 12 }) + P_INT((int) { 24 }) + P_CHR((char) { 'i' }) + P_STR("hi"));

	StO_Print_V("Name: %s section: %c Age: %i some numbers: %i %i %i %i %i\n",
		P_STR("Joe") +
		P_CHR((char) { 'Q' }) +
		P_INT((int) { -19 }) +
		P_INT((int) { -20 }) +
		P_INT((int) { -21 }) +
		P_INT((int) { -22 }) +
		P_INT((int) { -23 }) +
		P_INT((int) { -24 }));
}



void TEST_StI_Console_Read_Update(void)
{
	printf("\n\nTEST_StI_Console_Read_Update():\n\n");

	console_line_buffer Con_Line_Buffer = {0};

	while (true)
	{
		command Cmd = StI_Console_Read_Update(&Con_Line_Buffer);

		Sleep(20);

		if (cmd_none_e == Cmd.Type) continue;
		//We end up here even if parsing the command failed.

		if (cmd_set_gpio_e == Cmd.Type)
		{
			StO_Print_V("set gpio pin: %i to %d\n", P_INT(Cmd.Command_Union.Command_Set_GPIO.GPIO_Id) + P_INT(Cmd.Command_Union.Command_Set_GPIO.Value));
		}

		StO_Print(CONSOLE_NEW_LINE_STR);
	}

}



struct test_struct_1
{
	intptr_t V;
	int I;
	int S;
	char A;
	char B;
	char Chars[19];
};

void TEST_DBG_Display_Int_Member_Array_AND_TEST_PROC_Error(void)
{
	StO_Print("\n\nTEST DBG_Display_Int_Member_Array():\n\n");

	struct test_struct_1 Array[] = 
	{ 
		{.B = 'A',	.V = 660,		.S = -1 },
		{.B = 'A',	.V = 661,		.S = -2 },
		{.B = 'A',	.V = INT_MAX,	.S = -3 },
		{.B = 'A',	.V = INT_MIN,	.S = -4 }, 
		{.B = 'A',	.V = 664,		.S = -5 }, 
	};

	DBG_Display_Int_Member_Array(&(Array[0].V), sizeof(Array[0].V), sizeof(Array[0]), 5);

	STD_DEBUG_SEND_CHAR('\n');
	STD_DEBUG_SEND_CHAR('\n');

	DBG_Display_Int_Member_Array(&(Array[0].B), sizeof(Array[0].B), sizeof(Array[0]), 5);

	StO_Print("\n\nTEST PROC_Error():\n\n");

	const char* String = "a string";

	utl_error Err = PROC_Error(error_test_e,
		P_IARR(&(Array[0].S), sizeof(Array[0]), 5) +
		P_INT((int) { 7 }) +
		P_CHR((Array[0].B)) +
		P_STR(String)
	);
	StO_Print_V("Err .Number: %i .Type: %i\n\n", P_INT(Err.Number) + P_INT(Err.V));

	Err = PROC_Error(error_test2_e,
		P_IARR(&(Array[0].V), sizeof(Array[0]), 5) +
		P_INT((int) { 7 }) +
		P_CHR((Array[0].B)) +
		P_STR(String)
	);
	StO_Print_V("Err .Number: %i .Type: %i\n", P_INT(Err.Number) + P_INT(Err.V));

	
}



int main(void)
{
	TEST_String_Seeking_Functions();
	TEST_Parsing_Function();
	TEST_Formatting_Functions();
	//EXPERIMENT_Variadic_Arguments();
	TEST_Variadic_Arguments();
	TEST_DBG_Display_Int_Member_Array_AND_TEST_PROC_Error();

	return 0;
}



/*
thoughts on multi threaded error streams:


Thread 1:

		mutex locks stream A

		-- threat 2 tries to write to A --

		sets msg id (A) atomically to 3

		writes "Err 56 : ..."

		done writing

		mutex unlocks stream A


threat 2:

		fails to mutex lock stream A

		mutex locks stream B

		reads msg id of stream A as 2

		sets msg id (B) atomically to 3

		Writes "Err 333 : ..."

		-- threat 1 sets msg id (A) to 3 --

		done writing

		mutex unlocks stream B



THERE NEED TO BE AS MANY SEPERATE STREAMS AS THREATS.

I have shown that this is possible, although this solution is clunky it would work.
It would be possible to approximately tell the order off errors, which is good enough,
since multithreading means that I cannot guarantee a correct order of messages anyway.

stream A:

	most recent msg: 3

	msg id				msg

	1					...

	2					...

	3					Err 56 : ...


stream B:

	msg Id				msg

	3					Err 333 : ...


stream C:
*/
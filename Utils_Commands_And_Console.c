#include "Utils.h"



const command_entry G_CONST_Command_Entry_Table[] =
{
	[cmd_none_e] =
	{
		.Arguments_Entry_Array = NULL,
		.Argument_Array_Len = 0,
		.Name = "CMD_NONE",
		.Name_Len = 8,
		.Help_Description = "The none command \"CMD_NONE\" is a place holder when any diagnostic indicates its name this might indicate an error.\nThe command takes no arguments and does nothing",
	},

	/*[cmd_help_e] =
	{
		.Arguments_Entry_Array = NULL,
		.Argument_Array_Len = 0,
		.Name = "help",
		.Name_Len = 4,
		.Help_Description = "help <command name> displays help messages, like this one on the command whose name is given in the brackets.",
	},*/

	[cmd_set_gpio_e] =
	{
		.Arguments_Entry_Array = (cmd_arg_entry[2])
		{
			{.Arg_Type = int_e, .Member_Offset = (int)(intptr_t) & (((command_set_gpio*)NULL)->GPIO_Id)},
			{.Arg_Type = int_e, .Member_Offset = (int)(intptr_t) &(((command_set_gpio*)NULL)->Value) },
		},
		.Argument_Array_Len = 2,
		.Name = "setgpio",
		.Name_Len = 7,
		.Help_Description = "setGPIO <pin id> <value> sets the pin with <pin id> either to either on or off depending on value of <value>. 0 : off, !0 : on"
	},

	[cmd_shutdown_e] =
	{
		.Arguments_Entry_Array = NULL,
		.Argument_Array_Len = 0,
		.Name = "shutdown",
		.Name_Len = 8,
		.Help_Description = "shutdown shuts down the raspberry pi."
	},

	[cmd_reboot_e] =
	{
		.Arguments_Entry_Array = NULL,
		.Argument_Array_Len = 0,
		.Name = "reboot",
		.Name_Len = 6,
		.Help_Description = "reboot restarts the raspberry pi."
	},
};



bool Parse_Args_S(const char* const String, const int Str_Len, const cmd_arg_entry* Arg_Entry_Array, const int N_Of_Args, command_types_union* const Target_Struct_Ptr)
{
	Utils_Assert(NULL != String);
	Utils_Assert(0 <= Str_Len);
	Utils_Assert(!(0 < N_Of_Args) || NULL != Arg_Entry_Array);
	Utils_Assert(0 <= N_Of_Args);
	Utils_Assert(!(0 < N_Of_Args) || Arg_Entry_Array[N_Of_Args - 1].Arg_Type < c_type_enum_max);
	Utils_Assert(!(0 < N_Of_Args) || Arg_Entry_Array[N_Of_Args - 1].Arg_Type >= 0);
	Utils_Assert(NULL != Target_Struct_Ptr);

	int Crr_Chr = 0;
	//12 1 "jeff"
	//^

	//for each argument parse the input as that arguments type
	for (int Itr_Command_Arg = 0; Itr_Command_Arg < N_Of_Args; Itr_Command_Arg++)
	{
		//12   1    "jeff"
		//  ^
		Crr_Chr += Seek_Space_End_S(&String[Crr_Chr], Str_Len - Crr_Chr);
		//12   1    "jeff"
		//     ^

		const cmd_arg_entry This_Arg_Entry = Arg_Entry_Array[Itr_Command_Arg];
		//12   1    "jeff"
		//^ or ^ or ^

		switch (This_Arg_Entry.Arg_Type)
		{
		case int_e:

			(void)0;
			int Res = Parse_Int_Save(&String[Crr_Chr], (int*)(((char*)Target_Struct_Ptr) + This_Arg_Entry.Member_Offset), Str_Len - Crr_Chr);
			if (-1 == Res)
				return false;
			//12   1    "jeff"
			//^
			Crr_Chr += Res;
			//12   1    "jeff"
			//  ^
			break;

		default:
			return false;
		}
	}

	return true;
}



command StI_Console_Read_Update(console_line_buffer* Line_Buffer)
{
	Utils_Assert(NULL != Line_Buffer);
	Utils_Assert(0 <= Line_Buffer->Chars_Got_Cnt);
	Utils_Assert(CONSOLE_LINE_BUFFER_LEN > Line_Buffer->Chars_Got_Cnt);

	command Ret_Command = { .Type = cmd_none_e, };

	int Std_In_Chr = 0;
	while (-1 != (Std_In_Chr = STD_IN_GET_CHAR()))
	{
		if (CONSOLE_LINE_BUFFER_LEN - 1 == Line_Buffer->Chars_Got_Cnt)
		{
			StO_Print_V("To many chars send. Line buffer length is: %i", P_INT((int) { CONSOLE_LINE_BUFFER_LEN }));
			Ret_Command.Type = cmd_parsing_failed_e; //so that in the external code `:: ` or equilvant can be sent
			goto ret_cmd_processed_l;
		}

		Line_Buffer->Buffer[Line_Buffer->Chars_Got_Cnt] = (char)Std_In_Chr;
		Line_Buffer->Chars_Got_Cnt++;
		//StO_Print_V("got chr: %i\n", P_INT(Std_In_Chr));

		if (CONSOLE_DELETE_PREV_CHAR == Std_In_Chr)
		{
			//    v
			//hell-.....-----
			//got: \b
			//     v
			//hell-.....-----
			if (1 < Line_Buffer->Chars_Got_Cnt)
			{
				Line_Buffer->Chars_Got_Cnt -= 2;
				STD_OUT_SEND_CHAR(Std_In_Chr);
			}
			else
			{
				// v
				//-----.....
				Line_Buffer->Chars_Got_Cnt = 0;
				//v
				//-----.....
			}
			//hell-.....-----
			//   ^
		}
		else if (CONSOLE_RETURN_CHAR == Std_In_Chr)
		{
			Line_Buffer->Buffer[Line_Buffer->Chars_Got_Cnt - 1] = '\0';
			STD_OUT_SEND_CHAR('\n');

			/***********************
			*    parse command     *
			***********************/

			//setgpio 12 1
			//^

			//for all names of commands, find command with matching name
			const command_entry* Found_Command = NULL;
			for (int Itr_Cmd_Type = 0; Itr_Cmd_Type < cmd_e_command_cnt; Itr_Cmd_Type++)
			{
				if (SubStr_Str_S(Line_Buffer->Buffer, CONSOLE_LINE_BUFFER_LEN, G_CONST_Command_Entry_Table[Itr_Cmd_Type].Name, COMMAND_NAME_MAX_LEN))
				{
					Found_Command = &G_CONST_Command_Entry_Table[Itr_Cmd_Type];
					Ret_Command.Type = Itr_Cmd_Type;
					break;
				}
			}

			if (NULL == Found_Command)
			{
				StO_Print("Command not found!\n");
				Ret_Command.Type = cmd_parsing_failed_e;
				goto ret_cmd_processed_l;
			}
			else
			{
				//setgpio 12 1
				//       ^idx passed to Parse_Args_S() 
				bool Res = Parse_Args_S(&Line_Buffer->Buffer[Found_Command->Name_Len],
					Line_Buffer->Chars_Got_Cnt - Found_Command->Name_Len,
					Found_Command->Arguments_Entry_Array,
					Found_Command->Argument_Array_Len,
					&Ret_Command.Command_Union);

				if (false == Res)
				{
					StO_Print("Invalid Args!\n");
					Ret_Command.Type = cmd_parsing_failed_e;
					goto ret_cmd_processed_l;
				}

				goto ret_cmd_processed_l;
			}

		}
		else
		{
			STD_OUT_SEND_CHAR(Std_In_Chr);
		}
	}

	return (command) { .Type = cmd_none_e, };

ret_cmd_processed_l:
	Line_Buffer->Chars_Got_Cnt = 0;
	return Ret_Command;
}
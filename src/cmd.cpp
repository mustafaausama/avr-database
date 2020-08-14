/*
 * cmd.cpp
 *
 * Created: 8/14/2020 5:21:10 AM
 *  Author: Usama Mustafa
 */ 

#include "cmd.h"



/*
 *
 * This function parses the given command (argv) and calls the appropriate function.
 * Should be run in a while(1) loop for continuous operation.
 * 
 * The command currently implemented are:
 * 		user	Performs operations on user database.
 * 		lcd	Grants access to LCD hardware.
 *  		clear	Clears the terminal window.	
 * 
 * The command 'user' has the following format.
 * Usage: user [-option(s)]
 * The options are:
 * 		-l		--login		Authenticate the user with an ID and Password.
 * 		-a		--add		Add a user to the database. (Requires admin privileges)
 * 		-d		--delete	Delete the provided user entry from database. (Requires admin privileges)
 * 		-s		--show		Show the entire database from EEPROM. (Requires admin privileges)
 * 
 * The command 'lcd' has the following format.
 * Usage: lcd [-option(s)] [argument(s)]
 * The options are:
 * 		-s		--clear		Clear the LCD screen.
 * 		-p		--print		Print the provided phrase [argument] on LCD screen.
 * 		-l		--line		Switch the cursor to second line.
 * 		-b		--blink		[on/off] as argument for cursor blink.
 * 		-c		--cursor	[on/off] as argument for the cursor.
 * 
 * Note that the default login for Admin access is:
 * 		ID:	1234
 * 		PW:	1234
 * 
 */
void CMD::parse()
{
	CMD::pgm_printf(prompt);
	
	/* Reads the command line from user and breaks down to first word. */
	char* argv = (char*)SIO::scanf();
	char* token = strtok(argv, " ");
	
	/* The rest is simple conditional statements for each command. */
	if ((strcmp(token, "exit")==0)|(strcmp(token, "")==0))
	{
		return;
	}
	if (strcmp(token, "help")==0)
	{
		CMD::pgm_printf(help_prompts[0]);
		CMD::pgm_printf(help_prompts[1]);
		CMD::pgm_printf(help_prompts[2]);
		CMD::pgm_printf(help_prompts[3]);
		return;
	}
	
	else if (strcmp(token, "user")==0)
	{
		token = strtok(NULL, " ");
		if ((strcmp(token, "--help")==0)|(strcmp(token, "-h")==0))
		{
			CMD::pgm_printf(user_prompts[0]);
			CMD::pgm_printf(user_prompts[1]);
			CMD::pgm_printf(user_prompts[2]);
			CMD::pgm_printf(user_prompts[3]);
			CMD::pgm_printf(user_prompts[4]);
			CMD::pgm_printf(user_prompts[5]);
			CMD::pgm_printf(user_prompts[6]);
		}
		else if ((strcmp(token, "--login")==0)|(strcmp(token, "-l")==0))
		{
			CMD::User_Login();
		}
		else if ((strcmp(token, "--add")==0)|(strcmp(token, "-a")==0))
		{
			CMD::User_Add();
		}
		else if ((strcmp(token, "--delete")==0)|(strcmp(token, "-d")==0))
		{
			CMD::User_Delete();
		}
		else if ((strcmp(token, "--show")==0)|(strcmp(token, "-s")==0))
		{
			CMD::User_Show();
		}
		else
		{
			CMD::pgm_printf(u_help);
		}
	}
	else if(strcmp(token, "lcd")==0)
	{
		token = strtok(NULL, " ");
		if ((strcmp(token, "--help")==0)|(strcmp(token, "-h")==0))
		{
			CMD::pgm_printf(lcd_prompts[0]);
			CMD::pgm_printf(lcd_prompts[1]);
			CMD::pgm_printf(lcd_prompts[2]);
			CMD::pgm_printf(lcd_prompts[3]);
			CMD::pgm_printf(lcd_prompts[4]);
			CMD::pgm_printf(lcd_prompts[5]);
			CMD::pgm_printf(lcd_prompts[6]);
			CMD::pgm_printf(lcd_prompts[7]);
			
		}
		else if ((strcmp(token, "--clear")==0)|(strcmp(token, "-s")==0))
		{
			LCD::command(CLEAR);
			_delay_ms(2.5);
		}
		else if ((strcmp(token, "--print")==0)|(strcmp(token, "-p")==0))
		{
			token = strtok(NULL, " ");
			LCD::print(token);
		}
		else if ((strcmp(token, "--line")==0)|(strcmp(token, "-l")==0))
		{
			LCD::command(NEXTLINE);
		}
		else if ((strcmp(token, "--blink")==0)|(strcmp(token, "-b")==0))
		{
			token = strtok(NULL, " ");
			if ((strcmp(token, "on")==0)|(strcmp(token, "On")==0)|(strcmp(token, "ON")==0))
			{
				LCD::command(0b00001111);
			}
			else if ((strcmp(token, "off")==0)|(strcmp(token, "Off")==0)|(strcmp(token, "OFF")==0))
			{
				LCD::command(0b00001110);
			}
			else
			{
				CMD::pgm_printf(b_help);
			}
		}
		else if ((strcmp(token, "--cursor")==0)|(strcmp(token, "-c")==0))
		{
			token = strtok(NULL, " ");
			if ((strcmp(token, "on")==0)|(strcmp(token, "On")==0)|(strcmp(token, "ON")==0))
			{
				LCD::command(0b00001110);
			}
			else if ((strcmp(token, "off")==0)|(strcmp(token, "Off")==0)|(strcmp(token, "OFF")==0))
			{
				LCD::command(0b00001100);
			}
			else
			{
				CMD::pgm_printf(c_help);
			}
		}
		else
		{
			CMD::pgm_printf(l_help);
		}
	}
	else if (strcmp(token, "clear")==0)
	{
		UART::Send(CLC);
	}
	else
	{
		SIO::printf("\'");
		SIO::printf(token);
		CMD::pgm_printf(err_1);
		CMD::pgm_printf(err_2);
	}
	free(token);
	free(argv);	
}

/*
 * Is used in CMD::parse() to authenticate and login the user.
 * The user is logged in based on the ID and Password stored in 
 * the EEPROM data base.
 */
void CMD::User_Login()
{
	CMD::pgm_printf(msc_1);
	long ID = atol(SIO::scanf());
	User use = DB::Read(ID*LOAD_OFFSET);
	
	if ((use.ID == 0xFF) | (use.ID > 63))
	{
		CMD::pgm_printf(msc_2);
		return;
	}
	
	CMD::pgm_printf(msc_3);
	long PW = atol(SIO::_scanf());

	if (use.authenticate(PW))
	{
		CMD::pgm_printf(msc_4);
		SIO::printf(use);
		DB::display(use);
	}
	else
	{
		CMD::pgm_printf(msc_5);
		UART::Send(BELL);
	}
}

/* 
 * Adds the user to the database. This function requires Admin privileges because
 * this function can actually overwrite and potentially destroy data.
 * 
 * The ID and Password for admin is "1234".
 */
void CMD::User_Add()
{
	if (CMD::Admin())
	{
		CMD::pgm_printf(msc_6);
		long ID = atol(SIO::scanf());
		User check = DB::Read(ID*LOAD_OFFSET);
		
		if (check.ID != 0xFF)
		{
			CMD::pgm_printf(msc_7);
			byte* choice = (byte*)SIO::scanf();
			if ((strcmp((const char*)choice, "Y")==0) | (strcmp((const char*)choice, "Yes")==0) | (strcmp((const char*)choice, "y")==0) | (strcmp((const char*)choice, "yes")==0) | (strcmp((const char*)choice, "YES")==0))
			{
				CMD::pgm_printf(msc_8);
				long PW = atol(SIO::_scanf());
								
				byte Rec = 0x00;
				
				CMD::pgm_printf(msc_9);
				int i=0;
				byte DT[10];
				do
				{
					Rec = (byte)UART::Receive();
					if (Rec == '\r')
					{
						DT[i] = '\0';
						break;
					}
					DT[i] = Rec;
					UART::Send(DT[i]);
					i++;
				}
				while(i < 10);
				
				User use(ID, PW, DT);
				
				SIO::printf("\r");

				DB::Write(use);				
				return;
			}
			else
			{
				return;
			}
		}
		else
		{
			CMD::pgm_printf(msc_10);
			long PW = atol(SIO::_scanf());
			
			byte Rec = 0x00;
			
			CMD::pgm_printf(msc_11);
			int i=0;
			byte DT[10];
			do
			{
				Rec = (byte)UART::Receive();
				if (Rec == '\r')
				{
					DT[i] = '\0';
					break;
				}
				DT[i] = Rec;
				UART::Send(DT[i]);
				i++;
			}
			while(i < 10);
			
			User use(ID, PW, DT);
			
			SIO::printf("\r");

			DB::Write(use);
			return;
		}
	}
	else
	{
		CMD::pgm_printf(msc_12);
		return;
	}
}

/*
 * This function deletes the given user ID. This function also requires Admin
 * privileges because this function does not ask for password before deleting 
 * the user entry from the database.
 * 
 * The ID and Password for admin is "1234".
 */
void CMD::User_Delete()
{
	if (CMD::Admin())
	{
		CMD::pgm_printf(msc_13);
		long ID = atol(SIO::scanf());
		User use(0xFF, 0xFFFFFFFF, (byte*)"NULL");
		use.ADDRESS = ID*LOAD_OFFSET;
		DB::Write(use);
		
		CMD::pgm_printf(msc_14);
		SIO::printf(ID);
		CMD::pgm_printf(msc_15);
	}
	else
	{
		CMD::pgm_printf(msc_16);
	}
}

/*
 * This function shows all the user data in database. This function requires admin privileges
 * because this function also shows the passwords for all the users along with their data and ID.
 * 
 * The ID and Password for admin is "1234".
 */
void CMD::User_Show()
{
	if (CMD::Admin())
	{
		CMD::pgm_printf(msc_17);
		User use;
		int i=0;
		do 
		{
			use = DB::Read(i*LOAD_OFFSET);
			if(use.ID == 0xFF)
			{
				i++;
				continue;
			}
			SIO::printf("\r");
			SIO::printf(use);
			CMD::pgm_printf(msc_18);
			SIO::printf(use.get_PW());
			SIO::printf("\r");
			i++;
		} while (i<64);
	}
	else
	{
		CMD::pgm_printf(msc_19);
	}
}

/*
 * Checks to see whether or not Admin privileges can be granted based
 * on and admin ID and Password.
 * 
 * The ID and Password for admin is "1234".
 */
bool CMD::Admin()
{
	CMD::pgm_printf(msc_20);
	long ID = atol(SIO::scanf());
	CMD::pgm_printf(msc_21);
	long PW = atol(SIO::_scanf());
	
	if ((ID == ADMIN_ID) & (PW == ADMIN_PW))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMD::pgm_printf(const char* entry)
{
	for (unsigned int i=0; i<strlen(entry); i++)
	{
		UART::Send(pgm_read_byte(&entry[i]));
	}
}

/*
 * cmd.h
 *
 * Created: 8/14/2020 5:20:27 AM
 *  Author: Usama Mustafa
 */ 


#ifndef CMD_H_
#define CMD_H_

#include "User.h"
#include "serialio.h"
#include "eepio.h"
#include "lcd.h"

#include <avr/pgmspace.h>

#define ADMIN_ID 1234
#define ADMIN_PW 1234


/*
 * Since there is very limited amount of SRAM present in our ATMega328P and
 * we have used a lot of string in our applications. The SRAM will easily reach
 * 100 percent capacity. For this reason all of our constant strings are stored
 * in program memory and pointer to program memory is stored in SRAM instead of
 * the whole string. This method is much more efficient and stable.
 */

const char prompt[] PROGMEM = "cmd@avr:~$ ";

const char help_1[] PROGMEM = "The commands are:\r";
const char help_2[] PROGMEM = "    user    Performs operations on user database.\r";
const char help_3[] PROGMEM = "    lcd     Grants access to LCD hardware.\r";

const char user_1[] PROGMEM = "Usage: user [-option(s)]\r";
const char user_2[] PROGMEM = "Performs operations on user database.\r";
const char user_3[] PROGMEM = "The options are:\r";
const char user_4[] PROGMEM = "    -l    --login    Authenticate the user with an ID and Password.\r";
const char user_5[] PROGMEM = "    -a    --add      Add a user to the database. (Requires admin privileges)\r";
const char user_6[] PROGMEM = "    -d    --delete   Delete the provided user entry from database. (Requires admin privileges)\r";
const char user_7[] PROGMEM = "    -s    --show     Show the entire database from EEPROM. (Requires admin privileges)\r";

const char lcd_1[]  PROGMEM = "Usage: lcd [-option(s)] [argument(s)]\r";
const char lcd_2[]  PROGMEM = "Grants access to LCD hardware.\r";
const char lcd_3[]  PROGMEM = "The options are:\r";
const char lcd_4[]  PROGMEM = "    -s    --clear    Clear the LCD screen.\r";
const char lcd_5[]  PROGMEM = "    -p    --print    Print the provided phrase on LCD screen.\r";
const char lcd_6[]  PROGMEM = "    -l    --line     Switch the cursor to second line.\r";
const char lcd_7[]  PROGMEM = "    -b    --blink    [on/off] as argument for cursor blink.\r";
const char lcd_8[]  PROGMEM = "    -c    --cursor   [on/off] as argument for the cursor.\r";

const char u_help[] PROGMEM = "Type \'user --help\' or \'user -h\' for usage details.\r";
const char l_help[] PROGMEM = "Type \'lcd --help\' or \'lcd -h\' for usage details.\r";
const char b_help[] PROGMEM = "Type \'lcd --blink on\' to turn on and \'lcd --blink off\' to turn off the cursor blink.\r";
const char c_help[] PROGMEM = "Type \'lcd --cursor on\' to turn on and \'lcd --cursor off\' to turn off the lcd cursor.\r";

const char err_1[]  PROGMEM = "\' is not recognized as a command.\r";
const char err_2[]  PROGMEM = "Type \'help\' for an overview of all the commands.\r";

const char msc_1[]  PROGMEM = "Enter User ID: ";
const char msc_2[]  PROGMEM = "User does not exist.\r";
const char msc_3[]  PROGMEM = "Enter User Password: ";
const char msc_4[]  PROGMEM = "Authentication Complete.\r";
const char msc_5[]  PROGMEM = "Authentication Failed.\r";
const char msc_6[]  PROGMEM = "Enter User ID between 0 and 63: ";
const char msc_7[]  PROGMEM = "User already exits. Overwrite? (y) / (n): ";
const char msc_8[]  PROGMEM = "Enter User Password: ";
const char msc_9[]  PROGMEM = "Enter User Data: ";
const char msc_10[] PROGMEM = "Enter User Password: ";
const char msc_11[] PROGMEM = "Enter User Data: ";
const char msc_12[] PROGMEM = "Not an admin.\r";
const char msc_13[] PROGMEM = "Enter User ID to be deleted: ";
const char msc_14[] PROGMEM = "User ";
const char msc_15[] PROGMEM = " is deleted.\r";
const char msc_16[] PROGMEM = "Not an admin.\r";
const char msc_17[] PROGMEM = "User Database:\r";
const char msc_18[] PROGMEM = "Password: ";
const char msc_19[] PROGMEM = "Not an admin.\r";
const char msc_20[] PROGMEM = "Enter Admin ID: ";
const char msc_21[] PROGMEM = "Enter Admin Password: ";

PGM_P const help_prompts[] PROGMEM =
{
	help_1,
	help_2,
	help_3
};

PGM_P const user_prompts[] PROGMEM =
{
	user_1,
	user_2,
	user_3,
	user_4,
	user_5,
	user_6,
	user_7
};

PGM_P const lcd_prompts[] PROGMEM =
{
	lcd_1,
	lcd_2,
	lcd_3,
	lcd_4,
	lcd_5,
	lcd_6,
	lcd_7,
	lcd_8
};


/*
 * Contains a very simple command line interface to debug and
 * perform operations on database and LCD hardware.
 */
namespace CMD
{
	/*
	 * This function should be run when a command is to be entered.
	 */
	void parse();
	
	/*
	 * Internal functions for void parse. However, can be called by
	 * user in main().
	 */
	void User_Login(void);
	void User_Add(void);
	void User_Delete(void);
	void User_Show(void);
	
	void pgm_printf(const char*);
	
	/*
	 * Returns whether user is admin or not
	 */
	bool Admin(void);
}



#endif /* CMD_H_ */
/*
 * lcd.cpp
 *
 * Created: 8/13/2020 5:18:55 AM
 *  Author: Usama Mustafa
 */ 

#include "lcd.h"


/*
 *
 * Initializes the LCD by setting the initial boot sequence and set-up 
 * of LCD.
 * 
 * The default set-up is
 * Display: 				On
 * Cursor:  				Off
 * Blink:   				Off
 * Font:    				5x8
 * Interface Data Length:	4-bit
 * Line Mode:				2-line
 * 
 * The four LCD data bits are [PD4, PD5, PD6, PD7]
 * The three LCD control bits are [PB0, PB1, PB2]
 * 
 */ 
void LCD::Init()
{
	DDRD = 0xF0;
	DDRB = 0x07;
	
	PORTD = FUNCTION_SET;
	_toggle_control_command();
	_delay_ms(1);
	LCD::command(FUNCTION_SET);
	_delay_ms(1);
	LCD::command(DISPLAY_ON);
	_delay_ms(1);
	LCD::command(ENTRY_MODE);
	_delay_ms(1);
}

/*
 * Executes the provided command on the LCD.
 */
void LCD::command(byte comm)
{
	PORTD = (comm & 0xF0);
	_toggle_control_command();
	PORTD = (comm & 0x0F) << 4;
	_toggle_control_command();
}

/*
 * Displays a single byte character on LCD.
 */
void LCD::display(byte character)
{
	LCD::_check_bf();
	PORTD = (character & 0xF0);
	_toggle_control_display();
	PORTD = (character & 0x0F) << 4;
	_toggle_control_display();
}

/*
 * Displays an array of characters on LCD.
 */
void LCD::print(const char* array)
{
	/* Move the array to a disposable variable. */
	const char* buffer = array;
	
	/* Output the array character by character. */
	for (int i=0; *(buffer+i) != '\0'; i++)
	{
		LCD::display(*(buffer+i));
	}/*
	LCD::command(NEXTLINE);*/
}

/*
 * Displays an integer number on LCD
 */
void LCD::print(int number)
{
	/* Allocate memory for a int no more than 10 digits long. */
	char* buffer = (char*)malloc(sizeof(char)*10);
	
	/* Conversion to string. */
	LCD::print(itoa(number, buffer, 10));
	
	/* Cleanup */
	free(buffer);
}

/*
 * Displays a long number on lCD
 */
void LCD::print(long number)
{
	/* Allocate memory for a int no more than 10 digits long. */
	char* buffer = (char*)malloc(sizeof(char)*10);
	
	/* Conversion to string. */
	LCD::print(ltoa(number, buffer, 10));
	
	/* Cleanup */
	free(buffer);
}

/*
 * Displays a double or float number on LCD.
 */
void LCD::print(double number)
{
	/* Allocate memory for a double no more than 9 digits long. */
	char* buffer = (char*)malloc(sizeof(char)*10);
	
	/* Conversion to string. */
	LCD::print(dtostrf(number, 10, 4, buffer));
	
	/* Cleanup */
	free(buffer);
}

inline void LCD::_toggle_control_command()
{
	PORTB = 0;
	_delay_us(DELAY);
	PORTB = E;
	_delay_us(DELAY);
	PORTB = 0;
	_delay_us(DELAY);
}

inline void LCD::_toggle_control_display()
{
	PORTB = RS;
	_delay_us(DELAY);
	PORTB = RS|E;
	_delay_us(DELAY);
	PORTB = RS;
	_delay_us(DELAY);
}

/* 
 * Checks the busy flag from LCD. Does not return the function
 * as long as the LCD is busy
 */
inline void LCD::_check_bf()
{
	byte BF = 0x00;
	do
	{
		PORTD = 0x00;
		DDRD = 0x00;
		PORTB = RW;
		_delay_us(DELAY);
		PORTB = RW|E;
		_delay_us(DELAY);
		PORTB = RW;
		_delay_us(DELAY);
		BF = PIND>>7;
		PORTB = RW;
		_delay_us(DELAY);
		PORTB = RW|E;
		_delay_us(DELAY);
		PORTB = RW;
		_delay_us(DELAY);
	}
	while(!BF);
	DDRD = 0xF0;
}

inline void LCD::_clear()
{
	LCD::command(CLEAR);
	_delay_ms(2.5);
}

/*
 * Displays the inputted User data object on LCD. Does not display
 * passwords.
 */
void DB::display(User use)
{
	LCD::_clear();
	LCD::print("ID: ");
	LCD::print(use.ID);
	LCD::command(NEXTLINE);
	LCD::print("DATA: ");
	LCD::print((const char*)use.DATA);
}
/*
 * lcd.h
 *
 * Created: 8/11/2020 5:41:09 AM
 *  Author: Usama Mustafa
 */ 



#ifndef LCD_H_
#define LCD_H_

#define RS 0b100
#define RW 0b010
#define E  0b001

#define CLEAR        0b00000001
#define FUNCTION_SET 0b00101000
#define DISPLAY_ON   0b00001100
#define ENTRY_MODE   0b00000110
#define NEXTLINE     0b11000000

#define DELAY 20

#include "User.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

namespace LCD
{
	/* Internal functions for LCD namespace.
	   Should not be called by user in main(). */
	inline void _toggle_control_command(void);
	inline void _toggle_control_display(void);
	inline void _check_bf(void);
	inline void _clear();
	
	/* Set-up functions */
	void Init(void);
	void command(byte);
	void display(byte);

	/* Display functions */
	void print(const char*);
	void print(int);
	void print(long);
	void print(double);
}

namespace DB
{
	/* Displays the provided User object from database onto LCD */
	void display(User);
}



#endif /* LCD_H_ */
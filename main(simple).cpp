/*
 * main.cpp
 *
 * Created: 8/11/2020 2:00:12 AM
 * Author : Usama Mustafa
 */ 

#include "User.h"
#include "serialio.h"
#include "eepio.h"
#include "lcd.h"
#include "cmd.h"

#include <avr/io.h>
#include <stdlib.h>
#include <avr/eeprom.h>


using namespace SIO;

int main(void)
{
	LCD::Init();
	UART::Init(UBRR);

	while(1)
	{
		long ID = atol(scanf("Enter your ID: "));
		printf("Enter your Password: ");
		long PW = atol(_scanf());
			
		User use = DB::Read(ID*LOAD_OFFSET);
			
		if (use.authenticate(PW))
		{
			printf("Authentication Complete.\r");
			printf(use);
			DB::display(use);
		}
		else
		{
			printf("Authentication Failed.\r");
			UART::Send(BELL);
		}
		printf("Press Enter to try again.");
		scanf();
		UART::Send(CLC);
	}
}


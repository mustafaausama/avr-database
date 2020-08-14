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
		CMD::parse();
	}
}


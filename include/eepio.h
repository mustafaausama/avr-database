/*
 * eepio.h
 *
 * Created: 8/11/2020 10:04:50 AM
 *  Author: Usama Mustafa
 */ 


#ifndef EEPIO_H_
#define EEPIO_H_

#include "User.h"
#include "serialio.h"
#include <avr/io.h>
#include <stdlib.h>


/*
 * This namespace reads/writes directly to/from EEPROM, one byte
 * at a time.
 */
namespace EEP
{
	void Write(unsigned int address, byte data);
	byte Read(unsigned int address);
}

/*
 * This namespace reads/writes User object to EEPROM
 * at its appropriate address place. Refer to definitions of these
 * functions for more detail (eepio.cpp).
 */
namespace DB
{
	void Write(User);
	User Read(unsigned int address);
}

#endif /* EEPIO_H_ */
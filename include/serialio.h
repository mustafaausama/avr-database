/*
 * serialio.h
 *
 * Created: 8/11/2020 2:04:02 AM
 *  Author: Usama Mustafa
 */ 



#ifndef SERIALIO_H_
#define SERIALIO_H_

#define BELL 0x07
#define CLC  0x0C
#define TAB  0x09

#include "User.h"
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

/*
 * Low level functions to deal with UART module of 
 * ATMega328P. Most likely will not be used in main()
 * except Init method.
 */
namespace UART
{
	void Init(unsigned int ubrr);
	byte Receive(void);
	void Send(byte data);
}

/*
 * Receives and sends a variety of data types. Note that for scanf 
 * functions data conversions must be performed later if expecting
 * user to enter an integer as the function will only return a string
 * of ASCII characters.
 */
namespace SIO
{
	void printf(User use);
	void printf(const char* array);
	void printf(double number);
	void printf(int number);
	void printf(long number);
	
	const char* scanf();
	const char* _scanf();
	const char* scanf(const char* array);
}

#endif /* SERIALIO_H_ */
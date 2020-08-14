/*
 * eepio.cpp
 *
 * Created: 8/11/2020 10:05:01 AM
 *  Author: Usama Mustafa
 */ 

#include "eepio.h"



/* 
 * This is a low level function to write the contents of
 * EEPROM one byte at a time. This is implemented according
 * to the data sheet of ATMega328P.
 * Important thing to note is that the use of interrupts
 * is not suitable in our design because of the linear nature
 * of our design. i.e. the program will not move forward as 
 * long as user does not enter their credentials. This is by
 * design.
 */
void EEP::Write(unsigned int address, byte data)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));

	/* Set up address and Data Registers */
	EEAR = address;
	EEDR = data;
	
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

/* 
 * This is a low level function to read the EEPROM when provided
 * with an address. This function is not aware of User data structure
 * and will not read that. For reading User data structure refer to
 * Read function in DB namespace.
 */
byte EEP::Read(unsigned int address)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));

	/* Set up address register */
	EEAR = address;
	
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	
	/* Return data from Data Register */
	return EEDR;
}

/* 
 * This is a high level function which will store the given User object
 * in EEPROM in its appropriate address location with proper spaces for
 * each data types.
 */
void DB::Write(User use)
{
	/* ID storage */
	EEP::Write(use.ADDRESS+ID_OFFSET, use.ID);

	/* Password storage */
	long PW = use.get_PW();
	EEP::Write(use.ADDRESS+PW_OFFSET, PW);
	EEP::Write(use.ADDRESS+PW_OFFSET+1, PW>>8);
	EEP::Write(use.ADDRESS+PW_OFFSET+2, PW>>16);
	EEP::Write(use.ADDRESS+PW_OFFSET+3, PW>>24);
	
	/* Data storage */
	for (int i=0; i<10; i++)
	{
		EEP::Write(use.ADDRESS+DATA_OFFSET+i, use.DATA[i]);
	}
	
}

/* 
 * This is a high level function which will read and return
 * a User object present at the given address. Note that if 
 * a User object is not present on the given address then this
 * function will give a User object with garbage values as all
 * its features. So only those Users should be accessed that 
 * are stored in data base either manually or through excel and
 * python provided alongside this code base.
 */ 
User DB::Read(unsigned int address)
{
	/* ID read */
	long ID = EEP::Read(address+ID_OFFSET);
	
	/* Password read */
	long PW0 = EEP::Read(address+PW_OFFSET);
	long PW1 = EEP::Read(address+PW_OFFSET+1);
	long PW2 = EEP::Read(address+PW_OFFSET+2);
	long PW3 = EEP::Read(address+PW_OFFSET+3);
	long PW = PW0+(PW1<<8)+(PW2<<16)+(PW3<<24);

	/* User object initialization for return value */
	User use;
	use.ID = ID;
	use.set_PW(PW);
	
	/* Data read */
	for (int i=0; i<10; i++)
	{
		use.DATA[i] = (byte)EEP::Read(address+DATA_OFFSET+i);
	}

	/* User return */
	return use;
}
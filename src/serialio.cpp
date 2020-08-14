/*
 * serialio.cpp
 *
 * Created: 8/11/2020 2:04:15 AM
 *  Author: Usama Mustafa
 */ 

# include "serialio.h"


/*
 * This function initializes the UART module of ATMega328P with
 * given value of UBRR. Note that UBRR is different for simulation 
 * and hardware. So if compiling of either, User.h must be checked 
 * before to ensure the macros are set correctly.
 */
void UART::Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (byte)(ubrr>>8);
	UBRR0L = (byte)ubrr;
	
	/*set double speed operation to reduce Baud rate Error*/
	UCSR0A |= (1<<U2X0);
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 2stop bit */
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}

/* 
 * Returns only one byte of data received by the MCU.
 * This is done using polling and interrupts are not used.
 */
byte UART::Receive()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	
	/* Get and return received data from buffer */
	return UDR0;
}

/* 
 * This sends ony byte of data from the MCU.
 */
void UART::Send(byte data)
{ 
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	   
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/*
 * This overload of printf function takes an integer and 
 * prints it on the terminal. If unsigned integer is used
 * then it must be casted to (int) before entering this
 * function.
 */
void SIO::printf(int number)
{
	/* Allocate memory for a int no more than 10 digits long. */
	char* buffer = (char*)malloc(sizeof(char)*10);
	
	/* Conversion to string. */
	SIO::printf(itoa(number, buffer, 10));
	
	/* Cleanup */
	free(buffer);
}

/* 
 * This overload of printf function takes a long data type and
 * prints it on the terminal.
 */
void SIO::printf(long number)
{
	/* Allocate memory for a int no more than 10 digits long. */
	char* buffer = (char*)malloc(sizeof(char)*10);
	
	/* Conversion to string. */
	SIO::printf(ltoa(number, buffer, 10));
	
	/* Cleanup */
	free(buffer);
}

/*
 * This overload of printf function takes a double data type and
 * prints it on the terminal. If float is to be displayed then it
 * must be cast to a (double) before passing to this function.
 */
void SIO::printf(double number)
{
	/* Allocate memory for a double no more than 9 digits long. */
	char* buffer = (char*)malloc(sizeof(char)*10);
	
	/* Conversion to string. */
	SIO::printf(dtostrf(number, 10, 4, buffer));
	
	/* Cleanup */
	free(buffer);
}

/* 
 * This overload of printf is most used because it takes an array 
 * of ASCII characters as input. Note that the carriage return must
 * be added at the end of string to go to next line.
 */
void SIO::printf(const char* array)
{
	/* Move the array to a disposable variable. */
	const char* buffer = array;
	
	/* Output the array character by character. */
	for (int i=0; *(buffer+i) != '\0'; i++)
	{
		UART::Send(*(buffer+i));
	}
	/*UART::Send(ENDL);*/
}

void SIO::printf(User use)
{
	printf("ID: ");
	printf(use.ID);
	printf("\r");
	printf("Data: ");
	printf((const char*)use.DATA);
	printf("\r");
}

/*
 * This overload of scanf function takes a string from user.
 * This function will not return as long as the user has not
 * typed 20 characters or the user has not pressed line break
 * (Enter) key.
 */
const char* SIO::scanf()
{
	/* Allocate the memory and initialize it with zeros so 
	   as to stop the corruption of data and the mix-up with
	   previously stored data. */
	char* buffer = (char*)calloc(20, sizeof(char));
	int i = 0;
	byte REC;
	
	/* Takes the characters one by one and stores them in 
	   the buffer. The entered character is also sent back 
	   so that the user can see what they typed. */
	do
	{
		REC = UART::Receive();
		if (REC==ENDL) 
		{	
			UART::Send(ENDL);
			break;
		}
		UART::Send(REC);
		*(buffer+i) = REC;
		i++;
	}
	while(i < 20);
	return buffer;
}

/*
 * This function is different from the standard scanf function is 
 * such a manner that it will not send back the characters that the
 * user typed. Instead this function sends back aestrisks (*) in the
 * place of every character entered making this function suitable for
 * entering passwords or other protected information.
 */
const char* SIO::_scanf()
{
	/* Allocate the memory and initialize it with zeros so 
	   as to stop the corruption of data and the mix-up with
	   previously stored data. */
	char* buffer = (char*)calloc(20, sizeof(char));
	int i = 0;
	byte REC;

	/* Takes the characters one by one and stores them in 
	   the buffer. Sends back aestrisks (*). */
	do
	{
		REC = UART::Receive();
		if (REC==ENDL)
		{
			UART::Send(ENDL);
			break;
		}
		
		UART::Send('*');
		*(buffer+i) = REC;
		i++;
	}
	while(i < 20);
	return buffer;
}

/*
 * This is an overload of standard scanf function as it takes a string
 * as input. This function is suitable for asking the user a question and
 * expecting a response in one single function call.
 */
const char* SIO::scanf(const char* array)
{
	SIO::printf(array);
	return SIO::scanf();
}

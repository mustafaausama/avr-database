/*
 * User.h
 *
 * Created: 8/11/2020 5:42:20 AM
 *  Author: Usama Mustafa
 */ 


#ifndef COMPDIR_H_
#define COMPDIR_H_

#define LOAD_OFFSET 0x0010
#define ID_OFFSET 0
#define PW_OFFSET 1
#define DATA_OFFSET 5 

/* Comment the following line if compiling for Hardware */
#define SIMULATION

#ifdef SIMULATION
	//#define F_CPU 8000000UL
	//#define UBRR 103
	#define F_CPU 1000000UL
	#define UBRR 12
	#define BAUD 9600
	#define ENDL '\r'
#else
	#define F_CPU 16000000UL
	#define UBRR 16
	#define BAUD 115200
	#define ENDL 0x0D
#endif


typedef unsigned char byte;


/* 
 * Definition of User data structure. This data structure
 * is crucial for proper storage optimization of EEPROM
 * memory space as each User will be alloted same amount
 * of space for their ID, Password and Data. Address is 
 * calculated on the basis of User ID. The format of this
 * structure in the 16 data bytes in EEPROM record entry
 * is as follows.
 * 
 * ID:			[0] 	byte(s)
 * Password:	[1:4] 	byte(s)
 * DATA:		[5:14]	byte(s)
 * Null Byte:	[15]	byte(s)
 * 
 *           ____________________________________________________________________________
 * bytes:	|0 | 1    2     3    4 |  5    6    7    8   9    10   11   12   13   14 | 15|
 * data:	|ID|PW0, PW1,  PW2, PW3| DT0, DT1, DT2, DT3, DT4, DT5, DT6, DT7, DT8, DT9| /0|
 *          |__|___________________|_________________________________________________|___|
 * 
 * This data is stored in little endian format.
 * The choice of number of bits allocated to each User features is important.
 * ->	ID was alloted only one byte because address of User data entry is generated
 * 		according to User ID. And we have only 1024 bytes of EEPROM in ATMega328P. So
 * 		this means if each User data entry takes 16 bytes then we can only initialize 
 * 		User ID up to 63. So this limits the number of Users that can be implemented 
 * 		on EEPROM (64 Users only).
 * ->	Password was alloted four bytes because password is 8 digits long. and to 
 * 		represent 8 decimal digits 27 binary bits are needed. So we use 32 bits to
 * 		represent the password field.
 * ->	Data as per design requirement must be 10 bytes.
 * ->	An extra zero byte is added so that if user data is a string of ASCII characters
 * 		a null terminator is good to have.
 * 
 */
struct User
{
	// ID and Password declaration.
	long ID;
private:
	long PW;
public:

	// This DATA array contains the zero byte at the end.
	byte DATA[11];
	
	unsigned int ADDRESS;
	
	/* 
	 * This overload of the constructor simply initializes
	 * the object without setting any values. These vales
	 * can be later set.
	 */
	User()
	{
		DATA[10] = 0x00;
	}
	
	/* 
	 * This overload of the constructor sets the ID, Password
	 * and DATA of the object. This data is entered when creating
	 * the object. It is not written to the EEPROM yet. That can be
	 * done by using Write function in DB namespace (eepio.h).
	 */
	User(long id, long pw, byte *data)
	{
		ID = id;
		PW = pw;
		for (int i=0; i<10; i++)
		{
			DATA[i] = *(data+i);
		}
		DATA[10] = 0x00;
		ADDRESS = id * (LOAD_OFFSET);
	}
	
	/*
	 * Authenticates the user without giving password
	 * away to outside scope.
	 */
	bool authenticate(long entered_password)
	{
		if (entered_password == PW)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/*
	 * Encapsulation for private PW member variable.
	 * Setter and Getter methods.
	 */
	const void set_PW(long password)
	{
		PW = password;
	}
	long get_PW()
	{
		return PW;
	}
};

#endif /* COMPDIR_H_ */
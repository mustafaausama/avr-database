"""
database.py

This file is a part of semester project of MPSI(CPE342-SP20).
Should be run with a csv file of same name present in 
same directory.

Creates a eep file of Intel Hex format using the provided CSV
file that contains the database. This CSV file must have user
ID in first column, user Passwords in second column and user
data in third column. By default the first row of CSV will be 
ignored.

"""

import csv


def record(ID, PW, DT, reclen='10', rectype='00'):
	"""
	Generates a record entry to be stored in eep file. This entry
	represents one single user object. For more detail on user
	object data structure, refer to the C++ code provided (User.h).
	Function takes user ID, user Password and user data to generate
	this record entry. ID is stored in first one byte (out of 16) 
	and password is stored in subsequent four bytes. Then data is
	stored in ten bytes. One byte is reserved for zero byte. These
	16 bytes are just the data bytes. The whole record contains
	a record length, starting address, record type, data, and 
	checksum. For more detail refer to official documentation of 
	Intel Hex format provided here:
	http://www.precma.it/download/intelhex.pdf
	
	@param: ID:		User ID
	@param: PW:		User Password
	@param: DT:		User Data (Ideally this would be 10 bytes but if
					that's not the case then this function will truncate
					the data accordingly. If the data is less than 10 bytes
					then additional zeros will be added to data.)
	@param:	reclen:	Record Length (This is a constant of 16 bytes. int(16)=hex(10))
	@param: rectype:Record Type (Refers to different types of records in Intel Hex.)

	@return:		A record entry containing everything above and checksum.

	Note that the data is stored in little endian format as per Intel
	Hex requirement.
	"""
	data = ""

	# Address is stored on the basis of user ID. Address starts
	# from 0x0000 and each subsequent user entry increases the 
	# address by 0x0010.
	address = (int(ID) * int(16)) & 0xFFFF
	address = str(hex(address)[2:]).zfill(4).upper()
	ID0 = ID & 0xFF

	# To ensure compatibility with User object defined in C++ code.
	# provided.
	PW0 = PW & 0xFF
	PW1 = PW >> 8 & 0xFF
	PW2 = PW >> 16 & 0xFF
	PW3 = PW >> 24 & 0xFF

	DH = ""

	# Converts the data to hexadecimal.
	for c in DT:
		DH += str(hex(ord(c))[2:]).zfill(2).upper()

	# Truncates or padds zeros to data accordingly.
	if int(len(DH)/2) > 10:
		DH = DH[0:20]
	if int(len(DH)/2) < 10:
		for i in range(10-int(len(DH)/2)):
			DH += '00'

	# An extra zero byte.
	DH+='00'

	# User ID.
	ID0 = str(hex(ID0)[2:]).zfill(2)
	ID = (ID0).upper()

	# User Password.
	PW0 = str(hex(PW0)[2:]).zfill(2)
	PW1 = str(hex(PW1)[2:]).zfill(2)
	PW2 = str(hex(PW2)[2:]).zfill(2)
	PW3 = str(hex(PW3)[2:]).zfill(2)
	PW = (PW0 + PW1 + PW2 + PW3).upper()

	# Pack this in our 16 byte long data entry.
	data += ID
	data += PW
	data += DH

	genrecor = ""

	# Addition of record length, address, record type and data
	# to the actual record entry to be outputted.
	# Only checksum is left at this point.
	genrecor += reclen
	genrecor += address
	genrecor += rectype
	genrecor += data

	# Calculate the checksum for data based on all the entries 
	# included above.
	splt = []
	sum = 0
	for i in range(20):
		splt.append(genrecor[i*2:(i*2)+2])
	for i in splt:
		sum += int(i, 16)
	checksum =  '%X' %(-sum & 0xFF)

	# Adding the checksum to the end of record entry.
	genrecor = genrecor + (str(checksum)).zfill(2)
	genrecor = ':' + genrecor

	# Returns the entry.
	genrecor += '\n'
	return genrecor



if __name__ == '__main__':

	# Import the CSV data.
	file = open('database.csv', 'r')
	reader = csv.reader(file, delimiter = ',')

	# Generate and save Intel Hex in database.eep. This database.eep 
	# will be fed to EEPROM of our ATMega328P. This can be included 
	# in avrdudess software that is used to burn firmware to AVR chips.
	# Note that if the intent is to simulate on Proteus then there is a 
	# .bin file that is required to define the initial contents of EEPROM
	# in Proteus. Refer to database.bat for the command for that.
	# This .py file will not create that .bin file.
	eep  = open("database.eep", "w")
	# hexx = open("database.hex", "w")
	i = 0
	bytes = 0
	print("Intel Hex:")
	for row in reader:
		# Skip the first row of csv.
		if i==0:
			i += 1
			continue

		# Addition of each entry increases the EEPROM utilization by
		# 16 bytes.
		bytes+=16

		# If the memory is exceeded then the rest of the database is
		# not generated.
		if bytes > 1024:
			print('EEPROM memory exceeded. Some data is lost.')
			break

		# Generate the records and save them in appropriate files.
		eep.write(record(ID = int(row[0]), PW = int(row[1]), DT=row[2]+'\0'))
		# hexx.write(record(ID = int(row[0]), PW = int(row[1]), DT=row[2]+'\0'))
		print(record(ID = int(row[0]), PW = int(row[1]), DT=row[2]+'\0'), end='')
		i += 1

	# End of File Record.
	eep.write(":00000001FF")
	eep.close()
	# hexx.write(":00000001FF")
	# hexx.close()
	file.close()
	print(":00000001FF")

	# Print the size of data and utilization of EEPROM.
	print(f'\n{bytes} bytes of data generated for EEPROM.')
	print(f'{(bytes/1024)*100}% memory reached.')

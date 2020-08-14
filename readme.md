# AVR Database

A communication system with password and user ID authentication. UART is used for 
communication with the Terminal and ATMega328P. The user database is stored in
EEPROM. The user ID and Password is entered on the terminal and upon authentication
a ten byte data is displayed on LCD and on terminal. An extended command line interface
approach is also implemented with more control on user data.

## Prerequisites

The softwares required for compilation and deployment are:
```r
WinAVR 20100110     ~= 2.19
Python              ~= 3.7
avrdude             ~= 5.10
Proteus 23525       ~= 8.6
```

## Compilation

Make file is not provided. Make a project in Atmel Studio for ATMega328P and add these code
files and then compile.

## Deployment

Use the provided main(cmd).hex or main(simple).hex files in 'HEX' folder to enter on Proteus simulation software.
For hardware implementation the code will have to be recompiled (User.h). The EEPROM files (.eep for
hardware and .bin for Proteus simulation software) are also provided in 'database' folder.

::
:: database.bat
::
::
:: Must run this script if CSV file is modified. After running
:: this script the .eep and .bin files will be updated.
::
:: Requirement(s):  Python                          ~= 3.7
::                  GNU objcopy (WinAVR 20100110)   ~= 2.19
::
::

@echo off

:: Creates the .eep file. If the intent is to run
:: on hardware then this file (only .eep) is enough.
call python database.py
echo database.eep created.

:: Creates the .bin file. This file is required by Proteus 
:: Simulation Software for simulating the initial contents 
:: of EEPROM.
call avr-objcopy -I ihex -O binary database.eep database.bin
echo database.bin created.

:: Press any key...
pause


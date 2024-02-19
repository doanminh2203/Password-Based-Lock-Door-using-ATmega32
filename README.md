# Password-Based-Door-Lock-Using-ATmega32
# C Programme
1. The locked door using MCU ATmega32 to control entering, checking, and storing password
2. The program has 2 versions, one in C language, and another in Assembly
3. The project is simulated on Proteus and programmed on Microchip Studio
4. The door has a preset password, which is set by the manufacturer, as "1111" stored in the EEPROM of the MCU to ensure the password will not disappear when turned off. The user can choose between 2 modes, entering and changing password. Data can be input from a 4x4 Matrix keypad and using a DC motor to open the door
6. The interrupt0 button is used to adjust the cursor to choose the mode, and the interrupt1 button is used to enter the chosen mode
7. If the user enters the wrong password too many times, the door will be locked

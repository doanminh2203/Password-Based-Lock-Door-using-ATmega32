# Password-Based-Door-Lock-Using-ATmega32

1. The locked door using MCU ATmega32 to control entering, checking, and storing password
2. The program has 2 versions, one in C language, and another in Assembly
3. The project is simulated on Proteus and programmed on Microchip Studio

# C Programme   

- The door has a preset password, which is set by the manufacturer, as "1111" stored in the EEPROM of the MCU to ensure the password will not disappear when turned off. The user can choose between 2 modes, entering and changing password. Data can be input from a 4x4 Matrix keypad and using a DC motor to open the door
- The interrupt0 button is used to adjust the cursor to choose the mode, and the interrupt1 button is used to enter the chosen mode
![image](https://github.com/doanminh2203/Password-Based-Lock-Door-using-ATmega32/assets/153622274/6b7d73d5-906e-48a3-a169-1f59dbdf9016)
- If te user enters the right password, the DC motor will turn counter clock wise to open the door
![image](https://github.com/doanminh2203/Password-Based-Lock-Door-using-ATmega32/assets/153622274/7b86c0be-9a00-4c12-b9e1-e95a8cda2f13)
- If the user enters the wrong password too many times, the door will be locked
![image](https://github.com/doanminh2203/Password-Based-Lock-Door-using-ATmega32/assets/153622274/fd888625-d455-40cd-ab46-8ba4e51b447c)
- In changing password mode, the user will be to enter the old password before changing
![image](https://github.com/doanminh2203/Password-Based-Lock-Door-using-ATmega32/assets/153622274/78f35f6e-7904-4af5-a036-398036b66809)


# Assembly Programme

- The door has a preset password, which is set by the manufacturer, as "12345" stored in the EEPROM of the MCU to ensure the password will not disappear when turned off.
- First of all, the user has to enter the old password to open the door
  ![image](https://github.com/doanminh2203/Password-Based-Lock-Door-using-ATmega32/assets/153622274/b2b1b125-b48a-4518-abc0-e3d895f5d13c)
- If the password is correct, the user can press INT1 to change the password
 ![image](https://github.com/doanminh2203/Password-Based-Lock-Door-using-ATmega32/assets/153622274/8be262d4-ce10-4608-98da-4d0476e9aa35)
- However, the Assembly program hasn't had a delay function already. Therefore, the controller's clock should be set at 100 kHz

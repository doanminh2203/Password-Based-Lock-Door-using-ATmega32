#ifndef F_CPU
#define F_CPU 16000000UL // 8 MHz clock speed
#endif

#define D4 eS_PORTC4
#define D5 eS_PORTC5
#define D6 eS_PORTC6
#define D7 eS_PORTC7

#define RS eS_PORTC0
#define EN eS_PORTC2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "lcd.h"

//declare global variables
volatile int r = 0;
volatile int c = 14;
volatile int mode = 0;

char preset_password[4] = {'1', '1', '1', '1'} ;
char enter_password[4];
char set_password[4];
char new_password[4];
void write_data_eeprom(int address, char arr[])
{
	for (int n = 0; n < 4; n++)
	{
		/* Wait for completion of previous write */
		while(EECR & (1<<EEWE));
		/* Set up address and data registers */
		EEAR = address;
		EEDR = arr[n];
		/* Write logical one to EEMWE */
		EECR |= (1<<EEMWE);
		/* Start eeprom write by setting EEWE */
		EECR |= (1<<EEWE);
		address++;
	}
}
uint8_t read_data_eeprom(int address)
{
	for (int n = 0; n < 4; n++)
	{
		while(EECR & (1<<EEWE));
		/* Set up address register */
		EEAR = address;
		/* Start eeprom read by writing EERE */
		EECR |= (1<<EERE);
		/* Return data from data register */
		set_password[n] = EEDR;
		address++; 
	}
	return EEDR;
}
//keypad interface
char keypad()
{
	unsigned char k_in = 0xFF;
	while(1)
	{
		//scan 1st row
		PORTB = 0xEF;	//select 1st row
		_delay_ms(10);
		k_in = PINB & 0x0F; //check button on what column of 1st row
		switch(k_in)
		{
			case 0x07:
			return ('1');
			case 0x0B:
			return ('2');
			case 0x0D:
			return ('3');
			case 0x0E:
			return ('A');
		}
		
		//scan 2nd row
		PORTB = 0xDF;	//select second row
		_delay_ms(10);
		k_in = PINB & 0x0F;	//check button on what column of 2nd row
		switch(k_in)
		{
			case 0x07:
			return ('4');
			case 0x0B:
			return ('5');
			case 0x0D:
			return ('6');
			case 0x0E:
			return ('B');
		}
		//scan 3rd row
		PORTB = 0xBF; //select 3rd row
		_delay_ms(10);
		k_in = PINB & 0x0F; //check button on what column of 3rd row
		switch(k_in)
		{
			case 0x07:
			return ('7');
			case 0x0B:
			return ('8');
			case 0x0D:
			return ('9');
			case 0x0E:
			return ('C');
		}
		//scan 4th row
		PORTB = 0x7F;	//select 4th row
		_delay_ms(10);
		k_in = PINB & 0x0F;	//check button on what column of 4th row
		switch(k_in)
		{
			case 0x07:
			return ('*');
			case 0x0B:
			return ('0');
			case 0x0D:
			return ('#');
			case 0x0E:
			return ('D');
		}
		
	}
}

void open_the_door()
{
	PORTA = 0x06;
}

void clock_the_door()
{
	PORTA = 0x05;
}

void stop_the_door()
{
	PORTA = 0x03;
}

void entered_password()
{
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("ENTER PASSWORD");
	
	Lcd4_Set_Cursor(2,1);
	for(int i = 0; i < 4; i++)
	{	 
		enter_password[i] =  keypad();
		Lcd4_Write_Char(keypad());
		_delay_ms(500);
	}
}

int check_password()
{
	Lcd4_Clear();
	int k = 0;
	int address = 200;
	//read password from EEPROM
	read_data_eeprom(address);
	//check	password
	Lcd4_Set_Cursor(1,1);
	for(int i = 0; i < 4; i++)
	{
		if (enter_password[i] == set_password[i])
		k++;
	}
	if (k == 4)
	{
		Lcd4_Write_String("PASSWORD RIGHT");
		return 1;
	}
	else
	{
		Lcd4_Write_String("PASSWORD WRONG");
		return 0;
	}
}
void choose_mode()
{		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("1.Change PW");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String("2.Enter PW");

}

int main()
{
	SP = RAMEND;
	// config DDRC for LCD
	DDRC= 0xFF;
	PINC = 0xFF;
	Lcd4_Init();
	//config DDRB for keypad
	DDRB = 0xF0;
	PORTB = 0xFF;
	
	//config DDRA for DC motor
	DDRA = 0xFF;
	//external interrupt initialize
	DDRD = 0x0;
	PORTD = 0xFF;
	MCUCR = 0b00001010; //make INT0 falling edge triggered
	GICR = (1<<INT0)|(1<<INT1);
	sei();


	//set preset password
	int address = 200;

	read_data_eeprom(address);
	uint8_t data;
	data = read_data_eeprom(address);
	if (data == 0xFF)
	{
		write_data_eeprom(address, preset_password);
	}

	//choosing mode
	choose_mode();

	while(1){
	
		switch(mode)
		{
			case 1:	//change password mode
			{
				Lcd4_Clear();
				Lcd4_Set_Cursor(1,1);
				Lcd4_Write_String("CHANGE "); //notify user that you are in change password mode
				Lcd4_Set_Cursor(2,1);
				Lcd4_Write_String("PASSWORD MODE");
				_delay_ms(2000);
			
				Lcd4_Clear();
				Lcd4_Set_Cursor(1,1);
				Lcd4_Write_String("ENTER YOUR");
				Lcd4_Set_Cursor(2,1);
				Lcd4_Write_String("OLD PASSWORD");	//require user to enter old password
				_delay_ms(2000);
			
				//operation loop
				do
				{
					entered_password();
					_delay_ms(500);
					check_password();
					_delay_ms(1000);
				} while (check_password() == 0); //if password wrong, enter again
			
				//set new password
				Lcd4_Clear();
				Lcd4_Set_Cursor(1,1);
				Lcd4_Write_String("NEW PASSWORD");	//notify user that you are in set new password mode

				Lcd4_Set_Cursor(2,1);
				for (int i = 0; i < 4; i++)
				{
					new_password[i] = keypad();
					Lcd4_Write_Char(new_password[i]);
					_delay_ms(500);
				}
				//write new password to eeprom
				address = 200;
				write_data_eeprom(address, new_password);
				read_data_eeprom(address);
				
				Lcd4_Clear();
				_delay_ms(3000);
				Lcd4_Write_String("PASSWORD"); //notify user that password has been changed
				Lcd4_Set_Cursor(2,1);
				Lcd4_Write_String("CHANGED");
				_delay_ms(1000);
				mode = 0;
				choose_mode();
				break;
			case 2:
			{
				int m = 0;
				//notify user that you are in normal mode
				Lcd4_Clear();
				Lcd4_Set_Cursor(1,1);
				//Lcd4_Write_String("NORMAL MODE");
				//_delay_ms(1000);
				Lcd4_Clear();
				//operation loop
				do
				{
					entered_password();
					check_password();
					_delay_ms(1000);
					if (check_password() == 1)
					{
						open_the_door();
						_delay_ms(2000);
						stop_the_door();
					}
					else
					{
						m++;
					}
					if (m >= 4)
					{
						Lcd4_Clear();
						Lcd4_Set_Cursor(1,1);
						Lcd4_Write_String("TRY AGAIN IN");
						Lcd4_Set_Cursor(2,1);
						Lcd4_Write_String("5 MINUTES");
						_delay_ms(5000);
						m = 0;
					}
				} while (check_password() == 0);
				mode = 0;
				choose_mode();
				break;
				}
			default: break;
			}
		}
	}
}

ISR(INT0_vect)
{
	r++;
	if(r == 3) r = 1; 
		_delay_ms(200);
		Lcd4_Set_Cursor(r-1,c);
		Lcd4_Write_String("  ");
		Lcd4_Set_Cursor(r+1,c);
		Lcd4_Write_String("  ");
		Lcd4_Set_Cursor(r,c);
		Lcd4_Write_String("<<");
}

ISR(INT1_vect)
{	
	_delay_ms(200);
	mode = r;
}


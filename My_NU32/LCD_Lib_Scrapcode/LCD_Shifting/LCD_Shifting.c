/*
	Does the LCD natively support text scrolling?
*/

#include "NU32.h"
#include "LCD.h"

#define MAX_USER_INPUT_LEN 100

int main()
{
	char buf[MAX_USER_INPUT_LEN]; 
	
	NU32_Startup(); 	//Cache on, interrupts on, LED/button init, UART init
	LCD_Setup();
	NU32_WriteUART3("Initiation complete\r\n");

	//Modifications to LCD Setup()
	LCD_Entry(1,0);
	LCD_Display(1,0,1);	
	
	while(1)
	{
		//Get user input from serial terminal
		NU32_WriteUART3("Enter a string to display on LCD:\r\n");
		NU32_ReadUART3(buf, MAX_USER_INPUT_LEN); 
		NU32_WriteUART3("Writing ... ");
		NU32_WriteUART3(buf);
		NU32_WriteUART3("\r\n");
		
		//Display on LCD
		LCD_Clear();
		LCD_Move(0,0);
		LCD_WriteString(buf); 
	}
	
	
	return 0; 
}
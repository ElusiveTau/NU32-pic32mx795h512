/* 
	Pinname = Silkscreen name of the pin on the dev board. 

	STATUS LEDS
	============================================================================
	Color 	Pinfunc			Pinname		Configuration
	============================================================================
	GREEN	RB1				B1			Open-drain 
	RED		RB7				B7			Open-drain
	============================================================================
	
	PUSHBUTTONS
	============================================================================
	Name 	Pinfunc			Pinname		Configuration
	============================================================================
	PB1		RD6,CN15		D6			Normally Open & pulled low
	PB2		RC13,CN1		C13			Normally Open & pulled low
	============================================================================
	
	LCD
	============================================================================
	LCDpin	PIC32pin		Pinname
	============================================================================
	VDD		5V
	VSS		GND
	V0		Contrast
	RS		PMA10			B13
	RW		PMRD			D5
	E		PMWR			D4
	D0:D7	PMD0:7			E0:E7
	A		Backlight
	K		GND
	============================================================================
	
*/

/* QUESTIONS
	1. Where can I find the list of C functions implemented by XC32 Compiler?
*/

#include "NU32.h"
#include "LCD.h" 

#define Enable On
#define Disable Off
#define On  1
#define Off 0

#define USE_PB_EXTERN_INT 

#define USE_PB_CN	On
#define MAX_MESSAGE_LENGTH 200
#define LCD_MAX_DIG_WIDTH 16
#define LCD_MSG_LEN 32

/*
The following LAT register bits are configured in Open-Drain Config. They're
attached the LEDs. Therefore, setting the LAT bit turns the LED off and 
clearing the LAT bit (completes the circuit) and turns it on.

	1 = Internal switch is open;   High Impedance 
	0 = Internal switch is closed; Driven to 0V
*/
#define R_TOGGLE(x)	(LATBbits.LATB7 = (x))
#define G_TOGGLE(x) (LATBbits.LATB1 = (x))
#define RLED(x)	((x) == On ? R_TOGGLE(0) : R_TOGGLE(1))
#define GLED(x) ((x) == On ? G_TOGGLE(0) : G_TOGGLE(1))

void init_status_leds(void); 	//Open-Drain output
void init_pushbuttons(void);	//Configures registers for change notification

volatile unsigned int oldD = 0, newD = 0; 
volatile unsigned int oldC = 0, newC = 0; 

void __ISR(_CHANGE_NOTICE_VECTOR, IPL5SOFT) handle_pushbutton(void)
{
	static volatile int triggered_count = 0; 	
	static char buf[LCD_MAX_DIG_WIDTH];
	
	triggered_count++; 
	
	//Location 1. Clear flag
	//IFS1bits.CNIF = 0; //Clear interrupt flag
	//NU32_WriteUART3("Inside ISR");
	
	//Location 2. Clear flag
	IFS1bits.CNIF = 0; 
	
	LCD_Clear();
	LCD_Move(0,0);
	LCD_WriteString("CN:");
	LCD_WriteString(itoa(buf,triggered_count,10));
	
	
	//Read pin states from ports C,D
	newC = PORTC; 
	newD = PORTD;
	
	if( (newC >> 14 & 1) )
		GLED(On); 
	else
		GLED(Off);
	
	//Save new input values
	oldC = newC; 
	oldD = newD; 
	
	NU32_WriteUART3("Done ISR.");
	//Location 3. Clear flag
	//IFS1bits.CNIF = 0; //Clear interrupt flag
}

int main(void) 
{	
	char message[MAX_MESSAGE_LENGTH];
	char LCD_msg[LCD_MSG_LEN];
  
	//Initiation Routines
	init_status_leds();
	NU32_Startup(); 	//cache on, interrupts on, LED/button init, UART init
	LCD_Setup();		//Set up LCD
	init_pushbuttons();
	NU32_WriteUART3("Initiation routines completed...\r\n");
   
  
	while (1) 
	{	  
		NU32_ReadUART3(message, MAX_MESSAGE_LENGTH);  // get message from computer
		NU32_WriteUART3(message);                     // send message back
		NU32_WriteUART3("\r\n");
		
		LCD_Move(1,0);
		LCD_WriteString(message); 
	}
  
  return 0;
}

void init_status_leds(void)
{
	//Designate port pins as outputs
	TRISBbits.TRISB1 = 0; 
	TRISBbits.TRISB7 = 0; 
	
	//Configure the port pins as Open-Drain
	ODCBbits.ODCB1 = 1; 
	ODCBbits.ODCB7 = 1; 
	
	//Turn LEDs off 
	RLED(Off); 
	GLED(Off);
}

void init_pushbuttons(void)
{
	#if USE_PB_CN
	__builtin_disable_interrupts();
	
	//Designate port pins as inputs
	TRISDbits.TRISD6 = 1; 
	TRISCbits.TRISC13 = 1;
	
	//Set up the pushbuttons for CN interrupts p.119
	//Step 3. Configure device 
	CNENbits.CNEN15 = 1; //Enable CN interrupts on CN15 (PB1)
	CNENbits.CNEN1  = 1; //Enable CN interrupts on CN1  (PB2) 
	CNCONbits.ON = 1;    //Enable CN peripheral to generate interrupts
	
	//Step 4. Configure Priority and Subpriority
	IPC6bits.CNIP = 5;	//3 bits: 0-7
	IPC6bits.CNIS = 3;	//2 bits: 0-2
	
	//Step 5. Clear Interrupt Flag Status bit
	IFS1bits.CNIF = 0; 
	
	//Step 6. Set Interrupt Enable bit
	IEC1bits.CNIE = 1; 	
	
	__builtin_enable_interrupts();
	#endif 
}

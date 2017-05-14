/*
GOAL: 
	Use the PIC32's core timer to generate an interrupt every rollover of the Core Timer. 
	When a rollover occurs, toggle the state of LED1 and write to UART3 the message
	"Rollever!\r\n".
	
	Additionally, include an interrupt to service a button push. Upon a button push, 
	write to UART3 the message:
		"Button Pushed! Reseting counter from %CurrentTime% to 0.\r\n"
	
	
NOTES:
	SYSCLK is a 32 bit timer that runs at 80MHz (12.5ns period). Rollover occurs
	at (2^32)*12.5ns = ~53.687 seconds. 
	
	Core timer ticks once for every two ticks of SYSCLK (80MHz) (p.71). 80MHz corresponds
	to a period of 12.5ns. Thus the period of the core timer is 25ns. Rollover 
	occurs at 2*53.687s = 107.374s.
	
	Info. about NU32_Startup(), NU32_ReadUART3()
*/ 

#include "NU32.h" 

#define MSG_LEN 200 

int main()
{
	NU32_Startup(); //Cache on, Interrupts On, LED/
	char message[MSG_LEN]; 
	
	return 0; 
}
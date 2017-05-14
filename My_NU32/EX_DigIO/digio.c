/* 
	This program blinks LED1 every 250ms using an interrupt on the core timer. 
	Button 1  
	
	Button1 (CN2/RB0) Normally high
	Button2 (CN3/RB1) Normally high
	LED1 	(RD5) 	  Sinks current 
	LED2 	(RD4)	  Sinks current 
	
	Core timer period is 25ns (40MHz). 
*/ 

#include "NU32.h" 
#define TICKS 10000000 //.25s

volatile unsigned int mode = 0; 

void __ISR(_CHANGE_NOTICE_VECTOR, IPL5SRS) button_inputs()
{
	
}

void __ISR(_CORE_TIMER_VECTOR, IPL6SRS) quartertimerISR()
{
	IFS0bits.CTIF = 0;  
	LATDINV = 0x30;			//See PORTD Register Map
	_CP0_SET_COUNT(0); 
	_CP0_SET_COMPARE(TICKS);
}

int main()
{
	NU32_Startup(); //cache on, min flash wait, interrupts on, LED/button init, UART3 init
		
	//Configure Button Pins: Inputs
	TRISBbits.TRISB0 = 1; 
	TRISBbits.TRISB1 = 1; 
	//Configure LED Pins: Outputs 
	TRISDbits.TRISD4 = 0; 
	TRISDbits.TRISD5 = 0; 
		
	//Configure Core Timer interrupts
	__builtin_disable_interrupts();  //2. Disable interrupts at CPU
	_CP0_SET_COUNT(0); 
	_CP0_SET_COMPARE(TICKS); 
	IPC0bits.CTIP = 6; 	//4. Interrupt priority
	IPC0bits.CTIS = 0;  //4. Interrupt subpriority
	IFS0bits.CTIF = 0;  //5. Clear CT interrupt flag
	IEC0bits.CTIE = 1; 	//6. Enable CT IRQ
	__builtin_enable_interrupts();	 //7. Re-enable interrupts at CPU 	
		
	//Configure CN interrupts	
	__builtin_disable_interrupts(); //2. See p.119
	CNCONbits.ON = 1; 				//3. Change Notice Control ON
	CNENbits.CNEN2; 				//3. Configure CN2 to cause CN interrupt
	CNENbits.CNEN3; 				//3. Configure CN3 to cause CN interrupt 
	IPC6bits.CNIP = 5;				//4. Set interrupt priority = 5 (of 7) 
	IPC6bits.CNIS = 3;				//4. Set interrupt subpriority = 3 (of 3) 
	IFS1bits.CNIF = 0; 				//5. Clear CN interrupt flag
	IEC1bits.CNIE = 1; 				//6. Enable all configured CN pins for interrupt
	__builtin_enable_interrupts();  //7. Re-enable interrupts at CPU 
	
	//Set both LEDs on; alternatively done with LATD = 0x0018
	LATDbits.LATD4 = 0; 
	LATDbits.LATD5 = 0; 
	
	while(1)
	{
	} 
	
	return 0; 
}
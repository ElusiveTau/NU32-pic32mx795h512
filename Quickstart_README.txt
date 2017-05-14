================================================================================
See Quickstart p5.
================================================================================
3. We use the MinGW GNU gcc compiler (http://www.mingw.org/) is installed in: 
	
		<Native C Compiler> 	D:\MinGW\bin

5. Microchip XC32 Cross-compiler (converts C source files into PIC32 machine 
	code; a cross compiler is a program that runs on one processor architecture 
	(e.g., x86-64 CPU) that creates machine code for another (e.g., MIPS32)). 
	
	Microchip XC32 cross compiler installation package contains C libraries 
	that enableusers to access hardware features of the PIC32. 
	
		<xc32dir>/<xc32ver> 	D:\MPLAB_XC32_Compiler
		
	Note: <xc32dir> and <xc32dir>/<xc32ver> are used interchangeably. The 
	XC32 cross compiler version is 1.42V
	
	** Be sure to add XC32 to environment PATH variable ** 
	
6. MPLAB Harmony

		<harmony> 	D:\MPLAB_Harmony

8. Terminal Emulator

		<puttyPath> C:\Users\Minh Tran\Desktop\Applications
		
9. Quickstart code 

		<PIC32> 	D:\Minh\NU32\embedded_computing_code\01_Quickstart

================================================================================
Compiling Bootloader Utility See p.6
================================================================================
nu32utility.c contains code that sends compiled code to PIC32. Compile it using
the following to get an executable (.exe): 

	gcc nu32utility.c -o nu32utility -lwinmm	(windows)
	gcc nu2uttility.c -o nu32utility			(Linux/Mac) 
	
The location of nu32utility.exe is referered to in the makefile as NU32PATH. 
	
================================================================================
Compiling a PIC32 Program (Manually) See p.8
================================================================================
Use xc32-gcc cross compiler (should be added to path) to compile a PIC32 program 
to get an .elf and then an executable .hex file. First, navigate to the directory
where the PIC32 program resides. 

Then, issue the following command into console: 
	
	xc32-gcc -mprocessor=32MX795F512H -o simplePIC.elf -Wl,--script=skeleton/NU32bootloaded.ld simplePIC.c

	xc32-bin2hex simplePIC.elf
	
First instruction produces an .elf file. The second creates a .hex file. The hex file 
contains MIPS32 machine code in a format that the bootloader understands. 

	==========================================
	Loading a PIC32 Program (Manually) See p.8
	==========================================
When the PIC32 is powered and connected to your CPU's USB port, your computer 
will create a new serial communication (COM) port. Check the connection COM port. 
Check that FTDI drivers are installed. Note the COM port number. 

	mode

Put the NU32 into program-receive mode: 

	Press and hold both the USER and RESET button. Then release the RESET and USER
	button (in that order, in sequence). The PIC32 will then flash LED1, indicating
	that it has entered program received mode. 

Load the program: Run the command
	
	nu32utility <COM> simplePIC.hex

Where <COM> is substituted for "\\.\COMx" and "x" is the COM port number.
There should be output indicating that the program load is complete. 

================================================================================
Using makefiles to Compile and Load a Project See p. 10-12
================================================================================
The make program needs to be added to environment Path variable, otherwise you'll
get the following feedback: 

	'make' is not recognized as an internal or external command, operable program
	or batch file. 

The 'D:\MPLAB_XC32++\gnuBins\GnuWin32\bin' directory contains make.exe. Add this
directory to the path variable. 

Note: 
	You add a directory to the Path variable by adding the full path of the 
	directory to the variable. Do NOT add the full path to the executable (ie.,
	D:\MPLAB_XC32++\gnuBins\GnuWin32\bin\make.exe). This will not work. 
	
Run the makefile to build a .hex file, which is ready to be loaded onto the NU32. 
Put the NU32 into program-receive mode (see above). Then issue the command: 

	make write 

	=====================================
	Creating a New Project See p.11,12,61
	=====================================
	The NU32 library contains useful startup code for beginners who don't already
	know how to configure the PIC32 for basic applications. It also configures the
	UART3 and LED1/LED2 Peripherals of the NU32 board so that they are readily
	available. Assuming the new project directory's name is <dir>:

	1. Copy the contents of "..\embedded_computing_code\01_Quickstart\skeleton" into <dir>. The 
	content that should be copied are 4 files: 
		
		Makefile
		NU32.c
		NU32.h
		NU32bootloaded.ld	
		
		Hint: 
			robocopy Source_Folder Destination_Folder [filename]	-- Copies single file
			robocopy Source_Folder Destination_Folder				-- Copies entire folder
	
	2. Modify the makefile so the paths are correct. 
		
		NOTES: 
		1. NU32PATH in the makefile refers to the location containing the compiled
		nu32utility.exe. See "Compiling Bootloader Utility" section for how to 
		compile nu32utility.c
		
	3. Include the .c code for your main program (do not include multiple programs
	containing main(). Limit 1 main() program per project folder). If your program
	requires additional libraries, include their header and source files into
	the project folder. 
		
	4. Compile everything using the Makefile. Run 'make', put the NU32 to 
	program-receive mode and issue 'make write'. If the program uses UART3, 
	run 'make putty'
		
================================================================================
NU32 File Description
================================================================================

Quickstart_README.txt 		
	This document
	
p32mx795f512h_REFERENCE.h
	Header file from \MPLAB_XC32_Compiler\pic32mx\include\proc\ for the proc
	on the NU32. It contains the macros for the interrupt vectors. IRQs #'s. 
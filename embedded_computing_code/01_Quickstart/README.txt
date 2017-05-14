Hardware Requirements: 
	NU32 Board (with NU32 bootloader)
	
In this example, we compile simplePIC.c manually. The files present in the
working directory (...\embedded_computing_code\01_Quickstart\) should be: 

	<DIR> skeleton
	nu32utility.c
	simplePIC.c
	talkingPIC.c
	
Make sure you have the native C-compiler (gcc), and Microchip XC32 cross-compiler (XC32)
installed. Quickstart_README.txt for details. 

1. 
	Compile the bootloader: 
		gcc nu32utility.c -o nu32utility -lwinmm	(windows)
	This yields the executable nu32utility.exe.
	
2. 
	Compile the user program which will be bootloaded onto the NU32. We use the cross compiler
	to obtain a .hex file from a .c source file. 
	
	The source file is compiled into assembly code (.s). The assembler then takes all assembly 
	files and generates (.o) files - relocatable object code. The linker script (.ld) links grouped
	object files (aka archived library file (.a)) and (.o) files into an executable 
	(.elf/executable linker format). The XC32 utility converts the .elf file into a (.hex) file 
	suitable for the bootloader. (See p.38) 
	
	Use the following commands to invoke the cross compiler on the .c program on simplePIC.c: 
	
		xc32-gcc -mprocessor=32MX795F512H -o simplePIC.elf -Wl,--script=skeleton/NU32bootloaded.ld simplePIC.c

	This yields the executable simplePIC.elf. Invoke xc32-bin2hex to convert the .elf file to a .hex file. 
	
		xc32-bin2hex simplePIC.elf
		
	This yields the executable simplePIC.hex. Afterwards, the directory should contain: 
	
		nu32utility.exe
		simplePIC.elf
		simplePIC.hex

3.  Load the program: 
		mode
		<Put NU32 in program-receive mode> 
		
	Then issue the command to load the program: 
	
		nu32utility <COM> simplePIC.hex
	
	
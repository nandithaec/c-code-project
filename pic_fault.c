#include <stdio.h>
#include <string.h>
//#include <conio.h>

#include "decode_fault.h"
#include "execute.h"



int main()

{
	
	int instruction=0, opcode = 0;

	int reg_index=0,  reg_file_addr = 0, d=0;
	int n=0, i=0;
	int	decode_bits=0;
	int bit=0, immediate_value=0;
	int program_memory[PROGRAM_MEM_SIZE] ={0};
//	int program[PROGRAM_MEM_SIZE] ={0};
	char instr_mnemonic[MNEMONIC_SIZE];
	//int PCL = 0, PC = 0, PCLATH=0;
	int loop=0;
	char line[FILE_CHARS];	
	int instr_from_file=0;
	int starting_PC_value = 0;
	int random_reg=0, random_mem=0;
	int repeat_program_execution=0;
	int initial_PCL=0, initial_PCLATH=0;

	struct registers pic_registers;
	
//-------------------------------Initialising registers------------------------------------
//Max register content= 255 (dec) or FF (hex)
	pic_registers.configuration_word[11]= 1; //WDT Enabler bit

//clear all registers
for(i=0;i<REG_MAX;++i)
		pic_registers.GP_Reg[i]=0; //clear all registers in register file map
	
	//INTCON Register
		pic_registers.GP_Reg[0x0B]=0; //INTCON register at address 0B in the register file map
		pic_registers.GP_Reg[0x8B]= pic_registers.GP_Reg[0x0B];

	//for(i=0;i<MEM_WIDTH;++i)
	//	pic_registers.PC=0;
	//STACK	
		pic_registers.stack_pointer = 0;
		pic_registers.stack[1] = 0x62; 
		pic_registers.stack[0] = 0x88; 

	// Status register = GP_Reg[3] 
	//Assigning some value to the carry in status reg
//		pic_registers.GP_Reg[3]= pic_registers.GP_Reg[3] & 0xFE; //carry = 0
		//pic_registers.GP_Reg[3]= pic_registers.GP_Reg[3] | 0x01; //carry = 1
		pic_registers.GP_Reg[3]= 0x00;
		pic_registers.GP_Reg[0x83]= pic_registers.GP_Reg[3]; //Bank 1 and Bank 0

		pic_registers.W = 0x00; 
		
	//TMRO
		pic_registers.GP_Reg[1]= 0x00;

	//OPTION REG
		pic_registers.GP_Reg[0x81]= 0x00;

		//FSR
		pic_registers.GP_Reg[4]= 0x00;
		pic_registers.GP_Reg[0x84]= pic_registers.GP_Reg[4];

	//PORTA
		pic_registers.GP_Reg[5]= 0x00;

	//PORTB
		pic_registers.GP_Reg[6]= 0x00;

	//TRISA
		pic_registers.GP_Reg[0x85]= 0x00;

	//TRISB
		pic_registers.GP_Reg[0x86]= 0x00;

	//EEDATA
		pic_registers.GP_Reg[8]= 0x00;

	//EEADR
		pic_registers.GP_Reg[9]= 0x00;

	//EECON1
		pic_registers.GP_Reg[0x88]= 0x00;

	//EECON2 - not a physical register
	//	pic_registers.GP_Reg[0x89]= 0x00;

//Initialise PC values

//use this address 0x02 to write into PCL:
//PCL= GP_Reg[2] and GP_Reg[0x82]
		pic_registers.GP_Reg[2]= 0x00;
		pic_registers.GP_Reg[0x82]= pic_registers.GP_Reg[2]; //Bank 1 and Bank 0
		pic_registers.PCL= pic_registers.GP_Reg[2];

	//PCLATH.. use this address to write into PCLATH
		pic_registers.GP_Reg[0x0A]= 0x00;
		pic_registers.GP_Reg[0x8A]= pic_registers.GP_Reg[0x0A]; //Bank 1 and Bank 0
		pic_registers.PCLATH= pic_registers.GP_Reg[0x0A];

		pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits

		PRINT("-----------------------------------------------------------------\n");
		PRINT("Initial values (hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",pic_registers.PCL, pic_registers.PCLATH, pic_registers.PC);

//Read in the program counter initial value

		printf("Enter starting PCL value (in hex): \n");
		scanf("%x", &pic_registers.GP_Reg[2]);
		initial_PCL=pic_registers.GP_Reg[2];

		printf("Enter starting PCLATH value (in hex): \n");
		scanf("%x", &pic_registers.GP_Reg[0x0A]);
		initial_PCLATH=pic_registers.GP_Reg[0x0A];

		pic_registers.GP_Reg[0x82]= pic_registers.GP_Reg[2]; //PCL Bank 1 and Bank 0
		pic_registers.PCL= pic_registers.GP_Reg[2];

		pic_registers.GP_Reg[0x8A]= pic_registers.GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
		pic_registers.PCLATH= pic_registers.GP_Reg[0x0A];

		pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits

	PRINT("New values as read from the user(hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",pic_registers.GP_Reg[2], pic_registers.PCLATH, pic_registers.PC);


//-----------Reading instruction from file---------------------------
	n= pic_registers.PC; // new value of n is initialised to the PC value that is read from the disassembly listing
	starting_PC_value = pic_registers.PC;
	PRINT("Starting PC value=%x\n",starting_PC_value);

	FILE *fp;
	fp= fopen("simple_add_assembly.c","rt");
	if( fp == NULL )
         {
               puts ( "cannot open file" ) ;
          //     exit(0) ;
         }	

	  while(fgets(line, FILE_CHARS, fp) != NULL)
	   {
		 /* get a line, up to 80 chars from fp.  done if NULL */
		 sscanf (line, "%x", &instr_from_file);
		program_memory[n]= instr_from_file;
		n++;
		
	   }
	 
		for(i=starting_PC_value;i<n;i++)
			printf("Instructions read from file= program_memory[%x]= %x\n",i, program_memory[i]);

   fclose(fp);  /* close the file prior to exiting the routine */




//-------------------------------------------------------------------------------------------
	// Reg file starts only from 0CH = 12
//program_memory[0] means, fetch instruction from address location 0

//	program_memory[0] = 0x2FFF; //GOTO
//	program_memory[1] = 0x27FF; //CALL
//	program_memory[0] = 0x3FF0; //ADDLW
//	program_memory[0] = 0x3C00; //SUBLW
//	program_memory[0] = 0x3A01; //XORLW
//	program_memory[1] = 0x3900; //ANDLW
//	program_memory[0] = 0x3800; //IORLW
//	program_memory[0] = 0x37FF; //RETLW
//	program_memory[0] = 0x30F0; //MOVLW
//	program_memory[0] = 0x1F8C; //BTFSS - test bit 1
//	program_memory[0] = 0x1B8C; //BTFSC - test bit 1
//	program_memory[0] = 0x148C; //BSF - set  bit 1
//	program_memory[0] = 0x138C; //BCF - clear bit 7 (8th bit)
//	program_memory[0] = 0x0F0C; //INCFSZ or 0x0F8C
//	program_memory[0] = 0x0E0C; //SWAPF or 0x0E8C
//	program_memory[0] = 0x0D8C; //RLF or 0x0D8C
//	program_memory[0] = 0x0C8C; //RRF or 0x0C8C
//	program_memory[0] = 0x0B0C; //DECFSZ or 0x0B8C
//	program_memory[0] = 0x0A0C; //INCF or 0x0A8C
//	program_memory[0] = 0x090C; //COMF or 0x098C
//	program_memory[0] = 0x080C; //MOVF or 0x088C
//	program_memory[0] = 0x070C; //ADDWF or 0x078C
//	program_memory[0] = 0x068C; //XORWF or 0x068C
//	program_memory[0] = 0x050C; //ANDWF or 0x050C
//	program_memory[0] = 0x040C; //IORWF or 0x048C
//	program_memory[0] = 0x038C; //DECF or 0x030C
//	program_memory[0] = 0x020C; //SUBWF or 0x020C
//	program_memory[0] = 0x018C; //CLRF
//	program_memory[0] = 0x010C; //CLRW
//	program_memory[0] = 0x008C; //MOVWF or 0x000C
//	program_memory[0] = 0x0000; //NOP
//	program_memory[0] = 0x0009; //RETFIE
//	program_memory[0] = 0x0008; //RETURN
//	program_memory[0] = 0x0064; //CLRWDT
//	program_memory[0] = 0x0063; //SLEEP
	
	printf("Status register contents:(hex) at the beginning of all operations: ");
	printf("%x", pic_registers.GP_Reg[3]);
	printf("\n");

	loop= starting_PC_value;


//Repeat the same program 10 times - condition is specified at the end of the while loop
while (loop < n)
	{
		
		//Flip bits before instruction fetch
		bit_flips(&pic_registers, program_memory, &random_reg, &random_mem);

		printf("****************************************************************\n");
		printf("INSTRUCTION NUMBER %d\n", loop-starting_PC_value+1);


		//Instruction fetch	
		instruction= instruction_fetch(&pic_registers, program_memory);
	

		//Instruction decode
		decode_bits= (instruction & 0x3000)>> 12;  // bits 13 and 14
		PRINT("Decode bits= %d \n", decode_bits);

		struct instructions pre_decode, post_decode;
	
		pre_decode.instruction= instruction;
		pre_decode.opcode= opcode;
		pre_decode.reg_file_addr= reg_file_addr;
		pre_decode.d= d;
		pre_decode.reg_index= reg_index;
		pre_decode.bit = 0;
		pre_decode.immediate_value = 0;


	
		switch (decode_bits)
		{
			case 0:
				decode_byte_instr(&pre_decode);
				break;

			case 1:
				decode_bit_instr(&pre_decode);	
				break;

			case 2:
				call_goto_instr(&pre_decode);	
				break;

			case 3:
				literal_control_instr(&pre_decode);	
				break;

			default:
				printf("Invalid instruction\n");
				break;
		}
	

		post_decode= pre_decode;
			

		PRINT("Instruction format (hex) = %x \n",post_decode.instruction);
		PRINT("Opcode (hex) = %x \n",post_decode.opcode);
		PRINT("Register file address (hex) = %x, Register number= %d \n", post_decode.reg_file_addr, post_decode.reg_index);
		PRINT("Destination bit = %d, bit=%d, Immediate value (hex)= %x \n", post_decode.d, post_decode.bit, post_decode.immediate_value);
		PRINT("Instruction mnemonic enum = %d\n",post_decode.instr_mnemonic_enum);

//Ignore: pic_registers.GP_Reg[post_decode.reg_index]= 0x01; //Content of register f location in program memory
	
		//Instruction execute
		printf("Status register contents:(hex) at the end of decode: ");
		printf("%x", pic_registers.GP_Reg[3]);
		printf("\n");

		instruction_execute(&pic_registers,&post_decode);
		//post_execute= pre_execute;
		printf("****************************************************************\n");	
		loop++;	

		if (loop == n) //If end of program is reached
			{
				loop= starting_PC_value; //Reset loop to beginning of program and begin execution again

				//----------------------------------------------------------------------------------------------------------------------------
				//Reset program counter to beginning of the program
				pic_registers.GP_Reg[2]= initial_PCL;
				pic_registers.GP_Reg[0x82]= pic_registers.GP_Reg[2]; //PCL Bank 1 and Bank 0
				pic_registers.PCL= pic_registers.GP_Reg[2];

				pic_registers.GP_Reg[0x0A]= initial_PCLATH;
				pic_registers.GP_Reg[0x8A]= pic_registers.GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
				pic_registers.PCLATH= pic_registers.GP_Reg[0x0A];

				pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits
				//----------------------------------------------------------------------------------------------------------------------------

				repeat_program_execution++; //Keep track of the number of times the program is re-executed

				PRINT("\n-----------Program execution number %d completed-------------\n\n",repeat_program_execution);

			}
	
	if(repeat_program_execution == 100) //Repeat program execution 10 times
	break;
	
	}


		printf("Status register contents:(hex) at the end of all operations: ");
		printf("%x", pic_registers.GP_Reg[3]);
		printf("\n");

return 0;

}



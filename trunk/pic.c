#include <stdio.h>
#include <string.h>

#include "functions.h"
#include "execute.h"



int main()

{
	
	int instruction=0, opcode = 0;

	int reg_index=0,  reg_file_addr = 0, d=0;
	int i=0;
	int	decode_bits=0;
	int bit=0, immediate_value=0;
	int program_memory[PROGRAM_MEM_SIZE] ={0};
	char instr_mnemonic[MNEMONIC_SIZE];
	
	struct registers pic_registers;
//-------------------------------Initialising registers------------------------------------
//Max register content= 255 (dec) or FF (hex)
	pic_registers.configuration_word[11]= 1; //WDT Enabler bit

	for(i=0;i<REG_MAX;++i)
		pic_registers.GP_Reg[i]=0;
	

	for(i=0;i<REG_WIDTH;++i)
		pic_registers.status_reg[i]=0;

	for(i=0;i<REG_WIDTH;++i)
		pic_registers.INTCON_reg[i]=0;

	//for(i=0;i<MEM_WIDTH;++i)
		pic_registers.PC=0;
		pic_registers.stack_pointer = 1;
		pic_registers.stack[0] = 0x62; //98
	//Assigning some value to the reg file location 
		pic_registers.status_reg[7]=0;
		pic_registers.GP_Reg[reg_index]= 0xFE;
		pic_registers.W = 0x0F; //18
//-------------------------------------------------------------------------------------------
	// Reg file starts only from 0CH = 12

	program_memory[0] = 0x0F0C; //INCFSZ or 0x0F8C
//	program_memory[0] = 0x0E0C; //SWAPF or 0x0E8C
//	program_memory[0] = 0x0D0C; //RLF or 0x0D8C
//	program_memory[0] = 0x0C0C; //RRF or 0x0C8C
//	program_memory[0] = 0x0B0C; //DECFSZ or 0x0B8C
//	program_memory[0] = 0x0A0C; //INCF or 0x0A8C
//	program_memory[0] = 0x090C; //COMF or 0x098C
//	program_memory[0] = 0x080C; //MOVF or 0x088C
//	program_memory[0] = 0x070C; //ADDWF or 0x078C
//	program_memory[0] = 0x068C; //XORWF or 0x068C
//	program_memory[0] = 0x050C; //ANDWF or 0x050C
//	program_memory[0] = 0x040C; //IORWF or 0x048C
//	program_memory[0] = 0x038C; //DECF or 0x030C
//	program_memory[0] = 0x028C; //SUBWF or 0x020C
//	program_memory[0] = 0x018C; //CLRF
//	program_memory[0] = 0x010C; //CLRW
//	program_memory[0] = 0x008C; //MOVWF or 0x000C
//	program_memory[0] = 0x0000; //NOP
//	program_memory[0] = 0x0009; //RETFIE
//	program_memory[0] = 0x0008; //RETURN
//	program_memory[0] = 0x0064; //CLRWDT
//	program_memory[0] = 0x0063; //SLEEP


	
	//Instruction fetch	
	instruction= instruction_fetch(&pic_registers, program_memory);

	//Instruction decode
	decode_bits= (instruction & 0x3000)>> 12;  // bits 13 and 14
	
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
	PRINT("Destination bit = %d, bit=%d, Immediate value= %d \n", post_decode.d, post_decode.bit, post_decode.immediate_value);
	PRINT("Instruction mnemonic enum = %d\n",post_decode.instr_mnemonic_enum);
	
	//Instruction execute
	
	instruction_execute(&pic_registers,&post_decode);
	//post_execute= pre_execute;
	

	return 0;

}



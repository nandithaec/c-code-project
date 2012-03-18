#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INSTR_SIZE 10
#define REG_MAX 255
#define REG_WIDTH 8
#define PROGRAM_MEM_SIZE 8192
#define MNEMONIC_SIZE 10
#define CONFIG_WORD_SIZE 14
#define MEM_WIDTH 13
#define FILE_CHARS 80

#define DEBUG
#ifdef DEBUG
 #define PRINT printf
#else
 #define PRINT print_null
#endif
void print_null (char* n,...) {return;}

//All declarations

struct registers
{
	int GP_Reg[REG_MAX]; //General purpose register file map
	int W; //Accumulator/ W register
	int PC; //13-bit Program counter. Can address max 8k x 14 memory space- actual value derived from PCL and PCLATH
	//int status_reg[REG_WIDTH];
	//int option_reg[REG_WIDTH];
	//int INTCON_reg[REG_WIDTH];
	//int EECON1_reg[REG_WIDTH];
	int configuration_word[CONFIG_WORD_SIZE]; //Actually each bit has been configured as an int byte
	int WDT; //8 bit wide
	int WDT_prescaler; //8-bit counter
	int stack[REG_WIDTH]; //8-level deep stack of 13-bit wide. Max number to enter is 8192
	int stack_pointer; //max length = 8
	int PCLATH; //8bit register -- actual value taken from GP_Reg
	int PCL;//8 bit register- actual value taken from GP_Reg
};



enum Mnemonic
{

//Specific opcodes
	SLEEP,
	CLRWDT,
	RETURN,
	RETFIE,
//Byte-oriented instructions	
	NOP,
	MOVWF,
	CLRW,
	CLRF,
	SUBWF,
	DECF,
	IORWF,
	ANDWF,
	XORWF,
	ADDWF,
	MOVF,
	COMF,
	INCF,
	DECFSZ,
	RRF,
	RLF,
	SWAPF,
	INCFSZ,
//Bit-oriented instructions
	BCF,
	BSF,
	BTFSC,
	BTFSS,
//Literal and control instructions
	MOVLW,
	RETLW,
	IORLW,
	ANDLW,
	XORLW,
	SUBLW,
	ADDLW,
//Call and goto instructions
	CALL,
	GOTO
	
};


struct instructions
{
	int instruction;
	int opcode;
	int reg_file_addr;
	int d;
	int reg_index;
	int bit;
	int immediate_value;
	char instr_mnemonic[MNEMONIC_SIZE];
	enum Mnemonic instr_mnemonic_enum;	
};


//Function declarations
int instruction_fetch(struct registers*, int []);
int increment_PC(struct registers **);

int decode_byte_instr(struct instructions *i1);
int decode_bit_instr(struct instructions *i1);
int call_goto_instr(struct instructions *i1);
int literal_control_instr(struct instructions *i1);


int instruction_execute(struct registers *, struct instructions *);
int push(struct registers *);
int pop (struct registers *);

int bit_flips(struct registers *, int [], int *,int *);
//----------------------------------------Function definitions---------------------------------------------------------//



int bit_flips(struct registers *r2,  int program_memory[],int *random_reg, int *random_mem)
{
	int random_bit=0, random_bit_mem =0;
	int i=0;

/*PRINT("Original content of registers\n");

for(i=0;i<=REG_MAX;++i)
	PRINT("GP_Reg[%d] = %x\n",i,r2->GP_Reg[i]);*/

	srand ( time(NULL) );

//Flip 1 bit in General purpose register
	/* generate random number: */
		*random_reg = rand() % 256 ; // Random number between 0 and 255
		random_bit = rand() % 8 ; // Random number between 0 and 7

		PRINT("\nBit flip function called\n");
		PRINT("Random reg selected:%d, random bit to flip in this reg is %d\n",*random_reg,random_bit);
		PRINT("Content of the random reg location[%d] is (in hex) %x\n",*random_reg,r2->GP_Reg[*random_reg]);
		switch(random_bit)
		{
			case 0:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 0);
			break;
		
			case 1:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 1);
			break;

			case 2:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 2);
			break;

			case 3:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 3);
			break;

			case 4:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 4);
			break;

			case 5:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 5);
			break;

			case 6:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 6);
			break;

			case 7:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 7);
			break;

			case 8:
			r2->GP_Reg[*random_reg]=	r2->GP_Reg[*random_reg] ^(1 << 8);
			break;

			default: printf("Invalid bit flip case\n");
			break;
		}
	
	PRINT("After bitflip number %d: Content of the reg[%x] is (in hex) %x\n\n",i+1, *random_reg, r2->GP_Reg[*random_reg]);
	


//Flip 1 bit in program memory 
// generate random number: 
		*random_mem = rand() % 8192; // Random number between 0 and 8192
		random_bit_mem = rand() % 8 ; // Random number between 0 and 7

		PRINT("Program memory location selected:%x, random bit to flip in this location is %d\n",*random_mem,random_bit_mem);
		PRINT("Content of the program memory location[%x] is (in hex): %x\n",*random_mem, program_memory[*random_mem]);

	
		switch(random_bit_mem)
		{
			case 0:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 0);
			break;
		
			case 1:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 1);
			break;

			case 2:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 2);
			break;

			case 3:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 3);
			break;

			case 4:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 4);
			break;

			case 5:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 5);
			break;

			case 6:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 6);
			break;

			case 7:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 7);
			break;

			case 8:
			program_memory[*random_mem]=	program_memory[*random_mem] ^(1 << 8);
			break;

			default: printf("Invalid bit flip case\n");
			break;
		}
	
	PRINT("After bitflip number %d: Content of the program_memory[%x] is (in hex) %x\n\n",i+1, *random_mem, program_memory[*random_mem]);
	


//Condition for program crash if Program counter value changes:

if (*random_reg == 0x02 || *random_reg == 0x82 || *random_reg == 0x0A || *random_reg == 0x8A)
	{
		PRINT("Program crash due to PC value at location %x getting affected\n", *random_reg);
		exit(0);
	}

//Condition for program crash if illegal memory access
if ( (0x4F < *random_mem && *random_mem < 0x7F) || (0xCF < *random_mem && *random_mem < 0xFF)) //Invalid memory location range
	{ 
		PRINT("Program crash due to illegal memory access at location %x getting affected\n", *random_mem);
		exit(0);
	}

return 0;
}


int instruction_fetch(struct registers *r, int program_memory[])
{
	int instruction;
	instruction = program_memory[r-> PC];
	
	PRINT("-------------------------------------------------------------------\n");
	PRINT("INSTRUCTION FETCH >>\n");
	PRINT("Fetching instruction from program_memory[%x]\n",r->PC);
	PRINT("Before incrementing PC: PCL=%x, PCLATH=%x, PC = %x \n",r->GP_Reg[2],r->PCLATH, r->PC);

	//Increment PC
	increment_PC(&r);
	PRINT("After Incrementing PC: PCL=%x, PCLATH=%x, PC = %x \n",r->GP_Reg[2],r->PCLATH, r->PC);
	PRINT("-------------------------------------------------------------------\n");
	return instruction;
	
}			


int increment_PC(struct registers **r)
{
	//PCL= GP_Reg[2] and GP_Reg[0x82]
		(*r)-> GP_Reg[2]= (*r)-> GP_Reg[2] + 1;
		(*r)-> GP_Reg[0x82]= (*r)-> GP_Reg[2]; //Bank 1 and Bank 0
		(*r)-> PCL= (*r)-> GP_Reg[2];

	//PCLATH
		//r-> GP_Reg[0x0A]= 0x00; //PCLATH not changed.. always comment this out
		(*r)-> GP_Reg[0x8A]= (*r)-> GP_Reg[0x0A]; //Bank 1 and Bank 0
		(*r)-> PCLATH= (*r)-> GP_Reg[0x0A];

		(*r)-> PC = ((*r)-> PCL | ((*r)-> PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits


return 0;
}  


int decode_byte_instr(struct instructions *i1)

{
	
	i1-> reg_file_addr = (i1->instruction) & 0x007F;
	i1->d= (i1->instruction & 0x80) >> 7;
//	i1->reg_index = (i1->reg_file_addr) - 12; // Reg file starts only from 0CH = 12
	i1->reg_index = (i1->reg_file_addr); // Reg file starts only from 0CH = 12
	i1->opcode = (i1->instruction & 0xFF00) >> 8;

	PRINT("---------------------------------------------------------------------\n");
	PRINT("INSTRUCTION DECODE >> Byte instructions\n");

	if (i1->instruction==0x0063)
		{
			i1->instr_mnemonic_enum = SLEEP;
			PRINT("Instruction mnemonic = SLEEP\n");
		}
	else if (i1->instruction==0x0064)
		{
			i1->instr_mnemonic_enum = CLRWDT;
			PRINT("Instruction mnemonic = CLRWDT\n");
		}
	else if (i1->instruction==0x0008)
		{
			i1->instr_mnemonic_enum = RETURN;
			PRINT("Instruction mnemonic = RETURN\n");
		}
	else if (i1-> instruction==0x0009)
		{
			i1->instr_mnemonic_enum = RETFIE;
			PRINT("Instruction mnemonic = RETFIE\n");
		}
	else

	switch (i1-> opcode)
	{
		case 0:
			if (i1-> d==0)
				{
				PRINT("Instruction mnemonic = NOP\n");
				i1->instr_mnemonic_enum = NOP;
				}
			else
				{
				PRINT("Instruction mnemonic = MOVWF\n");
				i1->instr_mnemonic_enum = MOVWF;
				}
				
		break;

		case 1:
				if (i1-> d==0)
				{
				PRINT("Instruction mnemonic = CLRW\n");
				i1->instr_mnemonic_enum = CLRW;
				}
			else
				{
				PRINT("Instruction mnemonic = CLRF\n");
				i1->instr_mnemonic_enum = CLRF;
				}
		break;
		
		case 2:
			PRINT("Instruction mnemonic = SUBWF\n");
			i1->instr_mnemonic_enum = SUBWF;			
		break;

		case 3:
			PRINT("Instruction mnemonic = DECF\n");
			i1->instr_mnemonic_enum = DECF;
		break;

		case 4:
			PRINT("Instruction mnemonic = IORWF\n");
			i1->instr_mnemonic_enum = IORWF;
		break;

		case 5:
			PRINT("Instruction mnemonic = ANDWF\n");
			i1->instr_mnemonic_enum = ANDWF;
		break;
		
		case 6:
			PRINT("Instruction mnemonic = XORWF\n");
			i1->instr_mnemonic_enum = XORWF;
		break;

		case 7:
			PRINT("Instruction mnemonic = ADDWF\n");
			i1->instr_mnemonic_enum = ADDWF;
		break;

		case 8:
			PRINT("Instruction mnemonic = MOVF\n");
			i1->instr_mnemonic_enum = MOVF;
		break;

		case 9:
			PRINT("Instruction mnemonic = COMF\n");
			i1->instr_mnemonic_enum = COMF;
		break;
		
		case 10:
			PRINT("Instruction mnemonic = INCF\n");
			i1->instr_mnemonic_enum = INCF;
		break;

		case 11:
			PRINT("Instruction mnemonic = DECFSZ\n");
			i1->instr_mnemonic_enum = DECFSZ;
		break;

		case 12:
			PRINT("Instruction mnemonic = RRF\n");
			i1->instr_mnemonic_enum = RRF;
		break;

		case 13:
			PRINT("Instruction mnemonic = RLF\n");
			i1->instr_mnemonic_enum = RLF;
		break;
		
		case 14:
			PRINT("Instruction mnemonic = SWAPF\n");
			i1->instr_mnemonic_enum = SWAPF;
		break;

		case 15:
			PRINT("Instruction mnemonic = INCFSZ\n");
			i1->instr_mnemonic_enum = INCFSZ;
		break;

		default: printf("Invalid instruction\n");
		break;

	}
PRINT("-------------------------------------------------------------------\n");
return 0;

}


int decode_bit_instr(struct instructions *i1)
{
						
	i1-> reg_file_addr = (i1->instruction) & 0x007F;
	i1-> bit= ((i1->instruction) & 0x0380) >> 7;
//	i1-> reg_index = (i1-> reg_file_addr) - 12; // Reg file starts only from 0CH = 12
	i1-> reg_index = (i1-> reg_file_addr) ; // Reg file starts only from 0CH = 12
	i1-> opcode = ((i1-> instruction) & 0x1C00) >> 10;
	PRINT("-------------------------------------------------------------------\n");
	PRINT("INSTRUCTION DECODE >> Bit instructions\n");


	switch (i1-> opcode)
	{
		case 4:
			PRINT("Instruction mnemonic = BCF\n");
			i1->instr_mnemonic_enum = BCF;
		break;

		case 5:
			PRINT("Instruction mnemonic = BSF\n");
			i1->instr_mnemonic_enum = BSF;
		break;
		
		case 6:
			PRINT("Instruction mnemonic = BTFSC\n");
			i1->instr_mnemonic_enum = BTFSC;
		break;

		case 7:
			PRINT("Instruction mnemonic = BTFSS\n");
			i1->instr_mnemonic_enum = BTFSS;
		break;

		
		default: printf("Invalid instruction\n");
		break;

	}

PRINT("-------------------------------------------------------------------\n");		
return 0;

}
	


int literal_control_instr(struct instructions *i1)
{
						
	i1-> immediate_value = (i1-> instruction) & 0x00FF;
	i1-> opcode = ((i1->instruction) & 0x3F00) >> 8;
PRINT("-------------------------------------------------------------------\n");
PRINT("INSTRUCTION DECODE >> Literal and control instructions\n");
	switch (i1-> opcode)
	{
		case 0x30: 
		case 0x31: 
		case 0x32: 
		case 0x33:
			PRINT("Instruction mnemonic = MOVLW\n");
			i1->instr_mnemonic_enum = MOVLW;
		break;

		case 0x34: 
		case 0x35: 
		case 0x36:
        case 0x37:
			PRINT("Instruction mnemonic = RETLW\n");
			i1->instr_mnemonic_enum = RETLW;
		break;
		
		case 0x38:
			PRINT("Instruction mnemonic = IORLW\n");
			i1->instr_mnemonic_enum = IORLW;
		break;

		case 0x39:
			PRINT("Instruction mnemonic = ANDLW\n");
			i1->instr_mnemonic_enum = ANDLW;
		break;

		case 0x3A:
			PRINT("Instruction mnemonic = XORLW\n");
			i1->instr_mnemonic_enum = XORLW;
		break;

		case 0x3C: 
		case 0x3D:
			PRINT("Instruction mnemonic = SUBLW\n");
			i1->instr_mnemonic_enum = SUBLW;
		break;
		
		case 0x3E: 
		case 0x3F:
			PRINT("Instruction mnemonic = ADDLW\n");
			i1->instr_mnemonic_enum = ADDLW;
		break;

		default: printf("Invalid instruction\n");
		break;

	}
PRINT("-------------------------------------------------------------------\n");
return 0;
}

int call_goto_instr(struct instructions *i1)
{
						
	i1-> immediate_value = (i1-> instruction) & 0x07FF;
	i1-> opcode = ((i1-> instruction) & 0x3800) >> 11;
PRINT("-------------------------------------------------------------------\n");
PRINT("INSTRUCTION DECODE >> CALL/GOTO instructions\n");
	
	switch (i1-> opcode)
	{
		case 4:
			PRINT("Instruction mnemonic = CALL\n");
			i1->instr_mnemonic_enum = CALL;
		break;

		case 5:
			PRINT("Instruction mnemonic = GOTO\n");
			i1->instr_mnemonic_enum = GOTO;
		break;
		
		default: printf("Invalid instruction\n");
		break;

	}
		
PRINT("-------------------------------------------------------------------\n");
return 0;
}
	




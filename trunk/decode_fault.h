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
#define MAX_CRASHES 1
#define NUM_OF_PGM_RUNS 10 
#define NUM_OF_INSTR 15
#define CLOCKS_PER_INSTR 4
#define PROBABILITY_INVERSE 1000
#define FLOW_CHANGE_MAX 10000
#define NUM_OF_BITFLIPS 10000

#define DEBUG
//#ifdef DEBUG
 //#define PRINT printf
//#else
 #define PRINT print_null
//#endif
void print_null (char* n,...) {return;}
unsigned int previous_time = -1;
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
    int PCLATH; //8 bit register -- actual value taken from GP_Reg
    int PCL;//8 bit register- actual value taken from GP_Reg
	int initial_PCL;
	int initial_PCLATH;
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

struct crash_parameters
{

	int random_number;
	int random_reg[NUM_OF_BITFLIPS];
	int random_mem[NUM_OF_BITFLIPS];
	unsigned long long int instr_cycles,instr_cycles_for_error;

	int crash;
	unsigned long long int crash_at_instr[MAX_CRASHES]; // Store the number of clock cycles at which each time a crash occurs
	unsigned long long int errors_repeated[NUM_OF_BITFLIPS];
	long int crash_time_array[MAX_CRASHES];
	unsigned long long int program_runs;
	int crash_dueto_illegal_mem;
	int crash_dueto_PC;
	int crash_dueto_illegal_opcode;
	int error;
	int control_flow_change;
	unsigned long long int error_at_instr[FLOW_CHANGE_MAX];
	int incorrect_data;
	int flip_bit_flag;
	int reg_count;
	int mem_count;
	int store_same_reg_modification[NUM_OF_BITFLIPS];
	int same_reg;
	int store_PC_same[NUM_OF_BITFLIPS];
	int same_PC;
	int reg_index_match;
	int set_no_same_PC;
};


//Function declarations
int instruction_fetch(struct registers*, int [],struct crash_parameters *);
int increment_PC(struct registers **);

int decode_byte_instr(struct instructions *i1,struct crash_parameters *,struct registers *, int [], FILE *);
int decode_bit_instr(struct instructions *i1, struct crash_parameters *, FILE *);
int call_goto_instr(struct instructions *i1, struct crash_parameters *, FILE *);
int literal_control_instr(struct instructions *i1, struct crash_parameters *, FILE *);


int instruction_execute(struct registers *, struct instructions *, int [], struct crash_parameters *, FILE *);
int push(struct registers *);
int pop (struct registers *);

int bit_flips(struct registers *, int [], struct crash_parameters *, time_t, struct instructions *, FILE *);
int check_pgm_crash(struct crash_parameters *, time_t, struct registers*);
int check_pgm_error(struct crash_parameters *, struct registers *, struct instructions *, int program_memory[], FILE *f);

//----------------------------------------Function definitions---------------------------------------------------------//


int instruction_fetch(struct registers *r, int program_memory[],struct crash_parameters *cp)
{
    int instruction;
    instruction = program_memory[r-> PC];
    
    PRINT("-------------------------------------------------------------------\n");
    PRINT("INSTRUCTION FETCH >>\n");
//    printf("Fetching instruction from program_memory[%x]\n",r->PC);
	
	/*if(r-> GP_Reg[2] > 15)
		{
		printf("Random reg selected: %x\n",cp->random_reg);
		printf("PC changed to %x\n\n", r->GP_Reg[2]);
		
		//exit(0);
		}*/
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


int decode_byte_instr(struct instructions *i1, struct crash_parameters *cp, struct registers *r1, int program_memory[], FILE *fnew)

{
        
        i1-> reg_file_addr = (i1->instruction) & 0x007F;
        i1->d= (i1->instruction & 0x80) >> 7;
//      i1->reg_index = (i1->reg_file_addr) - 12; // Reg file starts only from 0CH = 12
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
					printf("Instruction fetched from program_memory[%x] is %x\n",((r1-> PC)-1), program_memory[(r1-> PC)-1]);
		fprintf(fnew,"Instruction fetched from program_memory[%x] is %x\n",((r1-> PC)-1), program_memory[(r1-> PC)-1]);
		//printf("Bit flipped, Content of the program_memory[%x] is (in hex) %x\n\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
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

                default: printf("Crash due to illegal opcode\n");
						fprintf(fnew,"Crash due to illegal opcode\n");
						 cp->crash= (cp->crash)+1;
						 cp->crash_dueto_illegal_opcode++;
						 cp->crash_at_instr[cp->crash] = cp->instr_cycles; //Store the instr cycles
						 printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
						fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
                break;

        }
PRINT("-------------------------------------------------------------------\n");
return 0;

}


int decode_bit_instr(struct instructions *i1, struct crash_parameters *cp, FILE *fnew)
{
                                                
        i1-> reg_file_addr = (i1->instruction) & 0x007F;
        i1-> bit= ((i1->instruction) & 0x0380) >> 7;
//      i1-> reg_index = (i1-> reg_file_addr) - 12; // Reg file starts only from 0CH = 12
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

                
                default: printf("Crash due to illegal opcode\n");
						fprintf(fnew,"Crash due to illegal opcode\n");
						 cp->crash= (cp->crash)+1;
						 cp->crash_dueto_illegal_opcode++;
						 cp->crash_at_instr[cp->crash] = cp->instr_cycles;
						 printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
						fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
                break;

        }

PRINT("-------------------------------------------------------------------\n");         
return 0;

}
        


int literal_control_instr(struct instructions *i1, struct crash_parameters *cp, FILE *fnew)
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

                default: printf("Crash due to illegal opcode\n");
						fprintf(fnew,"Crash due to illegal opcode\n");
						 cp->crash= (cp->crash)+1;
						 cp->crash_dueto_illegal_opcode++;
						 cp->crash_at_instr[cp->crash] = cp->instr_cycles;
						 printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
						fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
                break;

        }
PRINT("-------------------------------------------------------------------\n");
return 0;
}

int call_goto_instr(struct instructions *i1,struct crash_parameters *cp, FILE *fnew)
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
                
                default:printf("Crash due to illegal opcode\n");
						fprintf(fnew,"Crash due to illegal opcode\n");
  					    cp->crash= (cp->crash)+1;
						cp->crash_dueto_illegal_opcode++;
						cp->crash_at_instr[cp->crash] = cp->instr_cycles;
						printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
						fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
                break;

        }
                
PRINT("-------------------------------------------------------------------\n");
return 0;
}



int bit_flips(struct registers *r2,  int program_memory[], struct crash_parameters *cp, time_t start_seconds,struct instructions *i1, FILE *fnew)

{
        int random_bit=0, random_bit_mem =0;
        int i=0, c=0;
		int less=0, more=0;

     	
		time_t seconds = time(NULL);
		if (seconds == previous_time) 
			return;
		previous_time = seconds;
		//Get value from system clock and place in seconds variable. 
		PRINT("Value of seconds %lu\n", seconds);
		srand ((unsigned int)seconds);

		time_t crash_time;

		//Flip 1 bit in General purpose register
		// generate random number between 0 and PROBABILITY_INVERSE

		cp->random_number = rand() % PROBABILITY_INVERSE; // probability of flipping is (1/ (probability_inverse))
		less=PROBABILITY_INVERSE - 500;
		more=PROBABILITY_INVERSE - 1;
		//printf("less=%d, more=%d\n",less,more);
		  // 	printf("Random number generated: %d\n",cp->random_number);
		//	printf("Instruction cycle=%llu\n\n",cp->instr_cycles);

//Flip bits only under this condition
	if((less< cp->random_number) && (cp->random_number < more)) // probability of generating some number within the range: (1/ (probability_inverse))
	{
        printf("\nFlip function called: Random number generated: %d\n",cp->random_number);
		cp->flip_bit_flag=1; //Set flag when bit flips function is called. Check for program errors only when this is set.
		cp->random_reg[cp->reg_count] = rand() % 256 ; // Random number between 0 and 255
				
                random_bit = rand() % 8 ; // Random number between 0 and 7
		
		PRINT("Flip flag set to %d\n",cp->flip_bit_flag);
	
                //printf("\nBit flip function called\n");
               // printf("Random reg selected:%d, random bit to flip in this reg is %d\n",cp-> random_reg[cp->reg_count],random_bit);
           PRINT("Content of the random reg location[%d] is (in hex) %x\n",cp-> random_reg[cp->reg_count],r2->GP_Reg[cp-> random_reg[cp->reg_count]]);
                switch(random_bit)
                {
                        case 0:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 0);
                        break;
                
                        case 1:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 1);
                        break;

                        case 2:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 2);
                        break;

                        case 3:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 3);
                        break;

                        case 4:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 4);
                        break;

                        case 5:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 5);
                        break;

                        case 6:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 6);
                        break;

                        case 7:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 7);
                        break;

                        case 8:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 8);
                        break;

                        default: 
						printf("Invalid bit flip case\n");
						fprintf(fnew,"Invalid bit flip case\n");
                        break;
                }
        
        printf("Bit flipped, Content of the reg[%x] is (in hex) %x\n", cp-> random_reg[cp->reg_count], r2->GP_Reg[cp-> random_reg[cp->reg_count]]);

//Condition for program crash if Program counter value changes:
		if (cp-> random_reg[cp->reg_count] == 0x02 || cp-> random_reg[cp->reg_count] == 0x82 || cp-> random_reg[cp->reg_count] == 0x0A || cp-> random_reg[cp->reg_count] == 0x8A)
       		 {
			 			
			cp->crash= (cp->crash)+1;
			(cp-> crash_dueto_PC) ++;            
			printf("\nCrash number:%d\n",(cp->crash));
			fprintf(fnew,"\nCrash number:%d\n",(cp->crash));

			printf("Program crash due to PC value at location %x getting affected\n", cp-> random_reg[cp->reg_count]);
			fprintf(fnew,"Program crash due to PC value at location %x getting affected\n", cp-> random_reg[cp->reg_count]);

           // PRINT("Content of the reg[%x] is (in hex): %x\n", cp-> random_reg[cp->reg_count], r2->GP_Reg[cp-> random_reg[cp->reg_count]]);

			crash_time= time(NULL);
			//printf("Number of successful program runs before the crash: %llu\n",cp->program_runs);
			printf("Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);
			fprintf(fnew,"Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);
			
			printf("At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash, (crash_time-start_seconds));
			fprintf(fnew,"At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash, (crash_time-start_seconds));
			cp->crash_time_array[cp->crash] = (crash_time-start_seconds- (cp->crash_time_array[cp->crash] -1));
    
			//cp->program_runs= (cp->instr_cycles)/(NUM_OF_INSTR * CLOCKS_PER_INSTR * NUM_OF_PGM_RUNS);
			cp->crash_at_instr[cp->crash] = cp->instr_cycles;
			printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
           fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);

            cp->instr_cycles=0; //Reset instruction cycles after every crash
        //   exit(0);

				 //Reset program counter to beginning of the program
                    r2->GP_Reg[2]= r2->initial_PCL;
                    r2->GP_Reg[0x82]= r2->GP_Reg[2]; //PCL Bank 1 and Bank 0
                    r2->PCL= r2->GP_Reg[2];

                    r2->GP_Reg[0x0A]= r2->initial_PCLATH;
                    r2->GP_Reg[0x8A]= r2->GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
                    r2->PCLATH= r2->GP_Reg[0x0A];

                    r2->PC = (r2->PCL | (r2->PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. PC= 13 bits
                //----------------------------------------------------------------------------------------------------------------------------
          
   		  }



		//Flip 1 bit in program memory 
		// generate random number: 
                cp->random_mem[cp->mem_count] = rand() % 8192; // Random number between 0 and 8192

                random_bit_mem = rand() % 8 ; // Random number between 0 and 7

               // printf("Program memory location selected:%x, random bit to flip in this location is %d\n",cp-> random_mem[cp->mem_count],random_bit_mem);
                PRINT("Content of the program memory location[%x] is (in hex): %x\n",cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);

        
                switch(random_bit_mem)
                {
                        case 0:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 0);
                        break;
                
                        case 1:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 1);
                        break;

                        case 2:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 2);
                        break;

                        case 3:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 3);
                        break;

                        case 4:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 4);
                        break;

                        case 5:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 5);
                        break;

                        case 6:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 6);
                        break;

                        case 7:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 7);
                        break;

                        case 8:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 8);
                        break;

                        default: printf("Invalid bit flip case\n");
                        fprintf(fnew,"Invalid bit flip case\n");
						break;
                }
        
        printf("Bit flipped, Content of the program_memory[%x] is (in hex) %x\n\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
     
//Condition for program crash if illegal memory access
		if ( (0x4F < cp-> random_mem[cp->mem_count] && cp-> random_mem[cp->mem_count] < 0x7F) || (0xCF < cp-> random_mem[cp->mem_count] && cp-> random_mem[cp->mem_count] < 0xFF)) //Invalid memory location range
		{ 
		    cp->crash= (cp->crash)+1;
				(cp-> crash_dueto_illegal_mem)++;
				printf("\nCrash number:%d\n",(cp->crash));
				fprintf(fnew,"\nCrash number:%d\n",(cp->crash));
				
				printf("Program crash due to illegal memory access: Content of location %x got affected\n", cp-> random_mem[cp->mem_count]);
		   		fprintf(fnew,"Program crash due to illegal memory access: Content of location %x got affected\n", cp-> random_mem[cp->mem_count]);
			// printf("Content of the program_memory[%x] is (in hex): %x\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
		    
		     crash_time= time(NULL);  

				//cp->program_runs= (cp->instr_cycles)/(NUM_OF_INSTR * CLOCKS_PER_INSTR * NUM_OF_PGM_RUNS);
				cp->crash_at_instr[cp->crash] = cp->instr_cycles; 
				printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
	    		fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
			
				//printf("Number of successful program runs before the crash: %llu\n",cp->program_runs);
				printf("Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);
			    fprintf(fnew,"Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);

				printf("At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash,(crash_time-start_seconds));
				fprintf(fnew,"At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash,(crash_time-start_seconds));
				cp->crash_time_array[cp->crash] = (crash_time-start_seconds);

		 /* for(c=1;c<= (cp->crash); c++)
		  {
		    //Print the entire array containing the instruction cycles at which the crash occured each time
		   printf("Crash[%d]: Number of instruction cycles executed before the crash: %d\n", c,cp->crash_at_instr[c]);
		   printf("Crash[%d]: Seconds elapsed since the beginning of the program, before crashing: %d\n",c,cp->crash_time_array[c]);             
		  } */
		      
	  
		    cp->instr_cycles=0; //Reset instruction cycles after every crash
		      
	   	  }

	cp->reg_count = cp->reg_count + 1;
	cp->mem_count = cp->mem_count + 1;
 	 }  // End the "If probability is met"
PRINT("Ending bitflips function\n");

return 0;
}


//Its better not to use this function. The condition for program crash is being checked within the bit_flips function itself.
int check_pgm_crash(struct crash_parameters *cp, time_t start_seconds, struct registers *r2)

{

        int random_bit=0, random_bit_mem =0;
        int i=0, c=0;
		     	       
//Be careful with this time definition.. it has to be defined when the random number is being generated..not here..
		time_t seconds = time(NULL);
		if (seconds == previous_time) 
			return;
		previous_time = seconds;
		//Get value from system clock and place in seconds variable. 
		PRINT("Value of seconds %lu\n", seconds);
		srand ((unsigned int)seconds);

		time_t crash_time;

//Condition for program crash if Program counter value changes:
		if (cp-> random_reg[cp->reg_count] == 0x02 || cp-> random_reg[cp->reg_count] == 0x82 || cp-> random_reg[cp->reg_count] == 0x0A || cp-> random_reg[cp->reg_count] == 0x8A)
        {
			cp->crash= (cp->crash)+1;
			(cp-> crash_dueto_PC) ++;            
			printf("\nCrash number:%d\n",(cp->crash));
			printf("Program crash due to PC value at location %x getting affected\n", cp-> random_reg[cp->reg_count]);
           // PRINT("Content of the reg[%x] is (in hex): %x\n", cp-> random_reg[cp->reg_count], r2->GP_Reg[cp-> random_reg[cp->reg_count]]);

			
            crash_time= time(NULL);
				

			//cp->program_runs= (cp->instr_cycles)/(NUM_OF_INSTR * CLOCKS_PER_INSTR * NUM_OF_PGM_RUNS);
			cp->crash_at_instr[cp->crash] = cp->instr_cycles;
			printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);

			//printf("Number of successful program runs before the crash: %llu\n",cp->program_runs);
			printf("Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);
			printf("At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash, (crash_time-start_seconds));
			cp->crash_time_array[cp->crash] = (crash_time-start_seconds);
           
            cp->instr_cycles=0; //Reset instruction cycles after every crash
          // exit(0);

				 //Reset program counter to beginning of the program
                    r2->GP_Reg[2]= r2->initial_PCL;
                    r2->GP_Reg[0x82]= r2->GP_Reg[2]; //PCL Bank 1 and Bank 0
                    r2->PCL= r2->GP_Reg[2];

                    r2->GP_Reg[0x0A]= r2->initial_PCLATH;
                    r2->GP_Reg[0x8A]= r2->GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
                    r2->PCLATH= r2->GP_Reg[0x0A];

                    r2->PC = (r2->PCL | (r2->PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. PC= 13 bits
                    //----------------------------------------------------------------------------------------------------------------------------


           
         }

//Condition for program crash if illegal memory access
        if ( (0x4F < cp-> random_mem[cp->mem_count] && cp-> random_mem[cp->mem_count] < 0x7F) || (0xCF < cp-> random_mem[cp->mem_count] && cp-> random_mem[cp->mem_count] < 0xFF)) //Invalid memory location range
        { 
            cp->crash= (cp->crash)+1;
			(cp-> crash_dueto_illegal_mem)++;
			printf("\nCrash number:%d\n",(cp->crash));
			printf("Program crash due to illegal memory access: Content of location %x got affected\n", cp-> random_mem[cp->mem_count]);
           // printf("Content of the program_memory[%x] is (in hex): %x\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
            
             crash_time= time(NULL);  

			//cp->program_runs= (cp->instr_cycles)/(NUM_OF_INSTR * CLOCKS_PER_INSTR * NUM_OF_PGM_RUNS);
			cp->crash_at_instr[cp->crash] = cp->instr_cycles; 
			printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
    
			//printf("Number of successful program runs before the crash: %llu\n",cp->program_runs);
			printf("Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);
            printf("At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash,(crash_time-start_seconds));
			cp->crash_time_array[cp->crash] = (crash_time-start_seconds);

         /* for(c=1;c<= (cp->crash); c++)
          {
            //Print the entire array containing the instruction cycles at which the crash occured each time
           printf("Crash[%d]: Number of instruction cycles executed before the crash: %d\n", c,cp->crash_at_instr[c]);
           printf("Crash[%d]: Seconds elapsed since the beginning of the program, before crashing: %d\n",c,cp->crash_time_array[c]);             
          } */
              
  
            cp->instr_cycles=0; //Reset instruction cycles after every crash
              
   	  }

return 0;
}


		//Error
int check_pgm_error(struct crash_parameters *cp, struct registers *r2, struct instructions *i1, int program_memory[], FILE *fnew)
{

	int i=0,j=0;

    //Data at the reg_index (which was decoded in decode step) has changed.. and hence leads to an error in computed data
	if (cp->flip_bit_flag==1) //This flag is set only when the bit is flipped.
	{ 
		printf("\nCheck function called... Reg count=%d \n", cp-> reg_count);

		cp->flip_bit_flag=0; //Reset flag.
		PRINT("Reg index= %x\n",i1-> reg_index);
		for(i=0;i < cp-> reg_count;i++) //Check if the reg index is equal to any of the previously flipped registers
		{
			if (i1->reg_index == cp->random_reg[i]) // && cp->first_error== 0) //If first error
				{
				cp-> reg_index_match = 1;
				printf("\nReg index: %x, matches with random reg:%x. Reg count array index=%d\n",i1-> reg_index,cp-> random_reg[i], i);
				printf("Bit flipped, Content of the random reg[%x] is (in hex) %x\n", cp-> random_reg[i], r2->GP_Reg[cp-> random_reg[i]]);

				}
		}
	
		if (cp-> reg_index_match == 1)
		// This can be equal even if the same instruction is repeated after a certain number of cycles, hence check for PC value
		{
			printf("Reg index match, inside if\n");
			printf("PC value= %x\n",(r2-> PC) -1);
			printf("Same PC =%d\n",cp->same_PC);
			printf("cp-> store_PC_same[0]=%x\n",cp-> store_PC_same[0]);

			cp-> reg_index_match = 0; //reset
			for(j=0;j < cp->same_PC; j++) //hence check for additional constraints
			{
				printf("cp-> store_PC_same[%d]=%x\n", j, cp-> store_PC_same[j]);
				//if(r2->GP_Reg[cp-> random_reg[i]] != cp->store_same_reg_modification[j]) // The reg content shouldnt be the same
				if( ((r2-> PC)-1) != cp-> store_PC_same[j] ) // error for the same instruction should be recorded only once
					{
					cp->set_no_same_PC = 1; //For loop is not entered for the very first time..hence initialised to 1 in main()
					printf("j=%d, PC not same as previous ones..counting as error\n",j);
					//printf("same_PC count=%d, j=%d, cp-> store_PC_same[%d]=%x\n",cp->same_PC, j,j, cp-> store_PC_same[j]);
					}

				else 
					{
					cp->set_no_same_PC = 0; // If same PC value
					printf("j=%d,PC %d same as previous ones.. and same reg index. So, not counting \n",j,(r2-> PC)-1);
					printf("Number of instruction cycles executed before the error: %llu\n",cp->instr_cycles_for_error);
					}

			if(cp->set_no_same_PC ==0)
				break; //The moment the PC matches with any one of the values stored already, exit the for loop
			}
	

			if(cp->set_no_same_PC == 1)
			{ //Prevent the same error from printing multiple times..
				cp->set_no_same_PC =0; //reset
				
				printf("\nERROR: Incorrect data\n");
				fprintf(fnew,"\nERROR: Incorrect data\n");
				
				//cp->store_same_reg_modification[cp->same_reg++] = r2->GP_Reg[cp-> random_reg[j]];
		
				printf("PC value (in hex)=%x, instruction opcode being executed (in hex)=%x\n", (r2-> PC)-1, program_memory[ (r2-> PC) -1]);
				fprintf(fnew,"PC value (in hex)=%x, instruction opcode being executed (in hex)=%x\n", (r2-> PC)-1, program_memory[ (r2-> PC) -1]);

				printf("Reg index %x\n",i1-> reg_index);
				fprintf(fnew,"Reg index %x\n",i1-> reg_index);

				cp-> store_PC_same[cp->same_PC]= (r2-> PC)-1; //PC 
			//same_PC will have same value as that of same_reg
		  	
			printf("cp->store_PC_same[%d]=%x\n",cp->same_PC, cp-> store_PC_same[cp->same_PC]);
			printf("same_PC count before incrementing=%d\n", cp->same_PC);
				cp->incorrect_data++;
				cp->error_at_instr[cp->error] = cp->instr_cycles_for_error;
				cp->error= (cp->error)+1;
				cp->same_PC= (cp->same_PC) +1; //gets incremented only if unique PC value is stored
				printf("Same PC incremented to %d\n",cp->same_PC);
				printf("Number of instruction cycles executed before the error: %llu\n\n",cp->instr_cycles_for_error);

				printf("******Errors so far...****** %d\n",cp->error);
				fprintf(fnew,"******Errors so far...****** %d\n",cp->error);
				//cp->first_error = 1;
				}
		
		} //end if (cp-> reg_index_match == 1)

	} //end if (cp->flip_bit_flag==1)
return 0;

}

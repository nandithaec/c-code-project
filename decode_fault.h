#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INSTR_SIZE 10
#define REG_MAX 255
#define REG_WIDTH 8
#define PROGRAM_MEM_SIZE 8192
#define MNEMONIC_SIZE 10
#define CONFIG_WORD_SIZE 14
#define MEM_WIDTH 14
#define FILE_CHARS 80
#define MAX_CRASHES 10
#define NUM_OF_PGM_RUNS 10
#define NUM_OF_INSTR 15
#define CLOCKS_PER_INSTR 4
#define PROBABILITY_INVERSE 100
#define RANDOM_GUESS_RANGE 101
#define INSTR_CYCLES_NUMBER 10000
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
    int GP_Reg[REG_MAX]; //General purpose register file map 255 locations accessed through reg_index or reg_file_addr
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
	int  starting_PC_value;
	int max_instr;
	int instruction;

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
	int decode_bits;     
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
	int first_error;
	int control_flow_change;
	unsigned long long int first_error_at_instr[INSTR_CYCLES_NUMBER];
	unsigned long long int rest_of_the_errors[INSTR_CYCLES_NUMBER];
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
	int crash_mem_access;
	int opcode_count;
	int random_bit_mem;
	int opcode_change;
	int crash_reg_index;
	int erroneous_instruction;
	int flip_bit_flag_for_illegal_inst;
	int incorrect_data_flag;
	int errors_so_far;
	int other_errors;

};


//Function declarations
int read_instr_from_file(FILE *,int program_memory[],struct registers *, FILE *);
int instruction_fetch(struct registers*, int [],struct crash_parameters *);
int PC_increment(struct registers *);
int increment_PC(struct registers **);

int reset_PC_to_beginninng(struct registers *);
int initialise_regs(struct registers*);
int initialise_crash_param(struct crash_parameters *);

int instruction_decode(struct registers *, struct instructions *, int[] ,struct crash_parameters *, FILE *, FILE *, time_t start_seconds);

int decode_byte_instr(struct instructions *i1,struct crash_parameters *,struct registers *, int [], FILE *, FILE *, time_t start_seconds);
int decode_bit_instr(struct registers *, struct instructions *i1, struct crash_parameters *, FILE *, FILE *, time_t start_seconds, int[]);
int call_goto_instr(struct registers *, struct instructions *i1, struct crash_parameters *, FILE *, FILE *, time_t start_seconds, int[]);
int literal_control_instr(struct registers *, struct instructions *i1, struct crash_parameters *, FILE *, FILE *, time_t start_seconds, int[]);


int instruction_execute(struct registers *, struct instructions *, int [], struct crash_parameters *, FILE *, FILE *, time_t start_seconds);
int push(struct registers *);
int pop (struct registers *);

int bit_flips(struct registers *, int [], struct crash_parameters *, time_t, struct instructions *, FILE *, FILE *);
int check_pgm_crash(struct crash_parameters *, time_t, struct registers*);
int check_pgm_error(struct crash_parameters *, struct registers *, struct instructions *, int program_memory[], FILE *);
int check_illegal_instr(struct registers *,  int [], struct crash_parameters *, time_t ,struct instructions *, FILE *, FILE *);
int report_crash(struct registers *,  int program_memory[], struct crash_parameters *, time_t start_seconds,struct instructions *, FILE *, FILE *);

	
int handle_byte_instruction_error(struct registers *r2,int program_memory[],struct crash_parameters *cp,time_t start_seconds,struct instructions *i1,FILE *fnew,FILE *fp);

	
int handle_bit_instruction_error(struct registers *r2,int program_memory[],struct crash_parameters *cp,time_t start_seconds,struct instructions *i1,FILE *fnew,FILE *fp);

int handle_literal_instruction_error(struct registers *r2,int program_memory[],struct crash_parameters *cp,time_t start_seconds,struct instructions *i1,FILE *fnew,FILE *fp);

int report_error(struct crash_parameters *, struct registers *, struct instructions *, int program_memory[], FILE *);
int reset_after_crash(struct registers *,  int program_memory[], struct crash_parameters *, time_t start_seconds, FILE *, FILE *);

//----------------------------------------Function definitions---------------------------------------------------------//
int initialise_regs(struct registers *r)
{
	int i=0;
	printf("Initialising all registers\n");
		r->max_instr=0;
		r->instruction=0;
	//Max register content= 255 (dec) or FF (hex)
        r->configuration_word[11]= 1; //WDT Enabler bit
		r->starting_PC_value=0;
//clear all registers
			for(i=0;i<REG_MAX;++i)
                r->GP_Reg[i]=0; //clear all registers in register file map
       
        //INTCON Register
                r->GP_Reg[0x0B]=0; //INTCON register at address 0B in the register file map
                r->GP_Reg[0x8B]= r->GP_Reg[0x0B];

        //for(i=0;i<MEM_WIDTH;++i)
        //      r->PC=0;
        //STACK
                r->stack_pointer = 0;
                r->stack[1] = 0x62;
                r->stack[0] = 0x88;

        // Status register = GP_Reg[3]
        //Assigning some value to the carry in status reg
//              r->GP_Reg[3]= r->GP_Reg[3] & 0xFE; //carry = 0
                //r->GP_Reg[3]= r->GP_Reg[3] | 0x01; //carry = 1
                r->GP_Reg[3]= 0x00;
                r->GP_Reg[0x83]= r->GP_Reg[3]; //Bank 1 and Bank 0

                r->W = 0x00;
               
        //TMRO
                r->GP_Reg[1]= 0x00;

        //OPTION REG
                r->GP_Reg[0x81]= 0x00;

                //FSR
                r->GP_Reg[4]= 0x00;
                r->GP_Reg[0x84]= r->GP_Reg[4];

        //PORTA
                r->GP_Reg[5]= 0x00;

        //PORTB
                r->GP_Reg[6]= 0x00;

        //TRISA
                r->GP_Reg[0x85]= 0x00;

        //TRISB
                r->GP_Reg[0x86]= 0x00;

        //EEDATA
                r->GP_Reg[8]= 0x00;

        //EEADR
                r->GP_Reg[9]= 0x00;

        //EECON1
                r->GP_Reg[0x88]= 0x00;

        //EECON2 - not a physical register
        //      r->GP_Reg[0x89]= 0x00;

//Initialise PC values
				 r->initial_PCL =0;
				  r->initial_PCLATH=0;

//use this address 0x02 to write into PCL:
//PCL= GP_Reg[2] and GP_Reg[0x82]
                r->GP_Reg[2]= 0x00;
                r->GP_Reg[0x82]= r->GP_Reg[2]; //Bank 1 and Bank 0
                r->PCL= r->GP_Reg[2];

        //PCLATH.. use this address to write into PCLATH
                r->GP_Reg[0x0A]= 0x00;
                r->GP_Reg[0x8A]= r->GP_Reg[0x0A]; //Bank 1 and Bank 0
                r->PCLATH= r->GP_Reg[0x0A];

                r->PC = (r->PCL | (r->PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits

                PRINT("-----------------------------------------------------------------\n");
                PRINT("Initial values (hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",r->PCL, r->PCLATH, r->PC);

return 0;
}

int initialise_crash_param(struct crash_parameters *cp)
{
	int i=0;
	printf("Initialising crash parameters\n");
		cp->random_number=0;
		cp->random_bit_mem=0;
		cp->reg_count=0;
		cp->mem_count=0;
		cp->instr_cycles=0;
		cp->instr_cycles_for_error=0;
		cp->crash=0;
		cp->program_runs=0;
		cp->crash_dueto_illegal_mem=0;
		cp->crash_dueto_PC=0;
		cp->crash_dueto_illegal_opcode=0;
		cp->first_error=0;
		cp->control_flow_change=0;
		cp->incorrect_data=0;
		cp->same_reg=0;
		cp->same_PC=0;
		cp-> reg_index_match=0;
		cp->set_no_same_PC=1; // for the very first PC
		cp->crash_mem_access = 0;
		cp->opcode_count =0;
		cp->opcode_change=0;
		cp->crash_reg_index=0;
		cp->erroneous_instruction=0;
		cp->flip_bit_flag_for_illegal_inst=0;
		cp->incorrect_data_flag=0;
		cp->other_errors=0;
		cp->errors_so_far=0;

		//clear all locations
			for(i=0;i<NUM_OF_BITFLIPS;++i)
                cp->store_PC_same[i]=0;
			
			for(i=0;i<MAX_CRASHES;++i)
                cp->crash_at_instr[i]=0;
			
			for(i=0;i<MAX_CRASHES;++i)
                cp->crash_time_array[i]=0;
			
			for(i=0;i<INSTR_CYCLES_NUMBER;++i)
                cp->first_error_at_instr[i]=0;

			for(i=0;i<INSTR_CYCLES_NUMBER;++i)
                cp->rest_of_the_errors[i]=0;
	
			for(i=0;i<NUM_OF_BITFLIPS;++i)
                cp->random_reg[i]=0;

			for(i=0;i<NUM_OF_BITFLIPS;++i)
                cp->random_mem[i]=0;

			for(i=0;i<NUM_OF_BITFLIPS;++i)
                cp->store_same_reg_modification[i]=0;

return 0;
}



int read_instr_from_file(FILE *fp, int program_memory[], struct registers *r, FILE *fnew)
{

	int i=0;
    char line[FILE_CHARS]; 
    //int starting_PC_value = 0;
	int instr_from_file=0;

	reset_PC_to_beginninng(r); //Initial PC values are set in this function
		
 		r->max_instr= r->PC; // new value of max_instr is initialised to the PC value that is read from the disassembly listing
        r->starting_PC_value = r->PC;
        PRINT("Starting PC value=%x\n",r->starting_PC_value);

		fp= fopen("simple_add_assembly.c","rt");
        if( fp == NULL )
         {
               puts ( "cannot open file" ) ;
          //     exit(0) ;
         }  

		while(fgets(line, FILE_CHARS, fp) != NULL)
           {
                // get a line, up to 80 chars from fp.  done if NULL 
                sscanf (line, "%x", &instr_from_file);
                program_memory[r->max_instr]= instr_from_file;
                r->max_instr = (r->max_instr) + 1; //max_instr indicates the max instructions
               
           }
         
		printf("\nLoading instructions to memory\n");
		fprintf(fnew,"\nLoading instructions to memory\n");

        for(i= r->starting_PC_value;i< (r->max_instr);i++)
        {
		    printf("Instructions read from file= program_memory[%x]= %x\n",i, program_memory[i]);
			fprintf(fnew,"Instructions read from file= program_memory[%x]= %x\n",i, program_memory[i]);
		}

	printf("Maximum number of instructions in the program is: %d\n\n", r->max_instr);
	fprintf(fnew,"Maximum number of instructions in the program is: %d\n\n", r->max_instr);


	fclose(fp);  // close the file prior to exiting the function

return 0;

}


int reset_PC_to_beginninng(struct registers *r)
{
//----------------------------------------------------------------------------------------------------------------------------
                    //Reset program counter to beginning of the program
                    r->GP_Reg[2]= r->initial_PCL;
                    r->GP_Reg[0x82]= r->GP_Reg[2]; //PCL Bank 1 and Bank 0
                    r->PCL= r->GP_Reg[2];

                    r->GP_Reg[0x0A]= r->initial_PCLATH;
                    r->GP_Reg[0x8A]= r->GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
                    r->PCLATH= r->GP_Reg[0x0A];

                    r->PC = (r->PCL | (r->PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. PC= 13 bits
                    //----------------------------------------------------------------------------------------------------------------------------

	//	printf("PC is reset to its initial values (in hex): PCL=%x, PCLATH=%x, PC=%x\n",r->PCL, r->PCLATH, r->PC);
		

return 0;
}



int instruction_fetch(struct registers *r, int program_memory[],struct crash_parameters *cp)
{
    //int instruction;
    r-> instruction = program_memory[r-> PC];
    
    PRINT("-------------------------------------------------------------------\n");
    PRINT("INSTRUCTION FETCH >>\n");
//    printf("Fetching instruction from program_memory[%x]\n",r->PC);
	
	/*if(r-> GP_Reg[2] > 15)
		{
		printf("Random reg selected: %x\n",cp->random_reg);
		printf("PC changed to %x\n\n", r->GP_Reg[2]);
		
		//exit(0);
		}*/
    PRINT("Current PC: PCL=%x, PCLATH=%x, PC = %x \n",r->GP_Reg[2],r->PCLATH, r->PC);


    PRINT("-------------------------------------------------------------------\n");
    return 0;
        
}                       

int PC_increment(struct registers *r)
{
    //Increment PC
    increment_PC(&r);
	PRINT("After Incrementing PC: PCL=%x, PCLATH=%x, PC = %x \n",r->GP_Reg[2],r->PCLATH, r->PC);
	
	return 0;
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




int instruction_decode(struct registers *r1, struct instructions *i1, int program_memory[] ,struct crash_parameters *cp, FILE *fnew, FILE *fp, time_t start_seconds)
{

  int opcode = 0;
  int reg_index=0,  reg_file_addr = 0, d=0;

		i1->decode_bits= (r1->instruction & 0x3000)>> 12;  // bits 13 and 14
        PRINT("Decode bits= %d \n", i1->decode_bits);

             
        i1->instruction= r1->instruction; //Instruction fetched 
        i1->opcode= opcode; 
        i1->reg_file_addr= reg_file_addr;
        i1->d= d;
        i1->reg_index= reg_index;
        i1->bit = 0;
        i1->immediate_value = 0;


        switch (i1->decode_bits)
        {
                case 0:
                        decode_byte_instr(i1,cp,r1,program_memory,fnew,fp, start_seconds);
                        break;

                case 1:
                        decode_bit_instr(r1,i1,cp,fnew,fp, start_seconds, program_memory);  
                        break;

                case 2:
                        call_goto_instr(r1,i1,cp,fnew,fp, start_seconds,program_memory);  
                        break;

                case 3:
                        literal_control_instr(r1, i1,cp,fnew,fp, start_seconds,program_memory);    
                        break;

                default:
                        printf("Invalid decode_bits inside main\n");
						fprintf(fnew,"Invalid decode_bits inside main\n");
                        break;
        }

		PRINT("decode done\n");
       

return 0;
}






int decode_byte_instr(struct instructions *i1, struct crash_parameters *cp, struct registers *r1, int program_memory[], FILE *fnew, FILE *fp, time_t start_seconds)

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
					//printf("Instruction fetched from program_memory[%x] is %x\n",((r1-> PC)), program_memory[(r1-> PC)]);
		//fprintf(fnew,"Instruction fetched from program_memory[%x] is %x\n",((r1-> PC)), program_memory[(r1-> PC)]);
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
                               // fprintf(fnew,"Instruction mnemonic = NOP\n");
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

                default: printf("\nCRASH: Crash due to illegal opcode\n");
						 fprintf(fnew,"\nCRASH: Crash due to illegal opcode\n");
						 
						 cp->crash_dueto_illegal_opcode++;
						 report_crash(r1,  program_memory, cp, start_seconds,i1, fnew, fp);

                break;

        }
PRINT("-------------------------------------------------------------------\n");
return 0;

}


int decode_bit_instr(struct registers *r1, struct instructions *i1, struct crash_parameters *cp, FILE *fnew, FILE *fp, time_t start_seconds, int program_memory[])
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

                
                default: printf("\nCRASH: Crash due to illegal opcode\n");
					 	 fprintf(fnew,"\nCRASH: Crash due to illegal opcode\n");
						  
						 cp->crash_dueto_illegal_opcode++;
						 
						 report_crash(r1,  program_memory, cp, start_seconds,i1, fnew, fp);
                break;

        }

PRINT("-------------------------------------------------------------------\n");         
return 0;

}
        


int literal_control_instr(struct registers *r1, struct instructions *i1, struct crash_parameters *cp, FILE *fnew, FILE *fp, time_t start_seconds, int program_memory[])
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

                default: printf("\nCRASH: Crash due to illegal opcode\n");
						 fprintf(fnew,"\nCRASH: Crash due to illegal opcode\n");
						 
						 cp->crash_dueto_illegal_opcode++;
						 report_crash(r1,  program_memory, cp, start_seconds,i1, fnew, fp);
                break;

        }
PRINT("-------------------------------------------------------------------\n");
return 0;
}

int call_goto_instr(struct registers *r1, struct instructions *i1,struct crash_parameters *cp, FILE *fnew, FILE *fp, time_t start_seconds, int program_memory[])
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
                
                default:printf("\nCRASH: Crash due to illegal opcode\n");
						fprintf(fnew,"\nCRASH: Crash due to illegal opcode\n");
  					    
						cp->crash_dueto_illegal_opcode++;
						report_crash( r1,  program_memory, cp, start_seconds,i1, fnew, fp);
                break;

        }
                
PRINT("-------------------------------------------------------------------\n");
return 0;
}



int bit_flips(struct registers *r2,  int program_memory[], struct crash_parameters *cp, time_t start_seconds,struct instructions *i1, FILE *fnew, FILE *fp)

{
        int random_bit=0;
        int i=0, c=0, ii=0;
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
		less=PROBABILITY_INVERSE - RANDOM_GUESS_RANGE;
		more=PROBABILITY_INVERSE - 1;
		//printf("less=%d, more=%d\n",less,more);
		  // 	printf("Random number generated: %d\n",cp->random_number);
		//	printf("Instruction cycle=%llu\n\n",cp->instr_cycles);

//Flip bits only under this condition
	if((less< cp->random_number) && (cp->random_number < more)) // probability of generating some number within the range: (1/ (probability_inverse))
	{
        printf("\nFlip function called: Random number generated: %d\n",cp->random_number);
		if(cp->flip_bit_flag==0) //If you enter this if statement without previous bit flips, then set thsi to 1.
			cp->flip_bit_flag=1; //Set flag when bit flips function is called. Check for program errors only when this is set.

		else 
/*This else part will be entered only if the probability is high.
If you enter this, before resetting the flag, that is, still when the check_error function is checking for errors at all instructions, then it means that 
a bit has flipped even before all instructions have been checked for errors. Hence reset the count to zero so that all instructions will be checked again
*/			cp->opcode_count = 0; //Can be used for counting double errors if it happens at the same PC location


//Set this flag to use in the function:check_illegal_instr and clear it in that function
		cp->flip_bit_flag_for_illegal_inst=1; 

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

                      /*   case 8:
                        r2->GP_Reg[cp-> random_reg[cp->reg_count]]=  r2->GP_Reg[cp-> random_reg[cp->reg_count]] ^(1 << 8);
                        break; */

                        default: 
						printf("Invalid bit flip case\n");
						fprintf(fnew,"Invalid bit flip case\n");
                        break;
                }
        
        printf("Bit flipped, Content of the reg[%x] is (in hex): %x\n", cp-> random_reg[cp->reg_count], r2->GP_Reg[cp-> random_reg[cp->reg_count]]);

//Condition for program crash if Program counter value changes:
		if (cp-> random_reg[cp->reg_count] == 0x02 || cp-> random_reg[cp->reg_count] == 0x82 || cp-> random_reg[cp->reg_count] == 0x0A || cp-> random_reg[cp->reg_count] == 0x8A)
       		 {
			 			
			cp->crash= (cp->crash)+1;
			(cp-> crash_dueto_PC) ++;            
			printf("\nCrash number:%d\n",(cp->crash));
			fprintf(fnew,"\nCrash number:%d\n",(cp->crash));

			printf("Program crash due to PC value at location %x getting affected\n", cp-> random_reg[cp->reg_count]);
			fprintf(fnew,"Program crash due to PC value at location %x getting affected\n", cp-> random_reg[cp->reg_count]);

			printf("Random number that got generated this time was: %d\n", cp->random_number );
			fprintf(fnew,"Random number that got generated this time was: %d\n", cp->random_number );
    
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
	
			//initialise_crash_param(cp);
			//initialise_regs(r2);
          
        //   exit(0);

//------------------------Reset conditions after crash---------------------------------------

			reset_after_crash(r2, program_memory, cp, start_seconds, fnew, fp);
//-------------------------------------------------------------------------------
   		  }



		//Flip 1 bit in program memory - will change the opcode
		// generate random number: 
                cp->random_mem[cp->mem_count] = rand() % 20; // Random number between 0 and 8192. Store it in an array to keep track and compare later
				
				//cp->random_mem[cp->mem_count] = 2; //Forcing a value

			//mem_count just keeps count of how many memory locations have been flipped. And is the array index for the array whoch stores the flipped location address

                cp->random_bit_mem = rand() % 14 ; // Random number between 0 and 13
			//	cp->random_bit_mem = 10 ; //Forcing a value

               // printf("Program memory location selected:%x, random bit to flip in this location is %d\n",cp-> random_mem[cp->mem_count],cp->random_bit_mem);
                PRINT("Content of the program memory location[%x] is (in hex): %x\n",cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);

        //Bit 0 is the LSB (rightmost) and Bit 13 is MSB (leftmost)
                switch(cp->random_bit_mem)
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

 						case 9:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 8);
                        break;

 						case 10:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 8);
                        break;

 						case 11:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 8);
                        break;

 						case 12:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 8);
                        break;

 						case 13:
                        program_memory[cp-> random_mem[cp->mem_count]]=    program_memory[cp-> random_mem[cp->mem_count]] ^(1 << 8);
                        break;

 						default: printf("Invalid bit flip case\n");
                        fprintf(fnew,"Invalid bit flip case\n");
						break;
                }
        
        printf("Bit flipped, Content of the program_memory[%x] is (in hex): %x\n\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
     

	cp->reg_count = cp->reg_count + 1;
	cp->mem_count = cp->mem_count + 1;
 	 }  // End the "If probability is met"
PRINT("Ending bitflips function\n");

return 0;
}


//Check for program error (error in data)

int check_pgm_error(struct crash_parameters *cp, struct registers *r2, struct instructions *i1, int program_memory[], FILE *fnew)
{

	int i=0,j=0;

    //Data at the reg_index (which was decoded in decode step) has changed.. and hence leads to an error in computed data
	if (cp->flip_bit_flag==1 && cp->opcode_count++ < NUM_OF_INSTR) //This flag is set only when the bit is flipped.
//And repeat this comparison for every opcode in the program, sine the flipped reg can be equal to the reg file in any of the instructions..
//Hopefully another bit doesnt flip during this comparison
	{ 
		//printf("\nCheck program error... Register bit flip count=%d.. (Prints every cycle)\n", cp-> reg_count); //Gets printed every cycle

		//cp->flip_bit_flag=0; //Reset flag.
		PRINT("Reg file address= %x\n",i1-> reg_index);
		for(i=0;i < cp-> reg_count;i++) //Check if the reg index is equal to any of the previously flipped registers
/*Reg count has been incremented at the end of the bit flips function. And this check program error has been called after the bitflips function.
Hence, the for loop should run only till less than reg_count and not equal to reg_count */
		{
			if (i1->reg_index == cp->random_reg[i]) // && cp->first_error== 0) //If first error
				{
				cp-> reg_index_match = 1;
				printf("\n*****Reg file address: %x, matches with random reg:%x. Register bit flip array index=%d*****\n",i1-> reg_index,cp-> random_reg[i], i);
				printf("Bit flipped, Content of the random reg[%x] is (in hex) %x\n", cp-> random_reg[i], r2->GP_Reg[cp-> random_reg[i]]);

				}
		}
	
		if (cp-> reg_index_match == 1)
		// This can be equal even if the same instruction is repeated after a certain number of cycles, hence check for PC value
		{
			printf("Reg file address match, inside if\n");
			printf("PC value= %x\n",(r2-> PC));
			printf("Same PC =%d\n",cp->same_PC);
			printf("cp-> store_PC_same[0]=%x\n",cp-> store_PC_same[0]);

			cp-> reg_index_match = 0; //reset
			for(j=0;j < cp->same_PC; j++) //hence check for additional constraints
			{
				printf("cp-> store_PC_same[%d]=%x\n", j, cp-> store_PC_same[j]);
				//if(r2->GP_Reg[cp-> random_reg[i]] != cp->store_same_reg_modification[j]) // The reg content shouldnt be the same
				if( ((r2-> PC)) != cp-> store_PC_same[j] ) // error for the same instruction should be recorded only once
					{
					cp->set_no_same_PC = 1; //For loop is not entered for the very first time..hence initialised to 1 in main()
					printf("j=%d, PC not same as previous ones..counting as error\n",j);
					//printf("same_PC count=%d, j=%d, cp-> store_PC_same[%d]=%x\n",cp->same_PC, j,j, cp-> store_PC_same[j]);
					}

				else 
					{
					cp->set_no_same_PC = 0; // If same PC value
					printf("j=%d,PC %x same as previous ones.. and same reg index. So, not counting \n",j,(r2-> PC));
					printf("Number of instruction cycles executed before the error: %llu\n",cp->instr_cycles_for_error);
					}

			if(cp->set_no_same_PC ==0)
				break; //The moment the PC matches with any one of the values stored already, exit the for loop
			}
	

			if(cp->set_no_same_PC == 1)
			{ //Prevent the same error from printing multiple times..
				cp->set_no_same_PC =0; //reset
				//*****report error because of incorrect data in the location that is being accessed********************
				cp->incorrect_data++;
				cp->incorrect_data_flag=1;
				
				printf("Reg file address %x\n",i1-> reg_index);
				fprintf(fnew,"Reg file address %x\n",i1-> reg_index);

				cp-> store_PC_same[cp->same_PC]= (r2-> PC); //PC 
				//same_PC will have same value as that of same_reg
		  	
				printf("cp->store_PC_same[%d]=%x\n",cp->same_PC, cp-> store_PC_same[cp->same_PC]);
				fprintf(fnew,"cp->store_PC_same[%d]=%x\n",cp->same_PC, cp-> store_PC_same[cp->same_PC]);
				//printf("same_PC count before incrementing=%d\n", cp->same_PC);
				cp->same_PC= (cp->same_PC) +1; //gets incremented only if unique PC value is stored
				//printf("Same PC incremented to %d\n",cp->same_PC);

				printf("\nERROR: Incorrect data\n");
				fprintf(fnew,"\nERROR: Incorrect data\n");

				report_error(cp,r2,i1,program_memory,fnew);
			}
		
		} //end if (cp-> reg_index_match == 1)

	} //end if (cp->flip_bit_flag==1)

	else
	if (cp->opcode_count == NUM_OF_INSTR)
	{
		cp->flip_bit_flag=0; //Reset flag.
		printf("Done comparing the register with flipped bit with all instructions in the program\n\n");
	}
		
return 0;

}


//Condition for program crash if illegal instruction 
int check_illegal_instr(struct registers *r2,  
                        int program_memory[], 
                        struct crash_parameters *cp, 
						time_t start_seconds,
						struct instructions *i1,
						FILE *fnew, 		
						FILE *fp)
{

int ii=0;
int  crash_decode_bits=0;

/*Algorithm:
if the random_mem is in the range of the PC values that the program uses, then there is a possibility that one of the opcode has changed.
Then map the random_mem to the PC value and get the instruction.
Decode the instruction and see which part of the instruction has changed
If 
	- the reg file index has changed 
		-and is one of the illegal mem locations- then it is a crash.
		-and is not an illegal mem location, but some other location, then it will lead to an error in the result

If
	-the opcode has changed, then it will result in error
	-will lead to crash if it is an illegal opcode

If
	-the immediate value has changed, then it will lead to error

If
	-the d bit or b bit has changed ,it will lead to error 

*/
	
//printf("\nCheck illegal instruction... \n"); //Gets printed every cycle

/*If the random_mem (that is the mem location flipped) in the program_memory is in the range of the PC values of the program..
For eg., if random_mem is 2, then program_memory[2] is flipped.
 Check if program_memory[2] contains any instruction. So, check if 2 is one of the PC values. If yes, then one of the instruction has changed.
So, now check for different error/crash conditions
*/
if (cp->flip_bit_flag_for_illegal_inst==1) //Check only if a bit has flipped
{
	cp->flip_bit_flag_for_illegal_inst=0; //reset it

	if( ((r2->starting_PC_value) < (cp-> random_mem[(cp->mem_count)-1])) && // if the flipped mem location is greater than the initial PC value
         ((cp-> random_mem[(cp->mem_count)-1]) < r2->max_instr) )           // if the flipped mem location is lesser than the max PC value
	{
/*Mem count has been incremented at the end of the bit flips function. And this check program error has been called after the bitflips function.
Hence, the condition should check for the mem_count -1 */

//Now check if the decode_bits itself has changed.. else decode the instruction and see which part of the instruction has changed.
        
		if ( (cp->random_bit_mem !=12) || (cp->random_bit_mem !=13) ) //Decode_bits hasnt changed. So, the category of instructions remains same as in the decode function
		{
			//Fetch instruction from the location which has its instruction flipped. PC = (cp-> random_mem[(cp->mem_count)-1])	
			cp->erroneous_instruction = program_memory[cp-> random_mem[(cp->mem_count)-1]];

			crash_decode_bits= (cp->erroneous_instruction & 0x3000)>> 12;  // bits 13 and 14
	        PRINT("Decode bits while verifying program crash= %d \n", crash_decode_bits);


	 		switch (crash_decode_bits)
		    {
		        case 0: //handle_byte_instruction_error()
		                //Byte instruction
						handle_byte_instruction_error(r2,program_memory, cp,start_seconds,i1,fnew,fp);
						
		        break;

		        case 1: //handle_bit_instruction_error()
		                //Bit instruction
						handle_bit_instruction_error(r2,program_memory, cp,start_seconds,i1,fnew,fp);
				 
		        break;

		        case 2: //handle_goto_instruction_error()
		                //CALL GOTO instruction
						// Code for report control flow error
						
						printf("\nCRASH: Control flow instruction. Control flow has changed..will lead to incorrect results\n");
						fprintf(fnew,"\nCRASH: Control flow instruction. Control flow has changed..will lead to incorrect results\n");
				  		
						cp->control_flow_change++;
						report_crash( r2,  program_memory, cp, start_seconds,i1, fnew, fp);

		        break;

		        case 3: //handle_literal_instruction_error()
		                //Literal and control instruction
						handle_literal_instruction_error(r2,program_memory, cp,start_seconds,i1,fnew,fp);
		        break;

		        default:
		                printf("Invalid decode_bits inside crash testing function\n");
						fprintf(fnew,"Invalid decode_bits inside crash testing function\n");
		        break;
		    } //switch close

		} //if random_bit close
		//handle_decode_bit_error()		
		else //If the decode_bits itself has flipped, report- Unable to decode instruction - report program crash
			 // Code for report crash
		{
			 printf("\nCRASH: Crash due to illegal opcode\n");
			 fprintf(fnew,"\nCRASH: Crash due to illegal opcode\n");
			 
			 cp->crash_dueto_illegal_opcode++;
			 report_crash( r2,  program_memory, cp, start_seconds,i1, fnew, fp);
			
		}
	} //close if PC range

} //close if cp->flip_bit_flag_for_illegal_inst
return 0;

}


	
int handle_byte_instruction_error(struct registers *r2,  
                        int program_memory[], 
                        struct crash_parameters *cp, 
						time_t start_seconds,
						struct instructions *i1,
						FILE *fnew, 		
						FILE *fp)
{


if ( (0 < cp->random_bit_mem) && (cp->random_bit_mem <= 6) ) //If one of the bits 0 to 7 are flipped, it means that the reg index has changed
						{
							cp->crash_reg_index = (cp->erroneous_instruction) & 0x007F; //Extract reg_index

							
							if ( ( 0x4F < cp->crash_reg_index && cp->crash_reg_index < 0x7F ) ||
								 ( 0xCF < cp->crash_reg_index && cp->crash_reg_index < 0xFF ) ) 

							{
								(cp-> crash_dueto_illegal_mem)++;
								printf("\nCRASH: Program crash due to illegal memory access: Trying to access location %x\n", cp->crash_reg_index);
						   		fprintf(fnew,"\nCRASH: Program crash due to illegal memory access: Trying to access location %x\n", cp->crash_reg_index);
								// printf("Content of the program_memory[%x] is (in hex): %x\n", cp->crash_reg_index, program_memory[cp->crash_reg_index]);
								report_crash( r2,  program_memory, cp, start_seconds,i1, fnew, fp);
								
							}

						}
						else 
							if ( (8 <= cp->random_bit_mem) && (cp->random_bit_mem <= 11) )
							//opcode has changed within the same group of instructions.. report error
							// Code for report error
							{
								cp-> opcode_change++;
								
								printf("\nERROR: Byte instruction: Opcode has changed, will lead to an error in program\n");
								fprintf(fnew,"\nERROR: Byte instruction: Opcode has changed, will lead to an error in program\n");
								report_error(cp,r2,i1,program_memory,fnew);
							}
							else if (cp->random_bit_mem == 7) //Destination has changed. report error
							// Code for report error
							{
								cp->incorrect_data++;
								
								printf("\nERROR: Byte instruction: Destination register has changed, will lead to an error in program\n");
								fprintf(fnew,"\nERROR: Byte instruction: Destination register has changed, will lead to an error in program\n");
								report_error(cp,r2,i1,program_memory,fnew);
							}

return 0;
}



int handle_bit_instruction_error(struct registers *r2,  
                        int program_memory[], 
                        struct crash_parameters *cp, 
						time_t start_seconds,
						struct instructions *i1,
						FILE *fnew, 		
						FILE *fp)
{


if ( (0 < cp->random_bit_mem) && (cp->random_bit_mem <= 6) ) //If one of the bits 0 to 7 are flipped, it means that the reg index has changed
						{
							cp->crash_reg_index = (cp->erroneous_instruction) & 0x007F; //Extract reg_index

							if ( ( 0x4F < cp->crash_reg_index && cp->crash_reg_index < 0x7F ) ||
								 ( 0xCF < cp->crash_reg_index && cp->crash_reg_index < 0xFF ) ) 
	
							{
								(cp-> crash_dueto_illegal_mem)++;
								printf("\nCRASH: Program crash due to illegal memory access: Trying to access location %x\n", cp->crash_reg_index);
						   		fprintf(fnew,"\nCRASH: Program crash due to illegal memory access: Trying to access location %x\n", cp->crash_reg_index);
								// printf("Content of the program_memory[%x] is (in hex): %x\n", cp->crash_reg_index, program_memory[cp->crash_reg_index]);
								report_crash(r2,  program_memory, cp, start_seconds,i1, fnew, fp);
							}

						}
						else 
							if ( (7 <= cp->random_bit_mem) && (cp->random_bit_mem <= 9) )
							//Bit b has changed within the same group of instructions.. report error
							// Code for report error
							{
								cp->incorrect_data++;
								
								printf("\nERROR: Bit instruction: Bit value in instruction has changed, will lead to an error in program\n");
								fprintf(fnew,"\nERROR: Bit instruction: Bit value in instruction has changed, will lead to an error in program\n");
								report_error(cp,r2,i1,program_memory,fnew);
							}
							else if ( (cp->random_bit_mem == 10) && (cp->random_bit_mem == 11) ) //
							//opcode has changed within the same group of instructions.. report error
							// Code for report error
							{
								cp->opcode_change++;

								printf("\nERROR: Bit instruction: Opcode has changed, will lead to an error in program\n");
								fprintf(fnew,"\nERROR: Bit instruction: Opcode has changed, will lead to an error in program\n");
								report_error(cp,r2,i1,program_memory,fnew);
							}

return 0;
}



int handle_literal_instruction_error(struct registers *r2,  
                        int program_memory[], 
                        struct crash_parameters *cp, 
						time_t start_seconds,
						struct instructions *i1,
						FILE *fnew, 		
						FILE *fp)
{


if ( (0 < cp->random_bit_mem) && (cp->random_bit_mem <= 7) ) //If one of the bits 0 to 7 are flipped, it means that the immediate value has changed
						{
							cp->incorrect_data++;
	
							printf("\nERROR: Literal and control instruction: immediate value has changed, will lead to an error in program\n");
							fprintf(fnew,"\nERROR: Literal and control instruction: Immediate value has changed, will lead to an error in program\n");	
							// Code for report error
							report_error(cp,r2,i1,program_memory,fnew);
				   		}
						else //whatever else has changed
						//opcode has changed within the same group of instructions.. report error
						{
						cp->opcode_change++;

						printf("\nERROR: Literal and control instruction: Opcode has changed, will lead to an error in program\n");
						fprintf(fnew,"\nERROR: Literal and control instruction: Opcode has changed, will lead to an error in program\n");	
						// Code for report error
						report_error(cp,r2,i1,program_memory,fnew);
						}

return 0;
}



int report_crash(struct registers *r2,  int program_memory[], struct crash_parameters *cp, time_t start_seconds,struct instructions *i1, FILE *fnew, FILE *fp)
{
	
		int ii=0;

		time_t crash_time;
		
    	cp->crash= (cp->crash)+1;
		
		printf("\nCrash number:%d\n",(cp->crash));
		fprintf(fnew,"\nCrash number:%d\n",(cp->crash));
		
    
    	 crash_time= time(NULL);  

		//cp->program_runs= (cp->instr_cycles)/(NUM_OF_INSTR * CLOCKS_PER_INSTR * NUM_OF_PGM_RUNS);
		cp->crash_at_instr[cp->crash] = cp->instr_cycles;  //Save the cycles in an array to finally print it out

		printf("Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
		fprintf(fnew,"Number of instruction cycles executed before the crash: %llu\n",cp->instr_cycles);
	
		//printf("Number of successful program runs before the crash: %llu\n",cp->program_runs);
		printf("Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);
	    fprintf(fnew,"Time of crash number %d is %ld seconds since January 1, 1970\n",cp->crash, crash_time);

		printf("At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash,(crash_time-start_seconds));
		fprintf(fnew,"At crash %d,time since the beginning of program execution is: %ld (in seconds)\n", cp->crash,(crash_time-start_seconds));
		cp->crash_time_array[cp->crash] = (crash_time-start_seconds);

		//**********************Reset conditions after mem crash*******************************
		reset_after_crash(r2,  program_memory, cp, start_seconds, fnew, fp);


return 0;
}

int report_error(struct crash_parameters *cp, struct registers *r2, struct instructions *i1, int program_memory[], FILE *fnew)
{
		
		//cp->store_same_reg_modification[cp->same_reg++] = r2->GP_Reg[cp-> random_reg[j]];

		//printf("PC value now(in hex)=%x, instruction opcode being executed (in hex)=%x\n", (r2-> PC), program_memory[ (r2-> PC)]);
		//fprintf(fnew,"PC value now (in hex)=%x, instruction opcode being executed (in hex)=%x\n", (r2-> PC), program_memory[ (r2-> PC)]);

		printf("PC value (in hex)=%x, instruction opcode being executed (in hex)=%x\n", (r2-> PC), program_memory[ (r2-> PC)]);
		fprintf(fnew,"PC value (in hex)=%x, instruction opcode being executed (in hex)=%x\n", (r2-> PC), program_memory[ (r2-> PC)]);

		
		if(cp->incorrect_data_flag ==1) //If error is due to incorrect data, save the cycles in his array. This saves only the cycle at which the first error occured
		{
			cp->first_error_at_instr[cp->first_error] = cp->instr_cycles_for_error; //Save the cycles in an array to finally print it out
			cp->first_error= (cp->first_error)+1;
			cp->incorrect_data_flag=0; //reset
		}
		else
		{
			cp->rest_of_the_errors[cp->other_errors] = cp->instr_cycles_for_error; //Save the cycles in an array to finally print it out
			cp->other_errors= (cp->other_errors)+1;
		}
		
		printf("Number of instruction cycles executed before the error: %llu\n\n",cp->instr_cycles_for_error);
		fprintf(fnew, "Number of instruction cycles executed before the error: %llu\n\n",cp->instr_cycles_for_error);

		cp->errors_so_far=cp->first_error+cp->other_errors;
		printf("******Errors so far...****** %d\n",cp->errors_so_far);
		fprintf(fnew,"******Errors so far...****** %d\n",cp->errors_so_far);
		//cp->first_error = 1;


return 0;
}


int reset_after_crash(struct registers *r2,  int program_memory[], struct crash_parameters *cp, time_t start_seconds, FILE *fnew, FILE *fp)

{

	int i=0, ii=0;

		cp->instr_cycles=0; //Reset instruction cycles after every crash

		//Reset program counter to beginning of the program
        reset_PC_to_beginninng(r2);
		printf("***PC reset after crash***\n");
		fprintf(fnew,"***PC reset after crash***\n");

		printf("PC is reset to its initial values (in hex): PCL=%x, PCLATH=%x, PC=%x\n",r2->PCL, r2->PCLATH, r2->PC);
		fprintf(fnew,"PC is reset to its initial values (in hex): PCL=%x, PCLATH=%x, PC=%x\n",r2->PCL, r2->PCLATH, r2->PC);

		//clear all registers
		for(ii=0;ii<REG_MAX;++ii)
            r2->GP_Reg[i]=0; //clear all registers in register file map

		printf("***All general purpose registers reset after crash***\n");
		fprintf(fnew,"***All general purpose registers reset after crash***\n");

		//Reset program memory
		for(ii=0;ii< PROGRAM_MEM_SIZE; ++ii)
			program_memory[ii]=0;
		printf("***Program memory reset after crash***\n");
		fprintf(fnew,"***Program memory reset after crash***\n");

		//Load instructions back to program memory after crash
	      	read_instr_from_file(fp,program_memory,r2,fnew);

return 0;

}


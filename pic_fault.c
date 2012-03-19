#include <stdio.h>
#include <string.h>
#include <time.h>

#include "decode_fault.h"
#include "execute.h"
//#include "bit_flips.h"

FILE *fout;


int main()

{
       
        int instruction=0, opcode = 0;

        int reg_index=0,  reg_file_addr = 0, d=0;
        int n=0, i=0;
        int     decode_bits=0;
        int bit=0, immediate_value=0;
        int program_memory[PROGRAM_MEM_SIZE] ={0};
//      int program[PROGRAM_MEM_SIZE] ={0};
        char instr_mnemonic[MNEMONIC_SIZE];
        //int PCL = 0, PC = 0, PCLATH=0;
        int loop=0;
        char line[FILE_CHARS];  
        int instr_from_file=0;
        int starting_PC_value = 0;
        int random_reg=0, random_mem=0;
        int repeat_program_execution=0;
        int initial_PCL=0, initial_PCLATH=0;
        unsigned long long int clock_cycles =0, total_pgm_runs=0,mean_pgm=0, program_runs=0, mean_seconds=0, total_seconds=0;
		int crash=0;
		int crash_at_pgm[MAX_CRASHES]={0}; // Store the number of clock cycles at which each time a crash occurs
		int crash_time_array[MAX_CRASHES]={0};	// Store the number of seconds elapsed, since beginning, at which each time a crash occurs
        struct registers pic_registers;

        time_t start_seconds= time (NULL);
  		printf ("At the start of execution: %ld seconds since January 1, 1970 \n", start_seconds);

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
        //      pic_registers.PC=0;
        //STACK
                pic_registers.stack_pointer = 0;
                pic_registers.stack[1] = 0x62;
                pic_registers.stack[0] = 0x88;

        // Status register = GP_Reg[3]
        //Assigning some value to the carry in status reg
//              pic_registers.GP_Reg[3]= pic_registers.GP_Reg[3] & 0xFE; //carry = 0
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
        //      pic_registers.GP_Reg[0x89]= 0x00;

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

        printf("Status register contents:(hex) at the beginning of all operations: ");
        printf("%x", pic_registers.GP_Reg[3]);
        printf("\n");

        loop= starting_PC_value;

//Repeat the same program forever till Ctrl-C is entered
	while (loop < n)
        {
               
                PRINT("****************************************************************\n");
                PRINT("INSTRUCTION NUMBER %d\n", loop - starting_PC_value + 1);
                PRINT("Entering execution loop with repeat = %d\n", repeat_program_execution);

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

       
                //Instruction execute
                PRINT("Status register contents:(hex) at the end of decode: ");
                PRINT("%x", pic_registers.GP_Reg[3]);
                PRINT("\n");

                instruction_execute(&pic_registers,&post_decode);
                clock_cycles= clock_cycles + CLOCKS_PER_INSTR; //Fetch + Execute = 8 instruction cycles

                PRINT("****************************************************************\n");    
                loop++;

                //Repeat program
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
                        //if(repeat_program_execution == 10) //Repeat program execution 10 times
                        //break; //break from the while loop

                        if(repeat_program_execution == NUM_OF_PGM_RUNS)
                        {
                                //Flip bits every 10 times the program repeats..
                           bit_flips(&pic_registers, program_memory, &random_reg, &random_mem, &clock_cycles,&crash, crash_at_pgm, &program_runs,start_seconds,crash_time_array);
								
                                repeat_program_execution=0; //Reset
                              //  printf("Inside main: Crash number:%d\n", crash);
								PRINT("Inside main, ending bitflips function\n");
                        }

                      //  printf("\n-----------Program execution number %d completed-------------\n",repeat_program_execution);
               
                }

        
		if (crash== MAX_CRASHES)
			break;

        }

                printf("\nTotal number of instructions in the program = %d\n",n);      
                printf("Each instruction takes 1 instruction cycles, i.e., 1clock cycle\n");

                printf("Status register contents:(hex) at the end of all operations: ");
                printf("%x", pic_registers.GP_Reg[3]);
                printf("\n");

        

for(i=1;i<= MAX_CRASHES;i++)
	{
	total_pgm_runs=total_pgm_runs+ crash_at_pgm[i];
	total_seconds=total_seconds+ crash_time_array[i];
	}

mean_pgm= total_pgm_runs/MAX_CRASHES;
printf("Mean time to failure in terms of the number of program runs: %llu \n", mean_pgm);

mean_seconds= total_seconds/MAX_CRASHES;
printf("Mean time to failure in terms of seconds: %llu \n", mean_seconds);

return 0;

}



#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "decode_fault.h"
#include "execute.h"
//#include "bit_flips.h"

FILE *fout;


int main()

{
       
        int instruction=0, opcode = 0;

        int reg_index=0,  reg_file_addr = 0, d=0;
        int n=0, i=0, c=0;
        int decode_bits=0;
        int bit=0, immediate_value=0;
        int program_memory[PROGRAM_MEM_SIZE] ={0};
//      int program[PROGRAM_MEM_SIZE] ={0};
        char instr_mnemonic[MNEMONIC_SIZE];
        //int PCL = 0, PC = 0, PCLATH=0;
        int loop=0;
        char line[FILE_CHARS];  
        int instr_from_file=0;
        int starting_PC_value = 0;
       
        int repeat_program_execution=0;
        int initial_PCL=0, initial_PCLATH=0;
        unsigned long long int total_instr_cycles=0,mean_instr_cycles=0,  mean_seconds=0, total_seconds=0;
		float percentage_crash=0.0;
			
        struct registers pic_registers;

        time_t start_seconds= time (NULL);
  		printf ("At the start of execution: %ld seconds since January 1, 1970 \n", start_seconds);

		 struct instructions pre_decode, post_decode;


		struct crash_parameters crash_param;
		//Initialisation
		crash_param.random_reg=0;
		crash_param.random_mem=0;
		crash_param.instr_cycles=0;
		crash_param.crash=0;
		crash_param.program_runs=0;
		crash_param.crash_dueto_illegal_mem=0;
		crash_param.crash_dueto_PC=0;
		crash_param.crash_dueto_illegal_opcode=0;

		//clear all locations
			for(i=0;i<MAX_CRASHES;++i)
                crash_param.crash_at_instr[i]=0;
			for(i=0;i<MAX_CRASHES;++i)
                crash_param.crash_time_array[i]=0;



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
                 // get a line, up to 80 chars from fp.  done if NULL 
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

                     
                pre_decode.instruction= instruction; //Instruction fetched 
                pre_decode.opcode= opcode; 
                pre_decode.reg_file_addr= reg_file_addr;
                pre_decode.d= d;
                pre_decode.reg_index= reg_index;
                pre_decode.bit = 0;
                pre_decode.immediate_value = 0;

       
                switch (decode_bits)
                {
                        case 0:
                                decode_byte_instr(&pre_decode,&crash_param);
                                break;

                        case 1:
                                decode_bit_instr(&pre_decode,&crash_param);  
                                break;

                        case 2:
                                call_goto_instr(&pre_decode,&crash_param);  
                                break;

                        case 3:
                                literal_control_instr(&pre_decode,&crash_param);    
                                break;

                        default:
                                printf("Invalid decode_bits inside main\n");
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
                crash_param.instr_cycles= crash_param.instr_cycles++; 

                PRINT("****************************************************************\n");    
                loop++;



				//Bit flip function called every cycle
				 bit_flips(&pic_registers, program_memory, &crash_param, start_seconds);

				
				//Check for program crash
				check_pgm_crash(&crash_param, start_seconds);

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

                    pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. PC= 13 bits
                    //----------------------------------------------------------------------------------------------------------------------------

                    repeat_program_execution++; //Keep track of the number of times the program is re-executed
                              
                }

  
		if (crash_param.crash == MAX_CRASHES)
			break;

        }

                printf("\nTotal number of instructions in the program = %d\n",n);      
                printf("Each instruction takes 1 instruction cycles, i.e., 1 clock cycle\n");

                printf("Status register contents:(hex) at the end of all operations: ");
                printf("%x", pic_registers.GP_Reg[3]);
                printf("\n");

        

for(i=1;i <= MAX_CRASHES;i++)
	{
	total_instr_cycles=total_instr_cycles+ crash_param.crash_at_instr[i];
	total_seconds=total_seconds+ crash_param.crash_time_array[i];
	}

printf("Number of instruction cycles executed before each crash:\n");
for(c=1;c<= (crash_param.crash); c++)
   //Print the entire array containing the instruction cycles at which the crash occured each time
   printf("%d\n",crash_param.crash_at_instr[c]);
  
   
printf("Seconds elapsed since the beginning of the program, before each crash: \n");    
for(c=1;c<= (crash_param.crash); c++)
   //Print the entire array containing the seconds at which the crash occured each time
   printf("%d\n",crash_param.crash_time_array[c]);             
      

percentage_crash=(crash_param.crash/total_instr_cycles)*100;

printf("Total number of crashes:%d\n",MAX_CRASHES);
//printf("Percentage of crashing, out of total number of instruction cycles executed=%f percent \n",percentage_crash);
printf("Percentage of crashes due to Program counter getting manipulated: %.2f percent\n",(((crash_param.crash_dueto_PC)/MAX_CRASHES)*100.0));
printf("Percentage of crashes due to illegal memory access: %.2f percent\n",(((crash_param.crash_dueto_illegal_mem)/MAX_CRASHES)*100.0));
printf("Percentage of crashes due to illegal memory access: %.2f percent\n",(((crash_param.crash_dueto_illegal_opcode)/MAX_CRASHES)*100.0));


mean_instr_cycles= total_instr_cycles/MAX_CRASHES;
printf("Mean time to failure in terms of the number of instruction cycles: %llu\n", mean_instr_cycles);

mean_seconds= total_seconds/MAX_CRASHES;
printf("Mean time to failure in terms of seconds: %llu\n", mean_seconds);



return 0;

}



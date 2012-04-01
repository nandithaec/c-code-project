#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <stdlib.h>

#include "decode_fault.h"
#include "execute.h"
//#include "bit_flips.h"



FILE *fnew;

int main()

{
       
        int instruction=0, opcode = 0;

        int reg_index=0,  reg_file_addr = 0, d=0;
        int  i=0, c=0;
        int decode_bits=0;
        int bit=0, immediate_value=0;
        int program_memory[PROGRAM_MEM_SIZE] ={0};//Fill the unused array elements wih NOP
//      int program[PROGRAM_MEM_SIZE] ={0};
        char instr_mnemonic[MNEMONIC_SIZE];
        //int PCL = 0, PC = 0, PCLATH=0;
        int loop=0;
     //   char line[FILE_CHARS];  
       // int instr_from_file=0;
     //   int starting_PC_value = 0;
       
        int repeat_program_execution=0;
       // int initial_PCL=0, initial_PCLATH=0;
        unsigned long long int total_instr_cycles=0,mean_instr_cycles=0,  mean_seconds=0, total_seconds=0, successful_cycles=0,sum_of_all_errors=0;
		float percentage_crash=0.0,percentage_error=0.0, percentage_success=0.0;
		


fnew = fopen( "output_pic_results_0p01.txt", "w" );
if( fnew != NULL )
   fprintf( fnew, "Hello\n" );


	
        struct registers pic_registers;

        time_t start_seconds= time (NULL);
  		printf ("At the start of execution: %ld seconds since January 1, 1970 \n", start_seconds);
		fprintf (fnew,"At the start of execution: %ld seconds since January 1, 1970 \n", start_seconds);
		//printf("Seconds in scientific notation:%.4e\n",(double)(start_seconds));
		 struct instructions pre_decode, post_decode;


		struct crash_parameters crash_param;
		//Initialisation of crash parameters
		 initialise_crash_param(&crash_param);
		

//-------------------------------Initialising registers------------------------------------
				initialise_regs(&pic_registers);

//Read in the program counter initial value from user

                printf("Enter starting PCL value (in hex): \n");

                scanf("%x", &pic_registers.GP_Reg[2]);
                pic_registers.initial_PCL=pic_registers.GP_Reg[2];

                printf("Enter starting PCLATH value (in hex): \n");
                scanf("%x", &pic_registers.GP_Reg[0x0A]);
                pic_registers.initial_PCLATH=pic_registers.GP_Reg[0x0A];

                pic_registers.GP_Reg[0x82]= pic_registers.GP_Reg[2]; //PCL Bank 1 and Bank 0
                pic_registers.PCL= pic_registers.GP_Reg[2];

                pic_registers.GP_Reg[0x8A]= pic_registers.GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
                pic_registers.PCLATH= pic_registers.GP_Reg[0x0A];

                pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits

        PRINT("New values as read from the user(hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",pic_registers.GP_Reg[2], pic_registers.PCLATH, pic_registers.PC);


//*******************Read instructions********************
        FILE *fp;               
		read_instr_from_file(fp,program_memory,&pic_registers,fnew);
//-------------------------------------------------------------------------------------------

        printf("Status register contents:(hex) at the beginning of all operations: ");
		fprintf(fnew,"Status register contents:(hex) at the beginning of all operations: ");
        printf("%x", pic_registers.GP_Reg[3]);
		fprintf(fnew,"%x", pic_registers.GP_Reg[3]);
        printf("\n");
		fprintf(fnew,"\n");

		printf("Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE)); //%g gives in terms of e.g.,1e-2
		fprintf(fnew,"Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE));
		printf("Executing...\n");
		fprintf(fnew,"Executing...\n");

        loop= pic_registers.starting_PC_value;

//Repeat the same program till a certain number of crashes occur
	while (loop < pic_registers.n)
        {
               
                PRINT("****************************************************************\n");
                PRINT("INSTRUCTION NUMBER %d\n", loop - (pic_registers.starting_PC_value) + 1);
                PRINT("Entering execution loop with repeat = %d\n", repeat_program_execution);
		
                
				//Check for program crash
				//check_pgm_crash(&crash_param, start_seconds,&pic_registers);

				 //Instruction fetch    
                instruction= instruction_fetch(&pic_registers, program_memory,&crash_param);
       			

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
                                decode_byte_instr(&pre_decode,&crash_param,&pic_registers, program_memory,fnew);
                                break;

                        case 1:
                                decode_bit_instr(&pre_decode,&crash_param,fnew);  
                                break;

                        case 2:
                                call_goto_instr(&pre_decode,&crash_param,fnew);  
                                break;

                        case 3:
                                literal_control_instr(&pre_decode,&crash_param,fnew);    
                                break;

                        default:
                                printf("Invalid decode_bits inside main\n");
								 fprintf(fnew,"Invalid decode_bits inside main\n");
                                break;
                }
       
				PRINT("decode done\n");
                post_decode= pre_decode;
                      				
				  //Bit flip function called every cycle
	    		PRINT("bit flip call\n");
				bit_flips(&pic_registers, program_memory, &crash_param, start_seconds, &post_decode,fnew,fp);
				

				if( (decode_bits ==0 || decode_bits ==1) && (pre_decode.instr_mnemonic_enum != NOP) && (pre_decode.instr_mnemonic_enum != CLRW))
				//Check reg file access error only for byte and bit oriented instructions and make sure it is not a NOP or CLRW
				check_pgm_error(&crash_param, &pic_registers, &pre_decode, program_memory,fnew);
		
                PRINT("Instruction format (hex) = %x \n",post_decode.instruction);
                PRINT("Opcode (hex) = %x \n",post_decode.opcode);
                PRINT("Register file address (hex) = %x, Register number= %d \n", post_decode.reg_file_addr, post_decode.reg_index);
                PRINT("Destination bit = %d, bit=%d, Immediate value (hex)= %x \n", post_decode.d, post_decode.bit, post_decode.immediate_value);
                PRINT("Instruction mnemonic enum = %d\n",post_decode.instr_mnemonic_enum);

	       		
               
                PRINT("Status register contents:(hex) at the end of decode: ");
                PRINT("%x", pic_registers.GP_Reg[3]);
                PRINT("\n");
             

				 //Instruction execute
				PRINT("execute\n");
				instruction_execute(&pic_registers,&post_decode,program_memory,&crash_param, fnew);
				

				crash_param.instr_cycles= crash_param.instr_cycles++; //Increment instruction cycles every cycle
				crash_param.instr_cycles_for_error= crash_param.instr_cycles_for_error++; //Increment instruction cycles every cycle
                PRINT("****************************************************************\n");    
                

				//Increment program counter
				PC_increment(&pic_registers);

				loop++;
				//exit(0);

                //Repeat program
                if (loop == pic_registers.n) //If end of program is reached
                {
                    loop= pic_registers.starting_PC_value; //Reset loop to beginning of program and begin execution again

                    reset_PC_to_beginninng(&pic_registers);

                    repeat_program_execution++; //Keep track of the number of times the program is re-executed
                              
                }

		 		//Repeat till a max number of crashes occue
				if (crash_param.crash == MAX_CRASHES)
					break;

        }

                printf("\nTotal number of instructions in the program = %d\n",pic_registers.n);   
				fprintf(fnew,"\nTotal number of instructions in the program = %d\n",pic_registers.n);         
               
				 printf("Each instruction takes 1 instruction cycles, i.e., 1 clock cycle\n");
				fprintf(fnew,"Each instruction takes 1 instruction cycles, i.e., 1 clock cycle\n");
              
			    printf("Status register contents:(hex) at the end of all operations: ");
			    fprintf(fnew,"Status register contents:(hex) at the end of all operations: ");
              
				  printf("%x", pic_registers.GP_Reg[3]);
				 fprintf(fnew,"%x", pic_registers.GP_Reg[3]);

                printf("\n");
				fprintf(fnew,"\n");
        

for(i=1;i <= MAX_CRASHES;i++)
	{
	total_instr_cycles=total_instr_cycles+ crash_param.crash_at_instr[i];
	total_seconds=total_seconds+ crash_param.crash_time_array[i];
	}

printf("Number of instruction cycles executed before each crash:\n");

fprintf(fnew,"Number of instruction cycles executed before each crash:\n");

for(c=1; c<= (crash_param.crash); c++)
   {//Print the entire array containing the instruction cycles at which the crash occured each time
   printf("%llu\n",crash_param.crash_at_instr[c]);
    fprintf(fnew,"%llu\n",crash_param.crash_at_instr[c]);
	}
   
printf("Seconds elapsed before each crash: \n");
fprintf(fnew,"Seconds elapsed before each crash: \n");        
for(c=1; c<= (crash_param.crash); c++)
   {//Print the entire array containing the seconds at which the crash occured each time
   printf("%ld\n",crash_param.crash_time_array[c]);             
  fprintf(fnew,"%ld\n",crash_param.crash_time_array[c]);             
	}

printf("\nCalculating the number of errors: \n");
fprintf(fnew,"\nCalculating the number of errors: \n");   

printf("Opcodes at which the errors occurred get accessed every program run and hence repeated once in every %d opcodes\n", NUM_OF_INSTR);    
fprintf(fnew,"Opcodes at which the errors occurred get accessed every program run and hence repeated once in every %d opcodes\n", NUM_OF_INSTR);

printf("\nInstruction cycle at which the errors first occured:\n");
fprintf(fnew,"\nInstruction cycle at which the errors first occured:\n");

for(c=0; c< (crash_param.error); c++)
{
//Calculating the errors.. 
	
	printf("%llu\n",crash_param.error_at_instr[c]);
	fprintf(fnew,"%llu\n",crash_param.error_at_instr[c]);
	crash_param.errors_repeated[c]=  (total_instr_cycles - crash_param.error_at_instr[c]) /(NUM_OF_INSTR);

}

printf("\nTotal number of times each error occured:\n");
fprintf(fnew,"\nTotal number of times each error occured:\n");

for(c=0; c< (crash_param.error); c++)
{

	printf("%llu\n",crash_param.errors_repeated[c]);
	fprintf(fnew,"%llu\n",crash_param.errors_repeated[c]); 
	sum_of_all_errors= 	sum_of_all_errors + crash_param.errors_repeated[c];
}

   
percentage_crash= ((double)crash_param.crash/total_instr_cycles)*100.0;
percentage_error= ((double)crash_param.error/total_instr_cycles)*100.0;
successful_cycles= (total_instr_cycles-sum_of_all_errors- MAX_CRASHES);


printf("\nTotal number of instruction cycles executed:%llu\n",(total_instr_cycles)); //%e
fprintf(fnew,"\nTotal number of instruction cycles executed:%llu\n",(total_instr_cycles)); 


printf("Total number of crashes:%d\n",MAX_CRASHES);
fprintf(fnew,"Total number of crashes:%d\n",MAX_CRASHES);

printf("Total number of errors: %d\n",crash_param.error);
fprintf(fnew,"Total number of errors: %d\n",crash_param.error);

printf("Summing up all errors which repeat every program run,Total number of errors: %llu\n",sum_of_all_errors);
fprintf(fnew,"Summing up all errors which repeat every program run,Total number of errors: %llu\n",sum_of_all_errors);

printf("Total number of successful instruction cycle executions without errors/crashes:%llu\n\n",(successful_cycles)); //%e
fprintf(fnew,"Total number of successful instruction cycle executions without errors/crashes:%llu\n\n",(successful_cycles)); 

printf("Number of crashes due to Program counter getting manipulated: %d \n",crash_param.crash_dueto_PC);
fprintf(fnew,"Number of crashes due to Program counter getting manipulated: %d \n",crash_param.crash_dueto_PC);

printf("Number of crashes due to illegal memory access: %d \n",crash_param.crash_dueto_illegal_mem);
fprintf(fnew,"Number of crashes due to illegal memory access: %d \n",crash_param.crash_dueto_illegal_mem);

printf("Number of crashes due to illegal memory access: %d \n",crash_param.crash_dueto_illegal_opcode);
fprintf(fnew,"Number of crashes due to illegal opcode: %d \n",crash_param.crash_dueto_illegal_opcode);

printf("Total number of errors due to incorrect control flow: %d\n",crash_param.control_flow_change);
fprintf(fnew,"Total number of errors due to incorrect control flow: %d\n",crash_param.control_flow_change);

printf("Total number of errors due to incorrect data: %d\n",crash_param.incorrect_data);
fprintf(fnew,"Total number of errors due to incorrect data: %d\n",crash_param.incorrect_data);

printf("Percentage of successful executions:%f\n\n", ((double) successful_cycles/total_instr_cycles)*100.0); //%e
fprintf(fnew,"Percentage of successful executions:%f\n\n", ((double)successful_cycles/total_instr_cycles)*100.0); 

printf("Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE)); //%g gives in terms of e.g.,1e-2
fprintf(fnew,"Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE));

printf("Percentage of crashes due to Program counter getting manipulated:%f percent\n",(((double)crash_param.crash_dueto_PC/MAX_CRASHES)*100.0));
printf("Percentage of crashes due to illegal memory access:%f percent\n",(((double) crash_param.crash_dueto_illegal_mem/MAX_CRASHES)*100.0));
printf("Percentage of crashes due to illegal memory access:%f percent\n",(((double) crash_param.crash_dueto_illegal_opcode/MAX_CRASHES)*100.0));

printf("\n");
printf("Percentage of crashes out of total number of instruction cycles: %f percent\n",percentage_crash);
printf("Percentage of errors out of total number of instruction cycles: %f percent\n",percentage_error);


mean_instr_cycles= total_instr_cycles/MAX_CRASHES;
printf("Mean time to failure in terms of the number of instruction cycles: %llu\n", mean_instr_cycles);
fprintf(fnew,"Mean time to failure in terms of the number of instruction cycles: %llu\n", mean_instr_cycles);


mean_seconds= total_seconds/MAX_CRASHES;
printf("Mean time to failure in terms of seconds: %llu\n", mean_seconds);
fprintf(fnew,"Mean time to failure in terms of seconds: %llu\n", mean_seconds);


fclose(fnew);

return 0;


}




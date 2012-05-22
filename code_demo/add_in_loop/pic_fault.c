#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <stdlib.h>

#include "decode_fault.h"
#include "execute.h"
//#include "bit_flips.h"



FILE *fnew;
FILE *fPC;
FILE *finstr;

int main()

{
        int  i=0, c=0;
       // int decode_bits=0;
        int bit=0, immediate_value=0;
        int program_memory[PROGRAM_MEM_SIZE] ={0};//Fill the unused array elements wih NOP
//      int program[PROGRAM_MEM_SIZE] ={0};
        char instr_mnemonic[MNEMONIC_SIZE];
        //int PCL = 0, PC = 0, PCLATH=0;
        int loop=0, instr_num=0;
     //   char line[FILE_CHARS];  
       // int instr_from_file=0;
     //   int starting_PC_value = 0;
       
        int repeat_program_execution=0;
       // int initial_PCL=0, initial_PCLATH=0;
        unsigned long long int total_instr_cycles=0,mean_instr_cycles=0,  mean_seconds=0, total_seconds=0, successful_cycles=0,total_error_count=0;
		float percentage_crash=0.0,percentage_error=0.0, percentage_success=0.0;
		


fnew = fopen( "output_pic_instr_fetched.txt", "w" );
if( fnew != NULL )
   fprintf( fnew, "Hello.. adding numbers in a loop\n" );


//finstr = fopen( "matrix_assembly_instruction_only.txt", "r" );

	
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

/* Read in the program counter initial value from user
For addition program use this section here..
For matrix multiplication use this section after reading instruction from file.. Well, doesnt matter, since PC is not being assigned in the read_inst function */

               printf("Read starting PCL value (in hex) (in hex - E8):: \n");

           //     scanf("%x", &pic_registers.GP_Reg[2]);
				pic_registers.GP_Reg[2] = 0xCC; //0x9F;
                pic_registers.initial_PCL=pic_registers.GP_Reg[2];

                printf("Read starting PCLATH value (in hex) (in hex - 3): \n");
               // scanf("%x", &pic_registers.GP_Reg[0x0A]);
                 pic_registers.GP_Reg[0x0A] = 0x03; //0x02;
				 pic_registers.initial_PCLATH=pic_registers.GP_Reg[0x0A];

                pic_registers.GP_Reg[0x82]= pic_registers.GP_Reg[2]; //PCL Bank 1 and Bank 0
                pic_registers.PCL= pic_registers.GP_Reg[2];

                pic_registers.GP_Reg[0x8A]= pic_registers.GP_Reg[0x0A]; //PCLATH Bank 1 and Bank 0
                pic_registers.PCLATH= pic_registers.GP_Reg[0x0A];

                pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits

				pic_registers.starting_PC_value = pic_registers.PC;
        PRINT("New values as read from the user(hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",pic_registers.GP_Reg[2], pic_registers.PCLATH, pic_registers.PC); 


//*******************Read instructions********************
  //      FILE *fp;               
		//read_instr_from_file(fp,program_memory,&pic_registers,fnew); //simple add program

		read_PC_array_for_matrix_mult(fPC,program_memory,&pic_registers,fnew);
		read_instr_for_matrix_mult(finstr,program_memory,&pic_registers,fnew);
		//fclose(fnew);
	//	exit(0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int endloop=0, num_of_inst=0;

        printf("Status register contents:(hex) at the beginning of all operations: ");
		fprintf(fnew,"Status register contents:(hex) at the beginning of all operations: ");
        printf("%x", pic_registers.GP_Reg[3]);
		fprintf(fnew,"%x", pic_registers.GP_Reg[3]);
        printf("\n");
		fprintf(fnew,"\n");

	//	printf("Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE)); //%g gives in terms of e.g.,1e-2
	//	fprintf(fnew,"Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE));
		printf("Executing...\n");
		fprintf(fnew,"Executing...\n");

        loop= pic_registers.starting_PC_value;
		//printf("Enter num of instr to be executed...\n");
       // scanf("%d", &num_of_inst);
		//printf("\n");

		//endloop = (loop+ num_of_inst);
		pre_decode.decode_bits=0; //Initialising decode_bits. This is a MUST

//Repeat the same program till a certain number of crashes occur
	while (pic_registers.PC <  pic_registers.Last_valid_PC )
        {
               
                PRINT("****************************************************************\n");
                PRINT("INSTRUCTION NUMBER %d\n", ++instr_num);
                PRINT("Entering execution loop with repeat = %d\n", repeat_program_execution);
		
                
				
				 //Instruction fetch    
                instruction_fetch_matrix_mult(&pic_registers, program_memory,&crash_param, fnew); //pic_registers.instruction is the instruction that is fetched

                //Instruction decode
				instruction_decode_matrix_mult(&pic_registers, &pre_decode, program_memory, &crash_param, fnew, fPC, finstr, start_seconds);
				
                post_decode= pre_decode; //Copy the structure
                      				
				  //Bit flip function called every cycle
	    	//	PRINT("bit flip call\n");
				//bit_flips(&pic_registers, program_memory, &crash_param, start_seconds, &post_decode,fnew,fp);
							

				//Check illegal memory access crash only if the memory location where the bit is flipped is being accessed by the opcode
			/*	check_illegal_instr(&pic_registers, program_memory,&crash_param, start_seconds ,&pre_decode, fnew, fp);

				//Check reg file access error only for byte and bit oriented instructions and make sure it is not a NOP or CLRW				
				if( (pre_decode.decode_bits ==0 || pre_decode.decode_bits ==1) && (pre_decode.instr_mnemonic_enum != NOP) && (pre_decode.instr_mnemonic_enum != CLRW))
				{
					check_pgm_error(&crash_param, &pic_registers, &pre_decode, program_memory,fnew);
				}		
			*/
                PRINT("Instruction format (hex) = %x \n",post_decode.instruction);
                PRINT("Opcode (hex) = %x \n",post_decode.opcode);
                PRINT("Register file address (hex) = %x, Register number (in hex)= %x \n", post_decode.reg_file_addr, post_decode.reg_index);
                PRINT("Destination bit = %d, bit=%d, Immediate value (hex)= %x \n", post_decode.d, post_decode.bit, post_decode.immediate_value);
                PRINT("Instruction mnemonic enum = %d\n",post_decode.instr_mnemonic_enum);
		
                PRINT("Status register contents:(hex) at the end of decode: ");
                PRINT("%x", pic_registers.GP_Reg[3]);
                PRINT("\n");
             

				 //Instruction execute
				PRINT("execute\n");
				instruction_execute(&pic_registers,&post_decode,program_memory,&crash_param, fnew, fPC, finstr, start_seconds);
				

				//crash_param.instr_cycles= crash_param.instr_cycles++; //Increment instruction cycles every cycle
			//		crash_param.instr_cycles_for_error= crash_param.instr_cycles_for_error + 1; //Increment instruction cycles every cycle
                PRINT("****************************************************************\n");    
                

				//Increment program counter
				PC_increment(&pic_registers);

				//loop++;
				

                //testing matrix mult
//				if (loop == pic_registers.max_instr) //If end of program is reached
	//				break;

				//if (loop == (loop+2)) //If end of program is reached
				//	break;

				//Repeat program
               /* if (loop == pic_registers.max_instr) //If end of program is reached
                {
                    loop= pic_registers.starting_PC_value; //Reset loop to beginning of program and begin execution again

                    reset_PC_to_beginninng(&pic_registers);

                    repeat_program_execution++; //Keep track of the number of times the program is re-executed
                              
                }

		 		//Repeat till a max number of crashes occue
				if (crash_param.crash == MAX_CRASHES)
					break; */

        } //ending while (loop==...)

///COMMENTING OUT STARTING HERE
/*

                printf("\nTotal number of instructions in the program = %d\n",pic_registers.max_instr);   
				fprintf(fnew,"\nTotal number of instructions in the program = %d\n",pic_registers.max_instr);         
               
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

printf("\nNumber of instruction cycles executed before each crash:\n");
fprintf(fnew,"\nNumber of instruction cycles executed before each crash:\n");

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

printf("\nInstruction cycle at which the incorrect data errors first occured:\n");
fprintf(fnew,"\nInstruction cycle at which the incorrect data errors first occured:\n");

for(c=0; c< (crash_param.first_error); c++)
{
//Calculating the errors.. 
	
	printf("%llu\n",crash_param.first_error_at_instr[c]);
	fprintf(fnew,"%llu\n",crash_param.first_error_at_instr[c]);
	crash_param.errors_repeated[c]=  (total_instr_cycles - crash_param.first_error_at_instr[c]) /(NUM_OF_INSTR);

}

printf("\nTotal number of times the incorrect data error occured:\n");
fprintf(fnew,"\nTotal number of times the incorrect data error occured:\n");

for(c=0; c< (crash_param.first_error); c++)
{
	printf("%llu\n",crash_param.errors_repeated[c]);
	fprintf(fnew,"%llu\n",crash_param.errors_repeated[c]); 
	total_error_count= 	total_error_count + crash_param.errors_repeated[c];
}

//printf("\nInstruction cycle at which the other errors (like illegal destination/opcode) occured:\n");
//fprintf(fnew,"\nInstruction cycle at which the other errors (like illegal destination/opcode) occured:\n");

//for(c=0; c< (crash_param.other_errors); c++)
//{
//Printing the errors.. 
	//printf("%llu\n",crash_param.rest_of_the_errors[c]);
//	fprintf(fnew,"%llu\n",crash_param.rest_of_the_errors[c]);

//	}

total_error_count= total_error_count+ crash_param.other_errors; //This adds the errors caused other than the incorrect data a reg file location
   
percentage_crash= ((double)crash_param.crash/total_instr_cycles)*100.0;
percentage_error= ((double)crash_param.first_error/total_instr_cycles)*100.0;
successful_cycles= (total_instr_cycles-total_error_count- MAX_CRASHES);


printf("\nTotal number of instruction cycles executed:%llu\n",(total_instr_cycles)); //%e
fprintf(fnew,"\nTotal number of instruction cycles executed:%llu\n",(total_instr_cycles)); 

//printf("\nTotal number of instruction cycles executed..(using instr_cycles_for_error) :%llu\n",(crash_param.instr_cycles_for_error)); //%e
//fprintf(fnew,"\nTotal number of instruction cycles executed.. (using instr_cycles_for_error) :%llu\n",(crash_param.instr_cycles_for_error)); 


printf("Total number of crashes:%d\n",MAX_CRASHES);
fprintf(fnew,"Total number of crashes:%d\n",MAX_CRASHES);

printf("Total number of errors: %d\n",crash_param.errors_so_far);
fprintf(fnew,"Total number of errors: %d\n",crash_param.errors_so_far);

printf("Summing up all errors which repeat every program run,Total number of errors: %llu\n",total_error_count);
fprintf(fnew,"Summing up all errors which repeat every program run,Total number of errors: %llu\n",total_error_count);

printf("Total number of successful instruction cycle executions without errors/crashes:%llu\n\n",(successful_cycles)); //%e
fprintf(fnew,"Total number of successful instruction cycle executions without errors/crashes:%llu\n\n",(successful_cycles)); 

printf("Number of crashes due to Program counter getting manipulated: %d \n",crash_param.crash_dueto_PC);
fprintf(fnew,"Number of crashes due to Program counter getting manipulated: %d \n",crash_param.crash_dueto_PC);

printf("Number of crashes due to illegal memory access: %d \n",crash_param.crash_dueto_illegal_mem);
fprintf(fnew,"Number of crashes due to illegal memory access: %d \n",crash_param.crash_dueto_illegal_mem);

printf("Number of crashes due to illegal opcode: %d \n",crash_param.crash_dueto_illegal_opcode);
fprintf(fnew,"Number of crashes due to illegal opcode: %d \n",crash_param.crash_dueto_illegal_opcode);

printf("Total number of errors due to incorrect control flow: %d\n",crash_param.control_flow_change);
fprintf(fnew,"Total number of errors due to incorrect control flow: %d\n",crash_param.control_flow_change);

printf("Total number of errors due to incorrect data being fetched from memory: %d\n",crash_param.incorrect_data);
fprintf(fnew,"Total number of errors due to incorrect data being fetched from memory: %d\n",crash_param.incorrect_data);

printf("Total number of errors due to incorrect data in the instruction: %d\n",crash_param.incorrect_data_in_instr);
fprintf(fnew,"Total number of errors due to incorrect data in the instruction: %d\n",crash_param.incorrect_data_in_instr); 

printf("Total number of errors due to opcode change: %d\n",crash_param.opcode_change);
fprintf(fnew,"Total number of errors due to opcode change: %d\n",crash_param.opcode_change);


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
printf("Mean time to failure in terms of seconds: %llu\n\n", mean_seconds);
fprintf(fnew,"Mean time to failure in terms of seconds: %llu\n\n", mean_seconds);

*/
fclose(fnew);

return 0;


}




#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <stdlib.h>

#include "decode_fault_ECC.h"
#include "execute_ECC.h"
#include "hamming_8bit_data.h"
//#include "bit_flips.h"



FILE *fnew;
FILE *fPC;
FILE *finstr;

int main()

{
        int  i=0, c=0;
        int bit=0, immediate_value=0;
        int program_memory[PROGRAM_MEM_SIZE] = {0};//Fill the unused array elements wih NOP
        int program_memory_encoded[PROGRAM_MEM_SIZE] = {0}; 
		char instr_mnemonic[MNEMONIC_SIZE];
        
        int loop=0;
		int read_PCL_from_user=0, read_PCLATH_from_user=0;
       
        int repeat_program_execution=0;

        unsigned long long int total_instr_cycles=0,mean_instr_cycles=0,  mean_seconds=0, total_seconds=0, successful_cycles=0,total_error_count=0;
		float percentage_crash=0.0,percentage_error=0.0, percentage_success=0.0;
		


fnew = fopen( "output_pic_matrix_mult_1p0_ECC_errorGPreg.txt", "w" );
if( fnew != NULL )
   fprintf( fnew, "Hello\n" );


finstr = fopen( "matrix_assembly_instruction_only.txt", "r" );

	
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

                printf("Enter starting PCL value (in decimal, not hex) (159 dec, 29F hex): \n");
				scanf("%d", &read_PCL_from_user); //9F or 159
                
				pic_registers.GP_Reg_encoded[2] = hamming_encoding(read_PCL_from_user);
                pic_registers.initial_PCL_encoded = pic_registers.GP_Reg_encoded[2];
 
                printf("Enter starting PCLATH value (in decimal, not hex) (2 dec/hex): \n");
                scanf("%d", &read_PCLATH_from_user); //02

                pic_registers.GP_Reg_encoded[0x0A]=  hamming_encoding(read_PCLATH_from_user);
				pic_registers.initial_PCLATH_encoded = pic_registers.GP_Reg_encoded[0x0A];

                pic_registers.GP_Reg_encoded[0x82]= pic_registers.GP_Reg_encoded[2]; //PCL Bank 1 and Bank 0
                pic_registers.PCL_encoded= pic_registers.GP_Reg_encoded[2];

                pic_registers.GP_Reg_encoded[0x8A]= pic_registers.GP_Reg_encoded[0x0A]; //PCLATH Bank 1 and Bank 0
                pic_registers.PCLATH_encoded= pic_registers.GP_Reg_encoded[0x0A];

				//PC needs to be calculated from PCL and PCLATH
				PRINT("In main, decoding PCL\n");
				pic_registers.PCL= error_detect_correct_decode( pic_registers.PCL_encoded, fnew, &crash_param); //decoded PCL
				pic_registers.GP_Reg[2]=pic_registers.PCL;
				pic_registers.GP_Reg[0x82]=pic_registers.PCL;

				PRINT("In main, decoding PCLATH\n");
				pic_registers.PCLATH= error_detect_correct_decode( pic_registers.PCLATH_encoded, fnew, &crash_param); //decoded PCLATH
                pic_registers.GP_Reg[0x0A]=pic_registers.PCLATH;
				pic_registers.GP_Reg[0x8A]=pic_registers.PCLATH;


				pic_registers.PC = (pic_registers.PCL | (pic_registers.PCLATH << 8)) & 0x1FFF; //Limit to 13 bits. Program counter is 13 bits
//				pic_registers.PC_encoded = hamming_encoding(pic_registers.temp_PC, pic_registers.hamming_code, pic_registers.parity);
				pic_registers.starting_PC_value = pic_registers.PC;
       	
	 printf("New values as read from the user(hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",pic_registers.GP_Reg[2], pic_registers.PCLATH, pic_registers.PC); 
	 fprintf(fnew,"New values as read from the user(hex): PCL=%x, PCLATH=%x, PC(testing) = %x \n",pic_registers.GP_Reg[2], pic_registers.PCLATH, pic_registers.PC); 


//*******************Read instructions********************
  //      FILE *fp;               
		//read_instr_from_file(fp,program_memory,&pic_registers,fnew); //simple add program

		read_PC_array_for_matrix_mult(fPC,program_memory,&pic_registers,fnew);
		read_instr_for_matrix_mult(finstr,program_memory,program_memory_encoded,&pic_registers,fnew);
		//fclose(fnew);
		//exit(0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int endloop=0, num_of_inst=0;

        printf("Status register contents:(hex) at the beginning of all operations: ");
		fprintf(fnew,"Status register contents:(hex) at the beginning of all operations: ");
        printf("%x", pic_registers.GP_Reg[3]); //No bits would have flipped here anyway..
		fprintf(fnew,"%x", pic_registers.GP_Reg[3]); 
        printf("\n");
		fprintf(fnew,"\n");

		printf("Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE)); //%g gives in terms of e.g.,1e-2
		fprintf(fnew,"Probability of a bit flip is set to: %g\n",((float)(RANDOM_GUESS_RANGE-1)/PROBABILITY_INVERSE));
		printf("Executing...\n");
		fprintf(fnew,"Executing...\n");

        loop= pic_registers.starting_PC_value;
	

		//endloop = (loop+ 2);
		pre_decode.decode_bits=0; //Initialising decode_bits. This is a MUST

//Repeat the same program till a certain number of crashes occur
	while (pic_registers.PC <  pic_registers.Last_valid_PC ) //Continue program execution till the last valid opcode is reached
	//while(loop < endloop)    
   {
              // 	loop++;
                PRINT("****************************************************************\n");
                //PRINT("INSTRUCTION NUMBER %d\n", loop - (pic_registers.starting_PC_value) + 1);
                PRINT("Entering execution loop with repeat = %d\n", repeat_program_execution);
		            
				
				 //Instruction fetch    
                instruction_fetch(&pic_registers, program_memory,program_memory_encoded, &crash_param); //pic_registers.instruction is the instruction that is fetched

				//Instruction decode
				instruction_decode_matrix_mult(&pic_registers, &pre_decode, program_memory,program_memory_encoded, &crash_param, fnew, fPC, finstr, start_seconds);
				
                post_decode= pre_decode; //Copy the structure
                      				
				//Bit flip function called every cycle
	    	
				bit_flips(&pic_registers, program_memory,program_memory_encoded, &crash_param, start_seconds, &post_decode,fnew,fPC, finstr);
							

				//Check illegal memory access crash only if the memory location where the bit is flipped is being accessed by the opcode
				check_illegal_instr(&pic_registers, program_memory,program_memory_encoded,&crash_param, start_seconds ,&pre_decode, fnew, fPC, finstr);

				//Check reg file access error only for byte and bit oriented instructions and make sure it is not a NOP or CLRW				
				if( (pre_decode.decode_bits ==0 || pre_decode.decode_bits ==1) && (pre_decode.instr_mnemonic_enum != NOP) && (pre_decode.instr_mnemonic_enum != CLRW))
				{
					check_pgm_error(&crash_param, &pic_registers, &pre_decode, program_memory,program_memory_encoded, fnew);
				}	
			
			
                PRINT("Instruction format (hex) = %x \n",post_decode.instruction);
                PRINT("Opcode (hex) = %x \n",post_decode.opcode);
                PRINT("Register file address (hex) = %x, Register number= %d \n", post_decode.reg_file_addr, post_decode.reg_index);
                PRINT("Destination bit = %d, bit=%d, Immediate value (hex)= %x \n", post_decode.d, post_decode.bit, post_decode.immediate_value);
                PRINT("Instruction mnemonic enum = %d\n",post_decode.instr_mnemonic_enum);
		
                PRINT("Status register contents:(hex) at the end of decode: ");
                PRINT("%x", pic_registers.GP_Reg[3]);
                PRINT("\n");          

				//execute
				PRINT("execute\n");
				instruction_execute(&pic_registers,&post_decode,program_memory,program_memory_encoded, &crash_param, fnew, fPC, finstr, start_seconds);
				

				crash_param.instr_cycles= crash_param.instr_cycles++; //Increment instruction cycles every cycle
				crash_param.instr_cycles_for_error= crash_param.instr_cycles_for_error + 1; //Increment instruction cycles every cycle
                PRINT("****************************************************************\n");    
                

				//Increment program counter
				PC_increment(&pic_registers, fnew, &crash_param);


				//Repeat program
                if (pic_registers.PC ==  pic_registers.Last_valid_PC) //If end of program is reached
                {
                    PRINT("Program execution number %d completed\n",repeat_program_execution);
                    reset_PC_to_beginninng(&pic_registers, fnew, &crash_param);

                    repeat_program_execution++; //Keep track of the number of times the program is re-executed
					
         
                }
			

		 		//Repeat till a max number of crashes occue
				if (crash_param.crash == MAX_CRASHES)
					break; 
				
		
	/*if (repeat_program_execution == 5)
		{
	printf("Program execution number %d completed\n",repeat_program_execution);
		break; //used only when running the program without bitflips, check pgm error/crash
		}		
     */

	} //ending while (loop==...) 




	

	//PRINT("Max instr cycles per program execution need not be same as number of instr, since some instructions can be skipped depending on checking status reg/GOTO etc): %llu\n",(crash_param.instr_cycles)/1);
///COMMENTING OUT STARTING HERE




                printf("\nTotal number of instructions in the program = %d\n",pic_registers.max_instr);   
				fprintf(fnew,"\nTotal number of instructions in the program = %d\n",pic_registers.max_instr);         
               
				 printf("Each instruction takes 1 instruction cycles, i.e., 1 clock cycle\n");
				fprintf(fnew,"Each instruction takes 1 instruction cycles, i.e., 1 clock cycle\n");
              
			    
                printf("\nTotal number of times the program got excecuted = %d\n",repeat_program_execution);   
			    fprintf(fnew,"\nTotal number of times the program got excecuted = %d\n",repeat_program_execution);           
               

				PRINT("Max instr cycles executed (will not be same as number of instructions since program can have loops): %llu\n",crash_param.instr_cycles);
				fprintf(fnew,"Max instr cycles executed (will not be same as number of instructions since program can have loops): %llu\n",crash_param.instr_cycles);

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

//printf("Opcodes at which the errors occurred get accessed every program run and hence repeated once in every %d opcodes\n", pic_registers.max_instr);    
//fprintf(fnew,"Opcodes at which the errors occurred get accessed every program run and hence repeated once in every %d opcodes\n", pic_registers.max_instr);

//printf("\nInstruction cycle at which the incorrect data errors first occured:\n");
//fprintf(fnew,"\nInstruction cycle at which the incorrect data errors first occured:\n");

printf("\nNumber of errors at unique PC values (not counting repetition):%d\n",crash_param.first_error);
fprintf(fnew,"\nNumber of errors at unique PC values (not counting repetition):%d\n",crash_param.first_error);

printf("Total number of errors due to incorrect data being fetched (counting repetition):%llu\n",crash_param.incorrect_data);
fprintf(fnew,"Total number of errors due to incorrect data being fetched (counting repetition):%llu\n",crash_param.incorrect_data);


//printf("\nInstruction cycle at which the other errors (like illegal destination/opcode) occured:\n");
//fprintf(fnew,"\nInstruction cycle at which the other errors (like illegal destination/opcode) occured:\n");

//for(c=0; c< (crash_param.other_errors); c++)
//{
//Printing the errors.. 
	//printf("%llu\n",crash_param.rest_of_the_errors[c]);
//	fprintf(fnew,"%llu\n",crash_param.rest_of_the_errors[c]);

//	}

//This adds all the errors: Errors due to incorrect data and all the other errors as well
total_error_count= total_error_count+ crash_param.incorrect_data+ crash_param.other_errors; 
   
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

printf("Total number of single errors corrected: %d\n",crash_param.single_error_corrected);
fprintf(fnew,"Total number of single errors corrected: %d\n",crash_param.single_error_corrected);

printf("Total number of double errors detected: %d\n",crash_param.double_error_detected);
fprintf(fnew,"Total number of double errors detected: %d\n",crash_param.double_error_detected);

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

printf("Total number of errors due to incorrect data being fetched from memory: %llu\n",crash_param.incorrect_data);
fprintf(fnew,"Total number of errors due to incorrect data being fetched from memory: %llu\n",crash_param.incorrect_data);

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


fclose(fnew);

return 0;


}



//Execute.h 

int instruction_execute(struct registers *r1, struct instructions *i1, int program_memory[], struct crash_parameters *cp, FILE *fnew, FILE *fPC, FILE *finstr, time_t start_seconds)
{
	PRINT("-------------------------------------------------------------------\n");
	PRINT("INSTRUCTION EXECUTION >>\n");
	int i=0;
	int twos_comp=0, temp_sub= 0, temp_add=0, temp_rotate=0, original_regfile=0;
	int swap_lower_nibble=0, swap_upper_nibble=0, temp_num=0;
	int bit_test;

	switch(i1-> instr_mnemonic_enum)
	{

	case 0: //SLEEP
	
		PRINT("SLEEP instruction\n");
		if (r1-> configuration_word[11]== 1) //Watch dog timer Enable bit in configuration word.
			{
				r1-> WDT = 0; //Reset Watch dog timer
				r1-> WDT_prescaler= 0; //Reset WDT prescaler
			
			
			PRINT("Watch dog timer cleared: %d \n", r1-> WDT);

			PRINT("Watch dog prescaler cleared: %d \n",r1-> WDT_prescaler );
			
			}
		else PRINT("WDT Enable bit not set, Watchdog timer cannot be cleared\n");	
		
		//GP_Reg[3]= status register
		r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xF7; //PD bar bit in status reg =0. Bit 3 = 0
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x10; //TO bar bit in status reg=1. Bit 4 = 1
		
		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

		
	break;

	
	case 1: //CLRWDT
	
		PRINT("CLRWDT instruction\n");
		if (r1-> configuration_word[11]== 1) //Watch dog timer Enable bit in configuration word
			{
				r1-> WDT = 0; //Reset Watch dog timer
				r1-> WDT_prescaler= 0; //Reset WDT prescaler
			
	
			PRINT("Watch dog timer cleared: %d \n", r1-> WDT);

			PRINT("Watch dog prescaler cleared: %d \n",r1-> WDT_prescaler );
			
			}

		else PRINT("WDT Enable bit not set, Watchdog timer cannot be cleared\n");	

		//GP_Reg[3]= status register
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x08; //PD bar bit in status reg=1. Bit 3= 1
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x10; //TO bar bit in status reg=1. Bit 4 = 1
		
		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

		
	break;
		
	case 2: //RETURN 
//This case will be entered even if there is a valid RETURN instruction. 

//But this if condition will be entered only if the processor is executing this instruction after the random mem location taht is flipped is same as the PC value
//That means, the instruction at the current PC value has been modified into a RETURN instruction through a bit flip
if (cp->flip_bit_flag_for_illegal_inst==1)
	{
		if (cp-> random_mem[(cp->mem_count) -1] == (r1-> PC)) 
		{
		
			printf("\nCRASH: Instruction has got changed to RETURN instruction: Control flow has changed..\n");
			fprintf(fnew,"\nCRASH: Instruction has got changed to RETURN instruction: Control flow has changed..\n");
				  		
			printf("PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);
			fprintf(fnew,"PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);

			cp->control_flow_change++;
			report_crash( r1,  program_memory, cp, start_seconds,i1, fnew, fPC, finstr);
		}
	}
		
		PRINT("Instruction cycle=%llu\n",cp->instr_cycles);

		PRINT("RETURN instruction\n");
		PRINT("Top of stack: %x , stack pointer=%d \n", r1-> stack[r1-> stack_pointer -1],r1-> stack_pointer);
		
		if (r1-> stack_pointer == 0)
			PRINT("Stack underflow, nothing to pop\n");
		else
			{
		//PC loaded from top of stack
			r1-> PC = r1-> stack[--r1-> stack_pointer]; //Decrement stack pointer and pop
			r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
			r1 -> GP_Reg[0x0A] = ((r1-> PC) & 0x1F00) >> 8; //PCLATH

			PRINT("First decrement Stack pointer: %d\n", r1-> stack_pointer);
			PRINT("PC popped from stack:(hex): %x, (dec): %d \n", r1-> PC,  r1-> PC);

			--r1-> PC;
			--r1-> GP_Reg[2]; //These will be incremented in main() towards the end anyway
		
			}
	
	break;

	
	case 3: //RETFIE
		
//This case will be entered even if there is a valid RETFIE instruction. 

//But this if condition will be entered only if the processor is executing this instruction after the random mem location taht is flipped is same as the PC value
//That means, the instruction at the current PC value has been modified into a RETFIE instruction through a bit flip
if (cp->flip_bit_flag_for_illegal_inst==1)
	{
	//Check for this condition only when the next bit flips
		cp->flip_bit_flag_for_illegal_inst==0;
		if (cp-> random_mem[(cp->mem_count) -1] == (r1-> PC)) //Control flow changes
		{
		
			printf("\nCRASH: Instruction has got changed to RETFIE instruction: Control flow has changed..\n");
			fprintf(fnew,"\nCRASH: Instruction has got changed to RETFIE instruction: Control flow has changed..\n");
				  		
			printf("PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);
			fprintf(fnew,"PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);
			cp->control_flow_change++;
			report_crash( r1,  program_memory, cp, start_seconds,i1, fnew, fPC, finstr);
		}
	}
//printf("\nERROR: RETFIE instruction. Control flow has changed..will lead to incorrect results\n");
		//fprintf(fnew,"\nERROR: RETFIE instruction. Control flow has changed..will lead to incorrect results\n");
  		//cp->other_errors= (cp->other_errors)+1;
		//cp->control_flow_change++;
		//cp->rest_of_the_errors[cp->other_errors] = cp->instr_cycles;
		//printf("Number of instruction cycles executed before the error: %llu\n",cp->instr_cycles);
		//fprintf(fnew,"Number of instruction cycles executed before the error: %llu\n",cp->instr_cycles);
			
		//printf("Bit flipped, Content of the program_memory[%x] is (in hex) %x\n\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
		//printf("Instruction cycle=%llu\n",cp->instr_cycles);
		//printf("RETFIE instruction\n");
		PRINT("Top of stack: %x , stack pointer=%d \n", r1-> stack[r1-> stack_pointer],r1-> stack_pointer);
		
		if (r1-> stack_pointer == 0)
			PRINT("Stack underflow, nothing to pop\n");
		else
			{
		//PC loaded from top of stack
			r1-> PC = r1-> stack[--r1->stack_pointer]; //Decrement stack pointer and pop
			r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
			r1 -> GP_Reg[0x0A] = ((r1-> PC) & 0x1F00) >> 8; //PCLATH
		
			PRINT("PC popped from stack: %x \n", r1-> PC);		

			--r1-> PC;
			--r1->GP_Reg[2]; //These will be incremented in main() towards the end anyway
			}

		//GP_Reg[11] = INTCON register at address 0BH
		r1-> GP_Reg[11] = r1-> GP_Reg[11] | 0x80; //Global interrupt enable (GIE) bit in INTCON reg is set- enables all unmasked interrupts
		
		PRINT("INTCON register contents:(hex): ");
		//for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[11]);
		PRINT("\n");
	
	break;

	case 4: //NOP
	
		PRINT("NOP instruction\n");

	break;


	case 5: //MOVWF
	
		PRINT("MOVWF instruction\n");
		
		r1-> GP_Reg[i1-> reg_index]= r1-> W;

		PRINT("Contents of destination is reg_file [%x] (hex)= %x \n", i1-> reg_index, r1-> GP_Reg[i1-> reg_index]);

	break;

	case 6: //CLRW
		
		PRINT("Contents of W before execution (hex)= %x \n", r1-> W);
		PRINT("CLRW instruction\n");
		
		r1-> W =0;
		PRINT("Contents of W cleared after execution (hex)= %x \n", r1-> W);
		
		//GP_Reg[3]= status register
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register
		
		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;
		
	case 7: //CLRF
		
		PRINT("Contents of reg file before execution (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		PRINT("CLRF instruction\n");
		
		r1-> GP_Reg[i1-> reg_index] = 0;
		PRINT("Contents of reg file[%x] cleared after execution (hex)= %x \n", i1-> reg_index, r1-> GP_Reg[i1-> reg_index]);

		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register
		
			PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;

	case 8: //SUBWF

	
	PRINT("SUBWF instruction\n");

	/*	W is the accumulator and d is the destination bit
	d=0 means- destination is W register i.e., accumulator 
	d=1 means- destination is the file register */
	PRINT("Before execution: Contents of Reg file (hex)= %x, W (hex)= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);		
		
	PRINT("After execution:\n");
	
	if (i1-> d==0)
		{

//	twos_comp = (~(r1-> W) + 1); //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits
	twos_comp = (~(r1-> W) & 0x000000FF)+1; 
	
	PRINT("~W (hex): %x\n", twos_comp);
		temp_sub = (r1-> GP_Reg[i1-> reg_index] + twos_comp);
		r1-> W = temp_sub & 0x000000FF;
	

		PRINT("Contents of destination is W= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				PRINT("Result is negative\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			}
		else 
			{
				PRINT("Result is positive\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			}
		}

	else if (i1-> d==1)
		{
		temp_num = ~(r1-> W);
		twos_comp = (~(r1-> W) & 0x000000FF)+1; //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits

	PRINT("temp_num (hex)=%x, ~W (hex): %x\n", temp_num, twos_comp);
		temp_sub = (r1-> GP_Reg[i1-> reg_index] + twos_comp);
		r1-> GP_Reg[i1-> reg_index] = temp_sub & 0x000000FF; //Keep only 8 bits

		PRINT("temp_sub (hex)= %x\n",temp_sub);

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				PRINT("Result is negative\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			}
		else 
			{
				PRINT("Result is positive\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			}
		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;

	case 9: //DECF

	
	PRINT("DECF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = (r1-> GP_Reg[3] | 0x04); //Z flag set in the status register

		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 10: //IORWF

	
	PRINT("IORWF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] | r1-> W) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = (r1-> GP_Reg[3] | 0x04); //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] | r1-> W) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = (r1-> GP_Reg[3] | 0x04); //Z flag set in the status register

		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 11: //ANDWF

	
	PRINT("ANDWF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] & r1-> W) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] & r1-> W) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 12: //XORWF

	
	PRINT("XORWF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution: Contents (hex) of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] ^ r1-> W) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] ^ r1-> W) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 13: //ADDWF

	
	PRINT("ADDWF instruction\n");

	//	W is the accumulator and d is the destination bit

	
	PRINT("Before execution: Contents (hex) of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);		
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

		temp_add = (r1-> GP_Reg[i1-> reg_index] + r1-> W);
		r1-> W = temp_add & 0x000000FF;

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_add & 0x100) >>8) == 0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			
		else 
			
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			
		}

	else if (i1-> d==1) //d=1 means- destination is the file register 
		{
		
		temp_add = (r1-> GP_Reg[i1-> reg_index] + r1-> W);
		r1-> GP_Reg[i1-> reg_index] = temp_add & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( (r1-> GP_Reg[i1-> reg_index]) ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_add & 0x100) >>8) == 0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			
		else 
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			
		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;

	
	case 14: //MOVF
		
		PRINT("MOVF instruction\n");
		
		
/*	W is the accumulator and d is the destination bit
	d=0 means- destination is W register i.e., accumulator 
	d=1 means- destination is the file register */
	
		if (i1-> d==0)
			r1-> W = r1-> GP_Reg[i1-> reg_index];
		else
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index];

		if (r1-> GP_Reg[i1-> reg_index] == 0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register
		else r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFB; //Z flag reset in the status register

	
		if(i1-> d==0)
			PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		else
			PRINT("Contents of destination is reg_file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

			
			PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;

	case 15: //COMF
		
		PRINT("Contents of reg file before execution (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		PRINT("COMF instruction\n");
		
		
		if (i1-> d==0) //d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = ~(r1-> GP_Reg[i1-> reg_index]) & 0x000000FF ; //limit to 8 bits

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = ~(r1-> GP_Reg[i1-> reg_index]) & 0x000000FF;  //limit to 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( (r1-> GP_Reg[i1-> reg_index]) ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}

		
			PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;

	case 16: //INCF

	
	PRINT("INCF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 17: //DECFSZ

	
	PRINT("DECFSZ instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			increment_PC(r1, fnew, cp); //PC will be incremented towards the end in main(). Now increment again to Skip next instruction

//comment out: r1-> PC = r1-> PC + 1; //PC already incremented in fetch step. Now increment again to Skip next instruction
	PRINT("PC will be incremented to(testing)= %d\n", (r1->PC)+1);
		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			increment_PC(r1, fnew, cp); //PC already incremented in fetch step. Now increment again to Skip next instruction

		}

//Status register not affected

		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
		
		PRINT("Program counter: PC= %d\n",r1->PC);
	
	break;


	case 18: //RRF

	
	PRINT("RRF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
	
	original_regfile= r1-> GP_Reg[i1-> reg_index];
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) & 0x000000FF) >> 1; //Keep only 8 bits
			if((r1-> GP_Reg[3] & 0x01) ==0) //Carry=0

				r1->W = temp_rotate;
			else //Carry = 1
				r1->W = temp_rotate | 0x80;

		PRINT("Contents of destination after right shift is W (hex)= %x \n", r1-> W);


		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) & 0x000000FF) >> 1; //Keep only 8 bits
			if((r1-> GP_Reg[3] & 0x01) ==0) //Carry=0
				r1-> GP_Reg[i1-> reg_index] = temp_rotate;
			else //Carry = 1
				r1-> GP_Reg[i1-> reg_index] = temp_rotate | 0x80;

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);


		}


	// LSB of reg file will go to carry through right shift
	if ((original_regfile & 0x00000001) ==1) 
			r1-> GP_Reg[3] | 0x01; //Set the carry to 1, that is the LSB of the reg file
	else
			r1-> GP_Reg[3] & 0xFE; //Set carry to 0

		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
		
	
	break;

	case 19: //RLF

	
	PRINT("RLF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
	
	original_regfile= r1-> GP_Reg[i1-> reg_index];
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			temp_rotate = ((r1-> GP_Reg[i1-> reg_index])  << 1) & 0x000000FF; //Keep only 8 bits
		PRINT("temp rotate=%x\n", temp_rotate);
		PRINT("Status reg: %x\n", r1->GP_Reg[3]);

			if((r1-> GP_Reg[3] & 0x01) == 0) //Carry=0
				{
		r1->W = temp_rotate;
		PRINT("Statusreg= 0, W=%x \n", r1->W);
		}

			else if((r1-> GP_Reg[3] & 0x01)==1) //Carry = 1
				r1->W = temp_rotate | 0x01;

		PRINT("Contents of destination after left shift is W (hex)= %x \n", r1-> W);


		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) <<1 ) & 0x000000FF; //Keep only 8 bits
			if((r1-> GP_Reg[3] & 0x01) ==0) //Carry=0
				r1-> GP_Reg[i1-> reg_index] = temp_rotate;
			else //Carry = 1
				r1-> GP_Reg[i1-> reg_index] = temp_rotate | 0x01;

		PRINT("Contents of destination after left shift is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);


		}


 // MSB of reg file will go to carry through right shift
	if(((original_regfile & 0x80) >>7) == 1)
		r1-> GP_Reg[3] | 0x01; //Set the carry to 1, that is the MSB of the reg file
	else
		r1-> GP_Reg[3] & 0xFE; //Set carry to 0

	PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
		
	
	break;


	case 20: //SWAPF

	
	PRINT("SWAPF instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
	
	original_regfile= r1-> GP_Reg[i1-> reg_index];
	swap_lower_nibble= (original_regfile & 0x0F) << 4;
	swap_upper_nibble= (original_regfile & 0xF0) >> 4;
	
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{
			r1->W = swap_upper_nibble | swap_lower_nibble;
			PRINT("Contents of destination after swap is W (hex)= %x \n", r1-> W);
		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
			r1-> GP_Reg[i1-> reg_index] = swap_upper_nibble | swap_lower_nibble;
			PRINT("Contents of destination after swap is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

		}


	
		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 21: //INCFSZ

	
	PRINT("INCFSZ instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	PRINT("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			increment_PC(r1, fnew, cp); //PC will be incremented towards the end in main(). Now increment again to Skip next instruction

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		PRINT("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			increment_PC(r1, fnew, cp); //PC already incremented in fetch step. Now increment again to Skip next instruction

		}

//Status register not affected

		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
		
		PRINT("Program counter will be incremented to: PC= %d\n", (r1->PC)+1);
	
	break;

	case 22: //BCF

	PRINT("BCF instruction\n");
	PRINT("Instruction cycle=%llu\n",cp->instr_cycles);
	  switch (i1-> bit)
		{
		
		
		case 0:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xFE;
		break;

		case 1:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xFD;
		break;

		case 2:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xFB;
		break;

		case 3:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xF7;
		break;

		case 4:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xEF;
		break;

		case 5:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xDF;

		break;

		case 6:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0xBF;
		break;

		case 7:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] & 0x7F;
		break;
		
		default: 
			printf ("INVALID instruction!\n"); 
		break;
		}	

	PRINT("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
	PRINT("Status register contents:(hex):");

	PRINT("%x", r1-> GP_Reg[3]);
	PRINT("\n");

	break;

	case 23: //BSF

	PRINT("BSF instruction\n");
	  switch (i1-> bit)
		{
		case 0:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x01;
		break;

		case 1:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x02;
		break;

		case 2:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x04;
		break;

		case 3:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x08;
		break;

		case 4:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x10;
		break;

		case 5:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x20;
		break;

		case 6:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x40;
		break;

		case 7:
			r1-> GP_Reg[i1-> reg_index] = r1-> GP_Reg[i1-> reg_index] | 0x80;
		break;
		
		default: 
			printf ("INVALID instruction!\n"); 
		break;
		}	

	PRINT("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
	PRINT("Status register contents:(hex):");

	PRINT("%x", r1-> GP_Reg[3]);
	PRINT("\n");
	break;

	case 24: //BTFSC

	PRINT("BTFSC instruction\n");
 	PRINT("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
	 switch (i1-> bit)
		{
		case 0:
			bit_test = r1-> GP_Reg[i1-> reg_index] & 0x01;
		break;

		case 1:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x02) >> 1;
		break;

		case 2:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x04) >> 2;
		break;

		case 3:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x08) >> 3;
		break;

		case 4:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x10) >> 4;
		break;

		case 5:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x20) >> 5;
		break;

		case 6:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x40) >> 6;
		break;

		case 7:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x80) >> 7;
		break;
		
		default: 
			printf ("INVALID instruction!\n"); 
		break;
		}	

		PRINT("Bit test = %d\n",bit_test);
		if (bit_test ==0)	
			increment_PC(r1, fnew, cp); //PC will be incremented towards the end in main(). Now increment again to Skip next instruction

		PRINT("Program counter: PC= %d\n",(r1->PC)+1);
		PRINT("Status register contents:(hex):");

 		PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

		break;


	case 25: //BTFSS

	PRINT("BTFSS instruction\n");
 	PRINT("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
	 switch (i1-> bit)
		{
		case 0:
			bit_test = r1-> GP_Reg[i1-> reg_index] & 0x01;
		break;

		case 1:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x02) >> 1;
		break;

		case 2:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x04) >> 2;
		break;

		case 3:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x08) >> 3;
		break;

		case 4:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x10) >> 4;
		break;

		case 5:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x20) >> 5;
		break;

		case 6:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x40) >> 6;
		break;

		case 7:
			bit_test = (r1-> GP_Reg[i1-> reg_index] & 0x80) >> 7;
		break;
		
		default: 
			printf ("INVALID instruction!\n"); 
		break;
		}	

		PRINT("Bit test = %d\n",bit_test);
		if (bit_test ==1)	
			increment_PC(r1, fnew, cp); //PC will be incremented towards the end in main(). Now increment again to Skip next instruction
		
		PRINT("Program counter: PC= %d\n",(r1->PC)+1);
		PRINT("Status register contents:(hex):");

		PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	break;

	
	case 26: //MOVLW
	
		PRINT("MOVLW instruction\n");


		PRINT("Contents of W(hex) before execution= %x \n", r1-> W);
		r1-> W = i1-> immediate_value;

		PRINT("Contents of W(hex) after execution= %x \n", r1-> W);

	break;

	case 27: //RETLW
	
		PRINT("RETLW instruction\n");
		
		PRINT(" Before execution, Contents of W(hex) = %x, PC (hex)=%x \n", r1-> W, r1-> PC);
		r1-> W = i1-> immediate_value;

		if (r1-> stack_pointer == 0)
			PRINT("Stack underflow, nothing to pop\n");
		else
			{
			
			//PC loaded from top of stack
			r1-> PC = r1-> stack[--r1-> stack_pointer]; //Decrement stack pointer and pop
			r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL

			-- r1-> PC; //Decrement and save the new PC value since it will be incremented in main() after execute..
			-- r1 -> GP_Reg[2];

			PRINT("PC popped from stack:(hex): %x, (dec): %d \n", (r1-> PC)+1,  (r1-> PC)+1);		
			}


		PRINT("Contents of W(hex) after execution= %x \n", r1-> W);

	break;

	case 28: //IORLW

	
	PRINT("IORLW instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of W= %x\n", r1-> W);
		
	PRINT("After execution:\n");
	
		r1-> W = (r1-> W | i1-> immediate_value) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;

	case 29: //ANDLW

	
	PRINT("ANDLW instruction\n");

//	W is the accumulator and d is the destination bit

	PRINT("Before execution (hex): Contents of W= %x\n ", r1-> W);
		
	PRINT("After execution:\n");
	
		r1-> W = (r1-> W & i1-> immediate_value) & 0x000000FF; //Bit wise AND and Keep only 8 bits in result

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;


	case 30: //XORLW

	
	PRINT("XORLW instruction\n");

//	W is the accumulator and d is the destination bit


	PRINT("Before execution (hex): Contents of W= %x\n ", r1-> W);
		
	PRINT("After execution:\n");
	
		r1-> W = (r1-> W ^ i1-> immediate_value) & 0x000000FF; //Bit wise AND and Keep only 8 bits in result

		PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register


		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");
	
	break;

	case 31: //SUBLW

	
	PRINT("SUBLW instruction\n");

//	W is the accumulator 
	
	PRINT("Before execution: Contents of  W (hex)= %x\n ", r1-> W);		
		
	PRINT("After execution:\n");
	
	twos_comp = (~(r1-> W) & 0x000000FF)+1;  //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits

	PRINT("~W (hex): %x\n", twos_comp);
		temp_sub = (i1-> immediate_value + twos_comp);
		r1-> W = temp_sub & 0x000000FF;

		PRINT("Contents of destination is W= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				PRINT("Result is negative\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			}
		else 
			{
				PRINT("Result is positive\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			}
		

		PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;


	case 32: //ADDLW

	
	PRINT("ADDLW instruction\n");

	//W is the accumulator and immediate_value is the immediate value to be added
	
	PRINT("Before execution: Contents (hex) of W= %x\n ", r1-> W);		
		
	PRINT("After execution:\n");
	
	temp_add = (r1-> W + i1->immediate_value);
	r1-> W = temp_add & 0x000000FF;

	PRINT("Contents of destination is W (hex)= %x \n", r1-> W);
	if( r1-> W == 0)
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

	if (((temp_add & 0x100) >>8) == 0)
		r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			
	else 
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			
		
	PRINT("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			PRINT("%x", r1-> GP_Reg[3]);
		PRINT("\n");

	break;


	case 33: 

	PRINT("CALL instruction\n");

//This case will be entered even if there is a valid CALL instruction. 

//But this if condition will be entered only if the processor is executing this instruction after the random mem location taht is flipped is same as the PC value
//That means, the instruction at the current PC value has been modified into a CALL instruction through a bit flip

	//W is the accumulator and immediate_value is the immediate value to be added
if (cp->flip_bit_flag_for_illegal_inst==1)
	{
		if (cp-> random_mem[(cp->mem_count) -1] == (r1-> PC)) //Control flow changes
		{
		
			printf("\nCRASH: Instruction has got changed to CALL instruction: Control flow has changed..\n");
			fprintf(fnew,"\nCRASH: Instruction has got changed to CALL instruction: Control flow has changed..\n");
				  		
			printf("PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);
			fprintf(fnew,"PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);

			PRINT("Instruction cycle=%llu\n",cp->instr_cycles);

			cp->control_flow_change++;
			report_crash( r1,  program_memory, cp, start_seconds,i1, fnew, fPC, finstr);
		}
	}
		//printf("Bit flipped, Content of the program_memory[%x] is (in hex) %x\n\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);
	
	
	PRINT("Before execution: Contents (hex) of PC= %x\n", r1-> PC);		
	PRINT("Stack pointer: %d\n",r1-> stack_pointer);		
	if (r1-> stack_pointer == 8)
		r1-> stack_pointer =0; //Reset stack pointer and overwrite

//PC+1 on top of stack and increment stack pointer
//PC not incremented in fetch stage..So,  push PC+1
	r1-> stack[r1-> stack_pointer++] = ((r1-> PC)+1); 
	
	PRINT("PCLATH before shifting=%x\n",r1->PCLATH);
		PRINT("PCLATH after ANDing=%x\n",(r1-> PCLATH)& 0x18);
	//Store in the decremented PC position
	r1-> PC = (i1-> immediate_value) | (((r1-> PCLATH)& 0x18) << 8); //Get only PCLATH<4:3> by ANDing and then shift them to PC<12:11>
	//PCL= GP_Reg[2]
	r1 -> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
	r1 -> GP_Reg[0x0A] = ((r1-> PC) & 0x1F00) >> 8; //PCLATH
	


	-- r1-> PC; //Decrement and save the new PC value since it will be incremented in main() after execute..
	-- r1 -> GP_Reg[2];

	PRINT("Stack pointer incremented: %d\n",r1-> stack_pointer);
	PRINT("After execution: Contents of PC= address specified in CALL instruction:(hex) %x\n", (r1-> PC));		
	PRINT("After execution: Contents of PCL will be = %x\n", (r1 -> GP_Reg[2])+1);
	PRINT("After execution: Contents of PCLATH will be = %x\n", (r1 -> GP_Reg[0x0A]));
	PRINT("Top of stack: Address of next instruction: %x \n", r1-> stack[r1-> stack_pointer -1]);
	PRINT("Stack pointer: %d\n",r1-> stack_pointer);
	break;

	case 34: 

	PRINT("GOTO instruction\n");
//This case will be entered even if there is a valid GOTO instruction. 

//But this if condition will be entered only if the processor is executing this instruction after the random mem location taht is flipped is same as the PC value
//That means, the instruction at the current PC value has been modified into a GOTO instruction through a bit flip

	//W is the accumulator and immediate_value is the immediate value to be added
if (cp->flip_bit_flag_for_illegal_inst==1)
	{
	if (cp-> random_mem[(cp->mem_count) -1] == (r1-> PC)) //Control flow changes
		{
		
			printf("\nCRASH: Instruction has got changed to GOTO instruction: Control flow has changed..\n");
			fprintf(fnew,"\nCRASH: Instruction has got changed to GOTO instruction: Control flow has changed..\n");
			
			printf("PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);
			fprintf(fnew,"PC value (in hex)=%x, instruction opcode that got modified was (in hex)=%x\n", (r1-> PC), program_memory[ (r1-> PC)]);
		
			cp->control_flow_change++;
			report_crash( r1,  program_memory, cp, start_seconds,i1, fnew, fPC, finstr);
			
			PRINT("Instruction cycle=%llu\n",cp->instr_cycles);
		}
	}
	//printf("Bit flipped, Content of the program_memory[%x] is (in hex) %x\n\n", cp-> random_mem[cp->mem_count], program_memory[cp-> random_mem[cp->mem_count]]);

	PRINT("Before execution: Contents (hex) of PC= %x\n", r1-> PC);		
	
	/*r1-> PC = (i1-> immediate_value) | ((r1-> PCLATH) << 8);
	r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL*/ //old code- not right

	r1-> PC = (i1-> immediate_value) | (((r1-> PCLATH)& 0x18) << 8); //Get only PCLATH<4:3> by ANDing and then shift them to PC<12:11>
	//PCL= GP_Reg[2]
	r1 -> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
	r1 -> GP_Reg[0x0A] = ((r1-> PC) & 0x1F00) >> 8; //PCLATH

	
	
	-- r1-> PC; //Decrement and save the new PC value since it will be incremented in main() after execute..
	-- r1 -> GP_Reg[2];

	PRINT("After execution: Contents (hex) of PC= %x\n", (r1-> PC)+1);		

	break;


	default: 
		printf ("INVALID instruction!\n"); 
	break;

	}

	return 0;
PRINT("-------------------------------------------------------------------\n");
}

//These functions are not used
int push(struct registers *r)
{
	if (r-> stack_pointer == 8)
		r-> stack_pointer =0; //Reset stack pointer and overwrite

	r-> stack[r-> stack_pointer++] = r-> PC;
	return 0;
}

int pop (struct registers *r)
{
	
	if (r-> stack_pointer == 0)
		PRINT("Stack underflow, nothing to pop\n");
	else
		r-> PC = r-> stack[--r-> stack_pointer]; //Decrement stack pointer and pop
	
return 0;
}




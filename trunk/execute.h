//Execute.h 

int instruction_execute(struct registers *r1, struct instructions *i1)
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
	
		printf("SLEEP instruction\n");
		if (r1-> configuration_word[11]== 1) //Watch dog timer Enable bit in configuration word.
			{
				r1-> WDT = 0; //Reset Watch dog timer
				r1-> WDT_prescaler= 0; //Reset WDT prescaler
			
			
			printf("Watch dog timer cleared: %d \n", r1-> WDT);

			printf("Watch dog prescaler cleared: %d \n",r1-> WDT_prescaler );
			
			}
		else PRINT("WDT Enable bit not set, Watchdog timer cannot be cleared\n");	
		
		//GP_Reg[3]= status register
		r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xF7; //PD bar bit in status reg =0. Bit 3 = 0
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x10; //TO bar bit in status reg=1. Bit 4 = 1
		
		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

		
	break;

	
	case 1: //CLRWDT
	
		printf("CLRWDT instruction\n");
		if (r1-> configuration_word[11]== 1) //Watch dog timer Enable bit in configuration word
			{
				r1-> WDT = 0; //Reset Watch dog timer
				r1-> WDT_prescaler= 0; //Reset WDT prescaler
			
	
			printf("Watch dog timer cleared: %d \n", r1-> WDT);

			printf("Watch dog prescaler cleared: %d \n",r1-> WDT_prescaler );
			
			}

		else PRINT("WDT Enable bit not set, Watchdog timer cannot be cleared\n");	

		//GP_Reg[3]= status register
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x08; //PD bar bit in status reg=1. Bit 3= 1
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x10; //TO bar bit in status reg=1. Bit 4 = 1
		
		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

		
	break;
		
	case 2: //RETURN 
		
		printf("RETURN instruction\n");
		printf("Top of stack: %x , stack pointer=%d \n", r1-> stack[r1-> stack_pointer -1],r1-> stack_pointer);
		
		if (r1-> stack_pointer == 0)
			printf("Stack underflow, nothing to pop\n");
		else
			{
		//PC loaded from top of stack
			r1-> PC = r1-> stack[--r1-> stack_pointer]; //Decrement stack pointer and pop
			r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
			PRINT("First decrement Stack pointer: %d\n", r1-> stack_pointer);
			printf("PC popped from stack:(hex): %x, (dec): %d \n", r1-> PC,  r1-> PC);		
			}
	
	break;

	
	case 3: //RETFIE
		
		printf("RETFIE instruction\n");
		printf("Top of stack: %x , stack pointer=%d \n", r1-> stack[r1-> stack_pointer],r1-> stack_pointer);
		
		if (r1-> stack_pointer == 0)
			printf("Stack underflow, nothing to pop\n");
		else
			{
		//PC loaded from top of stack
			r1-> PC = r1-> stack[--r1->stack_pointer]; //Decrement stack pointer and pop
			r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
			printf("PC popped from stack: %x \n", r1-> PC);		
			}

		//GP_Reg[11] = INTCON register at address 0BH
		r1-> GP_Reg[11] = r1-> GP_Reg[11] | 0x80; //Global interrupt enable (GIE) bit in INTCON reg is set- enables all unmasked interrupts
		
		printf("INTCON register contents:(hex): ");
		//for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[11]);
		printf("\n");
	
	break;

	case 4: //NOP
	
		printf("NOP instruction\n");

	break;


	case 5: //MOVWF
	
		printf("MOVWF instruction\n");
		
		r1-> GP_Reg[i1-> reg_index]= r1-> W;

		printf("Contents of destination is reg_file [%x] (hex)= %x \n", i1-> reg_index, r1-> GP_Reg[i1-> reg_index]);

	break;

	case 6: //CLRW
		
		printf("Contents of W before execution (hex)= %x \n", r1-> W);
		printf("CLRW instruction\n");
		
		r1-> W =0;
		printf("Contents of W cleared after execution (hex)= %x \n", r1-> W);
		
		//GP_Reg[3]= status register
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register
		
		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;
		
	case 7: //CLRF
		
		printf("Contents of reg file before execution (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		printf("CLRF instruction\n");
		
		r1-> GP_Reg[i1-> reg_index] = 0;
		printf("Contents of reg file[%x] cleared after execution (hex)= %x \n", i1-> reg_index, r1-> GP_Reg[i1-> reg_index]);

		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register
		
			printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;

	case 8: //SUBWF

	
	printf("SUBWF instruction\n");

	/*	W is the accumulator and d is the destination bit
	d=0 means- destination is W register i.e., accumulator 
	d=1 means- destination is the file register */
	printf("Before execution: Contents of Reg file (hex)= %x, W (hex)= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);		
		
	printf("After execution:\n");
	
	if (i1-> d==0)
		{

//	twos_comp = (~(r1-> W) + 1); //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits
	twos_comp = (~(r1-> W) & 0x000000FF)+1; 
	
	PRINT("~W (hex): %x\n", twos_comp);
		temp_sub = (r1-> GP_Reg[i1-> reg_index] + twos_comp);
		r1-> W = temp_sub & 0x000000FF;
	

		printf("Contents of destination is W= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				printf("Result is negative\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			}
		else 
			{
				printf("Result is positive\n");
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

		printf("temp_sub (hex)= %x\n",temp_sub);

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				printf("Result is negative\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			}
		else 
			{
				printf("Result is positive\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			}
		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;

	case 9: //DECF

	
	printf("DECF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = (r1-> GP_Reg[3] | 0x04); //Z flag set in the status register

		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 10: //IORWF

	
	printf("IORWF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] | r1-> W) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = (r1-> GP_Reg[3] | 0x04); //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] | r1-> W) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = (r1-> GP_Reg[3] | 0x04); //Z flag set in the status register

		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 11: //ANDWF

	
	printf("ANDWF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] & r1-> W) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] & r1-> W) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 12: //XORWF

	
	printf("XORWF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution: Contents (hex) of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] ^ r1-> W) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] ^ r1-> W) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 13: //ADDWF

	
	printf("ADDWF instruction\n");

	//	W is the accumulator and d is the destination bit

	
	printf("Before execution: Contents (hex) of Reg file= %x, W= %x\n ", r1-> GP_Reg[i1-> reg_index], r1-> W);		
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

		temp_add = (r1-> GP_Reg[i1-> reg_index] + r1-> W);
		r1-> W = temp_add & 0x000000FF;

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
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

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( (r1-> GP_Reg[i1-> reg_index]) ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_add & 0x100) >>8) == 0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			
		else 
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			
		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;

	
	case 14: //MOVF
		
		printf("MOVF instruction\n");
		
		
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
			printf("Contents of destination is W (hex)= %x \n", r1-> W);
		else
			printf("Contents of destination is reg_file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

			
			printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;

	case 15: //COMF
		
		printf("Contents of reg file before execution (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		printf("COMF instruction\n");
		
		
		if (i1-> d==0) //d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = ~(r1-> GP_Reg[i1-> reg_index]) & 0x000000FF ; //limit to 8 bits

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = ~(r1-> GP_Reg[i1-> reg_index]) & 0x000000FF;  //limit to 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( (r1-> GP_Reg[i1-> reg_index]) ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}

		
			printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;

	case 16: //INCF

	
	printf("INCF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		}


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 17: //DECFSZ

	
	printf("DECFSZ instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			increment_PC(&r1);
//comment out: r1-> PC = r1-> PC + 1; //PC already incremented in fetch step. Now increment again to Skip next instruction
PRINT("PC (testing)= %d\n", r1->PC);
		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			increment_PC(&r1); //PC already incremented in fetch step. Now increment again to Skip next instruction

		}

//Status register not affected

		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
		
		printf("Program counter: PC= %d\n",r1->PC);
	
	break;


	case 18: //RRF

	
	printf("RRF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
	
	original_regfile= r1-> GP_Reg[i1-> reg_index];
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) & 0x000000FF) >> 1; //Keep only 8 bits
			if((r1-> GP_Reg[3] & 0x01) ==0) //Carry=0

				r1->W = temp_rotate;
			else //Carry = 1
				r1->W = temp_rotate | 0x80;

		printf("Contents of destination after right shift is W (hex)= %x \n", r1-> W);


		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) & 0x000000FF) >> 1; //Keep only 8 bits
			if((r1-> GP_Reg[3] & 0x01) ==0) //Carry=0
				r1-> GP_Reg[i1-> reg_index] = temp_rotate;
			else //Carry = 1
				r1-> GP_Reg[i1-> reg_index] = temp_rotate | 0x80;

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);


		}


	// LSB of reg file will go to carry through right shift
	if ((original_regfile & 0x00000001) ==1) 
			r1-> GP_Reg[3] | 0x01; //Set the carry to 1, that is the LSB of the reg file
	else
			r1-> GP_Reg[3] & 0xFE; //Set carry to 0

		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
		
	
	break;

	case 19: //RLF

	
	printf("RLF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
	
	original_regfile= r1-> GP_Reg[i1-> reg_index];
	printf("After execution:\n");
	
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

		printf("Contents of destination after left shift is W (hex)= %x \n", r1-> W);


		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) <<1 ) & 0x000000FF; //Keep only 8 bits
			if((r1-> GP_Reg[3] & 0x01) ==0) //Carry=0
				r1-> GP_Reg[i1-> reg_index] = temp_rotate;
			else //Carry = 1
				r1-> GP_Reg[i1-> reg_index] = temp_rotate | 0x01;

		printf("Contents of destination after left shift is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);


		}


 // MSB of reg file will go to carry through right shift
	if(((original_regfile & 0x80) >>7) == 1)
		r1-> GP_Reg[3] | 0x01; //Set the carry to 1, that is the MSB of the reg file
	else
		r1-> GP_Reg[3] & 0xFE; //Set carry to 0

	printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
		
	
	break;


	case 20: //SWAPF

	
	printf("SWAPF instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
	
	original_regfile= r1-> GP_Reg[i1-> reg_index];
	swap_lower_nibble= (original_regfile & 0x0F) << 4;
	swap_upper_nibble= (original_regfile & 0xF0) >> 4;
	
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{
			r1->W = swap_upper_nibble | swap_lower_nibble;
			printf("Contents of destination after swap is W (hex)= %x \n", r1-> W);
		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
			r1-> GP_Reg[i1-> reg_index] = swap_upper_nibble | swap_lower_nibble;
			printf("Contents of destination after swap is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

		}


	
		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 21: //INCFSZ

	
	printf("INCFSZ instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of Reg file= %x\n ", r1-> GP_Reg[i1-> reg_index]);		
		
	printf("After execution:\n");
	
	if (i1-> d==0) //	d=0 means- destination is W register i.e., accumulator 
		{

			r1-> W = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			increment_PC(&r1); //PC already incremented in fetch step. Now increment again to Skip next instruction

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			increment_PC(&r1); //PC already incremented in fetch step. Now increment again to Skip next instruction

		}

//Status register not affected

		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
		
		printf("Program counter: PC= %d\n",r1->PC);
	
	break;

	case 22: //BCF

	printf("BCF instruction\n");

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
			PRINT ("INVALID instruction!\n"); 
		break;
		}	

	printf("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
	printf("Status register contents:(hex):");

	printf("%x", r1-> GP_Reg[3]);
	printf("\n");

	break;

	case 23: //BSF

	printf("BSF instruction\n");
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
			PRINT ("INVALID instruction!\n"); 
		break;
		}	

	printf("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
	printf("Status register contents:(hex):");

	printf("%x", r1-> GP_Reg[3]);
	printf("\n");
	break;

	case 24: //BTFSC

	printf("BTFSC instruction\n");
 	printf("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
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
			PRINT ("INVALID instruction!\n"); 
		break;
		}	

		printf("Bit test = %d\n",bit_test);
		if (bit_test ==0)	
			increment_PC(&r1); //PC already incremented in fetch step. Now increment again to Skip next instruction

		printf("Program counter: PC= %d\n",r1->PC);
		printf("Status register contents:(hex):");

 		printf("%x", r1-> GP_Reg[3]);
		printf("\n");

		break;


	case 25: //BTFSS

	printf("BTFSS instruction\n");
 	printf("Contents of is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
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
			PRINT ("INVALID instruction!\n"); 
		break;
		}	

		printf("Bit test = %d\n",bit_test);
		if (bit_test ==1)	
			increment_PC(&r1); //PC already incremented in fetch step. Now increment again to Skip next instruction
		
		printf("Program counter: PC= %d\n",r1->PC);
		printf("Status register contents:(hex):");

		printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	break;

	
	case 26: //MOVLW
	
		printf("MOVLW instruction\n");
		
		PRINT("Contents of W(hex) before execution= %x \n", r1-> W);
		r1-> W = i1-> immediate_value;

		printf("Contents of W(hex) after execution= %x \n", r1-> W);

	break;

	case 27: //RETLW
	
		printf("RETLW instruction\n");
		
		PRINT(" Before execution, Contents of W(hex) = %x, PC (hex)=%x \n", r1-> W, r1-> PC);
		r1-> W = i1-> immediate_value;

		if (r1-> stack_pointer == 0)
			printf("Stack underflow, nothing to pop\n");
		else
			{
			//PC loaded from top of stack
			r1-> PC = r1-> stack[--r1-> stack_pointer]; //Decrement stack pointer and pop
			r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL

			printf("PC popped from stack:(hex): %x, (dec): %d \n", r1-> PC,  r1-> PC);		
			}


		printf("Contents of W(hex) after execution= %x \n", r1-> W);

	break;

	case 28: //IORLW

	
	printf("IORLW instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of W= %x\n", r1-> W);
		
	printf("After execution:\n");
	
		r1-> W = (r1-> W | i1-> immediate_value) & 0x000000FF; //Bit wise OR and Keep only 8 bits in result

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;

	case 29: //ANDLW

	
	printf("ANDLW instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of W= %x\n ", r1-> W);
		
	printf("After execution:\n");
	
		r1-> W = (r1-> W & i1-> immediate_value) & 0x000000FF; //Bit wise AND and Keep only 8 bits in result

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;


	case 30: //XORLW

	
	printf("XORLW instruction\n");

//	W is the accumulator and d is the destination bit

	printf("Before execution (hex): Contents of W= %x\n ", r1-> W);
		
	printf("After execution:\n");
	
		r1-> W = (r1-> W ^ i1-> immediate_value) & 0x000000FF; //Bit wise AND and Keep only 8 bits in result

		printf("Contents of destination is W (hex)= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register


		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");
	
	break;

	case 31: //SUBLW

	
	printf("SUBLW instruction\n");

//	W is the accumulator 
	
	printf("Before execution: Contents of  W (hex)= %x\n ", r1-> W);		
		
	printf("After execution:\n");
	
	twos_comp = (~(r1-> W) & 0x000000FF)+1;  //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits

	PRINT("~W (hex): %x\n", twos_comp);
		temp_sub = (i1-> immediate_value + twos_comp);
		r1-> W = temp_sub & 0x000000FF;

		printf("Contents of destination is W= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				printf("Result is negative\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			}
		else 
			{
				printf("Result is positive\n");
				r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			}
		

		printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;


	case 32: //ADDLW

	
	printf("ADDLW instruction\n");

	//W is the accumulator and immediate_value is the immediate value to be added
	
	printf("Before execution: Contents (hex) of W= %x\n ", r1-> W);		
		
	printf("After execution:\n");
	
	temp_add = (r1-> W + i1->immediate_value);
	r1-> W = temp_add & 0x000000FF;

	printf("Contents of destination is W (hex)= %x \n", r1-> W);
	if( r1-> W == 0)
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x04; //Z flag set in the status register

	if (((temp_add & 0x100) >>8) == 0)
		r1-> GP_Reg[3] = r1-> GP_Reg[3] & 0xFE; //C flag reset in the status register
			
	else 
		r1-> GP_Reg[3] = r1-> GP_Reg[3] | 0x01; //C flag set in the status register
			
		
	printf("Status register contents:(hex):");

//		for (i=0;i<=7;i++)
			printf("%x", r1-> GP_Reg[3]);
		printf("\n");

	break;


	case 33: 

	printf("CALL instruction\n");

	//W is the accumulator and immediate_value is the immediate value to be added
	
	printf("Before execution: Contents (hex) of PC= %x\n", r1-> PC);		
	PRINT("Stack pointer: %d\n",r1-> stack_pointer);		
	if (r1-> stack_pointer == 8)
		r1-> stack_pointer =0; //Reset stack pointer and overwrite

//PC+1 on top of stack and increment stack pointer
//PC already incremented in fetch stage..So, just push PC
	r1-> stack[r1-> stack_pointer++] = (r1-> PC); 


	r1-> PC = (i1-> immediate_value) | ((r1-> PCLATH) << 8);
	//PCL= GP_Reg[2]
	r1 -> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL

	PRINT("Stack pointer incremented: %d\n",r1-> stack_pointer);
	printf("After execution: Contents of PC= address specified in CALL instruction:(hex) %x\n", r1-> PC);		
	printf("After execution: Contents of PCL= %x\n", r1 -> GP_Reg[2]);
	printf("Top of stack: Address of next instruction: %d \n", r1-> stack[r1-> stack_pointer -1]);
	PRINT("Stack pointer: %d\n",r1-> stack_pointer);
	break;

	case 34: 

	printf("GOTO instruction\n");

	//W is the accumulator and immediate_value is the immediate value to be added
	
	printf("Before execution: Contents (hex) of PC= %x\n", r1-> PC);		
	
	r1-> PC = (i1-> immediate_value) | ((r1-> PCLATH) << 8);
	r1-> GP_Reg[2] = (r1-> PC) & 0xFF; //PCL
	
	printf("After execution: Contents (hex) of PC= %x\n", r1-> PC);		

	break;


	default: 
		PRINT ("INVALID instruction!\n"); 
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
		printf("Stack underflow, nothing to pop\n");
	else
		r-> PC = r-> stack[--r-> stack_pointer]; //Decrement stack pointer and pop
	
return 0;
}




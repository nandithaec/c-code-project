//Execute.h 

int instruction_execute(struct registers *r1, struct instructions *i1)
{
	PRINT("-------------------------------------------------------------------\n");
	PRINT("INSTRUCTION EXECUTION >>\n");
	int i=0;
	int twos_comp=0, temp_sub= 0, temp_add=0, temp_rotate=0, original_regfile=0;
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

		r1-> status_reg[4] = 0; //PD bar bit in status reg
		r1-> status_reg[3] = 1; //TO bar bit in status reg
		
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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

		r1-> status_reg[4] = 1; //PD bar bit in status reg
		r1-> status_reg[3] = 1; //TO bar bit in status reg
		
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
		printf("\n");

		
	break;
		
	case 2: //RETURN 
		
		printf("RETURN instruction\n");
				
		if (r1-> stack_pointer == 0)
			printf("Stack underflow, nothing to pop\n");
		else
			{
			r1-> PC = r1-> stack[--r1-> stack_pointer]; //Decrement stack pointer and pop
			printf("PC popped from stack:(hex): %x, (dec): %d \n", r1-> PC,  r1-> PC);		
			}
	
	break;

	
	case 3: //RETFIE
		
		printf("RETFIE instruction\n");
				
		if (r1-> stack_pointer == 0)
			printf("Stack underflow, nothing to pop\n");
		else
			{
			r1-> PC = r1-> stack[--r1-> stack_pointer]; //Decrement stack pointer and pop
			printf("PC popped from stack: %d \n", r1-> PC);		
			}

		r1-> INTCON_reg[0] = 1; //Global interrupt enable (GIE) bit in INTCON reg is set- enables all unmasked interrupts
		
		printf("INTCON register contents: ");
		for (i=0;i<=7;i++)
			printf("%d", r1-> INTCON_reg[i]);
		printf("\n");
	
	break;

	case 4: //NOP
	
		printf("NOP instruction\n");

	break;


	case 5: //MOVWF
	
		printf("MOVWF instruction\n");
		
		r1-> GP_Reg[i1-> reg_index]= r1-> W;

		printf("Contents of destination is reg_file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

	break;

	case 6: //CLRW
		
		printf("Contents of W before execution (hex)= %x \n", r1-> W);
		printf("CLRW instruction\n");
		
		r1-> W =0;
		printf("Contents of W cleared after execution (hex)= %x \n", r1-> W);

		r1-> status_reg[5] = 1; //Z flag set
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
		printf("\n");

	break;
		
	case 7: //CLRF
		
		printf("Contents of reg file before execution (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		printf("CLRF instruction\n");
		
		r1-> GP_Reg[i1-> reg_index] = 0;
		printf("Contents of reg file cleared after execution (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

		r1-> status_reg[5] = 1; //Z flag set
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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

	twos_comp = (~(r1-> W) + 1) & 0x000000FF; //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits

	PRINT("~W (hex): %x\n", twos_comp);
		temp_sub = (r1-> GP_Reg[i1-> reg_index] + twos_comp);
		r1-> W = temp_sub & 0x000000FF;

		printf("Contents of destination is W= %x \n", r1-> W);
		if( r1-> W ==0)
			r1-> status_reg[5] = 1; //Z flag set

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				printf("Result is negative\n");
				r1-> status_reg[7] = 0; //C flag reset
			}
		else 
			{
				printf("Result is positive\n");
				r1-> status_reg[7] = 1; //C flag set
			}
		}

	else if (i1-> d==1)
		{
		twos_comp = (~(r1-> W) + 1) & 0x000000FF; //Take negation of W and add 1 for 2's comp addition. Keep only 8 bits

	PRINT("~W (hex): %x\n", twos_comp);
		temp_sub = (r1-> GP_Reg[i1-> reg_index] + twos_comp);
		r1-> GP_Reg[i1-> reg_index] = temp_sub & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		if (((temp_sub & 0x100) >>8) == 0)
			{	
				printf("Result is negative\n");
				r1-> status_reg[7] = 0; //C flag reset
			}
		else 
			{
				printf("Result is positive\n");
				r1-> status_reg[7] = 1; //C flag set
			}
		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] | r1-> W) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] & r1-> W) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] ^ r1-> W) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		if (((temp_add & 0x100) >>8) == 0)
			r1-> status_reg[7] = 0; //C flag reset
			
		else 
			
			r1-> status_reg[7] = 1; //C flag set
			
		}

	else if (i1-> d==1) //d=1 means- destination is the file register 
		{
		
		temp_add = (r1-> GP_Reg[i1-> reg_index] + r1-> W);
		r1-> GP_Reg[i1-> reg_index] = temp_add & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		if (((temp_add & 0x100) >>8) == 0)
			r1-> status_reg[7] = 0; //C flag reset
			
		else 
			r1-> status_reg[7] = 1; //C flag set
			
		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1;
		else r1-> status_reg[5]= 0;

	
		if(i1-> d==0)
			printf("Contents of destination is W (hex)= %x \n", r1-> W);
		else
			printf("Contents of destination is reg_file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);

			
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = ~(r1-> GP_Reg[i1-> reg_index]) & 0x000000FF;  //limit to 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		}

		
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> status_reg[5] = 1; //Z flag set

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] + 1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> status_reg[5] = 1; //Z flag set

		}


	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			r1-> PC = r1-> PC + 1; //PC already incremented in fetch step. Now increment again to Skip next instruction

		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		r1-> GP_Reg[i1-> reg_index] = (r1-> GP_Reg[i1-> reg_index] -1) & 0x000000FF; //Keep only 8 bits

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);
		if( r1-> GP_Reg[i1-> reg_index] ==0)
			r1-> PC = r1-> PC + 1; //PC already incremented in fetch step. Now increment again to Skip next instruction

		}

//Status register not affected

	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			if(r1-> status_reg[7]==0) //Carry=0
				r1->W = temp_rotate;
			else //Carry = 1
				r1->W = temp_rotate | 0x80;

		printf("Contents of destination after right shift is W (hex)= %x \n", r1-> W);


		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) & 0x000000FF) >> 1; //Keep only 8 bits
			if(r1-> status_reg[7]==0) //Carry=0
				r1-> GP_Reg[i1-> reg_index] = temp_rotate;
			else //Carry = 1
				r1-> GP_Reg[i1-> reg_index] = temp_rotate | 0x80;

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);


		}


	r1-> status_reg[7] = original_regfile & 0x00000001; // LSB of reg file will go to carry through right shift

	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
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
			if(r1-> status_reg[7]==0) //Carry=0
				r1->W = temp_rotate;
			else //Carry = 1
				r1->W = temp_rotate | 0x01;

		printf("Contents of destination after left shift is W (hex)= %x \n", r1-> W);


		
		}

	else if (i1-> d==1) //d=1 means- destination is the file register
		{
		
		temp_rotate = ((r1-> GP_Reg[i1-> reg_index]) <<1 ) & 0x000000FF; //Keep only 8 bits
			if(r1-> status_reg[7]==0) //Carry=0
				r1-> GP_Reg[i1-> reg_index] = temp_rotate;
			else //Carry = 1
				r1-> GP_Reg[i1-> reg_index] = temp_rotate | 0x01;

		printf("Contents of destination is Reg file (hex)= %x \n", r1-> GP_Reg[i1-> reg_index]);


		}


	r1-> status_reg[7] = (original_regfile & 0x80) >>7; // MSB of reg file will go to carry through right shift

	printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
		printf("\n");
		
	
	break;


	default: 
		PRINT ("INVALID instruction!\n"); 
	break;

	}

	return 0;
PRINT("-------------------------------------------------------------------\n");
}

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




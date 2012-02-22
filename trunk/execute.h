

int instruction_execute(struct registers *r1, struct instructions *i1)
{
	PRINT("-------------------------------------------------------------------\n");
	PRINT("INSTRUCTION EXECUTION >>\n");
	int i=0;

	switch(i1-> instr_mnemonic_enum)
	{

	case 0: //SLEEP
	
		printf("SLEEP instruction\n");
		if (r1-> configuration_word[11]== 1) //Watch dog timer Enable bit in configuration word
			{
				for (i=0;i<=7;i++)	
				{
					r1-> WDT[i] = 0; //Reset Watch dog timer
					r1-> WDT_prescaler[i]= 0; //Reset WDT prescaler
				}
			
			printf("Watch dog timer cleared: ");
			for (i=0;i<=7;i++)
				printf("%d", r1-> WDT[i]);
			printf("\n");

			printf("Watch dog prescaler cleared: ");
			for (i=0;i<=7;i++)
				printf("%d", r1-> WDT_prescaler[i]);
			printf("\n");
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
				for (i=0;i<=7;i++)	
				{
					r1-> WDT[i] = 0; //Reset Watch dog timer
					r1-> WDT_prescaler[i]= 0; //Reset WDT prescaler
				}
			
			printf("Watch dog timer cleared: ");
			for (i=0;i<=7;i++)
				printf("%d", r1-> WDT[i]);
			printf("\n");

			printf("Watch dog prescaler cleared: ");
			for (i=0;i<=7;i++)
				printf("%d", r1-> WDT_prescaler[i]);
			printf("\n");
			}

		else PRINT("WDT Enable bit not set, Watchdog timer cannot be cleared\n");	

		r1-> status_reg[4] = 1; //PD bar bit in status reg
		r1-> status_reg[3] = 1; //TO bar bit in status reg
		
		printf("Status register contents: ");

		for (i=0;i<=7;i++)
			printf("%d", r1-> status_reg[i]);
		printf("\n");

		
	break;
		
	//case 2: //RETURN 
		
		

		
	case 14:
		
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
			printf("Contents of destination is W= %d \n", r1-> W);
		else
			printf("Contents of destination is reg_file= %d \n", r1-> GP_Reg[i1-> reg_index]);

			
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

int push(struct registers *)
{
	if (r-> stack_pointer == 8)
		r-> stack_pointer =0; //Reset stack pointer
	r-> stack[r-> stack_pointer++] = r-> PC;
	return 0;
}

int pop (struct registers *r)
{
	
	r-> PC = r-> stack[--r-> stack_pointer]; //Decrement stack pointer and pop
	return 0;
}




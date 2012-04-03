/* Hamming code generation and error detection, correction*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


int check_if_power_of_two (int, int *);
int convert_decimal_to_binary(int, int[],int);
int	convert_binary_to_decimal(int[], int*);
int calculate_parity_bits(int[], int[]);
int detect_error(int[], int*);
int decode_received_data(int[],int[]);
 
int main()
{
    int i=0, j=0, p1=0, p2=0, p4=0, p8=0;
	int binary_input[10]={0}; //8-bit data
	int hamming_code[14]={0}, binary_received[14]={0}, parity[5]= {0}, decoded_data_binary[10]={0};
	int power_of_two=0, decimal_input=0, decimal_received=0, hamming_code_decimal=0, bit_in_error=0;
   
    printf("\nHamming code----- Encoding\n");
    printf("Enter the number to be encoded, in decimal (not binary) :\n ");
	scanf("%d", &decimal_input);
	
	printf("\nData has been read and is %d (decimal), %x (in hex) \n", decimal_input, decimal_input);
	
	convert_decimal_to_binary(decimal_input,binary_input,8); //8 is the number of bits in the binary

	//for(i=1; i<=8; i++)
	   // scanf("%d",&binary_input[i]);

	printf("\nIn binary, the data to be encoded is:\n");
	for(i=1; i<=8; i++)
	    printf("%d",binary_input[i]);

printf("\n \n");

j=1;


	for(i=1; i<=12; i++)
	{
			
		if(i==1)
			{
				printf("Parity bit: Position %d\n", i);
				//hamming_code[i]=9;
				//printf("hamming_code[%d]= %d\n\n",i,hamming_code[i]);
			}
		else
		if(i !=1)
			check_if_power_of_two(i, &power_of_two);

		if( (i != 1 ) && power_of_two == 0 )
			{
			hamming_code[i]=binary_input[j];
			printf("hamming_code[%d]= binary_input[%d]= %d\n\n",i,j,hamming_code[i]);

			j++;
			}
		//else printf("Calculating parity bits..\n");
			
	}

	printf("\nHamming code template, with all parity bits set to zero is:\n");
	for(i=1;i<=12;++i)
		printf("%d ",hamming_code[i]);
	printf("\n");


	calculate_parity_bits(hamming_code,parity);


	//Concatenating the parity bits to the hamming code. Data bits have already been added previously.
	printf("Concatenating parity bits\n");

	j=1;
	for(i=1;i<=12;i++)
	{
			if( i == 1 )
				{
				printf("parity[%d] = %d\n",j, parity[j]);
				hamming_code[i]= parity[j];
				printf("hamming_code[%d]= parity[%d]= %d\n\n",i,j,hamming_code[i]);
				j++;
				}

			if(i !=1)
				check_if_power_of_two(i, &power_of_two);

			if(  power_of_two == 1 )
				{
				printf("parity[%d] = %d\n",j, parity[j]);
				hamming_code[i]= parity[j];
				printf("hamming_code[%d]= parity[%d]= %d\n\n",i,j,hamming_code[i]);
				j++;
				}
	}

	printf("\n");
	printf("************************************************\n");
	printf("binary_input that is being encoded is:\n");
	for(i=1; i<=8; i++)
	    printf("%d ",binary_input[i]);
	printf("\n");
	printf("************************************************\n");

	printf("\n");
	printf("************************************************\n");
	printf("Parity bits are:\n");
	for(i=1;i<=4;i++)
		printf("%d ", parity[i]);

	printf("\n");
	printf("************************************************\n");

	printf("\n");
	printf("************************************************\n");
	printf("Single error correcting Even parity Hamming code:\n");
	for(i=1;i<=12;++i)
		printf("%d ",hamming_code[i]);

	printf("\n");
	printf("************************************************\n");

	

	//printf("Calculating SECDED Hamming code\n");

//Calculating the extra parity bit, hamming code bit 13.. Even parity
	for(i=0;i<=12;i++)
		hamming_code[13]= hamming_code[13]^hamming_code[i];

	printf("\n");
	printf("************************************************\n");
	printf("SECDED Even parity Hamming code:\n");
	for(i=1;i<=13;++i)
		printf("%d ",hamming_code[i]);

	printf("\n");
	printf("************************************************\n");

	convert_binary_to_decimal(hamming_code, &hamming_code_decimal);
	printf("Hamming code in decimal is %d, in hex is %x\n",hamming_code_decimal,hamming_code_decimal);

	printf("Enter the received encoded Hamming coded data in decimal (not binary):\n ");
	scanf("%d", &decimal_received);
	
	convert_decimal_to_binary(decimal_received,binary_received, 13);
	

	printf("\nIn binary, the data received is:\n");
	for(i=1; i<=13; i++)
	    printf("%d ",binary_received[i]);
	
	printf("\n");

	detect_error(binary_received, &bit_in_error);
//	decode_received_data(binary_received,decoded_data_binary);


    return 0;
}


int check_if_power_of_two (int num, int *power_of_two)
{
	
	int i=0, expo_result=0;
		
	for(i=1;i<5;i++)
	{
		expo_result = pow(2,i);		
		if(num >= expo_result )
		{
		//printf("Checking if number %d is divisible by %d\n", num, expo_result);
 			
		if(num % expo_result == 0)	
				{
					*power_of_two = 1;
					//printf("Number %d is divisible by %d and hence is a parity position\n", num, expo_result);
				}

			else
				{
					*power_of_two = 0;
					//printf("Number %d is NOT divisible by %d and hence is a data position, not parity\n", num, expo_result);
				}

		}
	}

/*if (*power_of_two ==1)
	printf("Parity bit: Position %d\n", num);

else
	printf("Data bit: Position %d \n", num);*/

return 0;

}


int convert_decimal_to_binary(int decimal_input, int binary_input[], int num_of_bits)
{
	int i=0;
	
	for(i=num_of_bits;i>0;i--)
	{
		binary_input[i]= decimal_input % 2 ;
		//printf("binary_input[%d]=%d\n",i,binary_input[i]);
	
		decimal_input= decimal_input/2;
		//printf("Decimal input after dividing by 2 is %d\n",decimal_input);
				
	
	}


return 0;
}



int	convert_binary_to_decimal(int hamming_code[], int *hamming_code_decimal)
{
	int i=0,j=0;
	
	for(i=13;i>0;i--)
	{
		*hamming_code_decimal= *hamming_code_decimal+ (hamming_code[i] * pow(2,j));
		j++;
	}


return 0;
}



int calculate_parity_bits(int hamming_code[], int parity[])
{

	int p1=0, p2=0, p4=0, p8=0, i=0;
	//Calculating parity bits
	p1= hamming_code[1]^hamming_code[3]^hamming_code[5]^hamming_code[7]^hamming_code[9]^hamming_code[11];
	p2= hamming_code[2]^hamming_code[3]^hamming_code[6]^hamming_code[7]^hamming_code[10]^hamming_code[11];
	p4= hamming_code[4]^hamming_code[5]^hamming_code[6]^hamming_code[7]^hamming_code[12];
	p8= hamming_code[8]^hamming_code[9]^hamming_code[10]^hamming_code[11]^hamming_code[12];

	parity[1]= p1;
	parity[2]= p2;
	parity[3]= p4;
	parity[4]= p8;

		printf("\nParity bits are:\n");
		for(i=1;i<=4;i++)
			printf("parity[%d]=%d\n", i, parity[i]);

		printf("\n\n");

return 0;
}


int detect_error(int binary_received[], int *bit_in_error)
{

	int p1_received=0, p2_received=0, p4_received=0, p8_received=0, parity_extra=0, parity_received[5]={0};
	int i=0;

	p1_received= binary_received[1];
	p2_received= binary_received[2];
	p4_received= binary_received[4];
	p8_received= binary_received[8];
	parity_extra= binary_received[13];

//clear these parity bits now and calculate afresh to see if they were correct

	binary_received[1] = 0;
	binary_received[2] = 0;
	binary_received[4] = 0;
	binary_received[8] = 0;
	binary_received[13] = 0;


	printf("\nIn binary, the data received, with parity bits set to 0 is:\n");
	for(i=1; i<=13; i++)
	    printf("%d ",binary_received[i]);
	
	printf("\n");

	//Calculating parity bits of received data

	calculate_parity_bits(binary_received, parity_received);
	
	


return 0;
}



/*int decode_received_data(int binary_received[],int decoded_data_binary[]);
{



return 0;
}


*/






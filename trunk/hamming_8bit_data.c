/* Hamming code generation and error detection, correction*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


int check_if_power_of_two (int, int *);
 
int main()
{
    int i=0, j=0, p1=0, p2=0, p4=0, p8=0;
	int data[9]={0}; //8-bit data
	int hamming_code[14]={0}, parity[5]= {0}; //12-bit hamming code
	int power_of_two=0;
   
    printf("\nHamming code----- Encoding\n");
    printf("Enter 8 bit data : ");

	for(i=1; i<=8; i++)
	    scanf("%d",&data[i]);

	printf("Data read is:\n");
	for(i=1; i<=8; i++)
	    printf("%d",data[i]);

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
			hamming_code[i]=data[j];
			printf("hamming_code[%d]= data[%d]= %d\n\n",i,j,hamming_code[i]);

			j++;
			}
		//else printf("Calculating parity bits..\n");
			
	}

	printf("\nHamming code template, before adding parity bits\n");
	for(i=1;i<=12;++i)
		printf("%d ",hamming_code[i]);
	printf("\n");

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

	printf("\n******************************\n");
	printf("Data that is being encoded is:\n");
	for(i=1; i<=8; i++)
	    printf("%d ",data[i]);
	printf("\n");
	printf("******************************\n");


	printf("\n***************************************\n");
	printf("Parity bits are:\n");
	for(i=1;i<=4;i++)
		printf("%d ", parity[i]);

	printf("\n");
	printf("***************************************\n");


	printf("\n***************************************\n");
	printf("Single error correcting Even parity Hamming code:\n");
	for(i=1;i<=12;++i)
		printf("%d ",hamming_code[i]);

	printf("\n");
	printf("***************************************\n");


	//printf("Calculating SECDED Hamming code\n");

	for(i=0;i<=12;i++)
		hamming_code[13]= hamming_code[13]^hamming_code[i];


	printf("\n***************************************\n");
	printf("SECDED Even parity Hamming code:\n");
	for(i=1;i<=13;++i)
		printf("%d ",hamming_code[i]);

	printf("\n");
	printf("***************************************\n");



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


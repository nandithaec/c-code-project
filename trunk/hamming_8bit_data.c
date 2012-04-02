/* Hamming code generation and error detection, correction*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


int check_if_power_of_two (int, int *);
 
int main()
{
    int i=0,j=0;
	int data[9]={0}; //8-bit data
	int hamming_code[13]={0}; //12-bit hamming code
	int power_of_two=0;
   
    printf("\nHamming code----- Encoding\n");
    printf("Enter 8 bit data : ");

	for(i=1;i<=8;i++)
	    scanf("%d",&data[i]);

	printf("Data read is:\n");
	for(i=1;i<=8;i++)
	    printf("%d",data[i]);

printf("\n \n");


	for(i=1;i<=12;i++)
	{
			
		if(i==1)
			printf("Parity bit: Position %d\n", i);
		else
		if(i !=1)
			check_if_power_of_two(i, &power_of_two);

		if( (i != 1 ) || power_of_two == 0 )
			{
			hamming_code[i]=data[j];

			j++;
			}
		else hamming_code[i]=9;
	}
			

	printf("\nHamming code template:\n");
	for(i=1;i<=12;i++)
		printf("%d",hamming_code[i]);

printf("\n");


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

if (*power_of_two ==1)
	printf("Parity bit: Position %d\n", num);

else
	printf("Data bit: Position %d \n", num);

return 0;

}


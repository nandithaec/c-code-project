/* Hamming code generation and error detection, correction*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


int check_if_power_of_two (int, int *);
 
int main()
{
    int i=0,j=0;
	int number=0; //8-bit data
	int hamming_code[13]={0}; //12-bit hamming code
	int power_of_two=0;
   
    printf("Enter number\n ");

    scanf("%d",&number);
	
		
		if(number !=1)
			check_if_power_of_two(number, &power_of_two);	
		else if (number == 1)
			printf("Parity bit: Position %d \n",number);
	

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
	

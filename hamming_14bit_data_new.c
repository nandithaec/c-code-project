/* Hamming code generation and error detection, correction*/

/*Throughout the program, bit positions are assigned from the left to right
For eg., if data is 1 0 0 1 1 0 1 0
positions are assigned starting from left, not from the right as is the general convention.
i.e., as, bit 1, bit 2 and so on.. so bit 1=1, bit 2=0, bit3=0 and so on till bit 8=0. 
Array index is also starting from 1 and not 0, to avoid confusion.
 
-Nanditha April 3rd 2012 */

#define DEBUG
#ifdef DEBUG
#define PRINT printf
#else
 #define PRINT print_null
#endif
void print_null (char* n,...) {return;}

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int hamming_encoding(int);
int error_detect_correct_decode(int, FILE *);
int check_if_power_of_two (int, int*);
int convert_decimal_to_binary(int, int[],int);
int	convert_binary_to_decimal(int[], int);
int calculate_parity_bits(int[], int[]);
int detect_error(int[], int*, int *, FILE *);
int decode_received_data(int[],int[]);
int flip_bit_for_correction(int*,int[],int*); 
int only_decode(int);


int main()
{

int hamming_code_decimal=0,decoded_data_decimal=0,decimal_input=0;
int decimal_received=0; 


FILE *fnew;

fnew = fopen( "hamming_testing.txt", "w" );
if( fnew != NULL )
   fprintf( fnew, "Hello\n" );

printf("\nHamming code----- Encoding\n");
    printf("Enter the number to be encoded, in decimal (not binary) :\n ");
	scanf("%d", &decimal_input);

hamming_code_decimal = hamming_encoding(decimal_input); //hamming_code is the binary array

printf("main()..Hamming code in decimal is %d, in hex is %x\n",hamming_code_decimal,hamming_code_decimal);


printf("Enter the received encoded Hamming coded data in decimal (not binary), to be detected for error and decoded:\n ");
	scanf("%d", &decimal_received);

decoded_data_decimal= only_decode( decimal_received);

decoded_data_decimal= error_detect_correct_decode( decimal_received, fnew);

fclose(fnew);
return 0;
}


int hamming_encoding(int decimal_input)
{
    int i=0, j=0, p1=0, p2=0, p4=0, p8=0, p16=0;
	int binary_input[16]={0}; //14-bit data
	int	hamming_code_decimal=0;
	int parity[8]= {0};
	int power_of_two=0;
    int hamming_code[21]={0};
    
	
	PRINT("\nData has been read and is %d (decimal), %x (in hex) \n", decimal_input, decimal_input);
	
	convert_decimal_to_binary(decimal_input,binary_input,14); //14 is the number of bits in the binary

	//for(i=1; i<=8; i++)
	   // scanf("%d",&binary_input[i]);


/*Throughout the program, bit positions are assigned from the left to right
For eg., if data is 1 0 0 1 1 0 1 0
positions are assigned starting from left, not from the right as is the general convention.
i.e., as, bit 1, bit 2 and so on.. so bit 1=1, bit 2=0, bit3=0 and so on till bit 8=0. 
Array index is also starting from 1 and not 0, to avoid confusion */

	PRINT("\nIn binary, the data to be encoded is:\n");
	for(i=1; i<=14; i++)
	    PRINT("%d",binary_input[i]);

PRINT("\n \n");


//Encode data from 8 bit to 12 bits

j=1;

	for(i=1; i<=19; i++)
	{
			
		if(i==1)
			{
				PRINT("Parity bit: Position %d\n", i);
				//hamming_code[i]=9;
				//PRINT("hamming_code[%d]= %d\n\n",i,hamming_code[i]);
			}
		else
		if(i !=1)
			check_if_power_of_two(i, &power_of_two);

		if( (i != 1 ) && power_of_two == 0 )
			{
			hamming_code[i]=binary_input[j];
			PRINT("hamming_code[%d]= binary_input[%d]= %d\n\n",i,j,hamming_code[i]);

			j++;
			}
		//else PRINT("Calculating parity bits..\n");
			
	}

	PRINT("\nHamming code template, with all parity bits set to zero is:\n");
	for(i=1;i<=19;++i)
		PRINT("%d ",hamming_code[i]);
	PRINT("\n");


	calculate_parity_bits(hamming_code,parity);


	//Concatenating the parity bits to the hamming code. Data bits have already been added previously.
	PRINT("Concatenating parity bits\n");

	j=1;
	for(i=1;i<=19;i++)
	{
			if( i == 1 )
				{
				PRINT("parity[%d] = %d\n",j, parity[j]);
				hamming_code[i]= parity[j];
				PRINT("hamming_code[%d]= parity[%d]= %d\n\n",i,j,hamming_code[i]);
				j++;
				}

			if(i !=1)
				check_if_power_of_two(i, &power_of_two);

			if(  power_of_two == 1 )
				{
				PRINT("parity[%d] = %d\n",j, parity[j]);
				hamming_code[i]= parity[j];
				PRINT("hamming_code[%d]= parity[%d]= %d\n\n",i,j,hamming_code[i]);
				j++;
				}
	}

	PRINT("\n");
	PRINT("************************************************\n");
	PRINT("binary_input that is being encoded is:\n");
	for(i=1; i<=14; i++)
	    PRINT("%d ",binary_input[i]);
	PRINT("\n");
	PRINT("************************************************\n");

	PRINT("\n");
	PRINT("************************************************\n");
	PRINT("Parity bits are:\n");
	for(i=1;i<=5;i++)
		PRINT("%d ", parity[i]);

	PRINT("\n");
	PRINT("************************************************\n");

	PRINT("\n");
	PRINT("************************************************\n");
	PRINT("Single error correcting Even parity Hamming code:\n");
	for(i=1;i<=19;++i)
		PRINT("%d ",hamming_code[i]);

	PRINT("\n");
	PRINT("************************************************\n");

	

	//PRINT("Calculating SECDED Hamming code\n");

//Calculating the extra parity bit, hamming code bit 13.. Even parity.. even number of ones
	for(i=1;i<=19;i++)
	{	
		hamming_code[20]= hamming_code[20]^hamming_code[i];
	}

	parity[6]=hamming_code[20]; //save it in parity array as well
	
	PRINT("\n");
	PRINT("************************************************\n");
	PRINT("SECDED Even parity Hamming code:\n");
	for(i=1;i<=20;++i)
		PRINT("%d ",hamming_code[i]);

	PRINT("\n");
	PRINT("************************************************\n");

	hamming_code_decimal= convert_binary_to_decimal(hamming_code,20);
	
	PRINT("Hamming code in decimal is %d, in hex is %x\n",hamming_code_decimal,hamming_code_decimal);


return hamming_code_decimal;

}

int error_detect_correct_decode(int decimal_received, FILE *fnew)
{

PRINT("****Invoking error_detect_correct_decode()****\n");
int  binary_received[21]={0},i=0,binary_corrected[21]={0};
int  binary_received_original[21]={0}, bit_in_error=0, decoded_data_binary[16]={0}, double_error=0 ;
int decoded_data_decimal= 0;
//Error detection
	PRINT("\nEncoded data received is:%d (dec), %x (hex),\n",decimal_received,decimal_received);
	
	convert_decimal_to_binary(decimal_received,binary_received, 20);
	
//Save it in an array
	PRINT("\nIn binary, the data received is:\n");
	for(i=1; i<=20; i++)
    {
	 PRINT("%d ",binary_received[i]);
	 binary_received_original[i]=binary_received[i];
	}
	
	PRINT("\n");

	detect_error(binary_received, &bit_in_error, &double_error, fnew);

//Error correction for single errors
	if (bit_in_error !=0 &&  // If bit_in_error=1, that means there was an error
		 double_error== 0) 		//double_error=0 means it is a single error and can be corrected
    {
	flip_bit_for_correction(&decimal_received,binary_received_original,&bit_in_error);
	
    }

	else if(double_error== 1) 
	{
		double_error=0; //pass this to the structure in .h
		PRINT("DOUBLE ERROR\n");
	}

	PRINT("Decimal corrected(if there was a single error) is: %x\n",decimal_received);

	convert_decimal_to_binary(decimal_received,binary_corrected,20);

	PRINT("\nIn binary, the Corrected encoded data(If there was a single error) is:\n");
	for(i=1; i<=20; i++)
	    PRINT("%d ",binary_corrected[i]);

	PRINT("\n");

	

	decode_received_data(binary_corrected,decoded_data_binary);

	PRINT("\nIn binary, the decoded data is:\n");
	for(i=1; i<=14; i++)
	    PRINT("%d ",decoded_data_binary[i]);

	PRINT("\n");

	decoded_data_decimal= convert_binary_to_decimal(decoded_data_binary,14);

	PRINT("Decoded data in decimal is: %d, in hex is: %x\n***Ending error_detect_correct_decode()***\n\n",decoded_data_decimal,decoded_data_decimal);

    return decoded_data_decimal;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int only_decode(int decimal_received)
{

PRINT("****Invoking only_decode()****\n");
int  binary_received[21]={0},i=0,binary_corrected[21]={0};
int  binary_received_original[21]={0}, bit_in_error=0, decoded_data_binary[16]={0}, double_error=0 ;
int decoded_data_decimal= 0;

	PRINT("\nEncoded data received is:%d (dec), %x (hex),\n",decimal_received,decimal_received);
	
	convert_decimal_to_binary(decimal_received,binary_received, 20);
	
//Save it in an array
	PRINT("\nIn binary, the data received is:\n");
	for(i=1; i<=20; i++)
    {
	 PRINT("%d ",binary_received[i]);
	 binary_received_original[i]=binary_received[i];
	}
	
	PRINT("\n");


	decode_received_data(binary_received,decoded_data_binary);

	PRINT("\nIn binary, the decoded data is:\n");
	for(i=1; i<=14; i++)
	    PRINT("%d ",decoded_data_binary[i]);

	PRINT("\n");

	decoded_data_decimal= convert_binary_to_decimal(decoded_data_binary,14);

	PRINT("Decoded data in decimal is: %d, in hex is: %x\n***Ending only_decode()***\n\n",decoded_data_decimal,decoded_data_decimal);

    return decoded_data_decimal;
}


//*****************Ending critical functions***********************

int check_if_power_of_two (int num, int *power_of_two)
{
	
	int i=0, expo_result=0;
		
	for(i=1;i<6;i++)
	{
		expo_result = pow(2,i);		
		if(num >= expo_result )
		{
		//PRINT("Checking if number %d is divisible by %d\n", num, expo_result);
 			
		if(num % expo_result == 0)	
				{
					*power_of_two = 1;
					//PRINT("Number %d is divisible by %d and hence is a parity position\n", num, expo_result);
				}

			else
				{
					*power_of_two = 0;
					//PRINT("Number %d is NOT divisible by %d and hence is a data position, not parity\n", num, expo_result);
				}

		}
	}

/*if (*power_of_two ==1)
	PRINT("Parity bit: Position %d\n", num);

else
	PRINT("Data bit: Position %d \n", num);*/

return 0;

}


int convert_decimal_to_binary(int decimal_input, int binary_input[], int num_of_bits)
{
	int i=0;
	
	for(i=num_of_bits;i>0;i--)
	{
		binary_input[i]= decimal_input % 2 ;
		//PRINT("binary_input[%d]=%d\n",i,binary_input[i]);
	
		decimal_input= decimal_input/2;
		//PRINT("Decimal input after dividing by 2 is %d\n",decimal_input);
				
	
	}


return 0;
}



int	convert_binary_to_decimal(int hamming_code[],int num_of_bits)
{
	int i=0,j=0, decimal=0;
	
	for(i=num_of_bits;i>0;i--)
	{
		decimal= decimal+ (hamming_code[i] * pow(2,j));
		j++;
	}


return decimal;
}



int calculate_parity_bits(int hamming_code[], int parity[])
{

	int p1=0, p2=0, p4=0, p8=0, p16 = 0, i=0;
	//Calculating parity bits
	p1= hamming_code[1]^hamming_code[3]^hamming_code[5]^hamming_code[7]^hamming_code[9]^hamming_code[11]^hamming_code[13]^hamming_code[15]^hamming_code[17]^hamming_code[19];
	p2= hamming_code[2]^hamming_code[3]^hamming_code[6]^hamming_code[7]^hamming_code[10]^hamming_code[11]^hamming_code[14]^hamming_code[15]^hamming_code[18]^hamming_code[19];
	p4= hamming_code[4]^hamming_code[5]^hamming_code[6]^hamming_code[7]^hamming_code[12]^hamming_code[13]^hamming_code[14]^hamming_code[15];
	p8= hamming_code[8]^hamming_code[9]^hamming_code[10]^hamming_code[11]^hamming_code[12]^hamming_code[13]^hamming_code[14]^hamming_code[15];
	p16 = hamming_code[16]^hamming_code[17]^hamming_code[18]^hamming_code[19];

	parity[1]= p1;
	parity[2]= p2;
	parity[3]= p4;
	parity[4]= p8;
	parity[5] = p16;

		PRINT("\nParity bits are:\n");
		for(i=1;i<=5;i++)
			PRINT("parity[%d]=%d\n", i, parity[i]);

		PRINT("\n\n");

return 0;
}


int detect_error(int binary_received[], int *bit_in_error, int *double_error, FILE *fnew)
{

	int p1_received=0, p2_received=0, p4_received=0, p8_received=0,p16_received=0, parity_extra_received=0, parity_received[6]={0}, parity_calculated_from_Rx[6]={0};
	int parity_extra_calculated_from_Rx=0;
	int i=0;
	int position=0, possible_double_error=0, double_error_check=0;
	
	parity_received[1]= binary_received[1];
	parity_received[2]= binary_received[2];
	parity_received[3]= binary_received[4];
	parity_received[4]= binary_received[8];
	parity_received[5]= binary_received[16];
	parity_extra_received= binary_received[20]; //to detect double errors

	for(i=1;i<=19;i++)
	{	
		parity_extra_calculated_from_Rx = parity_extra_calculated_from_Rx ^ binary_received[i];
	}


//clear these parity bits now and calculate afresh to see if they were correct

	binary_received[1] = 0;
	binary_received[2] = 0;
	binary_received[4] = 0;
	binary_received[8] = 0;
	binary_received[16] = 0;
	binary_received[20] = 0;


	PRINT("\nIn binary, the data received, with parity bits set to 0 is:\n");
	for(i=1; i<=20; i++)
	    PRINT("%d ",binary_received[i]);
	
	PRINT("\n");

	//Calculating parity bits of received data

	calculate_parity_bits(binary_received, parity_calculated_from_Rx);


		for(i=1;i<=5;i++)
		{
			if(parity_calculated_from_Rx[i] != parity_received[i])
			{
				PRINT("Parity[%d] is in error\n",i);
			
				switch (i)
		   		{
				    case 1:
					position=1;
					PRINT("Position %d is in error\n",position);
				    break;

					case 2:
					position=2;
					PRINT("Position %d is in error\n",position);
				    break;

					case 3:
					position=4;
					PRINT("Position %d is in error\n",position);
				    break;

					case 4:
					position=8;
					PRINT("Position %d is in error\n",position);
				    break;
				
					case 5:
					position=16;
					PRINT("Position %d is in error\n",position);
				    break;

					default: PRINT("Error in parity index\n");
					break;
				} //close switch

			*bit_in_error=*bit_in_error + position;

			}//close if parity
		} //close for



	if (*bit_in_error == 0 && parity_extra_received == parity_extra_calculated_from_Rx) //no error in parity_extra too
	{
		PRINT("No error in the parity bits, received data is correct\n");
		//fprintf(fnew,"No error in the parity bits, received data is correct\n");
		double_error=0;
	}
	else if (*bit_in_error == 0 && parity_extra_received != parity_extra_calculated_from_Rx) //error in parity_extra only
	{
		PRINT("Error in the extra parity bit only, received data is correct\n");
		fprintf(fnew,"Error in the extra parity bit only, received data is correct\n");
		double_error=0;
	}

	else if (*bit_in_error != 0 ) //There is an error in one of the data or parity bits other than the extra parity bit
	{
	 //There is an error because one of the parity bits is incorrect.. find out if single/double error
		if(parity_extra_received != parity_extra_calculated_from_Rx) //Single error
		{
			printf("SINGLE ERROR, can be corrected\n");
			fprintf(fnew,"SINGLE ERROR, can be corrected\n");
			*double_error=0;

			if (*bit_in_error == 1 || *bit_in_error == 2 || *bit_in_error == 4 || *bit_in_error == 8|| *bit_in_error == 16) 
			{
				printf("Bit in error is %d and is a parity bit. ****DATA BIT NOT IN ERROR****\n", *bit_in_error);
				fprintf(fnew,"Bit in error is %d and is a parity bit. ****DATA BIT NOT IN ERROR****\n", *bit_in_error);
			}
			else
			{
				printf("Bit in error is %d and is a data bit. Needs error correction.\n", *bit_in_error);
				fprintf(fnew,"Bit in error is %d and is a data bit. Needs error correction.\n", *bit_in_error);
			}		
		} 
		
			
		else if(parity_extra_received == parity_extra_calculated_from_Rx) //2 bits have flipped, hence parity_extra turns out to be same as original (even parity)
		{
			possible_double_error=1;
			
		}
	}

	if(possible_double_error==1)
	{
		possible_double_error==0; //reset it
		
		for(i=1;i<=5;i++)
		{	
		double_error_check = double_error_check ^ parity_received[i];
		}


		if (*bit_in_error == 1 || *bit_in_error == 2 || *bit_in_error == 4 || *bit_in_error == 8|| *bit_in_error == 16) 
		{ //if one of the bits flipped is a parity bit			
			if(parity_extra_received != double_error_check)	//One parity bit and one data are in error..
			{			
				printf("DOUBLE ERROR (one parity and one data), CANNOT CORRECT\n");
				fprintf(fnew,"DOUBLE ERROR (one parity and one data), CANNOT CORRECT\n");
				*double_error=1;
			}
		
			else if(parity_extra_received == double_error_check)	//Two parity bits are in error..
			{
				PRINT("DOUBLE ERROR: CANNOT CORRECT\n");
				fprintf(fnew,"DOUBLE ERROR:  CANNOT CORRECT\n");
				*double_error=1;
			}

		}
		else // if none of the parity bits have flipped,and since the parity_extra_received == parity_extra_calculated_from_Rx
		{
			printf("DOUBLE DATA ERROR .. CANNOT CORRECT\n");
			fprintf(fnew,"DOUBLE DATA ERROR .. CANNOT CORRECT\n");
			*double_error=1;
		}

	}

/*Bit error is also assigned from the left to right
For eg., if data is 1 0 0 1 1 0 1 0
positions are assigned starting from left, not from the right as is the general convention.
i.e., if bit 1 is in error, it means that the leftmost bit is in error*/	

return 0;
}


int flip_bit_for_correction(int *decimal_received,int binary_received_or[], int *bit_in_error)
{
int i=0;
PRINT("\nCorrecting the received data:\n");

	PRINT("\nIn binary, the erroneous data received was:\n");
	for(i=1; i<=20; i++)
	    PRINT("%d ",binary_received_or[i]);

	PRINT("\n");

/*Bit error is also assigned from the left to right
For eg., if data is 1 0 0 1 1 0 1 0
positions are assigned starting from left, not from the right as is the general convention.
i.e., if bit 1 is in error, it means that the leftmost bit is in error*/

	PRINT("Erroneous Data in decimal: %d, in hex: %x \n\n",*decimal_received,*decimal_received);

 switch(*bit_in_error)
	{
			case 20:
		    *decimal_received=  *decimal_received ^(1 << 0);
		    break;

			case 19:
		    *decimal_received=  *decimal_received ^(1 << 1);
		    break;

		    case 18:
		    *decimal_received=  *decimal_received ^(1 << 2);
		    break;

		    case 17:
		    *decimal_received=  *decimal_received ^(1 << 3);
		    break;

		    case 16:
		    *decimal_received=  *decimal_received ^(1 << 4);
		    break;

		    case 15:
		    *decimal_received=  *decimal_received ^(1 << 5);
		    break;

		    case 14:
		    *decimal_received=  *decimal_received ^(1 << 6);
		    break;

		    	    
			case 13:
		    *decimal_received=  *decimal_received ^(1 << 7);
		    break;

			case 12:
		    *decimal_received=  *decimal_received ^(1 << 8);
		    break;

		    case 11:
		    *decimal_received=  *decimal_received ^(1 << 9);
		    break;

		    case 10:
		    *decimal_received=  *decimal_received ^(1 << 10);
		    break;

		    case 9:
		    *decimal_received=  *decimal_received ^(1 << 11);
		    break;

		    case 8:
		    *decimal_received=  *decimal_received ^(1 << 12);
		    break;

		    case 7:
		    *decimal_received=  *decimal_received ^(1 << 13);
		    break;

		    case 6:
		    *decimal_received=  *decimal_received ^(1 << 14);
		    break;

		    case 5:
		    *decimal_received=  *decimal_received ^(1 << 15);
		    break;

   		    case 4:
		    *decimal_received=  *decimal_received ^(1 << 16);
		    break;
		   
			case 3:
		    *decimal_received=  *decimal_received ^(1 << 17);
		    break;
			
			case 2:
		    *decimal_received=  *decimal_received ^(1 << 18);
		    break;

	        case 1:
		    *decimal_received=  *decimal_received ^(1 << 19);
		    break;

		   	default: 
			PRINT("No error, nothing to correct\n");
			//fprintf(fnew,"Invalid bit flip case\n");
		    break;
	}

	if (*bit_in_error !=0)
        printf("Bit %d corrected, Corrected encoded data is %d (in decimal), %x (in hex)\n", *bit_in_error, *decimal_received, *decimal_received);


return 0;
}

int decode_received_data(int binary_received[],int decoded_data_binary[])
{

int i=0,j=0,pow_of_two=0;

j=1;

	for(i=1; i<=19; i++)
	{
			
		if(i==1)
			{
				PRINT("Parity bit: Position %d\n", i);
				//hamming_code[i]=9;
				//PRINT("hamming_code[%d]= %d\n\n",i,hamming_code[i]);
			}
		else
		if(i !=1)
			check_if_power_of_two(i, &pow_of_two);

		if( (i != 1 ) && pow_of_two == 0 )
			{
			decoded_data_binary[j]=binary_received[i];
			//PRINT("decoded_data[%d]=  %d\n\n",j,decoded_data_binary[j]);

			j++;
			}
		//else PRINT("Calculating parity bits..\n");
			
	}


return 0;
}








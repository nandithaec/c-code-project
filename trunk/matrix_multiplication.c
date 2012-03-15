# include "stdio.h"
#define MATRIX_SIZE 3

main()
{
    int m1[10][10],i,j,k,m2[10][10],mult[10][10],r1,c1,r2,c2;
   // printf("Enter number of rows and columns of first matrix (less than 10)\n");
   //scanf("%d%d",&r1,&c1);
   //printf("Enter number of rows and columns of second matrix (less than 10)\n");
   //scanf("%d%d",&r2,&c2);

	//Square matrix
	r1= MATRIX_SIZE;
	c1= MATRIX_SIZE;
	r2= MATRIX_SIZE;
	c2= MATRIX_SIZE;	

    if(r2==c1)
    {
        for(i=0;i<r1;i++)
            for(j=0;j<c1;j++)
               m1[i][j]= (j + 1) * (i + j + 1); //Some random number
        printf("First Matrix is :\n");
        for(i=0;i<r1;i++)
        {
            for(j=0;j<c1;j++)
                printf("%d\t",m1[i][j]);
            printf("\n");
        }

        for(i=0;i<r2;i++)
            for(j=0;j<c2;j++)
                m2[i][j] = (i + 2) * (i + j + 1); //Some random number
        printf("Second Matrix is:\n");
        for(i=0;i<r2;i++)
        {
            for(j=0;j<c2;j++)
                printf("%d\t",m2[i][j]);
            printf("\n");
        }
        printf("Multiplication of the Matrices:\n");
        for(i=0;i<r1;i++)
        {
            for(j=0;j<c2;j++)
            {
                mult[i][j]=0;
                for(k=0;k<r1;k++)
                    mult[i][j]+=m1[i][k]*m2[k][j];
                printf("%d\t",mult[i][j]);
            }
            printf("\n");
        }
    }
    else
    {
        printf("Matrix multiplication cannot be done");
    }
    return 0;
}

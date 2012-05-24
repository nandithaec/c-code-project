/* Bubble sort code */
 
#include <stdio.h>
 
int main()
{
  int array[10], n, c, d, swap;
 
 // printf("Enter number of elements\n");
 // scanf("%d", &n);
 
 // printf("Enter %d integers\n", n);
 
 // for (c = 0; c < n; c++)
 //   scanf("%d", &array[c]);
	array[0]=34;
	array[0]=3;
	array[0]=5;
	array[0]=47;
	array[0]=9;
	

for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] > array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
 
 // printf("Sorted list in ascending order:\n");
 
 // for ( c = 0 ; c < n ; c++ )
 //    printf("%d\n", array[c]);
 
  return 0;
}

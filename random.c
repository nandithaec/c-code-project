/* rand example: guess the number */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
  int iSecret, iGuess;

  /* initialize random seed: */
  srand ( time(NULL) );

  /* generate secret number: */
  iSecret = rand() % 8 ;

  printf("Secret number is %x\n", iSecret);

  iSecret= iSecret ^(1 << 2);
  printf("Secret number after 2nd bit flip is %x\n", iSecret);

  return 0;
}

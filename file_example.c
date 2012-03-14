//C - Reading from a Text File



/*
** File FILE_3.C
**
** Illustrates how to read from a file.
**
** The file is opened for reading.  Each line is successively fetched
** using fgets command.  The string is then converted to a long integer.
**
** Note that fgets returns NULL when there are no more lines in the file.
**
** In this example file ELAPSED.DTA consists of various elapsed times in
** seconds.  This may have been the result of logging the time  of events
** using an elapsed time counter which increments each second from the
** time the data logger was placed in service.
**
** Typical data in elapsed.dta might be;
**
** 65
** 142
** 1045
** 60493
** 124567
**
**
** Peter H. Anderson, 4 April, '97
*/

#include <stdio.h>   /* required for file operations */
//#include <conio.h>  /* for clrscr */
//#include <dos.h>  /* for delay */

FILE *fr;            /* declare the file pointer */

main()

{
   int n=0;
   int elapsed_seconds;
	int store[20];
	int i=0;
   char line[80];
//int x;
//	line[i] = malloc(80);

   fr = fopen ("pic_instr.c", "rt");  /* open the file for reading */
  /* "rt" means open the file for reading text */
if( fr == NULL )
         {
               puts ( "cannot open file" ) ;
               exit(0) ;
         }	
   while(fgets(line, 80, fr) != NULL)
   {
	 /* get a line, up to 80 chars from fr.  done if NULL */
	 sscanf (line, "%x", &elapsed_seconds);
	// printf ("%d\n", elapsed_seconds);
	//printf("-------\n");
	store[n]= elapsed_seconds;
	n++;
	//line[i] = malloc(80);
	 /* convert the string to a long int */
	

   }
	 
	for(i=0;i<n;i++)
		printf("words= %x\n", store[i]);

   fclose(fr);  /* close the file prior to exiting the routine */
} /*of main*/




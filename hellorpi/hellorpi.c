/*
 * hellorpi.c
 */
#include <unistd.h>
#include <fcntl.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 

#include "hellorpi.h"
  
/****************************************************************
 * Our main function with a little more than "Hello world!" only
 ****************************************************************/
 int main(void)
{ 
	int i;
	for(i=0;i<10;i++)
	{
		printf("Hello world! %d\n",i);

		if(i==5)
		{
			printf("FIVEx\n");
		}
		usleep(500000);
	}

	printf("calc = %d\n",calcsum(1,2));
 
    return 0; 
}
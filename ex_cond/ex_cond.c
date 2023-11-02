/*
 * ex_cond.c
 *
 *  Created on: Sep 1, 2014
 *      Author: gabor
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SYNC

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();
int  count = 0;
#define COUNT_DONE  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6


int main()
{
   pthread_t thread1, thread2;

   pthread_create( &thread1, NULL, &functionCount1, NULL);
   pthread_create( &thread2, NULL, &functionCount2, NULL);
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   exit(0);
}

void *functionCount1()
{
   for(;;)
   {
#ifdef SYNC
	  pthread_mutex_lock( &condition_mutex );
      while( count >= COUNT_HALT1 && count <= COUNT_HALT2 )
      {
         pthread_cond_wait( &condition_cond, &condition_mutex );
      }
      pthread_mutex_unlock( &condition_mutex );

      pthread_mutex_lock( &count_mutex );
#endif
      count++;
      printf("Counter value functionCount1: %d\n",count);
#ifdef SYNC
      pthread_mutex_unlock( &count_mutex );
#endif
      if(count >= COUNT_DONE) return(NULL);
    }
   return NULL;
}

void *functionCount2()
{
    for(;;)
    {
#ifdef SYNC
       pthread_mutex_lock( &condition_mutex );
       if( count < COUNT_HALT1 || count > COUNT_HALT2 )
       {
          pthread_cond_signal( &condition_cond );
       }
       pthread_mutex_unlock( &condition_mutex );

       pthread_mutex_lock( &count_mutex );
#endif
       count++;
       printf("Counter value functionCount2: %d\n",count);
#ifdef SYNC
       pthread_mutex_unlock( &count_mutex );
#endif
       if(count >= COUNT_DONE) return(NULL);
    }
    return NULL;
}

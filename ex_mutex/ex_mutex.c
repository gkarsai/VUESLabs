/*
 * Mutex demo 
 *
 * Copyright (C) 2002 Paolo Gai
 * Copyright (C) 2007 Luca Abeni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


static pthread_mutex_t mymutex;
static int th_cnt;

static void *body(void *arg)
{
  int j;
  
  fprintf(stderr, "Thread %d: %s\n", th_cnt++, (char *)arg); 
  pthread_mutex_lock(&mymutex);
  for (j=0; j<20; j++) {
    usleep(500000);
    fprintf(stderr,(char *)arg);
  }
  pthread_mutex_unlock(&mymutex);

  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t t1, t2, t3;
  int err;

  pthread_mutexattr_t mymutexattr;

  pthread_mutexattr_init(&mymutexattr);
  pthread_mutex_init(&mymutex, &mymutexattr);
  pthread_mutexattr_destroy(&mymutexattr);

  err = pthread_create(&t1, NULL, body, (void *)".");
  err = pthread_create(&t2, NULL, body, (void *)"#");
  err = pthread_create(&t3, NULL, body, (void *)"o");

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  printf("\n");

  return 0;
}

/*
 * pthread_create demo 
 *
 * Copyright (C) 2002 by Paolo Gai
 * Copyright (C) 2007 by Luca Abeni
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


#include <stdio.h>
#include <pthread.h>

static pthread_t main_id;
static pthread_t second_id;

static void *body(void *arg)
{
  int p = *(int *)arg;
  pthread_t mythread_id;

  printf("mythread: parameter=%d\n", p);

  mythread_id = pthread_self();

  printf("mythread: main_id==mythread_id:%d\n", 
	 pthread_equal(main_id, mythread_id) );
  printf("mythread: second_id==mythread_id:%d\n", 
	 pthread_equal(second_id, mythread_id) );

  return (void *)5678;
}

int main(int argc, char *argv[])
{
  pthread_attr_t myattr;
  int err;
  int parameter;
  void *returnvalue;

  parameter = 1234;

  main_id = pthread_self();
  puts("main: before pthread_create\n");

  /* initializes the thread attribute */
  pthread_attr_init(&myattr);
  /* creation and activation of the new thread */
  err = pthread_create(&second_id, &myattr, body, (void *)&parameter);
  /* the thread attribute is no more needed */
  pthread_attr_destroy(&myattr);

  /*
   * NOTE: We are not really using the thread attributes, here...
   * We could have done
   * err = pthread_create(&second_id, NULL, body, (void *)&parameter);
   */
  printf("main: after pthread_create\n");

  /* wait the end of the thread we just created */
  pthread_join(second_id, &returnvalue);

  printf("main: returnvalue is %d\n", (int)returnvalue);

  return 0;
}

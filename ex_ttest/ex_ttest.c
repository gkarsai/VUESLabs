/*
 * An example of how to use signals to implement a timer facility using
 * POSIX threads.
 *
 * Rationale: when using normal Unix signals (SIGALRM), the signal handler executes
 * in a signal handler context.  In a signal handler context, only async-signal-safe
 * functions may be called.  Few POSIX functions are async-signal-safe.
 *
 * Instead of handling the timer activity in the signal handler function,
 * we create a separate thread to perform the timer activity.
 * This timer thread receives a signal from a semaphore, which is signaled ("posted")
 * every time a timer signal arrives.
 *
 * You'll have to redefine "timer_func" below and "set_periodic_timer."
 *
 * Read the man pages of the functions used, such as signal(2), signal(7),
 * alarm(2).
 *
 * Written by Godmar Back <gback@cs.vt.edu>, February 2006.
 */

/* required header files */
#include <pthread.h>
#include <semaphore.h>  /* semaphore operations */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <errno.h>

/* */
static void timer_func();

/* */
static sem_t timer_sem;		/* */
static unsigned int timer_stopped;	/*  */
static pthread_t timer_thread;	/* */

/* */
static void
timersignalhandler(int sig)
{
	/*  */
	sem_post(&timer_sem);	/*  */
}

/* */
static void *
timerthread(void *_)
{
	while (!timer_stopped) {
		int rc = sem_wait(&timer_sem);		/* */
		if (rc == -1 && errno == EINTR)     /* */
		    continue;
		if (rc == -1) {                     /* */
		    printf("timerthread failed on sem_wait\n");
		    exit(-1);
		}

		timer_func();	/* */
	}
	return 0;
}

/*  */
void
init_timer(void)
{
	/* */
	sem_init(&timer_sem, /**/ 0, /**/0); /*  */
	pthread_create(&timer_thread, (pthread_attr_t*)0, timerthread, (void*)0); /*  */
	signal(SIGALRM, timersignalhandler); /* */
}

/* */
void
shutdown_timer()
{
	timer_stopped = 1;
	sem_post(&timer_sem); /* */
	pthread_join(timer_thread, 0); /*  */
}

/*  */
void
set_periodic_timer(long delay)
{
	struct itimerval tval = { /* */
		/*  */ .it_interval = { .tv_sec = 0, .tv_usec = delay },
		/* */       .it_value = { .tv_sec = 0, .tv_usec = delay }
	};

	setitimer(ITIMER_REAL, &tval, (struct itimerval*)0); /*  */
}

/* end provided code part */
/**/

static sem_t demo_over;	/* */

/* */
static void
timer_func()
{
	/* */
	static int i = 0;

	printf ("Timer called %d!\n", i);
	if (++i == 100) {
		shutdown_timer();
		sem_post(&demo_over);	/*  */
	}
}

int
main()
{
	sem_init(&demo_over, /**/ 0, /**/0);

	init_timer();
	set_periodic_timer(/**/200000);

	/* */
	while (sem_wait(&demo_over) == -1 && errno == EINTR)
	    continue;
	return 0;
}

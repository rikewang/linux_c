#include <signal.h>
#include <malloc.h>
#include <errno.h>

#include "anytimer.h"

typedef void (*sighandler_t)(int);

static struct  timer *timer_set[SETMAX];
static sighandler_t original_sig;

int get_pos(void)
{
	int i;

	for(i = 0; i < SETMAX; i++)
	{
		if(timer_set[i] == 0)
			return i;
	}

	return -1;
}


void alrm_handler(int s)
{
	int i;

	for(i = 0; i < SETMAX; i++)
	{
		if(timer_set[i] && (timer_set[i]->process_stat == running))
		{
			timer_set[i]->sec--;
			if(timer_set[i]->sec <= 0)
			{
				timer_set[i]->worker((char *)timer_set[i]->arg);
				timer_set[i]->process_stat = finished;
			}
		}
	}

	alarm(1);
}


void init_job(void)
{
	original_sig = signal(SIGALRM, alrm_handler);
	alarm(1);

	while(1)
	 	pause();
}

int add_job(int sec, func worker, char *arg)
{
	int index;
	struct timer *me;

	if((me = malloc(sizeof(me))) == NULL)
		return -ENOMEM;

	if((index = get_pos()) == -1)
		return -EFAULT;
	
	if (sec < 0)
		return -EINVAL;

	me->sec = sec;
	me->worker = worker;
	me->arg = arg;
	me->process_stat = running;

	timer_set[index] = me;
	
	return index;
}


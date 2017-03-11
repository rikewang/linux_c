#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>


#define THRNUM 4

static pthread_mutex_t mutex[THRNUM];

void *thr_func(void *p)
{
	int i = (int)p;
	int n = 'a' + i;
	
	while(1){
		pthread_mutex_lock(mutex+i);
		write(1, &n, 1);
		pthread_mutex_unlock(mutex+ (i+1)%THRNUM);		
	}

	pthread_exit(NULL);
}

int main()
{
	int i, errno;
	pthread_t tid[THRNUM];

	for(i = 0; i < THRNUM; i++)
	{
		pthread_mutex_init(mutex+i, NULL);
		pthread_mutex_lock(mutex+i);

		errno = pthread_create(tid+i, NULL, thr_func, (void *)i);
		if(errno)
		{
			fprintf(stderr, "thread create failed %s\n", strerror(errno));
			exit(1);
		}
	}
	pthread_mutex_unlock(mutex);


	alarm(1);

	for (i = 0; i < THRNUM; i++)
	{
		pthread_join(tid[i], NULL);
		pthread_mutex_destroy(mutex+i);
	}


	exit(0);
}
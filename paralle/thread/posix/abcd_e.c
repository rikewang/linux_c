#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>


#define THRNUM 4

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

void *thr_func(void *p)
{
	int i = (int)p;
	int n = 'a' + i;
	
	while(1){
		pthread_mutex_lock(&mutex);
		
		while(num != i)
			pthread_cond_wait(&cond, &mutex);
		write(1, &n, 1);
		num = (i+1)%THRNUM;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);		
	}

	pthread_exit(NULL);
}

int main()
{
	int i, errno;
	pthread_t tid[THRNUM];

	for(i = 0; i < THRNUM; i++)
	{
		errno = pthread_create(tid+i, NULL, thr_func, (void *)i);
		if(errno)
		{
			fprintf(stderr, "thread create failed %s\n", strerror(errno));
			exit(1);
		}
	}


	alarm(1);

	for (i = 0; i < THRNUM; i++)
	{
		pthread_join(tid[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define LEFT 		3000000
#define RIGHT		3002000
#define THREADNUM	4


static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

void *is_prime(void *p)
{
	int i, n, flag;

	while(1)
	{
		pthread_mutex_lock(&mut_num);
		while(num == 0)
		{
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}

		if(num == -1)
		{
			pthread_mutex_unlock(&mut_num);
			break;
		}

		n = num;
		num = 0;

		pthread_mutex_unlock(&mut_num);


		for(i = 2, flag = 0; (i<= n/2)&&(!flag); i++)
		{
			if((n % i) == 0)
				flag = 1;
		}

		if(!flag)
			printf("[%d] --> %d\n", (int)p, n);
	}
	pthread_exit(NULL); 
}


int main()
{
	int i, errno;
	pthread_t tid[THREADNUM];

	for(i = 0; i < THREADNUM; i++){
		errno = pthread_create(tid+i, NULL, is_prime, (void *)i);
		if(errno){
			fprintf(stderr, "thread create err: %s\n", strerror(errno));
			exit(1);
		}
	}

	for(i = LEFT; i <= RIGHT; i++)
	{
		pthread_mutex_lock(&mut_num);
		while(num != 0)
		{
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}

		num = i;

		pthread_mutex_unlock(&mut_num);
	}

	pthread_mutex_lock(&mut_num);
	while(num != 0)
	{
		pthread_mutex_unlock(&mut_num);
		sched_yield();
		pthread_mutex_lock(&mut_num);
	}
	num = -1;
	pthread_mutex_unlock(&mut_num);


	for(i = 0; i < THREADNUM; i++)
		pthread_join(tid[i], NULL);

	pthread_mutex_destroy(&mut_num);
	exit(0);
}
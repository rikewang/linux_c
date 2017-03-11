#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define LEFT 	512
#define RIGHT	1024
#define THREADNUM	(RIGHT-LEFT+1)

void *is_prime(void *p)
{
	int i;

	int n = (int)p;

	for(i = 2; i <= n/2; i++)
	{
		if((n % i) == 0)
			pthread_exit(NULL);
	}

	printf("%d\n", n);
	pthread_exit(NULL); 
}


int main()
{
	int i, errno;
	pthread_t tid[THREADNUM];

	for(i = LEFT; i <= RIGHT; i++){
		errno = pthread_create(tid+(i-LEFT), NULL, is_prime, (void *)i);
		if(errno){
			fprintf(stderr, "thread create err: %s\n", strerror(errno));
			exit(1);
		}
	}

	for(i = LEFT; i <= RIGHT; i++)
	{
		pthread_join(tid[i-LEFT], NULL);
	}

	exit(0);
}
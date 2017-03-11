#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *func(void *p)
{
	printf("in func thread\n");

	pthread_exit(0);
}

int main()
{
	pthread_t tid;
	int err;

	err = pthread_create(&tid, NULL, func, NULL)
	if(err)
	{
		fprintf(stderr, "create thread failed: %s\n", strerror(err));
		exit(1);
	}

	exit(0);
}
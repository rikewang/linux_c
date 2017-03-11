#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *func(void *p)
{
	write(1, "0\n", 2);

	pthread_join(pthread_self(), NULL);
	// pthread_exit(0);
}

int main()
{
	pthread_t tid;
	int err;

	err = pthread_create(&tid, NULL, func, NULL);
	if(err)
	{
		fprintf(stderr, "create thread failed: %s\n", strerror(err));
		exit(1);
	}

	// pthread_join(tid, NULL);
	sleep(5);
	exit(0);
}
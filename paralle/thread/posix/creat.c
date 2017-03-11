#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static void *func(void *p)
{
	puts("in function.");
}

int main()
{
	int errno;
	pthread_t tid;

	puts("starting...");

	errno = pthread_create(&tid, NULL, func, NULL);
	if(errno)
		fprintf(stderr, "%s\n", strerror(errno));

	sleep(2);
	puts("ending...");
	exit(0);
}
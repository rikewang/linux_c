#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static void *func(void *p)
{
	int status=1;
	puts("in function.");

	pthread_exit(status);
}

int main()
{
	int errno;
	pthread_t tid;
	int *rst;

	puts("starting...");

	errno = pthread_create(&tid, NULL, func, NULL);
	if(errno)
		fprintf(stderr, "%s\n", strerror(errno));

	pthread_join(tid, &rst);

	printf("%d\n", rst);
	puts("ending...");
	exit(0);
}
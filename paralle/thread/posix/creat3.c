#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void cleanup_handler(void *p)
{
	puts(p);
}

static void *func(void *p)
{
	int status=1;
	puts("in function.");

	pthread_cleanup_push(cleanup_handler, "cleanup_handler:1");
	pthread_cleanup_push(cleanup_handler, "cleanup_handler:2");

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);


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
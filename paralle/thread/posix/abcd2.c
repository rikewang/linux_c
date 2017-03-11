#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>


#define THRNUM 4

// static pthread_mutex_t mutex[THRNUM];


int flag = 1;

void *thr_func(void *p)
{

	while(1){
		switch (flag % THRNUM){
			case 0:
				printf("%c", 'a');
				break;
			case 1:
				printf("%c", 'b');
				break;
			case 2:
				printf("%c", 'c');
				break;
			case 3:
				printf("%c", 'd');
				break;
			default:
				printf("%c", '.');
				break;
		} 

		flag++;

	}

	pthread_exit(NULL);
}

int main()
{
	int i, errno;
	pthread_t tid[THRNUM];

	for(i = 0; i < THRNUM; i++)
	{

		errno = pthread_create(tid+i, NULL, thr_func, NULL);
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


	exit(0);
}
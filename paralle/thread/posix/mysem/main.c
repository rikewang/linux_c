#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mysem.h"

#define LEFT 	512
#define RIGHT	1024
#define THREADNUM	(RIGHT-LEFT+1)
#define SEMNUM	4

static struct mysem_st *me;

void *is_prime(void *p)
{
	int i, flag;

	int n = (int)p;
	write(1, ". ", 2);
	for(i = 2, flag = 0; (i<= n/2)&&(!flag); i++)
	{
		if((n % i) == 0)
			flag = 1;
	}

	if(!flag)
		printf("[%d] --> %d\n", (int)p, n);

	sleep(5);
	mysem_add(me, 1);
	pthread_exit(NULL); 
}


int main()
{
	int i, errno;
	pthread_t tid[THREADNUM];

	me = mysem_init(SEMNUM);

	if(!me)
	{
		fprintf(stderr, "create error\n");
		exit(1);
	}

	for(i = LEFT; i <= RIGHT; i++){
		mysem_sub(me, 1);
		/*if ... */
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

	mysem_destroy(me);

	exit(0);
}
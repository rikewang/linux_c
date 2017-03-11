#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNUM  20
#define FNAME	"/tmp/rst"
#define LINESIZE	1024

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thr_add(void *p)
{
	FILE *fp;
	char buf[LINESIZE];

	fp = fopen(FNAME, "r+");

	/* if ... */

	pthread_mutex_lock(&mutex);
	fgets(buf, LINESIZE, fp);

	fseek(fp, 0,  SEEK_SET);

	/* expand conpetive condition */
	sleep(1);


	fprintf(fp, "%d\n", atoi(buf)+1);
	fclose(fp);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main()
{
	int i, errno;
	pthread_t tid[THRNUM];

	for(i = 0; i < THRNUM; i++)
	{
		errno = pthread_create(tid+i, NULL, thr_add,NULL);
		if(errno)
		{
			fprintf(stderr, "thread create failed %s\n", strerror(errno));
			exit(1);
		}
	}

	for (i = 0; i < THRNUM; i++)
	{
		pthread_join(tid[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	exit(0);
}
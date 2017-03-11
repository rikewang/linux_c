#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#define CPS		10
#define BUFSIZE CPS
#define BRUST	100

//static volatile int token = 1;

static volatile sig_atomic_t token = 1;

static void alrm_handler(int s);

static void alrm_handler(int s)
{
	if(token < BRUST)
		token++;
	
//	alarm(1);
	return;
}

int main(int argc, char const *argv[])
{
	int fd;
	char buf[BUFSIZE];
	ssize_t len;
	struct itimerval itv;

	if(argc < 2)
	{
		fprintf(stderr, "Usage %s <filename> \n", argv[0]);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open(): ");
		exit(1);
	}


	signal(SIGALRM, alrm_handler);
//	alarm(1);

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 10;
	itv.it_value.tv_usec = 0;


	setitimer(ITIMER_REAL, &itv, NULL);


	while(1)
	{
		while(token <= 0)
			pause();
		token--;

		while((len = read(fd, buf, BUFSIZE)) < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read(): ");
			exit(1);

		}

		while(write(1, buf, len) == -1)
		{
			if(errno == EINTR)
				continue;

			perror("write() ");
			exit(1);
		}

		if(len == 0)
			break;
	}

	close(fd);

	exit(0);
}
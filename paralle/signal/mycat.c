#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


#define BUFSIZE 64

static volatile int loop = 0;

static void alrm_handler(int s);

static void alrm_handler(int s)
{
	loop = 0;
	alarm(1);
	return ;
}

int main(int argc, char const *argv[])
{
	int fd;
	char buf[BUFSIZE];
	ssize_t len;

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
	alarm(1);

	while(1)
	{
		while(loop)
			pause();
		loop = 1;

		if((len = read(fd, buf, BUFSIZE)) > 0)
		{
			if(write(1, buf, len) == -1)
			{
				perror("write() ");
				exit(1);
			}
		} 

		if(len == 0)
			break;
	}

	close(fd);

	exit(0);
}
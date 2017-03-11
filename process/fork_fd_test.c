#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE	32

int main()
{
	pid_t pid;
	int fd;
	char buf[BUFSIZE];


	if((fd = open("file", O_RDONLY)) < 0)
	{
		perror("open(): ");
		exit(1);
	}


	if(read(fd, buf, BUFSIZE) < 0)
	{
		perror("read");
		exit(1);	
	}

	printf("before fork: %s\n", buf);

	pid = fork();

	if(pid < 0)
	{
		perror("fork(): ");
		exit(1);
	}

	if(pid == 0)
	{
		if(read(fd, buf, BUFSIZE) < 0)
		{
			perror("read");
			exit(1);	
		}
		printf("child: %s\n", buf);
	} else {
		if(read(fd, buf, BUFSIZE) < 0)
		{
			perror("read");
			exit(1);	
		}
		printf("parent: %s\n", buf);

	}

	exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FNAME  "/tmp/rst"

int main()
{
	int fd;
	char buf[1024];

	fd = open(FNAME, O_RDONLY);
	dup2(fd, 0);

	/*
	close(1);
	dup(fd);
	*/

	if(fd != 0)
		close(fd);

	read(0, buf, 1024);
	printf("buf = %s\n", buf);

	exit(0);
}
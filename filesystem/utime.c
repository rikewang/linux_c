#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <utime.h>

int main()
{
	int fd;
	struct stat statbuf;
	// struct timespec times[2];
	struct utimbuf times;

	if(stat("/root/test/file", &statbuf) < 0)
	{
		perror("stat: ");
		exit(1);
	}

	if((fd = open("/root/test/file", O_WRONLY | O_TRUNC )) < 0)
	{
		perror("open: ");
		exit(1);
	}

	times.actime = statbuf.st_atime;
	times.modtime = statbuf.st_mtime;

	utime("/root/test/file", &times);

	close(fd);
}
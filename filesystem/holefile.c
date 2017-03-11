#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(fd < 0)
	{
		perror("open: ");
		exit(1);
	}

	if(lseek(fd, 5LL*1024LL*1024LL*1024LL-1L, SEEK_SET) == -1)
	{
		perror("lseek: ");
		exit(1);
	}

	write(fd, "", 1);

	exit(0);
}
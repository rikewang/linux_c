#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "mybtf.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
	int fd;
	char buf[BUFSIZE];
	ssize_t len;
	struct btf *me;
	int cps = 64, brust = 100;
	int get;

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


	me = btf_init(cps, brust);


	while(1)
	{
		if((get = btf_fetch(me, BUFSIZE)) < 0)
		{
			fprintf(stderr, "btf_fetch(): %s\n", strerror(-get));
			exit(1);
		}

		while((len = read(fd, buf, get)) < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read(): ");
			exit(1);

		}

		if(get - len > 0)
			if((get = btf_returnback(me, get-len)) < 0)	
			{
				fprintf(stderr, "btf_returnback(): %s\n", strerror(-get));
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

	//btf_destroy(me);
	exit(0);
}
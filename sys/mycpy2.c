#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE	1024

int main(int argc, char *argv[])
{
	FILE *fds, *fdd;
	char buf[BUFSIZE];

	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s <source_filename> <dest_filename>\n", argv[0]);
		exit(1);
	}

	fds = fopen(argv[1], "r");
	if(fds == NULL)
	{
		perror("Open source file: ");
		exit(1);
	}

	fdd = fopen(argv[2], "w");
	if(fdd == NULL)
	{
		fclose(fds);
		perror("Open dest file: ");
		exit(1);
	}


	while(fgets(buf, BUFSIZE, fds))
		fputs(buf, fdd);

	exit(0);
}
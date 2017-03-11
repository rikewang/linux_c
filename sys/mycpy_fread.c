#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE	1024

int main(int argc, char *argv[])
{
	FILE *fds, *fdd;
	char buf[BUFSIZE];
	int n;

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


	while((n = fread(buf, 1, BUFSIZE, fds)) > 0)
		fwrite(buf, 1, n, fdd);

	exit(0);
}
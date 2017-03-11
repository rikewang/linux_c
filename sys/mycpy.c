#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *fds, *fdd;
	int ch;

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


	while(1)
	{
		ch = fgetc(fds);
		if(ch == EOF)
			break;
		fputc(ch, fdd);
	}

	exit(0);
}
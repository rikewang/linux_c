#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

off_t flen(char *filename);

off_t flen(char *filename)
{
	struct stat meta_info;

	if(stat(filename, &meta_info) < 0)
	{
		perror("stat: ");
		exit(1);
	}

	return meta_info.st_size;
}

int main(int argc, char  **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Uages: %s filename \n", argv[0]);
		exit(1);
	}

	printf("size = %lld\n", flen(argv[1]));
}
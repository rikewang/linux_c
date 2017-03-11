#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *fp;

	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		perror("Error at open: ");
		exit(1);
	}

	fseek(fp, 0,  SEEK_END);
	printf("file_size = %ld\n", ftell(fp));

	fclose(fp);
}
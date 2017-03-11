#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *fp;
	long file_size;

	file_size = atoi(argv[2]);

	fp = fopen(argv[1], "w");
	if(fp == NULL)
	{
		perror("Error at open: ");
		exit(1);
	}

	printf("before file_size = %ld\n", ftell(fp));
	fseek(fp, file_size, SEEK_SET);
	fputc('c', fp);
	printf("after file_size = %ld\n", ftell(fp));

	fclose(fp);
}
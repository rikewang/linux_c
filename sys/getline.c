#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	FILE *fp;
	char *line = NULL;
	size_t size = 0;
	ssize_t read;

	fp = fopen("t.c", "r");
	if(fp == NULL)
	{
		perror("Error: ");
		exit(1);
	}

	while((read = getline(&line, &size, fp)) != -1){
		printf("getline %zu \n\n\n\n\n", read);
		printf("string = %s, length = %d \n", line, strlen(line));
		printf("size = %d\n\n", size);
	}

	if(line)
		free(line);
	exit(0);
}
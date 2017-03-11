#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE  1024

void *test(char *mode_info);

void *test(char *mode_info){

	mode_info[5] = 'm';
	mode_info[1] = 'c';
	mode_info[6] = 'w';
	return;
}

int main(int argc, char **argv)
{
	int i;
	char mode_info[10] = "----------";
	
	test(mode_info);

	for(i = 0; i < 10; i++)
		printf("%c", mode_info[i]);
	printf("\n");

	exit(0);
}
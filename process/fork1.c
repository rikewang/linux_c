#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t p;

	printf("[%d]: Begin \n", getpid());

	fflush(NULL);
	p = fork();
	if(p < 0)
	{
		perror("fork(): ");
		exit(1);
	}

	else if(p == 0)
		printf("[%d] child is working\n", getpid());

	else
		printf("[%d] parent is working\n", getpid());

	printf("End\n");
	exit(0);
}
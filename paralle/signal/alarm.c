#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void alrm_handler(int s)
{
	printf("%d\n", s);
}

int main()
{

	alarm(5);
	alarm(3);
	alarm(1);

	signal(SIGALRM, alrm_handler);

	while(1)
		pause();

	exit(0);
}
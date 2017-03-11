#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

static volatile int loop = 1;

void alarm_handler(int s);

void alarm_handler(int s){

	loop = 0;
}


int main()
{

	int64_t count;

	alarm(5);
	signal(SIGALRM, alarm_handler);

	while(loop)
		count++;

	printf("%lld\n", count);

	exit(0);
}

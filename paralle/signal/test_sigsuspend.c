#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static volatile sig_atomic_t flag;

void handler(int s)
{
	if(s == SIGINT){
		printf("interrupt\n");
		return ;
	}
	printf("get SIGQUIT \n");
	flag = 1;
}


int main()
{
	sigset_t zeroset;
	sigset_t blockset, origset;
	struct sigaction act;

	sigemptyset(&zeroset);
	sigemptyset(&blockset);

	sigaddset(&blockset, SIGINT);
	sigaddset(&blockset, SIGQUIT);

	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);

	sigprocmask(SIG_BLOCK, &blockset, &origset);

	while(flag == 0)
		sigsuspend(&zeroset);

	flag = 1;
	sigprocmask(SIG_SETMASK, &origset, NULL);

	exit(0);
}
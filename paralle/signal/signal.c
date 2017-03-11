#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void signal_handle(int s);

void signal_handle(int s){
	write(1, "M", 1);
	// sleep(2);
}

int main()
{
	int i;
	sigset_t set, oset;

	signal(SIGINT, signal_handle);

	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	sigprocmask(SIG_UNBLOCK, &set, &oset);

	while(1){
		sigprocmask(SIG_BLOCK, &set, NULL);
		for(i = 0; i < 5; i++)
		{
			write(1, "*", 1);
			sleep(1);
		}

		write(1, "\n", 1);
		sigprocmask(SIG_UNBLOCK, &set, NULL);
	}

	sigprocmask(SIG_SETMASK, &oset, NULL);
}
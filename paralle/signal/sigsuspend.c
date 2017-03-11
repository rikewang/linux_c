#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void signal_handle(int s);

void signal_handle(int s){
	write(1, "0", 1);
}

int main()
{
	int i;
	sigset_t set, saved_set, oset;

	signal(SIGINT, signal_handle);

	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	sigprocmask(SIG_UNBLOCK, &set, &saved_set);

	sigprocmask(SIG_BLOCK, &set, &oset);

	while(1){
	//	sigprocmask(SIG_BLOCK, &set, &oset);

		for(i = 0; i < 5; i++)
		{
			write(1, "*", 1);
			sleep(1);
		}

		write(1, "\n", 1);
		sigsuspend(&oset);
		/*
			sigset_t tmpset;

			sigprocmask(SIG_SETMASK, &oset, &tmpset);
			pause();
			sigprocmask(SIG_SETMASK, &tmpset, NULL);
			
			----------------

			clear set signal;
			pause;
			recovery original set continue block;
		*/
	}

	sigprocmask(SIG_SETMASK, &saved_set, NULL);
}
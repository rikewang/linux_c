#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void handler(int s)
{
	printf("[%d] recv: %d\n", getpid(), s);
}

int main(int argc, char **argv)
{
	pid_t pid;

	signal(SIGUSR1, handler);

	pid =  fork();

	if(pid < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	if(pid < 0)
	{
		// sleep(1024);
		execve("/code/paralle/signal/mycat", argv, NULL);
	}

	else{

		sleep(1024);
	}

	exit(0);
}

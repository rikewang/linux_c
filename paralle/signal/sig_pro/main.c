#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pcsync.h"

int main()
{
	pid_t pid;

	TELL_WAIT();

	pid =  fork();

	if(pid < 0){
		perror("fork(): ");
		exit(1);
	}

	if(pid == 0)
	{
		printf("In child\n");
		sleep(1);
		printf("init my env, need parent give me a ip\n");
		TELL_PARENT(getppid());
		WAIT_PARENT();
		printf("child finished\n");
		exit(0);
	}
	printf("In parent\n");
	sleep(3);
	printf("Parent work done, feedback to child\n");
	TELL_CHILD(pid);
	WAIT_CHILD();
	printf("parent finished\n");

	exit(0);
}
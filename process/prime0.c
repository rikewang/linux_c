#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int is_prime(int n);

int is_prime(int n)
{
	int i;

	for(i = 2; i <= n/2; i++)
	{
		if((n % i) == 0)
			return 0;
	}

	return 1;
}

int main()
{
	int i, status;
	pid_t pid;

	for (i = 3000000; i < 3000500; i++)
	{


		pid = fork();

		if(pid < 0)
		{
			perror("fork(): ");
			exit(1);
		}

		if(pid == 0)
		{

			if(is_prime(i))
				printf("%d\n", i);
//			sleep(60);
			exit(0);
		}
					// sleep(60);

	}


	for(i = 0; i < 501; i++)
	{
		pid = wait(&status);
		printf("%lld\n", pid);
	}

	exit(0);
}
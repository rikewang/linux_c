#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5

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
	int i, n;
	pid_t pid;
	int status;

	for(n = 0; n < N; n++){
		pid = fork();

		if(pid < 0)
		{
			perror("fork(): ");
			exit(1);
		}

		if(pid == 0){
			for (i = 3000000+n; i < 3000500; i+=N)
				{
					if(is_prime(i))
						printf("[%d]: [%d]: %d\n", n, getpid(), i);
				}
			exit(0);
		}

	}

	for(i = 0; i < N; i++)
	{
		pid = wait(&status);
		printf("%lld\n", pid);
	}
	exit(0);
}
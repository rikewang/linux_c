#include <stdio.h>
#include <stdlib.h>
#include <sys/acct.h>
#include <unistd.h>

int main()
{


	printf("hello world\n");

	acct("t.txt");

	sleep(1);

	printf("hello world\n");

	exit(0);
}
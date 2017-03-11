#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	puts("starting..");

	execl("/bin/date", "date", "+%s", NULL);

	puts("end");

	perror("execl: ");
	exit(1);
	exit(0);
}
#include <stdio.h>
#include <stdlib.h>

int main()
{

	enum stat {ready, running, finished, cancled};
	enum stat process_stat;

	process_stat = cancled;

	printf("%d\n", process_stat);


	exit(0);
}
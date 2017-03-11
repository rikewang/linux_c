#include <stdio.h>
#include <stdlib.h>

#define SIZE	5

int main()
{
	char buf[SIZE];
	FILE *fp;
	int count = 0;

	fp = fopen("t.txt", "r");

	printf("read: %s \n", fgets(buf, SIZE, fp));
	while(count < 5)
	{
		printf("read: %s \n", fgets(buf, SIZE, fp));
		count++;
	}

	printf("count = %d\n", count);

	exit(0);
}
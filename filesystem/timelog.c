#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE	256

int main()
{
	time_t stamp;
	struct tm *st_time;
	char format_rst[BUFSIZE];
	FILE *fp;
	int count = 0;

	fp = fopen("time.log", "a+");

	while(fgets(format_rst, BUFSIZE, fp))
		count++;


	while(1)
	{
		time(&stamp);

		if ((st_time = localtime(&stamp)) == NULL)
		{
			fprintf(stderr, "get localtime error\n");
			exit(1);	
		}
	
		fprintf(fp, "%-4d %d-%d-%d %d:%d:%d\n", ++count, (st_time->tm_year) + 1900, \
			(st_time->tm_mon)+1, st_time->tm_mday, st_time->tm_hour, st_time->tm_min, st_time->tm_sec);	
		/* must flush, because this is full buffer, otherwise the time.log is empty unless the buffer is full */
		fflush(fp);
		sleep(1);
	}

	fclose(fp);

	exit(0);
}
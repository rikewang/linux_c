#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	time_t stamp;
	struct tm *st_time;

	stamp = time(NULL);
	st_time = localtime(&stamp);

	/* Now */
	printf("%d-%d-%d %d:%d:%d\n",  (st_time->tm_year) + 1900, \
			(st_time->tm_mon)+1, st_time->tm_mday, st_time->tm_hour, st_time->tm_min, st_time->tm_sec);

	st_time->tm_mday += 100;

	/* mktime auto adjust */
	mktime(st_time);

	/* after 100 days */
	printf("%d-%d-%d %d:%d:%d\n",  (st_time->tm_year) + 1900, \
			(st_time->tm_mon)+1, st_time->tm_mday, st_time->tm_hour, st_time->tm_min, st_time->tm_sec);

}
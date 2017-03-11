#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define TIMEFMTSIZE	1024
#define TIMESIZE  	1024

int main(int argc, char **argv)
{
	int opt;
	FILE *fp = stdout;
	time_t stamp;
	struct tm *st_time;
	char time_format[TIMEFMTSIZE];
	char str_time[TIMESIZE];

	time_format[0] = '\0';
	stamp = time(NULL);
	st_time = localtime(&stamp);

	while((opt = getopt(argc, argv, "-y:h:MSmd")) != -1)
	{
		switch(opt)
		{
			case 1:
				if((fp = fopen(argv[optind-1], "w")) == NULL){
					fp = stdout;
				}

				break;

			case 'y':
				if(strcmp(optarg, "2") == 0){
					strncat(time_format, "%y ", TIMEFMTSIZE);
				} else if(strcmp(optarg, "4") == 0){
					strncat(time_format, "%Y ", TIMEFMTSIZE);					
				}
				break;
			case 'd':
				strncat(time_format, "%d ", TIMEFMTSIZE);
				break;
			case 'm':
				strncat(time_format, "%m ", TIMEFMTSIZE);
				break;
			case 'S':
				strncat(time_format, "%S ", TIMEFMTSIZE);
				break;
			case 'h':
				if(strcmp(optarg, "12") == 0){
					strncat(time_format, "%I (%p) ", TIMEFMTSIZE);
				} else{
					strncat(time_format, "%H ", TIMEFMTSIZE);					
				} 				
				break;
			case 'M':
				strncat(time_format, "%M ", TIMEFMTSIZE);
				break;
			default:
				fprintf(stderr, "Invalid opt: %c\n", opt);
				exit(1);
		}
	}


	strftime(str_time, TIMESIZE, time_format, st_time);
	strncat(str_time, "\n", TIMEFMTSIZE);
	fputs(str_time, fp);

	if(fp != stdout)
		fclose(fp);

	exit(0);
}
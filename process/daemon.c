#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <syslog.h>

#define FNAME	"/tmp/rst"


static char *daemonsize(void);

static char *daemonsize(void)
{
	int fd;
	pid_t pid;
	
	pid = fork();

	if(pid < 0)
	{
		return "fork error";
	}

	if(pid > 0)
	{
		exit(0);
	}

	if(setsid() < 0)
	{
		
		return "setsid(): ";
	}

	if((fd = open("/dev/null", O_RDWR)) < 0)
	{
		return "chongdingxiang";
	}

	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	if(fd > 2)
		close(fd);

	return (char *)0;
}


int main()
{
	int i;
	FILE *fp;
	char *rst;

	openlog("mydeamon",  LOG_PID, LOG_DAEMON);

	rst = daemonsize();
	if( rst!= 0)
	{
		syslog(LOG_ERR, "mydeamon create fail: %s", rst);
		exit(1);
	} else{
		syslog(LOG_INFO, "mydeamon create success");		
	}

	if((fp = fopen(FNAME, "w")) == NULL)
	{
		syslog(LOG_ERR, "file %s fopen fail", FNAME);		
		exit(1);
	}


	for(i = 0; ; i++)
	{
		fprintf(fp, "%d\n", i);
		fflush(fp);
		sleep(1);
	}

	eixt(0);
}
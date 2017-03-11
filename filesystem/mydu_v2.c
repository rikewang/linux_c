#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAXPATHNAME	1024

static int64_t mydup(char *pathname);
static int check_noloop(char *pathname);

static int check_noloop(char *pathname)
{
	char *pos;

	pos = strrchr(pathname, '/');
	
	if(!strcmp(pos+1, ".") || !strcmp(pos+1, "..")) 
		return 0;
	else
		return 1;
}

static int64_t mydup(char *pathname)
{
	int i;
	int64_t sum;
	char nextpath[MAXPATHNAME];
	struct stat statrst;
	DIR *dp;
	struct dirent *dir_info;

	if(lstat(pathname, &statrst) == -1)
	{
		perror("stat: ");
		exit(1);
	}

	if(!S_ISDIR(statrst.st_mode))
		return statrst.st_blocks;

	if((dp = opendir(pathname)) == NULL)
	{
		perror("opendir: ");
		exit(1);
	}

	sum = statrst.st_blocks;
	while((dir_info = readdir(dp)) != NULL)
	{
		strncpy(nextpath, pathname, MAXPATHNAME);
		strncat(nextpath, "/", MAXPATHNAME);	
		strncat(nextpath, dir_info->d_name, MAXPATHNAME);
		
		if(check_noloop(nextpath))
			sum += mydup(nextpath);			
	}

	closedir(dp);

	return sum;
}


int main(int argc, char **argv)
{
	printf("%lld\n", mydup(argv[1])/2);
}
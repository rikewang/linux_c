#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXPATHNAME	1024

static int64_t mydup(char *pathname);
static int check_noloop(char *pathname);

static int check_noloop(char *pathname)
{
	char *pos;

	pos = strrchr(pathname, '/');
	return (!(strcmp(pos+1, ".")) || !(strcmp(pos+1, "..")) ) ? 0 : 1;
}

static int64_t mydup(char *pathname)
{
	int i;
	int64_t sum;
	static char nextpath[MAXPATHNAME];
	static char nextpotpath[MAXPATHNAME];
	struct stat statrst;
	glob_t globrst;

	printf("pathname = %s\n", pathname);
	if(lstat(pathname, &statrst) == -1)
	{
		perror("stat: ");
		exit(1);
	}

	if(!S_ISDIR(statrst.st_mode))
		return statrst.st_blocks;

	strncpy(nextpath, pathname, MAXPATHNAME);
	strncat(nextpath, "/*", MAXPATHNAME);
	glob(nextpath, 0, NULL, &globrst);

	strncpy(nextpotpath, pathname, MAXPATHNAME);
	strncat(nextpotpath, "/.*", MAXPATHNAME);
	glob(nextpotpath, GLOB_APPEND, NULL, &globrst);

	sum = statrst.st_blocks;		// add current file size;
	for(i = 0; i < globrst.gl_pathc; i++)
	{
		if(check_noloop(globrst.gl_pathv[i]))
			sum += mydup(globrst.gl_pathv[i]);
	}

	globfree(&globrst);

	return sum;
}


int main(int argc, char **argv)
{
	printf("%lld\n", mydup(argv[1])/2);
}
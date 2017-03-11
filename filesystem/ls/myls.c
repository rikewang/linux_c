#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>

#define TIMEFMTSIZE	128
#define PATHSIZE	256

#define LNKCNT(x) 	(x).st_nlink
#define UID(x) 		(x).st_uid
#define GID(x)		(x).st_gid
#define MTIME(x)	(x).st_mtime
#define SIZE(x)		(x).st_size
#define INODE(x)	(x).st_ino

void ftype(const struct stat *statrst, char *mode_info);
char *getusername(const unsigned int uid); 
char *getgrpname(const unsigned int gid);
void gettime(const time_t mtime, char *timerst);

void gettime(const time_t mtime, char *timerst)
{
	struct tm *tm;

	tm = localtime(&mtime);

	strftime(timerst, TIMEFMTSIZE, "%b %2d %H:%M", tm);

	return;
}

char *getgrpname(const unsigned int gid)
{
	struct group *grprst;

	grprst = getgrgid(gid);

	return grprst->gr_name;
}

char *getusername(const unsigned int uid)
{
	struct passwd *pwrst;

	pwrst = getpwuid(uid);

	return pwrst->pw_name;
}

void ftype(const struct stat *statrst, char *mode_info)
{
	int i;

	 switch(statrst->st_mode & S_IFMT)
	 {
	 	case S_IFCHR: mode_info[0] = 'c';	break;
	 	case S_IFDIR: mode_info[0] = 'd';	break;
	 	case S_IFBLK: mode_info[0] = 'b'; 	break;
	 	case S_IFIFO: mode_info[0] = 'p'; 	break;
	 	case S_IFREG: mode_info[0] = '-'; 	break;
	 	case S_IFSOCK:mode_info[0] = 's'; 	break;	
	 	case S_IFLNK: mode_info[0] = 'l'; 	break;
	 	default:	  mode_info[0] = '?';
	 }

	 
	 if(S_IRUSR & statrst->st_mode)
	 	mode_info[1] = 'r';
	 if(S_IWUSR & statrst->st_mode)
	 	mode_info[2] = 'w';
	 if(S_IXUSR & statrst->st_mode)
	 	mode_info[3] = 'x';
	 if(S_IRGRP & statrst->st_mode)
	 	mode_info[4] = 'r';
	 if(S_IWGRP & statrst->st_mode)
	 	mode_info[5] = 'w';
	 if(S_IXGRP & statrst->st_mode)
	 	mode_info[6] = 'x';
	 if(S_IROTH & statrst->st_mode)
	 	mode_info[7] = 'r';
	 if(S_IWOTH & statrst->st_mode)
	 	mode_info[8] = 'w';
	 if(S_IXOTH & statrst->st_mode)
	 	mode_info[9] = 'x';
	 if(S_ISUID & statrst->st_mode)
	 {
	 	if(mode_info[3] == 'x')
	 		mode_info[3] = 's';
	 	else
	 		mode_info[3] = 'S';
	 }
	 if(S_ISGID & statrst->st_mode)
	 {
	 	if(mode_info[6] == 'x')
	 		mode_info[6] = 's';
	 	else
	 		mode_info[6] = 'S';
	 }
	 if(S_ISVTX & statrst->st_mode)
	 {
	 	if(mode_info[9] == 'x')
	 		mode_info[9] = 't';
	 	else
	 		mode_info[9] = 'T';
	 }



	return ;
}




int main(int argc, char const *argv[])
{
	int i;
	char mode_info[10] = "----------";
	char timerst[TIMEFMTSIZE];
	//char *filename;
	struct stat statrst;
	char *path;
	DIR *dp;
	struct dirent *rdirst;

	if(argc == 1)
	{
		path = get_current_dir_name();
	}

	if((dp = opendir(path)) == NULL)
	{
		perror("opendir: ");
		exit(1);
	}

	while((rdirst = readdir(dp)) != NULL)
	{

		if(stat(rdirst->d_name, &statrst) == -1)
		{
			perror("stat: ");
			exit(1);
		}

		ftype(&statrst, mode_info);

		for(i = 0; i < 10; i++)
			printf("%c", mode_info[i]);
		printf(" %d", LNKCNT(statrst));
		printf(" %s", getusername(UID(statrst)));
		printf(" %s", getgrpname(GID(statrst)));
		printf(" %lld", SIZE(statrst));
		gettime(MTIME(statrst), timerst);
		printf(" %s", timerst);
		printf(" %s", rdirst->d_name);
		puts("");
	}
	closedir(dp);

	exit(0);
}
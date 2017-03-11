#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int ftype(const char *filename);

static int ftype(const char *filename)
{
	struct stat statrst;

	if(stat(filename, &statrst) == -1)
	{
		perror("stat: ");
		exit(1);
	}

/*
           S_ISREG(m)  is it a regular file?

           S_ISDIR(m)  directory?

           S_ISCHR(m)  character device?

           S_ISBLK(m)  block device?

           S_ISFIFO(m) FIFO (named pipe)?

           S_ISLNK(m)  symbolic link? (Not in POSIX.1-1996.)

           S_ISSOCK(m) socket? (Not in POSIX.1-1996.)


*/

// method 1

/*

	if(S_ISREG(statrst.st_mode))
		return '-';
	else if(S_ISDIR(statrst.st_mode))
		return 'd';
	else if(S_ISLNK(statrst.st_mode))
		return 'l';
	else if(S_ISSOCK(statrst.st_mode))
		return 's';
	else if(S_ISCHR(statrst.st_mode))
		return 'c';
	else if(S_ISFIFO(statrst.st_mode))
		return 'p';
	else if(S_ISBLK(statrst.st_mode))
		return 'b';
	else
		return '?';
*/

/*
           S_IFMT     0170000   bit mask for the file type bit fields
           S_IFSOCK   0140000   socket
           S_IFLNK    0120000   symbolic link
           S_IFREG    0100000   regular file
           S_IFBLK    0060000   block device
           S_IFDIR    0040000   directory
           S_IFCHR    0020000   character device
           S_IFIFO    0010000   FIFO
           S_ISUID    0004000   set UID bit
           S_ISGID    0002000   set-group-ID bit (see below)
           S_ISVTX    0001000   sticky bit (see below)
           S_IRWXU    00700     mask for file owner permissions
           S_IRUSR    00400     owner has read permission
           S_IWUSR    00200     owner has write permission
           S_IXUSR    00100     owner has execute permission
           S_IRWXG    00070     mask for group permissions
           S_IRGRP    00040     group has read permission
           S_IWGRP    00020     group has write permission
           S_IXGRP    00010     group has execute permission
           S_IRWXO    00007     mask for permissions for others (not in group)
           S_IROTH    00004     others have read permission
           S_IWOTH    00002     others have write permission
           S_IXOTH    00001     others have execute permission

*/

// method 2;

	if(S_IFSOCK == (statrst.st_mode & S_IFMT))
		return 's';
	else if(S_IFLNK == (statrst.st_mode & S_IFMT))
		return 'l';	
	else if(S_IFREG == (statrst.st_mode & S_IFMT))
		return '-';
	else if(S_IFDIR == (statrst.st_mode & S_IFMT))
		return 'd';
	else
		return '?';
	

}


int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}


	printf("%c\n", ftype(argv[1]));

	exit(0);
}
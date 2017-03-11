#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void ftype(const char *filename, char *mode_info);

void ftype(const char *filename, char *mode_info)
{
	int i;
	struct stat statrst;

	if(stat(filename, &statrst) == -1)
	{
		perror("stat: ");
		exit(1);
	}

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

	 switch(statrst.st_mode & S_IFMT)
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

	 
	 if(S_IRUSR & statrst.st_mode)
	 	mode_info[1] = 'r';
	 if(S_IWUSR & statrst.st_mode)
	 	mode_info[2] = 'w';
	 if(S_IXUSR & statrst.st_mode)
	 	mode_info[3] = 'x';
	 if(S_IRGRP & statrst.st_mode)
	 	mode_info[4] = 'r';
	 if(S_IWGRP & statrst.st_mode)
	 	mode_info[5] = 'w';
	 if(S_IXGRP & statrst.st_mode)
	 	mode_info[6] = 'x';
	 if(S_IROTH & statrst.st_mode)
	 	mode_info[7] = 'r';
	 if(S_IWOTH & statrst.st_mode)
	 	mode_info[8] = 'w';
	 if(S_IXOTH & statrst.st_mode)
	 	mode_info[9] = 'x';
	 if(S_ISUID & statrst.st_mode)
	 {
	 	if(mode_info[3] == 'x')
	 		mode_info[3] = 's';
	 	else
	 		mode_info[3] = 'S';
	 }
	 if(S_ISGID & statrst.st_mode)
	 {
	 	if(mode_info[6] == 'x')
	 		mode_info[6] = 's';
	 	else
	 		mode_info[6] = 'S';
	 }
	 if(S_ISVTX & statrst.st_mode)
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

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	ftype(argv[1], mode_info);

	for(i = 0; i < 10; i++)
		printf("%c", mode_info[i]);
	printf("\n");

	exit(0);
}
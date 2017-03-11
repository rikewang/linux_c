#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	printf("pwd = %s\n", get_current_dir_name());
	chdir("/tmp");
	printf("pwd = %s\n", get_current_dir_name());
	mkdir("/tmp/test", 0700);
	mkdir("/tmp/test2", 0700);
	chdir("/tmp/test");
	printf("pwd = %s\n", get_current_dir_name());
	chdir("/tmp/test2");
	rmdir("/tmp/test");
	rename("/tmp/test2", "/tmp/mm");	
}
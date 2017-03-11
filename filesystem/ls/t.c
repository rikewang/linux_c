#include <stdio.h>
#include <unistd.h>



int main(void)
{
	char *path;
	path = get_current_dir_name();
	printf("%s\n", path);
}
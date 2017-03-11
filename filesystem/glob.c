#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#define PAT 	"/etc/*.conf"

int main()
{

	int i;
	glob_t globbuf;


    glob("/code/filesystem/*", GLOB_MARK, NULL, &globbuf);
   
    for(i = 0; i < globbuf.gl_pathc; i++)
    {
        puts(globbuf.gl_pathv[i]);
    }


	
	exit(0);
}

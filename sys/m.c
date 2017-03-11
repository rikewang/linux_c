#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){

	FILE *fp;
	fp = tmpfile();
	
	fputs("abcwxl", fp);
	sleep(5);

	fclose(fp);

    
	sleep(5);
	exit(0);

}

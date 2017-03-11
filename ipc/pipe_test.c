/*************************************************************************
	> File Name: pipe_test.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Wed 23 Nov 2016 01:03:11 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE   1024

int main(void)
{
    char buf[BUFSIZE];
    FILE *fp;

    if((fp = popen("myup2low", "r")) == NULL) {
        perror("popen()");
        exit(1);
    }

    for(;;){
        fputs("prompt >> ", stdout);
        fflush(stdout);
        if(fgets(buf, BUFSIZE, fp) == NULL)
            break;
        if(fputs(buf, stdout) == EOF){
            fprintf(stderr, "end of line\n");
            exit(1);
        }
        

    }

    pclose(fp);
    putchar('\n');
    exit(0);
}

/*************************************************************************
	> File Name: popen.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Wed 23 Nov 2016 11:37:11 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE    1024

int main(int argc, char *argv[])
{
 
    FILE *fout, *fin;
    char buf[BUFSIZE];

    if(argc != 2){
        fprintf(stderr, "Usage: a.out <filename>\n");
        exit(1);
    }

    if((fout = fopen(argv[1], "r")) == NULL) {
        perror("fopen()");
        exit(1);
    }

    if((fin =  popen("/bin/more", "w")) == NULL){
        perror("popen()");
        exit(1);
    }

    while(fgets(buf, BUFSIZE, fout) != NULL){
        if(fputs(buf, fin) == EOF){
            fprintf(stderr, "End\n");
        }
    }

    fclose(fout);
    pclose(fin);

    exit(0);
}

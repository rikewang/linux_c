/*************************************************************************
	> File Name: add2.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Wed 23 Nov 2016 01:51:32 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE   1024

int main(void)
{
    int n, int1, int2;
    char line[MAXLINE];

    while( (n = read(STDIN_FILENO, line, MAXLINE)) > 0){
        line[n] = 0;
        if(sscanf(line, "%d%d", &int1, &int2) == 2){
            sprintf(line, "%d\n", int1+int2);
            n = strlen(line);
            if((write(STDOUT_FILENO, line, n)) != n){
                perror("write(): ");
                exit(1);
            }
        } else{
            if((write(STDOUT_FILENO, "invalid args\n", 13)) != 13){
                perror("write(): ");
                exit(1);
            
            }
        }
    }

    exit(0);
}

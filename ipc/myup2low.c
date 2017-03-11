/*************************************************************************
	> File Name: myup2low.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Wed 23 Nov 2016 12:58:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


int main(void)
{
    int c;

    while((c = getchar()) != EOF){
        if(isupper(c))
            c = tolower(c);
        if(putchar(c) == EOF){
            fprintf(stderr, "end of file");
            exit(1);
        }
        if(c == '\n');
            fflush(stdout);
    }

    exit(0);
}

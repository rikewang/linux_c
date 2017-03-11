/*************************************************************************
	> File Name: pipe_fork.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sun 20 Nov 2016 11:39:55 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t pid;
    char buf;

    if(argc != 2)
    {
        fprintf(stderr, "need string");
        exit(1);
    }
    
    if(pipe(pipefd) == -1 ){
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(1);
    }

    if(pid == 0){
        close(pipefd[1]);

        while(read(pipefd[0], &buf, 1) > 0)
            write(1, &buf, 1);
        write(1, "\n", 1);
        close(pipefd[0]);
        _exit(0);
    } else {
        close(pipefd[0]);
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);
        wait(NULL);
        exit(0);
    }
}

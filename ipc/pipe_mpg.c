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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE     1024

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t pid;
    int fd;
    char buf[BUFSIZE];
    int len;

    if(argc != 2)
    {
        fprintf(stderr, "need fname");
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
        
        dup2(pipefd[0], 0);
        fd = open("/tmp/rst", O_RDWR);
        dup2(fd, 1);
        dup2(fd, 2);

        execl("/usr/bin/wc", "wc", "-l", NULL);
        
        printf("%d\n", __LINE__);
        perror("execl error");
        exit(1);
    } else {
        close(pipefd[0]);
        fd = open(argv[1], O_RDONLY);
        while((len = read(fd, buf, BUFSIZE)) > 0)
            write(pipefd[1], buf, len);
        close(pipefd[1]);
        wait(NULL);
        exit(0);
    }
}

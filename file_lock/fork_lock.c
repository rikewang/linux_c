/*************************************************************************
	> File Name: fork_lock.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sun 04 Dec 2016 08:35:23 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int fd;
    pid_t pid;

    fd = open(argv[1], O_RDWR);
    
    pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid == 0)
    {
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_len = 128;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;

        if(fcntl(fd, F_SETLK, &fl) < 0)
            perror("fcntl:");

        exit(0);
    }


    sleep(1);
    write(fd, "hello\n", 6);
    close(fd);

    exit(0);
}

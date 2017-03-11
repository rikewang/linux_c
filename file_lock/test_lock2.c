/*************************************************************************
	> File Name: test_lock.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sun 04 Dec 2016 03:32:51 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int lock = 0, fd, fd2, fd3;

    fd = open(argv[1], O_RDWR);
    fd2 = open(argv[1], O_RDWR);
    printf("fd = %d, fd2 = %d\n", fd, fd2);
   
    fd3 = dup(fd);

    write(fd, "hello\n", 6);
    write(fd2, "world\n", 6);
    write(fd3, "test\n", 5);
    flock(fd, LOCK_EX);
    flock(fd2, LOCK_EX);

    exit(0);
}

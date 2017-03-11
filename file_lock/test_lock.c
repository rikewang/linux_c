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
    int lock = 0, fd;
    
    if(argc <=2){
        fprintf(stderr, "Usage %s <filename> [s|x][n] [sleep_time]\n");
        exit(1);
    }
    
    if(argv[2][0] == 's')
        lock |= LOCK_SH;
    else if(argv[2][0] == 'x')
        lock |= LOCK_EX;
    else{
        fprintf(stderr, "invalid lock type\n");
        exit(1);
    }
    
    if('n' == argv[2][1])
        lock |= LOCK_NB;

    fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("open");
        exit(1);
    }
    if(flock(fd, lock) < 0){
        if(errno == EWOULDBLOCK){
            fprintf(stderr, "already locked\n");
            exit(1);
        }
            
        perror("flock");
        exit(1);
    }
    printf("[time: %ld] request by %ld, lock_type: %s\n", time(NULL), getpid(), (lock & LOCK_SH) ? "share " : "except ");

    sleep(atoi(argv[3]));

    if(flock(fd, LOCK_UN) < 0){
        perror("flock");
        exit(1);
    }
    printf("[time: %ld] released by %d, lock_type: %s\n", time(NULL), getpid(), (lock & LOCK_SH) ? "share " : "except ");

    close(fd);
    exit(0);
}

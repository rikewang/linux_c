#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>

#include "locklib.h"
#include "mydaemon.h"

int main(int argc, char **argv)
{
    int fd;
    pid_t pid;
    int rst;
    
    if(argc != 2) {
        fprintf(stderr, "Usage: a.out <filename>\n");
        exit(1);
    }

    openlog("main", 0, 0);

    if(become_daemon() == -1)
    {
        fprintf(stderr, "start daemon failed\n");
        exit(-1);
    }

    syslog(LOG_INFO, "become deamon success");

    rst = create_pidfile("main", "/var/run/main.pid", 0);
    if(rst < 0){
        syslog(LOG_ERR, "error @create_pidfile: %s\n", strerror(-rst));
        exit(1);
    }

    syslog(LOG_INFO, "start %s success", "main");

    sleep(100);

    fd = open(argv[1], O_RDWR);
    if(fd < 0) {
        perror("open");
        exit(1);
    }

    pid = fork();
    if(pid < 0) {
        perror("fork()");
        exit(1);
    }

    if(pid == 0) {
        rst = lock_set(fd, F_WRLCK, 0, 0, SEEK_SET);
        printf("locked by %d\n", getpid());
        if(rst < 0){
            perror("lock_set");
            exit(1);
        }
        sleep(3);
        exit(0);
    }
    sleep(1);
    rst = lock_get(fd);
    if (rst == 0) {
        rst = lock_set(fd, F_WRLCK, 0, 0, SEEK_SET);
        printf("locked by %d\n", getpid());
        if(rst < 0){
            perror("lock_set");
            exit(0);
        }
    } else {
        fprintf(stderr, "already locked by %d !!\n", rst);
        rst = lock_set_wait(fd, F_WRLCK, 0, 0, SEEK_SET);
        printf("locked by %d, wait success!\n", getpid());
        if(rst < 0){
            perror("lock_set");
            exit(0);
        }
    }


    exit(0);
}

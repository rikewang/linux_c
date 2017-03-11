#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mydaemon.h"
#include "locklib.h"

int become_daemon(void)
{
    int fd, max_open_file;

    switch(fork()){
        case -1:
            return -1;
        case 0:
            break;
        default:
            _exit(0);
    }

    setsid();

    switch(fork()){
        case -1:
            return -1;
        case 0:
            break;
        default:
            _exit(0);
    }

    if(chdir("/") < 0)
        return -1;
    
    umask(0);

    max_open_file =  sysconf(_SC_OPEN_MAX);
    max_open_file = (max_open_file == -1) ? MAX_OPEN_FD : max_open_file;

    for(fd = 0; fd < max_open_file; fd++)
        close(fd);

    close(STDIN_FILENO);
    fd = open("/dev/null", O_RDWR);
    if(fd != STDIN_FILENO)
        return -1;

    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);


    return 0;
}

int create_pidfile(const char *prog_name, const char *pidfile, int flags)
{
    int fd;
    char buf[MAX_PID_SIZE];
    
    fd = open(pidfile, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if(fd < 0)
        return -errno;

    if(flags & CPF_CLOSEXEC){  /* if program want to restart by exec, this flag should define! */
        flags = fcntl(fd, F_GETFD);
        if(flags == -1)
            return -errno;
        flags |= FD_CLOEXEC;
        
        if(fcntl(fd, F_SETFD, flags) == -1)
            return -errno;
    }

    if(lock_set(fd, F_WRLCK, 0, 0, SEEK_SET) == -1){
        return -errno;
    }

    if(ftruncate(fd, 0) == -1)
        return -errno;
    snprintf(buf, MAX_PID_SIZE, "%ld\n", (long)getpid());
    if(write(fd, buf, strlen(buf)) != strlen(buf))
        return -errno;

    return fd;
}

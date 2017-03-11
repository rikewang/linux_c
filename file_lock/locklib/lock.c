/*************************************************************************
	> File Name: lock.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sun 04 Dec 2016 08:14:13 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>

int lockreg(int fd, int cmd, short lock, short whence, off_t start, off_t len){
    struct flock fl;

    fl.l_type = lock;
    fl.l_whence = whence;
    fl.l_start = start;
    fl.l_len = len;

    return fcntl(fd, cmd, &fl);
}

int lockregion(int fd, short lock, short whence,off_t start, off_t len)
{
    return lockreg(fd, F_SETLK, lock, whence, start, len);
}

int lockregionwait(int fd, short lock, short whence,off_t start, off_t len)
{
    return lockreg(fd, F_SETLKW, lock, whence, start, len);
}

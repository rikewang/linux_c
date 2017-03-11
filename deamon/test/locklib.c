#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "locklib.h"

int lockbase(int fd, int cmd, short type, off_t start, off_t len, short whence)
{
    struct flock fl;

    fl.l_type = type;
    fl.l_start = start;
    fl.l_len = len;
    fl.l_whence = whence;

    return fcntl(fd, cmd, &fl);

}

int lock_set(int fd, short type, off_t start, off_t len, short whence)
{
    return lockbase(fd, F_SETLK, type, start, len, whence);
}

int lock_set_wait(int fd, short type, off_t start, off_t len, short whence)
{
    return lockbase(fd, F_SETLKW, type, start, len, whence);
}

int unlock(int fd, off_t start, off_t len, short whence)
{
    return lockbase(fd, F_SETLK, F_UNLCK, start, len, whence);
}

pid_t lock_get(int fd)
{
    struct flock fl;
    
    fcntl(fd, F_GETLK, &fl);

    return (fl.l_type == F_UNLCK) ? 0 : fl.l_pid;
}


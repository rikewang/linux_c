#include <unistd.h>

#ifndef _LOCKLIB_H_
#define _LOCKLIB_H_

int lockbase(int fd, int cmd, short type, off_t start, off_t len, short whence);
int lock_set(int fd, short type, off_t start, off_t len, short whence);
int lock_set_wait(int fd, short type, off_t start, off_t len, short whence);
pid_t lock_get(int fd);
int unlock(int fd, off_t start, off_t len, short whence);

#endif

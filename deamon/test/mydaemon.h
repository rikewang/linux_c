#ifndef _MYDAEMON_H_
#define _MYDAEMON_H_

#define MAX_OPEN_FD         8192
#define MAX_PID_SIZE        128
#define CPF_CLOSEXEC        01

int become_daemon(void);
int create_pidfile(const char *prog_name, const char *pidfile, int flags);

#endif

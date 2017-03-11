#ifndef _MY_DAEMON_H__
#define _MY_DAEMON_H__

#define BD_MAX_CLOSE                8192

#define BD_NO_MASK                  01
#define BD_NO_CHGDIR                02
#define BD_NO_REOPEN_STD_FDS        04
#define BD_NO_CLOSE_FILES           010

int tobe_daemon(int flags);

#endif

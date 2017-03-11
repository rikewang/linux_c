#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "my_daemon.h"

int tobe_daemon(int flags)
{
    int fd;
    int max_file_to_close;
    
    switch(fork()){
        case -1: return -1;
        case  0: break;
        default: _exit(EXIT_SUCCESS);
    }
   
    if(setsid() == -1){     /* become leader of new session */
        return -1;
    }

    switch(fork()){         /* ensure we are not session leader */
        case -1: return -1;
        case  0: break;
        default: _exit(EXIT_SUCCESS);
    }

    if(!(flags & BD_NO_CLOSE_FILES)){
        max_file_to_close = sysconf(_SC_OPEN_MAX);
        max_file_to_close = max_file_to_close == -1 ? BD_MAX_CLOSE : max_file_to_close;
        
        for(fd = 0; fd < max_file_to_close; fd++){
            close(fd);
        }
    }

    if(!(flags & BD_NO_MASK)){
        umask(0);
    }
    if(!(flags & BD_NO_CHGDIR)){
        chdir("/");
    }
    if(!(flags & BD_NO_REOPEN_STD_FDS)){
        close(STDIN_FILENO);
        fd = open("/dev/null", O_RDWR);
        if(fd != STDIN_FILENO)
            return -1;
        if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}

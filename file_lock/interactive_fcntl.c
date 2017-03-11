/*************************************************************************
	> File Name: interactive_fcntl.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sun 04 Dec 2016 06:53:11 PM CST
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_LINE 100

int main(int argc, char **argv)
{
    if(argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int status, fd, num_read, command;
    char line[MAX_LINE];
    char whence,cmd,lock; 
    long long len, st;
    struct flock fl;

    fd = open(argv[1], O_RDWR);
    if(fd < 0){
        perror("read()");
        exit(1);
    }

    printf("Enter ? get help:\n" );

    for(;;){
            printf("PID=%d> ", getpid());
            fflush(stdout);

            if(fgets(line, MAX_LINE, stdin) == NULL)
                exit(0);
            line[strlen(line) - 1] = '\0';

            if(*line == '\0')
                continue;
        if(line[0] == '?'){
            printf("cmd type start len where\n");
            printf("cmd: g s w\n");
            printf("opt: w r u\n");
            printf("start: 0\nlen: num\n wher: s e c\n");
            continue;
        }
        
        whence = 's';
        num_read = sscanf(line, "%c %c %lld %lld %c", &cmd, &lock, &st, &len, &whence);
        fl.l_start = st;
        fl.l_len = len;
        if(num_read < 4 || 
           strchr("gws", cmd) == NULL || 
           strchr("rwu", lock) == NULL || 
           strchr("sce", whence) == NULL){
            printf("Invalid cmd\n");
            continue;
        }
        
        command = (cmd == 'g') ? F_GETLK : (cmd == 's') ? F_SETLK : F_SETLKW;
        fl.l_type = (lock == 'r') ? F_RDLCK : (lock == 'w') ? F_WRLCK : F_UNLCK;
        fl.l_whence = (whence == 's') ? SEEK_SET : (whence == 'e') ? SEEK_END : SEEK_CUR;
        
        status = fcntl(fd, command, &fl);

        if(command == F_GETLK){
            if(status == -1){
                fprintf(stderr, "get lock info error\n");
                continue;
            } else {
                if(fl.l_type == F_UNLCK)
                    printf("[PID=%ld] lock can be placed\n", (long)getpid());
                else
                    printf("[PID=%ld] Denied by %s lock on %lld:%lld "
                          "(help by PID %ld)\n", (long)getpid(),
                           (fl.l_type == F_RDLCK) ? "READ" : "WRITE",
                          (long long)fl.l_start,
                          (long long)fl.l_len, (long)fl.l_pid);
            }
        } 
        else {
            if(status == 0){
                printf("[PID = %ld] %s\n", (long)getpid, (lock == 'u') ? "unlocked" : "got lock");
            } else if (errno == EAGAIN || errno == EACCES){
                printf("[PID = %ld] failed (incompatible lock)\n", (long)getpid());
            } else if(errno == EDEADLK){
                printf("[PID = %ld] failed (dead lock)\n", (long)getpid());
            } else {
                fprintf(stderr, "set lock error\n");
                exit(1);
            }
        }
    }


    exit(0);
}

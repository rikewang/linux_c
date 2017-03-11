#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#include "proto.h"


int main()
{
    key_t key;
    int msg_id;
    struct stu_st s1;

    key = ftok(PATHNAME, PROJID);
    if(key == -1){
        perror("ftok():");
        exit(1);
    }

     msg_id = msgget(key, 0);
     if(msg_id == -1){
         perror("msgget() ");
         exit(1);
     }

     strcpy(s1.name, "rikewang");
     s1.total = 100;
     s1.mtype = 1;
     if(msgsnd(msg_id, &s1, sizeof(s1)-sizeof(long), 0) < 0){
         printf("%d %s\n", errno, strerror(errno));
         perror("msgsnd() ");
         exit(1);
     }
    
     // msgctl(msg_id, IPC_RMID, NULL);
    
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#include "proto.h"

static int msg_id;

void hanler(int s)
{
    msgctl(msg_id, IPC_RMID, NULL);
    return;
}

int main()
{
    key_t key;
    struct stu_st s1;

    key = ftok(PATHNAME, PROJID);
    if(key == -1){
        perror("ftok():");
        exit(1);
    }

    signal(SIGINT, hanler);

    while(1)
    {

        msg_id = msgget(key, IPC_CREAT|0600);
        if(msg_id == -1){
            perror("msgget() ");
            exit(1);
        }

        if(msgrcv(msg_id, &s1, sizeof(s1)-sizeof(long), 0, 0) < 0){
            perror("msgrcv() ");
            exit(1);
        }

        printf("name: %s, total: %d\n", s1.name, s1.total);
    }


    exit(0);
}

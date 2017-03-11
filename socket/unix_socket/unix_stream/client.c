#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h>

#include "proto.h"


int main(int argc, char *argv[])
{
    int sd;
    struct sockaddr_un addr;
    char buf[BUFSIZE];

    if(argc != 2){
        fprintf(stderr, "Usage a.out <content_to_send>\n");
        exit(1);
    }

    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sd < 0){
        perror("socket");
        exit(1);
    }
    
    memset(&addr, '\0', sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

    if(connect(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("connect()");
        exit(1);
    }
    
    memset(buf, '\0', BUFSIZE);
    strcpy(buf, argv[1]);
    if(write(sd, buf, BUFSIZE) < 0){
        perror("write()");
        exit(1);
    }

    close(sd);
    exit(0);
}

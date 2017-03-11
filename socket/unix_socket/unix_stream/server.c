#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h>

#include "proto.h"

#define LISTEN_QUEUE_LEN 5

int main()
{
    int sd, newsd;
    int len;
    struct sockaddr_un addr;
    char buf[BUFSIZE];

    if((remove(SOCK_PATH) == -1) && (errno != ENOENT)){
        fprintf(stderr, "the sock path has problems\n");
    }

    memset(&addr, '\0', sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);
    
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sd < 0){
        perror("socket()");
        exit(1);
    }
    
    if(bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind()");
        exit(1);
    }

    if(listen(sd, LISTEN_QUEUE_LEN) < 0){
        perror("listen()");
        exit(1);
    }

    while(1)
    {
        newsd = accept(sd, NULL, 0);
        if(newsd == -1)
        {
            perror("accept()");
            exit(1);
        }

        if((len = read(newsd, buf, BUFSIZE)) <= 0){
            perror("read");
            break;
        }

        write(1, buf, len);        
        close(newsd);
    }

    close(sd);
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h>
#include <ctype.h>

#include "proto.h"

#define LISTEN_QUEUE_LEN 5

int main()
{
    int i;
    int sd, newsd;
    int len;
    struct sockaddr_un addr, caddr;
    char buf[BUFSIZE];
    socklen_t caddr_len;

    if((remove(SOCK_PATH) == -1) && (errno != ENOENT)){
        fprintf(stderr, "the sock path has problems\n");
    }

    memset(&addr, '\0', sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);
    
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sd < 0){
        perror("socket()");
        exit(1);
    }
    
    if(bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind()");
        exit(1);
    }

    caddr_len = sizeof(struct sockaddr_un);

    while(1)
    {
        len = recvfrom(sd, buf, BUFSIZE, 0, (struct sockaddr *)&caddr, &caddr_len);
        if(len <= 0){
            perror("read");
            break;
        }
    
        printf("recvfrom: %s \n", caddr.sun_path);
        for(i = 0; i < len; i++)
            buf[i] = toupper(buf[i]);

        write(1, buf, len);        
    }

    close(sd);
    exit(0);
}

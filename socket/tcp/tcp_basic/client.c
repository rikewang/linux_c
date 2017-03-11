#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "proto.h"


int main(int argc, char **argv)
{

    int sd;
    struct sockaddr_in server_ip;
    char buf[BUFSIZE];

    if(argc != 2){
        fprintf(stderr, "Usage a.out <server_ip>\n");
        exit(1);
    }
    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("socket(): ");
        exit(1);
    }

    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, argv[1], &server_ip.sin_addr.s_addr);

    if(connect(sd, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0)
    {
       perror("connect(): ");
       exit(1);
    }

    memset(buf, '\0', BUFSIZE);
    if(recv(sd, buf, BUFSIZE, 0) > 0){
        puts(buf);
    }
    
    close(sd);
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "proto.h"


int main(int argc, char **argv)
{

    int sd;
    struct sockaddr_in server_ip;
    char buf[BUFSIZE];
    int fd;
    int len;

    if(argc != 3){
        fprintf(stderr, "Usage a.out <server_ip> <file_to_send>\n");
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

    fd = open(argv[2], O_RDONLY);
    if(fd < 0){
        perror("read(): ");
        exit(1);
    }

    while(1){
        memset(buf, '\0', BUFSIZE);  
        len = read(fd, buf, BUFSIZE);
        if(len <= 0)
            break;
        if(send(sd, buf, BUFSIZE, 0) < 0){
            perror("send(): ");
            exit(1);
        }
    }

    close(sd);
    close(fd);
    exit(0);
}

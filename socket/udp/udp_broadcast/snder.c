#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "proto.h"


int main(int argc, char **argv)
{
    int sd;
    struct msg_st sndbuf;
    struct sockaddr_in server_ip;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0){
        perror("socket()");
        exit(1);
    }

    int flag = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0){
        perror("setsockopt():");
        exit(1);
    } 

    strcpy(sndbuf.name, "wxl");
    sndbuf.grade = 'A';
    sndbuf.math = htonl(rand()%100);

    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, "255.255.255.255", &server_ip.sin_addr.s_addr);

    if(sendto(sd, &sndbuf, sizeof(sndbuf), 0, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0){
        perror("sendto(): ");
        exit(1);
    }
    puts("ok");

    close(sd);

    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>

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
    
    struct ip_mreqn mreq;
    inet_pton(AF_INET, MGROUP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("eth0");

    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF , &mreq, sizeof(mreq)) < 0){
        perror("setsockopt():");
        exit(1);
    } 

    strcpy(sndbuf.name, "wxl");
    sndbuf.grade = 'A';
    sndbuf.math = htonl(rand()%100);

    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, "224.0.0.1", &server_ip.sin_addr.s_addr);

    if(sendto(sd, &sndbuf, sizeof(sndbuf), 0, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0){
        perror("sendto(): ");
        exit(1);
    }
    puts("ok");

    close(sd);

    exit(0);
}

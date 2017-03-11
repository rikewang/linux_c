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
    int bufsize;
    struct msg_st *sndbufptr;
    struct sockaddr_in server_ip;

    if(argc != 3){
        fprintf(stderr, "usage a.out <server_ip_addr> <username>\n");
        exit(1);
    }
    
    
    if (strlen(argv[2]) > NAMEMAX){
         fprintf(stderr, "username too long\n");
         exit(1);
    }
    

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0){
        perror("socket()");
        exit(1);
    }

    // bind();

    bufsize = sizeof(struct msg_st) + strlen(argv[2]);
    sndbufptr = (struct msg_st *)malloc(bufsize);

    strcpy(sndbufptr->name, argv[2]);
    sndbufptr->grade = 'A';
    sndbufptr->math = htonl(rand()%100);

    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, argv[1], &server_ip.sin_addr.s_addr);

    if(sendto(sd, sndbufptr, bufsize, 0, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0){
        perror("sendto(): ");
        exit(1);
    }
    puts("ok");

    close(sd);

    exit(0);
}

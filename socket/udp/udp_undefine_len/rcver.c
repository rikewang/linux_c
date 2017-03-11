#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "proto.h"


int main()
{
    int sd;
    int bufsize;
    struct sockaddr_in server_ip, client_ip;
    struct msg_st *rbufptr;
    socklen_t client_ip_len;
    char ipstr[IPSTRSIZE];

    /*
     * STEP 1:
     *  create socket file;
     *  get socket fd;
     *
     * */
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0){
        perror("socket");
        exit(1);
    }

    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, "0.0.0.0", &server_ip.sin_addr.s_addr);

    /*
     * STEP 2:
     *  bind addr and port; 
     *  addr need convert string to int32_t;
     *  port need convert network format by using htons;
     *
     * */

    if(bind(sd, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0){
       perror("bind()");
       exit(1);
    }

    /* do not forget initialize client_ip_len  */
    client_ip_len = sizeof(client_ip);

    /*
     * STEP 3:
     *  recv msg from client
     *
     * */

    bufsize = sizeof(struct msg_st) +  NAMEMAX;
    rbufptr = (struct msg_st *)malloc(bufsize);
    if(rbufptr == NULL){
        perror("malloc(): ");
        exit(1);
    }

    while(1)
    {        
        recvfrom(sd, rbufptr, bufsize, 0, (struct sockaddr *)&client_ip, &client_ip_len);
        inet_ntop(AF_INET, &client_ip.sin_addr, ipstr,  IPSTRSIZE);
        printf("-----Connected by %s : %d\n", ipstr, ntohs(client_ip.sin_port));
        printf("Name: %s\n",   rbufptr->name);
        printf("Grade: %c\n",  rbufptr->grade);
        printf("Math: %d\n\n", ntohl(rbufptr->math));
    }

    close(sd);

    exit(0);
}

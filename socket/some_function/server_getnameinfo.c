/*************************************************************************
	> File Name: server_getnameinfo.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Tue 29 Nov 2016 05:30:45 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE     1024

int main(int argc, char **argv)
{
    struct addrinfo  hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUFSIZE];

    if(argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if(s != 0) {
        fprintf(stderr, "getaddrinfo: %s \n", gai_strerror(s));
        exit(1);
    }
    


    for(rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1)
            continue;
        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(sfd);
    }

    if(rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        exit(1);
    }

    freeaddrinfo(result);

    for(;;) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buf, BUFSIZE, 0, 
                         (struct sockaddr *) &peer_addr, &peer_addr_len);
        if(nread == -1)
            continue;

        char host[NI_MAXHOST], service[NI_MAXSERV];

        s = getnameinfo((struct sockaddr *) &peer_addr,
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);
        if(s == 0)
            printf("recv %ld bytes from %s: %s\n", 
                   (long)nread, host, service);
        else 
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
        if(sendto(sfd, buf, nread, 0, 
                  (struct sockaddr *) &peer_addr,
                   peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");

    }


    exit(0);
}

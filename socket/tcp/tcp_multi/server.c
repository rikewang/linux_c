#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "proto.h"

static void handler_conn(int sd)
{

    int len;
    char buf[BUFSIZE];

    memset(buf, '\0', BUFSIZE);
    len = sprintf(buf, FMT_STAMP, (long long)time(NULL));

    if(send(sd, buf, len, 0) < 0){
        perror("send(): ");
        exit(1);
    }

}

int main()
{
    int sd, newsd;
    struct sockaddr_in server_ip, client_ip;
    socklen_t client_ip_len;
    char ipstr[IPSTRSIZE];
    pid_t pid;

    sd = socket(AF_INET, SOCK_STREAM, 0); /*0 means default IPPROTO_TCP */
    if(sd < 0){
        perror("socket");
        exit(1);
    }

    int flag = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0 ){
        perror("setsockopt(): ");
        exit(1);
    }

    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, "0.0.0.0", &server_ip.sin_addr.s_addr);

    if(bind(sd, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0){
        perror("bind()");
        exit(1);
    }

    if(listen(sd, 200) < 0){
        perror("listen():");
        exit(1);
    } 

    client_ip_len = sizeof(client_ip);
    
    while(1){
        newsd = accept(sd, (struct sockaddr *)&client_ip, &client_ip_len);
        if(newsd < 0){
            perror("accept(): ");
            exit(1);
        }

        pid = fork();
        if(pid < 0){
            perror("fork(): ");
            exit(1);
        }
        if(pid == 0 ){
            close(sd);
            inet_ntop(AF_INET, &client_ip.sin_addr.s_addr, ipstr, IPSTRSIZE);
            printf("Connected by %s: %d\n", ipstr, ntohs(client_ip.sin_port));
            handler_conn(newsd);
            close(newsd);
        }   
        close(newsd);
    }

    close(sd);


    exit(0);
}

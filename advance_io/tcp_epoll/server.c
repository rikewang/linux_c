#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#include "proto.h"

#define MAX_SOCKET_NUM 1024

int setnoblocking(int fd);
void addfd(int epollfd, int fd, int flag);
void et(struct epoll_event * events, int n, int epollfd, int sd);
void lt(struct epoll_event * events, int n, int epollfd, int sd);

int main()
{
    int sd, newsd;
    struct sockaddr_in server_ip, client_ip;
    socklen_t client_ip_len;
    char ipstr[IPSTRSIZE];
    int epollfd;
    struct epoll_event events[MAX_SOCKET_NUM];
    int is_et;
    int rst;

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

    epollfd = epoll_create(10);
    if(epollfd < 0){
        perror("epoll_create(): ");
        exit(1);
    }

    is_et = 1;
    addfd(epollfd, sd, is_et);

    while(1){
        rst = epoll_wait(epollfd, events, MAX_SOCKET_NUM, -1);
        if(rst == -1){
            perror("epoll_wait");
            exit(1);
        }
        // et(events, rst, epollfd, sd);
        lt(events, rst, epollfd, sd);
    }
    
    close(sd);
    exit(0);
}

int setnoblocking(int fd)
{
    int old_opt = fcntl(fd, F_GETFL);
    int new_opt = old_opt | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_opt);
    return new_opt;
}


void addfd(int epollfd, int fd, int flag)
{
    struct epoll_event event;
    memset(&event, '\0', sizeof(struct epoll_event));
    event.data.fd = fd;
    event.events = EPOLLIN;

    if(flag){
        event.events |= EPOLLET;
    }
    
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnoblocking(fd);
}

void lt(struct epoll_event * events, int n, int epollfd, int sd)
{
    int i;
    int fd, newsd;
    char buf[BUFSIZE];
    int rst;
    socklen_t client_ip_len;
    struct sockaddr_in client_ip;
    char ipstr[IPSTRSIZE];

    for(i = 0; i < n; i++){
        fd = events[i].data.fd;
        if(fd == sd){
            client_ip_len = sizeof(client_ip);
            newsd = accept(sd, (struct sockaddr *)&client_ip, &client_ip_len);
            if(newsd < 0){
                perror("accept(): ");
                exit(1);
            }

            inet_ntop(AF_INET, &client_ip.sin_addr.s_addr, ipstr, IPSTRSIZE);
            printf("Connected by %s: %d\n", ipstr, ntohs(client_ip.sin_port));
            addfd(epollfd, newsd, 0);
        }
        else if(events[i].events & EPOLLIN)
        {
            // printf("LT once \n");
            memset(buf, '\0', BUFSIZE);
            rst = recv(fd, buf, BUFSIZE, 0);
            if(rst > 0){
                printf("recv %d byts, data: %s \n", rst, buf);
            }
        }
        else {
            printf("something happened\n");
        }
    }
}

void et(struct epoll_event * events, int n, int epollfd, int sd)
{
    int i;
    int fd, newsd;
    char buf[BUFSIZE];
    int rst;
    socklen_t client_ip_len;
    struct sockaddr_in client_ip;
    char ipstr[IPSTRSIZE];

    for(i=0; i < n; i++){
        fd = events[i].data.fd;
        if(fd == sd){
            client_ip_len = sizeof(client_ip);
            newsd = accept(sd, (struct sockaddr *)&client_ip, &client_ip_len);
            if(newsd < 0){
                perror("accept(): ");
                exit(1);
            }

            inet_ntop(AF_INET, &client_ip.sin_addr.s_addr, ipstr, IPSTRSIZE);
            printf("Connected by %s: %d\n", ipstr, ntohs(client_ip.sin_port));
            addfd(epollfd, newsd, 1);
        }
        else if(events[i].events & EPOLLIN)
        {
            printf("ET once \n");
            while(1){
                memset(buf, '\0', BUFSIZE);
                rst = recv(fd, buf, BUFSIZE, 0);
                if(rst > 0){
                    printf("recv %d byts, data: %s \n", rst, buf);
                }
                else if (rst < 0)
                {
                    if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                        continue;
                    }
                }
                else {
                    close(fd);
                    break;
                }
            }
        }
        else {
            printf("something happened\n");
        }
    }
}


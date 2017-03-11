/*************************************************************************
	> File Name: sendfile.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sat 03 Dec 2016 07:43:40 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>

#define BUFSIZE 1024

int main(int argc, char **argv)
{
    int sd, newsd;
    int fd;
    struct sockaddr_in server_ip;
    char *http_header = "HTTP/1.1 200 OK\r\n\r\n";
    int numchars;
    char buf[BUFSIZE];
    int flag = 1;
    struct stat file_st;

    if(argc != 2){
        fprintf(stderr, "Usage %s <filename> \n", argv[0]);
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("read");
        exit(1);
    }

    if(fstat(fd, &file_st) < 0){
        perror("fstat:");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("socket:");
        exit(1);
    }

    setsockopt(sd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    memset(&server_ip, 0, sizeof(server_ip));
    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(8080);
    server_ip.sin_addr.s_addr = htonl(INADDR_ANY); 

    if(bind(sd, (struct sockaddr *) &server_ip, sizeof(server_ip)) < 0){
        perror("bind");
        exit(1);
    } 

    if(listen(sd, 100) < 0){
        perror("listen");
        exit(1);
    }

    while(1){
        lseek(fd, 0, SEEK_SET);
        flag = 1;
        setsockopt(newsd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));

        newsd = accept(sd, NULL, 0);
        if (newsd < 0){
            perror("accept");
            exit(1);
        }

        send(newsd, http_header, strlen(http_header), 0);
        sendfile(newsd, fd, NULL, file_st.st_size);
        close(newsd);
    }

    close(fd);
    close(sd);
    exit(0);
}

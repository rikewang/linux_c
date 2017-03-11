#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


#define BUFSIZE     1024

int main(int argc, char **argv)
{

    int sd;
    int len;
    struct sockaddr_in server_ip;
    char buf[BUFSIZE];
    char *request = "GET /9.jpg\r\n\r\n";

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
    server_ip.sin_port = htons(80);
    inet_pton(AF_INET, argv[1], &server_ip.sin_addr.s_addr);

    if(connect(sd, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0)
    {
       perror("connect(): ");
       exit(1);
    }

    
    if(send(sd, request, strlen(request), 0) < 0){
        perror("send()");
        exit(1);
    }

    memset(buf, '\0', BUFSIZE);
    while((len = read(sd, buf, BUFSIZE)) > 0){
        write(1, buf, len);
    }
    
    close(sd);
    exit(0);
}

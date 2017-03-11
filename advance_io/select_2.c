#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <sys/select.h>

#define BUFSIZE     1024
#define IPSTRSIZE   32

int main(int argc, char **argv)
{
    int sd, newsd;
    struct sockaddr_in server_ip, client_ip;
    socklen_t client_ip_len;
    char ipstr[IPSTRSIZE];
    char buf[BUFSIZE];
    fd_set rd_fs, expt_fs;
    int len;

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
    server_ip.sin_port = htons(atoi(argv[1]));
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
    
    newsd = accept(sd, (struct sockaddr *)&client_ip, &client_ip_len);
    if(newsd < 0){
        perror("accept(): ");
        exit(1);
    }


    while(1){
        FD_ZERO(&rd_fs);
        FD_ZERO(&expt_fs);
        memset(buf, '\0', BUFSIZE);

        FD_SET(newsd, &rd_fs);
        FD_SET(newsd, &expt_fs);
        
        inet_ntop(AF_INET, &client_ip.sin_addr.s_addr, ipstr, IPSTRSIZE);
        printf("Connected by %s: %d\n", ipstr, ntohs(client_ip.sin_port));
       
        if(select(newsd+1, &rd_fs, NULL, &expt_fs, NULL) < 0){
            fprintf(stderr, "error @select()\n");
            exit(1);
        }

        if(FD_ISSET(newsd, &rd_fs)){
            len = recv(newsd, buf, BUFSIZE, 0);
            if(len < 0){
                fprintf(stderr, "recv error\n");
                break;
            }
            printf("common data: %s\n", buf);
        }

        if(FD_ISSET(newsd, &expt_fs)){
            len = recv(newsd, buf, BUFSIZE, MSG_OOB);
            if(len < 0){
                fprintf(stderr, "recv error\n");
                break;
            }
            printf("exceptional data: %s\n", buf);
        }
    }
    
    close(newsd);
    close(sd);
    exit(0);
}

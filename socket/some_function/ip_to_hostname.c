/*************************************************************************
	> File Name: ip_to_hostname.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Wed 30 Nov 2016 12:59:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *result_ptr;
    int rst;

    if(argc != 2){
        fprintf(stderr, "Usage: %s IPADDR\n", argv[0]);
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME | AI_NUMERICHOST;

    rst = getaddrinfo(argv[1], NULL, &hints, &result);
    if(rst != 0){
        fprintf(stderr, "error in getaddrinfo %s\n", gai_strerror(rst));
        exit(1);
    }
    for(result_ptr = result; result_ptr != NULL; result_ptr=result_ptr->ai_next){
        char hostname[NI_MAXHOST];
        rst = getnameinfo(result_ptr->ai_addr, result_ptr->ai_addrlen,
                          hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD);
        if(rst != 0){
            fprintf(stderr, "error in getnameinfo %s\n", gai_strerror(rst));
            exit(1);
        }
        else{
            printf("hostname: %s \n", hostname);
        }
    }

    freeaddrinfo(result);

    exit(0);
}

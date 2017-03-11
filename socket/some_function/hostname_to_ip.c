/*************************************************************************
	> File Name: hostname_to_ip.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Wed 30 Nov 2016 12:42:52 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv)
{
    int i;
    struct addrinfo hints;
    struct addrinfo *result, *result_ptr;
    int rst;

    if(argc != 2)
    {
        fprintf(stderr , "Usage %s hostname\n", argv[0]);
        exit(0);
    }


    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;

    rst = getaddrinfo(argv[1], NULL, &hints, &result);

    if(rst != 0){
        fprintf(stderr, "getaddrinfo: %s \n", gai_strerror(rst));
        exit(1);
    }

    for(result_ptr = result; result_ptr != NULL; result_ptr=result_ptr->ai_next) {
        char hostname[NI_MAXHOST] = "";
        rst = getnameinfo(result_ptr->ai_addr, result_ptr->ai_addrlen,
                          hostname, sizeof(hostname), NULL, 0 , NI_NUMERICHOST);
        if(rst != 0){
            fprintf(stderr, "error @getnameinfo: %s\n", gai_strerror(rst));
            continue;
        }
        else{
            printf("IPADDR: %s\n", hostname);
        }
    }
    
    freeaddrinfo(result);
    exit(0);
}

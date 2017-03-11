/*************************************************************************
	> File Name: select.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Sun 27 Nov 2016 09:54:51 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int i;
    int num_read;
    int fd;
    int rst;
    struct timeval timeout;
    struct timeval *pto;
    char buf[10];
    int max_nfds = 0;
    fd_set rd_fs, wr_fs;

    if(argc <= 2){
        fprintf(stderr, "Usgae: a.out [-|timeout] fdnum + <r|w>\n");
        exit(1);
    }

    if(strcmp(argv[1],"-") == 0){
        pto = NULL;
    } else {
        timeout.tv_sec = atoi(argv[1]);
        timeout.tv_usec = 0;
        pto = &timeout;
    }

    FD_ZERO(&rd_fs);
    FD_ZERO(&wr_fs);
    memset(buf, '\0', 10);

    for(i = 2; i < argc; i++){
        num_read =  sscanf(argv[i], "%d%2[rw]", &fd, buf);
        if(num_read != 2){
           fprintf(stderr, "error format");
            exit(1);
        }

        max_nfds = (fd > max_nfds ? fd+1 : max_nfds);
        
        if(strchr(buf,'r') != NULL)
            FD_SET(fd, &rd_fs);
        if(strchr(buf, 'w') != NULL)
            FD_SET(fd, &wr_fs);
    }

    rst = select(max_nfds, &rd_fs, &wr_fs, NULL, pto);
    
    if(rst > 0){
        for(fd = 0; fd < max_nfds; fd++){
            printf("%d ", fd);
            if(FD_ISSET(fd, &rd_fs)){
                printf("%s ", "readable");
            }
            if(FD_ISSET(fd, &wr_fs)){
                printf("%s ", "writable");
            }
            puts("");
        }
    } else if(rst == 0){
        printf("time left: %ld.%03ld\n", (long)pto->tv_sec, ((long)pto->tv_usec)/10000);
    } else {
        fprintf(stderr, "error @select\n");
    }

    exit(0);
}

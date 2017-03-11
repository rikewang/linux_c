/*************************************************************************
	> File Name: poll.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Mon 28 Nov 2016 10:06:19 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv)
{
    int i;
    int rst;
    int pipe_total_num, rand_write_pipe_num;
    int rand_pipe;
    int (*pipe_ptr)[2];
    struct pollfd *poll_set;

    if(argc != 3){
        fprintf(stderr, "Uages: a.out pipe_total_num rand_write_pipe_num\n");
        exit(1);
    }

    pipe_total_num = atoi(argv[1]);
    rand_write_pipe_num = atoi(argv[2]);
   
    if(pipe_total_num < rand_write_pipe_num){
        fprintf(stderr, "invalid args\n");
        exit(1);
    }

    pipe_ptr = calloc(pipe_total_num, sizeof(int [2]));
    poll_set = calloc(pipe_total_num, sizeof(struct pollfd));

    for(i = 0; i < pipe_total_num; i++){
        if(pipe(pipe_ptr[i]) < 0){
            perror("pipe()");
            exit(1);
        }
    }

    srandom((long)time(NULL));
    for(i = 0; i < rand_write_pipe_num; i++){
        rand_pipe = random() % pipe_total_num;
        printf("write_pipe_index: %d, read_pipe_index: %d\n", pipe_ptr[rand_pipe][1], pipe_ptr[rand_pipe][0]);
        write(pipe_ptr[rand_pipe][1], "r", 1);
    }

    for(i = 0; i < pipe_total_num; i++){
        poll_set[i].fd = pipe_ptr[i][0];
        poll_set[i].events = POLLIN;
    }

    rst = poll(poll_set, pipe_total_num, -1);
    if(rst ==  -1){
        fprintf(stderr, "error @poll()\n");
        exit(1);
    }

    printf("\n------------------------\n");
    for(i = 0; i < pipe_total_num; i++)
    {
        if(poll_set[i].revents & POLLIN)
            printf("readable: %d %3d\n", i, poll_set[i].fd);
    }

    exit(0);
}

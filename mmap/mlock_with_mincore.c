/*************************************************************************
	> File Name: mlock_with_mincore.c
	> Author: rikewang 
	> Mail: 1138122262@qq.com 
	> Created Time: Mon 05 Dec 2016 04:49:00 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

static void display_mem_status(char *addr, int page_num)
{
    int i;
    unsigned char *vec;
    int page_size;
    long array_len;

    page_size = sysconf(_SC_PAGE_SIZE);
    array_len = (page_num + page_size - 1)/page_size;
    vec = malloc(array_len);
    if(vec == NULL){
        perror("malloc");
        exit(1);
    }

    mincore(addr, page_num, vec);

    for(i = 0; i < array_len; i++){
        if(i % 64 == 0)
            printf("%s%10p: ", (i == 0) ? "" : "\n", addr+(i*page_size));
        printf("%c", (vec[i] & 1) ? '*' : '.');
    }
    printf("\n");
    free(vec);
}

int main(int argc, char *argv[])
{
    int page_size;
    if(argc != 4) {
        fprintf(stderr, "Usage: %s page_num step lock_len \n");
        exit(1);
    }

    page_size =sysconf(_SC_PAGE_SIZE);
    if(page_size == -1) {
        perror("sysconf:");
        exit(1);
    }

    int page_num = atoi(argv[1]) * page_size;
    int step = atoi(argv[2]) * page_size;
    int lock_len = atoi(argv[3]) * page_size;
    char *addr;
    int j;

    addr = mmap(NULL, page_num, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED){
        perror("mmap:");
        exit(1);
    }

    printf("Before mlock: \n");
    display_mem_status(addr, page_num);

    for(j = 0; j + lock_len <= page_num; j += step)
        mlock(addr+j, lock_len);

    printf("After mlock: \n");
    display_mem_status(addr, page_num);

    munmap(addr, page_num);
    exit(0);
}

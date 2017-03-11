#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd;
    struct stat st;
    void *addr;

    if(argc != 2 ){
        fprintf(stderr, "usage: ..\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if(fd  < 0){
        perror("open()");
        exit(1);
    }

    fstat(fd, &st);

    addr = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        perror("mamp:");
        exit(1);
    }

    if(st.st_size != write(1, addr, st.st_size)){
        perror("write");
        exit(1);
    }

    if(munmap(addr, st.st_size) == -1 ){
        perror("munmap");
        exit(1);
    }

    exit(0);
}

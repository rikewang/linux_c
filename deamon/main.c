#include <stdio.h>
#include <unistd.h>

#include "my_daemon.c"

int main()
{
    printf("good luck\n");
    tobe_daemon(0);
    sleep(1000);
}

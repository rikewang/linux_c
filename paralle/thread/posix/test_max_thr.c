#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *func(void *p)
{
    
}

int main()
{
    pthread_t tid;
    int i, err;

    for(i = 0; ; i++){
        err = pthread_create(&tid, NULL, func, NULL);
        if(err)
        {
            break;
        }
    }

    printf("%d\n", i);
    exit(0);
}

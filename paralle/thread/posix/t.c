#include <stdio.h>

void prime(int n)
{
    int i, flag;

        for(i = 2, flag = 0; (i<= n/2)&&(!flag); i++)
        {
            if((n % i) == 0)
                flag = 1;
        }
        if(!flag)
            printf(" --> %d\n", n);
}

int main()
{

int i;

for(i=3; i<=100; i++)
    prime(i);

}

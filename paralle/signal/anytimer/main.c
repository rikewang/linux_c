#include <stdio.h>
#include <stdlib.h>

#include "anytimer.h"

void f1(char *arg)
{
	printf("f1: %s\n", arg);
}


void f2(char *arg)
{
	printf("f2: %s\n", arg);
}


int main()
{
	puts("starting....");

	// init_job();

	add_job(1, f1, "aaa");
	add_job(10, f2, "bbb");
	add_job(3, f1, "ccc");
	init_job();

	puts("ending....");

}
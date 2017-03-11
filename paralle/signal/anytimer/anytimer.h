#include <stdio.h>
#include <stdlib.h>

#define SETMAX	1024

typedef void (*func)(char *);
enum stat {running, finished, cancled};

struct timer {
	int sec;
	func worker;
	char *arg;
	enum stat process_stat;
};

void init_job(void);

int add_job(int, func, char *);
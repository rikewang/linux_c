#ifndef _MYSEM_H__
#define _MYSEM_H__

#include <pthread.h>

struct mysem_st
{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t	cond;
};


struct mysem_st * mysem_init(int value);

int mysem_add(struct mysem_st *, int value);

int mysem_sub(struct mysem_st *, int value);

int mysem_destroy(struct mysem_st *);

#endif
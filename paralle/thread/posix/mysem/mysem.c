#include <malloc.h>

#include "mysem.h" 

pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

struct mysem_st * mysem_init(int value)
{
	struct mysem_st *me;

	if((me = malloc(sizeof(*me))) == NULL)
		return NULL;
	me->value = value;
	pthread_mutex_init(&(me->mutex), NULL);
	pthread_cond_init(&(me->cond), NULL);

	return me;
}

int mysem_add(struct mysem_st *me, int value)
{
	if(value <= 0 || !me)
		return 0;

	pthread_mutex_lock(&(me->mutex));
	me->value += value;
	pthread_cond_broadcast(&(me->cond));
	pthread_mutex_unlock(&(me->mutex));

	return value;
}


int mysem_sub(struct mysem_st *me, int value)
{
	if(value <= 0 || !me)
		return 0;

	pthread_mutex_lock(&(me->mutex));
	while(me->value < value)
		pthread_cond_wait(&(me->cond), &(me->mutex));

	me->value -= value;
	pthread_mutex_unlock(&(me->mutex));

	return value;	
}

int mysem_destroy(struct mysem_st *me)
{
	if(!me)
		return 0;
	pthread_mutex_destroy(&(me->mutex));
	pthread_cond_destroy(&(me->cond));
	free(me);

	return 1;
}
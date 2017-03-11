#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <malloc.h>

#include "mybtf.h"

#define BTFSET_MAX	1024

typedef void (*sighandler_t)(int);

static struct btf *btfset[BTFSET_MAX];
static int inited = 0;
static sighandler_t original_sig;

static int get_notused_index(void)
{
	int i;
	for(i = 0; i < BTFSET_MAX; i++)
	{
		if(btfset[i] == 0)
			return i;
	}

	return -1;
}


void alrm_handler(int s)
{
	int i;

	for(i = 0; i < BTFSET_MAX; i++)
	{
		if(btfset[i])
		{
			btfset[i]->token += btfset[i]->cps;
			if(btfset[i]->token > btfset[i]->brust)
				btfset[i]->token = btfset[i]->brust;
		}
	}

	alarm(1);

	return;
}

static void module_install(void)
{
	original_sig = signal(SIGALRM, alrm_handler);
	alarm(1);	

	atexit(module_uninstall);
}

static void module_uninstall(void)
{
	int i;

	signal(SIGALRM, original_sig);
	alarm(0);

	for(i = 0; i < BTFSET_MAX; i++)
	{
		if(btfset[i])
			free(btfset[i]);
	}
}

struct btf *btf_init(int cps, int brust)
{
	struct btf *me;
	int index;

	if(!inited)
	{
		module_install();
		inited = 1;
	}

	if((me =  malloc(sizeof(me))) == NULL)
		return NULL;

	me->cps = cps;
	me->brust = brust;
	me->token = 0;
	
	if((index = get_notused_index()) == -1)
		return NULL;
	
	me->index = index;
	btfset[index] = me;

	return me;
}

int btf_fetch(struct btf *me, int want_size)
{
	int return_val;

	if(want_size <= 0)
		return -EINVAL;
	
	while(me->token <= 0)
		pause();

	return_val = me->token > want_size ? want_size : me->token;

	if(return_val > me->brust)
		return_val = me->brust;
	
	me->token -= return_val;

	return return_val;
}

int btf_returnback(struct btf *me, int return_size)
{
	if(return_size <= 0)
		return -EINVAL;	

	me->token += return_size;

	if(me->token > me->brust)
		me->token = me->brust;

	return return_size;
}

void btf_destroy(struct btf *me)
{
	if(me)
	{
		if((me->index >= 0) && (me->index < BTFSET_MAX))
		{
			btfset[me->index] = 0;
			free(me);
		}
	}
}

#ifndef _MYBTF_H_
#define _MYBTF_H_

struct btf {
	int token;
	int cps;
	int brust;
	int index;
};

struct btf *btf_init(int, int);

int btf_fetch(struct btf *, int);

int btf_returnback(struct btf *, int);

void btf_destroy(struct btf *);

#endif
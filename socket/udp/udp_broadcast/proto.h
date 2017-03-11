#ifndef __PROTO_H__
#define __PROTO_H__

#define NAMESIZE  11
#define PORT      "8090"
#define IPSTRSIZE 32

/*
* 1. 对齐
* tell gcc not duiqi
* */

struct msg_st {
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint8_t grade;
}__attribute__((packed));



#endif

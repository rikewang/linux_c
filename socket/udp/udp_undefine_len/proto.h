#ifndef __PROTO_H__
#define __PROTO_H__

/* 512 byt is recommandered by UDP,
* 8 is the head of UDP
* 9 is the struct size
* */
#define NAMEMAX   (512 - 8 - 9) 
#define PORT      "8090"
#define IPSTRSIZE 32

/*
* 1. 对齐
* tell gcc not duiqi
* */

struct msg_st {
    uint32_t math;
    uint8_t grade;
    uint8_t name[1];
}__attribute__((packed));



#endif

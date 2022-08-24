#include <stdio.h>
#include <stdbool.h>
#include "ringbuf.h"

#define RB_LEN 5

static RBN RBN_pool[RB_LEN] = {};
static RBN *head = NULL;
static bool init = false;

void init_pool() {
	if(init) return;
    for(int i = 0; i < RB_LEN; i++){
        RBN_pool[i].NO = i;
        RBN_pool[i].next = (i == RB_LEN - 1) ? &RBN_pool[0] : &RBN_pool[i+1];
    }
    head = RBN_pool;
    init = true;
}

void insert(uint32_t instr, uint32_t pc) {
    head->instr = instr;
    head->pc = pc;
    head = head->next;
}

void display_pool() {
    RBN *now = head;
    while(now->next != head){
        if(now->instr != 0) printf("\t0x%08x:\t\t%08x\n", now->pc, now->instr);
        now = now->next;
    }
    printf("-->\t0x%08x:\t\t%08x\n", now->pc, now->instr);
}

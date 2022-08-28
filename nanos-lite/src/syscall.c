#include <common.h>
#include "syscall.h"

void sys_exit(Context *c) {
  printf("EXIT\n");
  halt(c->GPRx);
}

void sys_yield(Context *c) {
  printf("CALL YIELD\n");
  yield();
  c->GPRx = 0;
}

void sys_write(Context *c, int fd, void *buf, size_t count) {
  printf("CALL WRITE\n");
  if(fd == 1 || fd == 2) {
    for(int i = 0; i < count; i++) putch(*((char*)buf + i));
    c->GPRx = count;
    return;
  }
  c->GPRx = -1;
}

void sys_brk(Context *c, intptr_t addr) {
  c->GPRx = 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  
  switch (a[0]) {
    case 0: sys_exit(c);break;
    case 1: sys_yield(c);break;
    case 4: sys_write(c, a[1], (void*)a[2], a[3]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

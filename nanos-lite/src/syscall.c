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

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  
  switch (a[0]) {
    case 0: sys_exit(c);break;
    case 1: sys_yield(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

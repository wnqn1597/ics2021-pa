#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    //case 0: halt(0);break;
    //icase 1: yield();break;
    default: printf("hhhh\n");panic("Unhandled syscall ID = %d", a[0]);
  }
}

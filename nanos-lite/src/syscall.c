#include <common.h>
#include "syscall.h"
#include <fs.h>

void* get_finfo(int index, int property);

int fs_open(const char *pathname, int flags, int mode);
int fs_write(int fd, const void *buf, size_t len);
int fs_read(int fd, void *buf, size_t len);
int fs_close(int fd);
int fs_lseek(int fd, size_t offset, int whence);


void sys_exit(Context *c) {
  printf("EXIT\n");
  halt(c->GPRx);
}

void sys_yield(Context *c) {
  printf("CALL YIELD\n");
  yield();
  c->GPRx = 0;
}

void sys_brk(Context *c, intptr_t addr) {
  //printf("CALL BRK\n");
  c->GPRx = 0;
}

void sys_open(Context *c, const char *pathname, int flags, int mode) {
  //printf("CALL OPEN\n");
  c->GPRx = fs_open(pathname, flags, mode);
}

void sys_write(Context *c, int fd, void *buf, size_t count) {
  //printf("CALL WRITE\n");
  WriteFn wfunc = get_finfo(fd, 4);
  if(wfunc != NULL) c->GPRx = wfunc(buf, 0, count);
  else c->GPRx = fs_write(fd, buf, count);
}

void sys_read(Context *c, int fd, void *buf, size_t count) {
  //printf("CALL READ\n");
  ReadFn rfunc = get_finfo(fd, 3);
  if(rfunc != NULL) c->GPRx = rfunc(buf, 0, count);
  else c->GPRx = fs_read(fd, buf, count);
}

void sys_close(Context *c, int fd) {
  //printf("CALL CLOSE\n");
  c->GPRx = fs_close(fd);
}

void sys_lseek(Context *c, int fd, size_t offset, int whence) {
  //printf("CALL LSEEK\n");
  c->GPRx = fs_lseek(fd, offset, whence);
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
    case 2: sys_open(c, (char*)a[1], a[2], a[3]);break;
    case 3: sys_read(c, a[1], (void*)a[2], a[3]);break;
    case 4: sys_write(c, a[1], (void*)a[2], a[3]);break;
    case 7: sys_close(c, a[1]);break;
    case 8: sys_lseek(c, a[1], a[2], a[3]);break;
    case 9: sys_brk(c, a[1]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

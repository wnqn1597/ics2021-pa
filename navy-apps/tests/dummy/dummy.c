//#include <stdint.h>
//#include <stdio.h>
#include <malloc.h>

#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#define SYS_yield 1
//extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  //return _syscall_(SYS_yield, 0, 0, 0);
	char *p = sbrk(4);
	return 0;
}

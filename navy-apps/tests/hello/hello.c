#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 10000) {
      printf("Hello World from Navy-apps for the %dth time!\n", i ++);
      j = 0;
			_syscall_(1, 0, 0, 0);
    }
  }
  return 0;
}

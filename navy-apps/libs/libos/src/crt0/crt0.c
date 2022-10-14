#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;

// The Following codes work with arguments
void call_main(uintptr_t *args) {
  exit(main(*args, args+1, args+(*args)+2));
  assert(0);
}

// The Following codes work without arguments
/*
void call_main(uintptr_t *args){
	char *empty[] = {NULL};
	environ = empty;
	exit(main(0, empty, empty));
	assert(0);
}
*/

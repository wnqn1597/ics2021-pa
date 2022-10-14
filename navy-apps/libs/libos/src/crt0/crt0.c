#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;

void load_argv_envp(char *arg_buf[], char *env_buf[], void *start){

}


void call_main(uintptr_t *args) {
  printf("call main: argc = %d\n", *args);
  
	uint32_t* ustart = (uint32_t*)args;
	char *ptr;
  ustart++;
  for(int i = 0; *ustart; i++){
      ptr = (char*)*ustart;
      printf("arg %d: %s\n", i, ptr);
      ustart++;
  }
  ustart++;
  for(int i = 0; *ustart; i++){
      ptr = (char*)*ustart;
      printf("env %d: %s\n", i, ptr);
      ustart++;
  }
	char *empty[] = {NULL};
	environ = empty;
	//printf("args:\n");
  //for(int i = 0; arg_buf[i] != NULL; i++) printf("%s\n", arg_buf[i]);
  //printf("envs:\n");
  //for(int i = 0; env_buf[i] != NULL; i++) printf("%s\n", env_buf[i]);
  exit(main(0, empty, empty));
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

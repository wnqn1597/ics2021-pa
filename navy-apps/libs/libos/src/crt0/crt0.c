#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

void load_argv_envp(char *arg_buf[], char *env_buf[], void *start){
    uint32_t* ustart = (uint32_t*)start;
    ustart++;
    for(int i = 0; *ustart; i++){
        char *ptr = (char*)*ustart;
        arg_buf[i] = ptr;
        ustart++;
    }
    ustart++;
    for(int i = 0; *ustart; i++){
        char *ptr = (char*)*ustart;
        env_buf[i] = ptr;
        ustart++;
    }
}

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  printf("call main: argc = %d\n", *args);
  char *arg_buf[10] = {};
  char *env_buf[10] = {};
  load_argv_envp(arg_buf, env_buf, (void*)args);

  printf("args:\n");
  for(int i = 0; arg_buf[i] != NULL; i++) printf("%s\n", arg_buf[i]);
  //printf("envs:\n");
  //for(int i = 0; env_buf[i] != NULL; i++) printf("%s\n", env_buf[i]);

  exit(main(0, arg_buf, env_buf));
  assert(0);
}

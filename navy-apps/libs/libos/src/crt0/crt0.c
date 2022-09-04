#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

void load_argv_envp(char *arg_buf[], char *env_buf[], void *start){
    uint32_t* ustart = (uint32_t*)start;
    ustart++;
    for(int i = 0; i < argc; i++){
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
  printf("call_main\n");
  printf("args:%x\n", *args);

  char *args[10] = {};
  char *envs[10] = {};
  load_argv_envp(args, envs, (void*)args);

  printf("args:\n");
  for(int i = 0; args[i] != NULL; i++) printf("%s\n", args[i]);
  printf("envs:\n");
  for(int i = 0; envs[i] != NULL; i++) printf("%s\n", envs[i]);

  exit(main(0, empty, empty));
  assert(0);
}

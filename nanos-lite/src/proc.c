#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);

uintptr_t loader(PCB *pcb, const char *filename);

void display_context(Context *c, int index) {
  if(index >= 0){
    printf("%d\t%x\n", index, *((uint32_t*)c+index));
  }else {
    for(int i = 0; i < 36; i++){
      printf("%d\t%x\n", i, *((uint32_t*)c+i));
    }
  }
}

uint32_t len(char *const arr[]) {
    if(arr == NULL) return 0;
    uint32_t ret;
    for(ret = 0;; ret++) {
        if(arr[ret] == NULL) break;
    }
    return ret;
}

void* set_mem(char *const argv[], char *const envp[]){
    uint32_t argc = len(argv);
    uint32_t n = len(envp);

    uint32_t pe[n+1];
    uint32_t pa[argc+1];

    void *end = heap.end;

    uint32_t l, i;
    for(i = 0; i < n; i++) {
        l = strlen(envp[i]) + 1;
        memcpy(end - l, (void*)envp[i], l);
        end -= l;
        pe[i] = (uintptr_t)(char*)end;
    }
    pe[i] = 0;
    for(i = 0; i < argc; i++) {
        l = strlen(argv[i]) + 1;
        memcpy(end - l, (void*)argv[i], l);
        end -= l;
        pa[i] = (uintptr_t)(char*)end;
    }
    pa[i] = 0;
    memcpy(end-4*(n+1), pe, 4*(n+1));
    end -= 4*(n+1);
    memcpy(end-4*(argc+1), pa, 4*(argc+1));
    end -= 4*(argc+1);
    end -= 4;
    *(uint32_t*)end = argc;
    return end;
}

void display(void *start){
    uint32_t* ustart = (uint32_t*)start;
    uint32_t argc = *ustart;
    printf("argc=%d\n", argc);
    ustart++;
    for(int i = 0; i < argc; i++){
        char *ptr = (char*)*ustart;
        printf("%s\n", ptr);
        ustart++;
    }
    ustart++;
    for(;*ustart;){
        char *ptr = (char*)*ustart;
        printf("%s\n", ptr);
        ustart++;
    }
}

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(uint32_t arg) {
  int j = 1;
  while (1) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    //printf("Hello World from Nanos-lite with arg '%p' for the %dth time!\n", arg, j);
    printf("'%p' %d\n", arg, j);
    j ++;
    yield();
  }
  while(1);
}

void context_kload(PCB *this_pcb, void (*entry)(uint32_t), uint32_t arg){
  this_pcb->as.area.start = (void*)this_pcb;
  this_pcb->as.area.end = (void*)(((uint8_t*)this_pcb) + 8*4096);
  this_pcb->cp = kcontext(this_pcb->as.area, entry, arg);
}

void context_uload(PCB *this_pcb, const char *filename, char *const argv[], char *const envp[]) {
  this_pcb->as.area.start = (void*)this_pcb;
  this_pcb->as.area.end = (void*)(((uint8_t*)this_pcb) + 8*4096);
  void *entry = (void*)loader(this_pcb, filename);
  this_pcb->cp = ucontext(NULL, this_pcb->as.area, entry);
  
  void *argc_ptr = set_mem(argv, envp);
  //display(argc_ptr);
  this_pcb->cp->GPRx = (uintptr_t)argc_ptr;
  //this_pcb->cp->GPRx = (uintptr_t)((uint8_t*)heap.end - 4 * 36);
}

void init_proc() {

  char *const arr[4] = {"1!5!", "L!T!C!", "JNTM!", NULL};

  context_kload(&pcb[0], hello_fun, 2);
  //context_kload(&pcb[1], hello_fun, 3);
  context_uload(&pcb[1], "/bin/nterm", arr, NULL);
  
  switch_boot_pcb();

  Log("Initializing processes...");

  hello_fun(1);
  // load program here
  //naive_uload(NULL, "/bin/nterm");
}

Context* schedule(Context *prev) {
  //printf("%x, %x\n", (uint32_t)pcb[0].cp, (uint32_t)pcb[1].cp);
  //display_context(prev);
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  //printf("from %x to %x\n", (uint32_t)prev, (uint32_t)current->cp);
  //if(current == &pcb[1]){
  //  display_context(current->cp, 2);
  //  display_context(current->cp, 10);
  //  display_context(current->cp, 34);
  //}
  return current->cp;
  //return NULL;
}

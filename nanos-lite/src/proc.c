#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    printf("Hello arg '%p' %dth\n", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void context_kload(PCB *this_pcb, void (*entry)(void*), void *arg){
  this_pcb->as.area.start = (void*)this_pcb;
  this_pcb->as.area.end = (void*)(((uint8_t*)this_pcb) + 8*4096);
  this_pcb->cp = kcontext(this_pcb->as.area, entry, arg);
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, NULL);
  switch_boot_pcb();

  Log("Initializing processes...");

  hello_fun(NULL);
  // load program here
  //naive_uload(NULL, "/bin/nterm");
}

Context* schedule(Context *prev) {
  current->cp = prev;
  current = &pcb[0];
  return current->cp;
	
  //return NULL;
}

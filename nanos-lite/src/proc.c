#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);

void display_context(Context *c) {
  for(int i = 0; i < 36; i++){
    printf("%d\t%x\n", i, *((uint32_t*)c+i));
  }
}

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(uint32_t arg) {
  int j = 1;
  while (j < 5) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    printf("'%d'-'%d'\n", arg, j);
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

void init_proc() {
  context_kload(&pcb[0], hello_fun, 2);
  context_kload(&pcb[1], hello_fun, 3);
  
  switch_boot_pcb();

  Log("Initializing processes...");

  hello_fun(1);
  // load program here
  //naive_uload(NULL, "/bin/nterm");
}

Context* schedule(Context *prev) {
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  //current->cp->gpr[2] = (uint32_t)current->cp;
  return current->cp;
  //return NULL;
}

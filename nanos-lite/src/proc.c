#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(uint32_t arg) {
  int j = 1;
  while (j < 10) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    printf("Hello arg '%d' %dth\n", arg, j);
    j ++;
    yield();
  }
  while(1);
}

void context_kload(PCB *this_pcb, void (*entry)(uint32_t), uint32_t arg){
  this_pcb->as.area.start = (void*)this_pcb;
  this_pcb->as.area.end = (void*)(((uint8_t*)this_pcb) + 8*4096);
  this_pcb->cp = kcontext(this_pcb->as.area, entry, arg);
  printf("cp=%x\n", (uint32_t)this_pcb->cp);
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, 0xabc);
  context_kload(&pcb[1], hello_fun, 0xdef);
  
  switch_boot_pcb();

  Log("Initializing processes...");

  hello_fun(0xace);
  // load program here
  //naive_uload(NULL, "/bin/nterm");
}

void display_context(Context *c) {
  for(int i = 0; i < 36; i++){
    printf("%d\t%x\t%x\t%x\n", i, *(((uint32_t*)pcb[0].cp)+i), *(((uint32_t*)pcb[1].cp)+i), *(((uint32_t*)pcb_boot.cp)+i));
  }
  for(int i = 0; i < 36; i++){
    printf("%d\t%x\n", i, *((uint32_t*)c+i));
  }
}

Context* schedule(Context *prev) {
  printf("==?%d-%d-%d\n", current==&pcb[0], current==&pcb[1], current==&pcb_boot);
  display_context(prev);
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
	
  //return NULL;
}

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
  while (j < 5) {
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
  //for(int i = 0; i < 36; i++){
  //  printf("%d\t%x\t%x\n", i, *(((uint32_t*)pcb[0].cp)+i), *(((uint32_t*)pcb[1].cp)+i));
  //}
  for(int i = 0; i < 36; i++){
    printf("%d\t%x\n", i, *((uint32_t*)c+i));
  }
}

Context* schedule(Context *prev) {
  //printf("==?%d-%d-%d\n", current==&pcb[0], current==&pcb[1], current==&pcb_boot);
  //printf("prev: %x == prev->sp: %x ?\n", (uint32_t)prev, prev->gpr[2]);
  //display_context(prev);
  current->cp = prev;
  //if(current == &pcb[0]){
  //  current = &pcb[1];
  //  printf("from 0: %x to 1: %x\n", (uint32_t)&pcb[0], (uint32_t)&pcb[1]);
  //}else {
  //  current = &pcb[0];
  //  printf("from 1: %x to 0: %x\n", (uint32_t)&pcb[1], (uint32_t)&pcb[0]);
  //}

  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  current->cp->gpr[2] = (uint32_t)current->cp;
  printf("%p\n", (uint32_t)current->cp);
  return current->cp;
	
  //return NULL;
}

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

PCB* get_pcb(int index){
  if(index < 0 || index >= MAX_NR_PROC) return NULL;
  return &pcb[index];
}

uint32_t len(char *const arr[]) {
    if(arr == NULL) return 0;
    uint32_t ret;
    for(ret = 0;; ret++) {
        if(arr[ret] == NULL) break;
    }
    return ret;
}

void* set_mainargs(AddrSpace *as, char *const argv[], char *const envp[]){
    uint32_t argc = len(argv); uint32_t n = len(envp);
    uint32_t pe[n+1]; uint32_t pa[argc+1];

    void *end = as->area.end;
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

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(uint32_t arg) {
  int j = 1;
  while (1) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    //printf("Hello World from Nanos-lite with arg '%p' for the %dth time!\n", arg, j);
    
    printf("'%p'-%d\n", arg, j);
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
  void *upage_start = new_page(8);	
  this_pcb->as.area.start = upage_start;
  this_pcb->as.area.end = upage_start + 8*4096;
  //this_pcb->as.area.start = (void*)this_pcb;
  //this_pcb->as.area.end = (void*)(((uint8_t*)this_pcb) + 8*4096);
  void *entry = (void*)loader(this_pcb, filename);
  Area kstack = {.end = (void*)this_pcb + 8*4096};
  this_pcb->cp = ucontext(&this_pcb->as, kstack, entry);
  
  void *argc_ptr = set_mainargs(&this_pcb->as, argv, envp);
  this_pcb->cp->GPRx = (uintptr_t)argc_ptr;
  //this_pcb->cp->GPRx = (uintptr_t)((uint8_t*)heap.end - 4 * 36);
}

void init_proc() {
  char *arr[] = {"/bin/exec-test", "1", NULL};

  //context_kload(&pcb[0], hello_fun, 2);
  //context_kload(&pcb[1], hello_fun, 3);
  context_uload(&pcb[0], "/bin/exec-test", arr, NULL);
  context_uload(&pcb[1], "/bin/exec-test", arr, NULL);
  
  switch_boot_pcb();

  Log("Initializing processes...");

  hello_fun(1);
  // load program here
  //naive_uload(NULL, "/bin/exec-test");
}

Context* schedule(Context *prev) {
  if(prev->mcause == 13 || prev->mcause == -1){
    //printf("%x, %x\n", (uint32_t)pcb[0].cp, (uint32_t)pcb[1].cp);
    //display_context(prev);
    current->cp = prev;
    current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
    //printf("from %x to %x\n", (uint32_t)prev, (uint32_t)current->cp);
    printf("change\n");
    return current->cp;
  }else {
    return prev;
  }
  //return NULL;
}

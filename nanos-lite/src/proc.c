#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);

uintptr_t loader(PCB *pcb, const char *filename);

PCB* get_pcb(int index){
  if(index < 0 || index >= MAX_NR_PROC) return NULL;
  return &pcb[index];
}

static uint32_t len(char *const arr[]) {
    if(arr == NULL) return 0;
    uint32_t ret;
    for(ret = 0;; ret++) {
        if(arr[ret] == NULL) break;
    }
    return ret;
}

static void* set_mainargs(AddrSpace *as, char *const argv[], char *const envp[]){
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
    Log("Hello World from Nanos-lite with arg '%d' for the %dth time!", arg, j);
    j ++;
    yield();
  }
  while(1);
}

void context_kload(PCB *this_pcb, void (*entry)(uint32_t), uint32_t arg){
  this_pcb->as.area.start = (void*)this_pcb;
  this_pcb->as.area.end = (void*)(((uint8_t*)this_pcb) + 8*4096);
  this_pcb->cp = kcontext(this_pcb->as.area, entry, arg);
	this_pcb->cp->GPRx = (uintptr_t)this_pcb->as.area.end;
}

static void map_ustack(AddrSpace *as){
	for(int i = 1; i <= 8; i++){
		void *pptr = new_page(1);
		//printf("ustack vptr=%p, pptr=%p\n", as->area.end - i*PGSIZE, pptr);
		map(as, as->area.end - i*PGSIZE, pptr, 0);
	}
}

void context_uload(PCB *this_pcb, const char *filename, char *const argv[], char *const envp[]) {
	protect(&(this_pcb->as));
	set_satp(this_pcb->as.ptr);
	void *entry = (void*)loader(this_pcb, filename);
	Area kstack = {.start = (void*)this_pcb, .end = (void*)this_pcb + 8*PGSIZE};
	map_ustack(&(this_pcb->as));
	this_pcb->cp = ucontext(&(this_pcb->as), kstack, entry);
  void *argc_ptr = set_mainargs(&(this_pcb->as), argv, envp); // *(uint32_t*)argc_ptr = argc
  this_pcb->cp->GPRx = (uintptr_t)argc_ptr;

	  
	// The Following codes work with arguments without Page
	/*
	void *entry = (void*)loader(this_pcb, filename);
  Area kstack = {.end = (void*)this_pcb + 8*PGSIZE};
  this_pcb->cp = ucontext(NULL, kstack, entry);
  void *upage_start = new_page(8);	
  AddrSpace as = {.area.start = upage_start, .area.end = upage_start + 8*4096}; //{.area.end = heap.end};
  void *argc_ptr = set_mainargs(&as, argv, envp); // *(uint32_t*)argc_ptr = argc
  this_pcb->cp->GPRx = (uintptr_t)argc_ptr;	
	*/

	// The Following codes work without arguments
	/*
	void *entry = (void*)loader(this_pcb, filename);
  Area kstack = {.end = (void*)this_pcb + 8*PGSIZE};
  this_pcb->cp = ucontext(NULL, kstack, entry);
  this_pcb->cp->GPRx = (uintptr_t)heap.end; // heap.end = 0x88000000
	*/
}

void init_proc() {
  char *argv[] = {"/bin/exec-test", "skip", NULL};
	char *envs[] = {"hello", "sdlpal"};

  //context_kload(&pcb[0], hello_fun, 2);
  //context_kload(&pcb[1], hello_fun, 3);
  context_uload(&pcb[0], "/bin/dummy", argv, envs);
  //context_uload(&pcb[1], "/bin/pal", argv, envs);
  
  switch_boot_pcb();

  Log("Initializing processes...");

	// The Following codes work in PA3
	/*
	switch_boot_pcb();
	Log("Initializing processes...");
  // load program here
  naive_uload(NULL, "/bin/menu");
	*/
}

Context* schedule(Context *prev) {
  current->cp = prev;
  //current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
	current = &pcb[0];
  //printf("from %x to %x\n", (uint32_t)prev, (uint32_t)current->cp);
  return current->cp;
}

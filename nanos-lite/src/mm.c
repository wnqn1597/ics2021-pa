#include <memory.h>
#include <proc.h>

static void *pf = NULL;
extern PCB *current;

void* new_page(size_t nr_page) {
  void *ret = pf;
  pf += nr_page * PGSIZE;
  if((uintptr_t)pf > (uintptr_t)heap.end) {
    printf("Insufficient memory.\n");
    pf = ret;
    return NULL;
  }
  return ret;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
	void *ret = new_page(n / PGSIZE);
	memset(ret, 0, n);
  return ret;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
	if(brk > current->max_brk){
		if((current->max_brk & 0xfff) == 0){
			void *pageptr = new_page(1);
			map(&(current->as), (void*)current->max_brk, pageptr, 0);
		}
		current->max_brk = ((current->max_brk)&(~0xfff))+PGSIZE;
		while(current->max_brk < brk){
			void *pageptr = new_page(1);
			map(&(current->as), (void*)current->max_brk, pageptr, 0);
			current->max_brk += PGSIZE;
		}
		current->max_brk = brk;
	}
	
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
	Log("Map Finish");
}

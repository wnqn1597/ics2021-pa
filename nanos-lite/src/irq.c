#include <common.h>

void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {

  // strace implement here

  switch (e.event) {
    case EVENT_YIELD: 
	    //printf("YIELD EXECUTED\n");
	    return c;
    case EVENT_SYSCALL: 
	    //printf("SYSCALL START\n");
	    break;
    default: panic("Unhandled event ID = %d", e.event);
  }
  do_syscall(c);
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}

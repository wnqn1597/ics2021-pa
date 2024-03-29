#include <common.h>

void do_syscall(Context *c);
Context* schedule(Context *prev);

static Context* do_event(Event e, Context* c) {
  // strace implement here
  switch (e.event) {
		case EVENT_YIELD: 
			c->mepc += 4;
		case EVENT_IRQ_TIMER:
	    return schedule(c);
    case EVENT_SYSCALL:
		 	c->mepc += 4;
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

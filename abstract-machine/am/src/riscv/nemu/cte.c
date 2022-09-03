#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case -1: ev.event = EVENT_YIELD;break;
      case 0: 
      case 1: 
      case 2: 
      case 3: 
      case 4: 
      case 5: 
      case 6: 
      case 7: 
      case 8: 
      case 9: 
      case 10: 
      case 11: 
      case 12: 
      case 13: 
      case 14: 
      case 15: 
      case 16: 
      case 17: 
      case 18: 
      case 19: ev.event = EVENT_SYSCALL; break;
      default: ev.event = EVENT_ERROR; break;
    }
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), uint32_t arg) {
  uint32_t *mstatus_ptr = (uint32_t*)(kstack.end - 3 * 4);
  uint32_t *mepc_ptr = (uint32_t*)(kstack.end - 2 * 4);
  uint32_t *a0_ptr = (uint32_t*)(kstack.end - 26 * 4);
  *mstatus_ptr = 0x1800;
  *mepc_ptr = (uintptr_t)entry;
  *a0_ptr = arg;
  return (Context*)(kstack.end - 36 * 4);
  //return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}

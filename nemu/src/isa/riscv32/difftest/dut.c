#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  printf("ID\tREF\t\tDUT\n");
  for(int i = 0; i < 32; i++) {
    printf("%d\t%08x\t%08x\n", i, ref_r->gpr[i]._32, cpu.gpr[i]._32);
  }
	
  return true;
}

void isa_difftest_attach() {
}

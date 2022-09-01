#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool ret = true;
  for(int i = 0; i < 32; i++) {
    if(ref_r->gpr[i]._32 != cpu.gpr[i]._32)ret = false; //printf("%d\t%08x\t%08x\n", i,ref_r->gpr[i]._32, cpu.gpr[i]._32);
  }
  if(ref_r->pc != cpu.pc){
    printf("!!!!!!!!!!!!!!!!!!!!!!!%08x, %08x\n", ref_r->pc, cpu.pc);
  }
  if(!ret){
    printf("ID\tREF\t\tDUT\n");
    for(int i = 0; i < 32; i++) printf("%d\t%08x\t%08x\n", i, ref_r->gpr[i]._32, cpu.gpr[i]._32);
  }
  return ret;
}

void isa_difftest_attach() {
}

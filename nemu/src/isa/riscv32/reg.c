#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  //for(int i = 0; i < 32; i++){
  printf("R[%s]: %08x\n", regs[0], cpu.gpr[0]._32);
  printf("R[%s]: %08x\n", regs[1], cpu.gpr[1]._32);
  printf("R[%s]: %08x\n", regs[2], cpu.gpr[2]._32);
  //}
}

int regname_to_index(const char *s){
  int i;
  for(i = 0; i < 32; i++){
    if(strcmp(s, regs[i]) == 0) break;
  }
  if(i == 32){
    printf("Unknown register name: %s\n", s);
  }
  return i;
}

word_t isa_reg_str2val(const char *s, bool *success) {
  int i = regname_to_index(s);
  if(i == 32){
    *success = false;
    return 0;
  }
  *success = true;
  return cpu.gpr[i]._32;
}

#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(addi) f(jal) f(jalr) f(auipc) \
f(add) f(sub) f(beq) f(bne) f(inv) f(nemu_trap)

def_all_EXEC_ID();

#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) \
f(lui) f(auipc) \
f(addi) f(slti) f(sltiu) f(xori) f(ori) f(andi) f(slli) f(srli) f(srai) \
f(add) f(sub) f(sll) f(slt) f(sltu) f(xor) f(srl) f(sra) f(or) f(and) \
f(jal) f(jalr) f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) \
f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw) \
f(mulu_lo) f(mulu_hi) f(muls_hi) f(divu_q) f(divu_r) f(divs_q) f(divs_r) \
f(inv) f(nemu_trap)

def_all_EXEC_ID();

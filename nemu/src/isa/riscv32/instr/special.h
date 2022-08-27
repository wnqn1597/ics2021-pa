#include <isa.h>

def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}

def_EHelper(ecall) {
  printf("a7: %d\n", cpu.gpr[17]._32);
  s->dnpc = isa_raise_intr(cpu.gpr[17]._32, s->pc); //TODO -1 -> gpr[17]
}

def_EHelper(mret) {
  s->dnpc = isa_out_intr(0);
}

def_EHelper(csrrw) {
  rtl_mv(s, ddest, get_csr(id_src2->imm));
  rtl_mv(s, get_csr(id_src2->imm), dsrc1);
}

def_EHelper(csrrs) {
  rtl_mv(s, ddest, get_csr(id_src2->imm));
  rtl_or(s, get_csr(id_src2->imm), get_csr(id_src2->imm), dsrc1);
}

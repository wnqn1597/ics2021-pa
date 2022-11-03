#include <isa.h>

def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}

def_EHelper(ecall) {
  s->dnpc = isa_raise_intr(((int)cpu.gpr[17]._32 < 0) ? 0x80000003 : 0xb , s->pc);
}

def_EHelper(mret) {
  s->dnpc = isa_out_intr(0);
}

def_EHelper(csrrw) {
	rtl_mv(s, s0, get_csr(id_src2->imm));
  rtl_mv(s, get_csr(id_src2->imm), dsrc1);
	rtl_mv(s, ddest, s0);
}

def_EHelper(csrrs) {
  rtl_mv(s, ddest, get_csr(id_src2->imm));
  rtl_or(s, get_csr(id_src2->imm), get_csr(id_src2->imm), dsrc1);
}

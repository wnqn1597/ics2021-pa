#include <isa.h>

CSR csr_reg;

uint32_t* get_csr(uint32_t code) {
  switch(code) {
    case 0x300: return &csr_reg.mstatus;
    case 0x305: return &csr_reg.mtvec;
    case 0x341: return &csr_reg.mepc;
    case 0x342: return &csr_reg.mcause;
    default: printf("Unknown csr code %x\n", code);assert(0);
  }
}

void write_csr(uint32_t code, word_t value) {
  switch(code) {
    case 0x300: csr_reg.mstatus = value;break;
    case 0x305: csr_reg.mtvec = value;break;
    case 0x341: csr_reg.mepc = value;break;
    case 0x342: csr_reg.mcause = value;break;
    default: printf("Unknown csr code %x\n", code);assert(0);
  }
}

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  csr_reg.mstatus = NO;
  csr_reg.mepc = epc;
  return csr_reg.mtvec;
}

word_t isa_out_intr(word_t NO) {
  csr_reg.mstatus = NO;
  return csr_reg.mepc;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}

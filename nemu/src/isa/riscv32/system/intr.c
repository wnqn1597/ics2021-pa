#include <isa.h>

#define IRQ_TIMER 0x80000007

CSR csr_reg = {.mstatus.val = 0x1800};

//TODO: add etrace here.

uint32_t* get_csr(uint32_t code) {
  switch(code) {
    case 0x180: return &csr_reg.satp;
    case 0x300: return &csr_reg.mstatus.val;
    case 0x305: return &csr_reg.mtvec;
		case 0x340: return &csr_reg.mscratch;
    case 0x341: return &csr_reg.mepc;
    case 0x342: return &csr_reg.mcause;
    default: printf("Unknown csr code %x\n", code);assert(0);
  }
}

void write_csr(uint32_t code, word_t value) {
  switch(code) {
    case 0x180: csr_reg.satp = value;printf("satp=%08x\n", value);break;
    case 0x300: csr_reg.mstatus.val = value;break;
    case 0x305: csr_reg.mtvec = value;break;
		case 0x340: csr_reg.mscratch = value;break;
    case 0x341: csr_reg.mepc = value;break;
    case 0x342: csr_reg.mcause = value;break;
    default: printf("Unknown csr code %x\n", code);assert(0);
  }
}

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  
	// mstatus
	csr_reg.mstatus.MPIE = csr_reg.mstatus.MIE;
	csr_reg.mstatus.MIE = 0;
	
	csr_reg.mcause = NO;
  /* + 4 ? */
  csr_reg.mepc = epc + 4;
  //printf("ECALL PC=%x\n", epc);
  return csr_reg.mtvec;
}

word_t isa_out_intr(word_t NO) {
	
	// mstatus
	csr_reg.mstatus.MIE = csr_reg.mstatus.MPIE;
	csr_reg.mstatus.MPIE = 1;
	
  //csr_reg.mcause = NO;
  // not + 4, otherwise the entry will be modified.
  //printf("MRET  PC=%x\n", csr_reg.mepc);
  return csr_reg.mepc;
}

word_t isa_query_intr() {

	if(cpu.INTR && csr_reg.mstatus.MIE){
		cpu.INTR = false;
		return IRQ_TIMER;
	}
  return INTR_EMPTY;
}

#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

int isa_mmu_check(vaddr_t vaddr, int len, int type) {
  uint32_t satp_val = *(get_csr(0x180));
  if((satp_val >> 31) == 1) return MMU_TRANSLATE;
  else return MMU_DIRECT;
}

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  return 0;  
}

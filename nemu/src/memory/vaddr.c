#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
	int flag = isa_mmu_check(addr, len, MEM_TYPE_IFETCH);
	if(flag) addr = isa_mmu_translate(addr, len, MEM_TYPE_IFETCH);
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
	int flag = isa_mmu_check(addr, len, MEM_TYPE_READ);
	if(flag) addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
	int flag = isa_mmu_check(addr, len, MEM_TYPE_WRITE);
	if(flag) addr = isa_mmu_translate(addr, len, MEM_TYPE_WRITE);
  paddr_write(addr, len, data);
}

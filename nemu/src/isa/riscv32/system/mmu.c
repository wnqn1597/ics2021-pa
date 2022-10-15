#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

typedef union{
	struct{
		uint32_t offs : 12;
		uint32_t vpn0 : 10;
		uint32_t vpn1 : 10;
	};
	uint32_t val;
}Vaddr;

int isa_mmu_check(vaddr_t vaddr, int len, int type) {
  uint32_t satp_val = *(get_csr(0x180));
  if((satp_val >> 31) == 1) {
		printf("TR\n");
		return MMU_TRANSLATE;
	}else{
		return MMU_DIRECT;
	}
}

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  uint32_t *pdirBase = (uint32_t*)(uintptr_t)((*get_csr(0x180)) << 12);
	printf("%p\n", pdirBase);
	Vaddr v = {.val = vaddr};
	printf("%08x\n", v.vpn1);
	uint32_t pdirPTE = *(pdirBase + v.vpn1);
	printf("%08x\n", pdirPTE);
	if((pdirPTE & 0x1) == 0){
		Log("MMU_TRANSLATE_ERROR");
		return 0;
	}
	uint32_t *ptabBase = (uint32_t*)(uintptr_t)((pdirPTE >> 10) << 12);
	uint32_t ptabPTE = *(ptabBase + v.vpn0);
	if((ptabPTE & 0x1) == 0){
		Log("MMU_TRANSLATE_ERROR");
		return 0;
	}
	return ((ptabPTE >> 10) << 12) | v.offs;
}

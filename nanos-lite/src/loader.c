#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

int fs_open(const char *pathname, int flags, int mode);
int fs_close(int fd);
void* get_finfo(int index, int property);

uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  if(fd == -1 && filename != NULL){
    printf("%s doesn't exist!!!\n", filename); 
    assert(0);
  }
  size_t offset = *((size_t*)get_finfo(fd, 2));

  Elf_Ehdr ehdr;
  size_t bias = ramdisk_read(&ehdr, offset, sizeof(Elf_Ehdr));
  //size_t bias = ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
  assert(*(uint32_t*)ehdr.e_ident == 0x464c457f);
  Elf_Phdr phdr[ehdr.e_phnum];
  ramdisk_read(phdr, offset + bias, ehdr.e_phnum * sizeof(Elf_Phdr));
  //ramdisk_read(phdr, bias, ehdr.e_phnum * sizeof(Elf_Phdr));
	printf("Load start\n");
	for(int i = 0; i < ehdr.e_phnum; i++){
		if(phdr[i].p_type == PT_LOAD){
			size_t rptr = offset + phdr[i].p_offset;
			int size = phdr[i].p_memsz;
			char *vptr = (char*)phdr[i].p_vaddr;
			while(size > 0){
				void *pptr = new_page(1);
				printf("vptr=%p, pptr=%p\n", vptr, pptr);
				map(&(pcb->as), (void*)vptr, pptr, 0);
				ramdisk_read((void*)vptr, rptr, PGSIZE);
				rptr += PGSIZE;
				vptr += PGSIZE;
				size -= PGSIZE;
			}
			printf("next\n");
      memset((void*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
		}
	}

	// The Following Codes work without Page
  /*
  for(int i = 0; i < ehdr.e_phnum; i++) {
    if(phdr[i].p_type == PT_LOAD) {
      ramdisk_read((void*)phdr[i].p_vaddr, offset + phdr[i].p_offset, phdr[i].p_memsz);
      memset((void*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
    }
  }
	*/
  fs_close(fd);
	printf("Load success.\n");
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}


#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

int get_finfo(int index, int property) {
  unsigned length = sizeof(file_table) / sizeof(Finfo);
  if(index >= 0 && index < length) {
    switch(property){
      case 1: return file_table[index].size;
      case 2: return file_table[index].disk_offset;
      default: assert(0);
    }
  }else assert(0);
}

int fs_open(const char *pathname, int flags, int mode) {
  unsigned length = sizeof(file_table) / sizeof(Finfo);
  int i;
  for(i = 0; i < length; i++) {
    if(strcmp(pathname, file_table[i].name) == 0) break;
  }
  if(i == length) return -1;
  else return i;
}

int fs_write(int fd, const void *buf, size_t len) {
  return 0;
}

int fs_read(int fd, void *buf, size_t len) {
  return 0;
}

int fs_close(int fd) {
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

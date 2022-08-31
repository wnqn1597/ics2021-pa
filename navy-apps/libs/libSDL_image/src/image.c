#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  FILE *f = fopen(filename, "r");
  int size = f->_bf._size;
  printf("SIZE: %d %d\n", f->_bf._size, f->_lbfsize);
  void *buf = SDL_malloc(size);
  memcpy(buf, f->_bf._base, size);
  SDL_Surface *s = STBIMG_LoadFromMemory(buf, size);
  fclose(filename);
  SDL_free(buf);
  return s;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}

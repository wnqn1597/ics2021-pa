#include <stdio.h>
#include <SDL.h>
#include <SDL_bdf.h>

static BDF_Font *font = NULL;

int main() {
  font = new BDF_Font("/share/fonts/Courier-7.bdf");

  printf("PASS TEST 2 with %d-%d\n", font->w, font->h);

  return 0;
}

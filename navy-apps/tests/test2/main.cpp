#include <stdio.h>
#include <SDL.h>
#include <SDL_bdf.h>


int main() {
  BDF_Font *font = new BDF_Font("/share/fonts/Courier-7.bdf");

  printf("PASS TEST 2 with %d-%d\n", font->w, font->h);

  return 0;
}

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
  NDL_Init(0);
  int w, h;

  //FILE *fp = fopen("/share/pictures/projectn.bmp", "r");
  //printf("1\n");
  //FILE *ffp = fopen("/share/pictures/projectn.bmp", "r");
  //printf("1\n");
  //while(1);

  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  assert(bmp);
  NDL_OpenCanvas(&w, &h);
  NDL_DrawRect(bmp, 0, 0, w, h);
  free(bmp);
  void *bmpp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while(1);
  return 0;
}

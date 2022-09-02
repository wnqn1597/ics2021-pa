#include <stdio.h>

int main() {
  FILE *fp = fopen("/share/files/num", "r+");
  fclose(fp);

  printf("PASS TEST 2\n");

  return 0;
}

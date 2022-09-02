#include <stdio.h>

int main() {
  FILE *fp = fopen("/share/files/num", "r+");
  fclose(fp);

  printf("PASS TEST 1\n");

  return 0;
}

#include <stdio.h>
#include <assert.h>

int main() {

  char buf[10];

  FILE *fp = fopen("/share/files/num", "r+");
  assert(fp);

  while(fgets(buf, 10, fp)) {
    printf("%s", buf);
  }

  //fseek(fp, 0, SEEK_END);
  //long size = ftell(fp);
  //assert(size == 5000);

  //fseek(fp, 500 * 5, SEEK_SET);
  //int i, n;
  //for (i = 500; i < 1000; i ++) {
  //  fscanf(fp, "%d", &n);
  //  assert(n == i + 1);
  //}
  //fseek(fp, 0, SEEK_SET);
  //for (i = 0; i < 500; i ++) {
  //  fprintf(fp, "%4d\n", i + 1 + 1000);
  //}

  //for (i = 500; i < 1000; i ++) {
  //  fscanf(fp, "%d", &n);
  //  assert(n == i + 1);
  //}
  //fseek(fp, 0, SEEK_SET);
  //for (i = 0; i < 500; i ++) {
  //  fscanf(fp, "%d", &n);
  //  assert(n == i + 1 + 1000);
  //}

  fclose(fp);
  
  printf("----------NEXT----------\n");

  fp = fopen("/share/files/num", "r+");
  assert(fp);

  while(fgets(buf, 10, fp)) {
    printf("%s", buf);
  }
  //fseek(fp, 0, SEEK_END);
  //size = ftell(fp);
  //assert(size == 5000);

  //fseek(fp, 500 * 5, SEEK_SET);
  //for (i = 500; i < 1000; i ++) {
  //  fscanf(fp, "%d", &n);
  //  assert(n == i + 1);
  //}
  //fseek(fp, 0, SEEK_SET);
  //for (i = 0; i < 500; i ++) {
  //  fprintf(fp, "%4d\n", i + 1 + 1000);
  //}

  //for (i = 500; i < 1000; i ++) {
  //  fscanf(fp, "%d", &n);
  //  assert(n == i + 1);
  //}
  //fseek(fp, 0, SEEK_SET);
  //for (i = 0; i < 500; i ++) {
  //  fscanf(fp, "%d", &n);
  //  assert(n == i + 1 + 1000);
  //}

  fclose(fp);


  printf("PASS!!!\n");
  while(1);
  return 0;
}

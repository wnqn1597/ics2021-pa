#include <unistd.h>
#include <stdio.h>

int main(){
  uint32_t pre = gettimeofday(NULL, NULL);
  while(1) {
    uint32_t now = gettimeofday(NULL, NULL);
    if(now - pre >= 1000000) {
      printf("skr~\n");
      pre = now;
    }
  }
  return 0;
}

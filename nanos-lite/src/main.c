#include <common.h>

void init_mm(void);
void init_device(void);
void init_ramdisk(void);
void init_irq(void);
void init_fs(void);
void init_proc(void);

int main() {
  //extern const char logo[];
  //printf("%s", logo);
  printf("1\n");
  Log("'Hello World!' from Nanos-lite");
  Log("Build time: %s, %s", __TIME__, __DATE__);

  init_mm();

  printf("1\n");
  init_device();

  printf("1\n");
  init_ramdisk();

  printf("1\n");
#ifdef HAS_CTE
  init_irq();
#endif

  printf("1\n");
  init_fs();

  printf("1\n");
  init_proc();

  Log("Finish initialization");

  printf("1\n");
#ifdef HAS_CTE
  yield();
#endif

  panic("Should not reach here");
}

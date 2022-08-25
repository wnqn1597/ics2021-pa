#include <am.h>
#include <nemu.h>
#include <klib.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
//  int i;
//  int w = 400;
//  int h = 300;
//  uint32_t *fb = (uint32_t*)(uintptr_t)FB_ADDR;
//  for(i = 0; i < w*h; i++) fb[i] = i;
//  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t wh = inl(VGACTL_ADDR);
  uint32_t height = wh & 65535; // 300
  uint32_t width = (wh & (65535 << 16)) >> 16; // 400
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = width, .height = height,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    printf("x:%d, y:%d, w: %d, h: %d\n", ctl->x, ctl->y, ctl->w, ctl->h);
    if(ctl->x == 0 && ctl->y == 0 && ctl->w == 0 && ctl->h == 0) printf("aaaaaaaaaa\n");
    uint32_t *fb = (uint32_t*)(uintptr_t)FB_ADDR;
    uint32_t bias = ctl->y * 400 + ctl->x;
    for(int i = 0; i < ctl->h; i++) {
      for(int j = 0; j < ctl->w; j++) {
        fb[bias + i * 400 + j] = ((uint32_t*)(ctl->pixels))[i * ctl->w + j];
      }
    }
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CENTER 1

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int fb_w = 0, fb_h = 0, fb_x = 0, fb_y = 0;

uint32_t NDL_GetTicks() {
  return (_syscall_(19, 0, 0, 0) / 1000);
}

int NDL_PollEvent(char *buf, int len) {
  const char *name = "/dev/events";
  int fd = _syscall_(2, (intptr_t)name, 0, 0);
  return _syscall_(3, fd, (intptr_t)buf, len);
}

void NDL_OpenCanvas(int *w, int *h) {
//  if (getenv("NWM_APP")) {
	  
    int fbctl = 4;
    fbdev = 5;

    char buf[64];
    if(*w == 0 && *h == 0) {
      *w = screen_w; *h = screen_h;
    }
    fb_w = *w; fb_h = *h;
    if(CENTER){
      fb_x = (screen_w - fb_w) / 2;
      fb_y = (screen_h - fb_h) / 2;
    }

    int len = sprintf(buf, "%d %d", *w, *h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
    printf("OPEN FB width=%d, height=%d\n", *w, *h);
//  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  uint32_t offset = (fb_y + y) * screen_w + (fb_x + x);
  const char *name = "/dev/fb";
  int fd = _syscall_(2, (intptr_t)name, 0, 0);
  _syscall_(8, fd, offset, SEEK_SET);
  for(int i = 0; i < h; i++) {
    _syscall_(4, fd, pixels+i*w, w);
    _syscall_(8, fd, screen_w - w, SEEK_CUR);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
//  if (getenv("NWM_APP")) {
    evtdev = 3;

    char buf[20];
    const char *name = "/proc/dispinfo";

    int fd = _syscall_(2, (intptr_t)name, 0, 0);
    _syscall_(3, fd, (intptr_t)buf, 20);
    
    char width[10], height[10];
    int i;
    for(i = 0; buf[i] != ':'; i++); i++;
    strncpy(width, (char*)buf+i, 10);
    for(; buf[i] != ':'; i++); i++;
    strncpy(height, (char*)buf+i, 10);
    screen_w = atoi(width); screen_h = atoi(height);
    printf("NDL Initializing finished.\n");
//  }
  return 0;
}

void NDL_Quit() {
  evtdev = -1;
  fbdev = -1;
  screen_w = 0, screen_h = 0;
  fb_w = 0, fb_h = 0, fb_x = 0, fb_y = 0;
  printf("NDL Quit.\n");
}

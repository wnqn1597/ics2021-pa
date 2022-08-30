#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  int i;
  for(i = 0; i < len; i++) putch(*((char*)buf + i));
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  uint32_t val = get_keyboard_val();
  if(val == 0) return 0;
  if(val < 256) {
    const char *name = keyname[val];
    strcpy(buf, "ku ");
    strncpy(buf+3, name, len);
  }else {
    uint32_t masked = val & 0x7fff;
    const char *name = keyname[masked];
    strcpy(buf, "kd ");
    strncpy(buf+3, name, len);
  }
  return len;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  uint32_t height = get_height();
  uint32_t width = get_width();
  return sprintf(buf, "WIDTH:%d\nHEIGHT:%d", width, height);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}

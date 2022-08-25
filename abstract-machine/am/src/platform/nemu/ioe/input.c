#include <am.h>
#include <nemu.h>
#include <klib.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t val = inl(KBD_ADDR);
  if(val != 0 )printf("key: %d\n", val);
  if(false && val != 0 && val != 32811){
    kbd->keydown = 1;
    kbd->keycode = val;
    return;
  }
  kbd->keydown = 0;
  kbd->keycode = AM_KEY_NONE;
}

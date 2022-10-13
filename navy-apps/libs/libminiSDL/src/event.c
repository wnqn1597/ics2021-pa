#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char buf[10];
  int key_val = NDL_PollEvent(buf, 10);
  if(key_val == 0) return 0;
  ev->type = (key_val > 0x7fff) ? 0 : 1;
  ev->key.keysym.sym = key_val & 0x7fff;
	printf("poll %d\n", ev->key.keysym.sym);
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[10];
  int key_val = NDL_PollEvent(buf, 10);
  event->type = (key_val > 0x7fff) ? 0 : 1;
  event->key.keysym.sym = key_val & 0x7fff;
	printf("wait %d\n", event->key.keysym.sym);
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

static uint8_t keyStates[256] = {};

uint8_t* SDL_GetKeyState(int *numkeys) {
	memset(keyStates, 0, 256 * sizeof(uint8_t));
	printf("get keyStates\n");
	return keyStates;
  //return NULL;
}

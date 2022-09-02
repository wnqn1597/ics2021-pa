#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static int sh_handle_cmd(char *cmd) {
  int len = sizeof(nitems) / sizeof(struct NtermItem);
  int i = 0;
  for(i = 0; i < len; i++) {
    if(strcmp(cmd, nitems[i].bin) == 0) return i;
  }
  sh_printf("Unknown command\n");
  return -1;
}

void builtin_sh_run(char *buf) {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
	  strncpy(buf, res, strlen(res)-1);
          int index = sh_handle_cmd(buf);
          if(index >= 0){
	    printf("Running program %s\n", nitems[index].name);
	    return;
	  }
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}

#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *buf, const char *fmt, va_list args) {
    char *str;

    for (str = buf; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            *str++ = *fmt;
            continue;
        }
        fmt++;
        if(*fmt == 'd') {
			int arg = va_arg(args, int);
			int b = 1, c = 0;
			while(arg / b != 0) {
			  b *= 10;
			  c++;
			}
			int a = c - 1;
			while(arg > 0) {
			  int r = arg % 10;
			  *(str+a) = (char)('0' + r);
			  arg /= 10;
			  a--;
			}
			str += c;
        }else if(*fmt == 's') {
            char *arg = va_arg(args, char*);
            strcpy(str, arg);
            str += strlen(arg);
        }
    } /* end of for (str = buf; *fmt; fmt++) */

    *str = '\0';
    return str - buf;
}

int sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    int n;

    va_start(args, fmt);
    n = vsprintf(buf, fmt, args);
    va_end(args);

    return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif

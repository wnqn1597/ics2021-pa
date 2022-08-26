#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <common.h>

//printf("\33[1;35m[%s,%d,%s] " format "\33[0m\n";

#define Log(format, ...) \
  printf("[%s,%d,%s] " format, \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#undef panic
#define panic(format, ...) \
  do { \
    Log("\33[1;31msystem panic: " format, ## __VA_ARGS__); \
    halt(1); \
  } while (0)

#ifdef assert
# undef assert
#endif

#define assert(cond) \
  do { \
    if (!(cond)) { \
      panic("Assertion failed: %s", #cond); \
    } \
  } while (0)

#define TODO() panic("please implement me")

#endif

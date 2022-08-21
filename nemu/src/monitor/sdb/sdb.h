#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

typedef struct watchpoint {
  int type;	// 0: watching 1: break
  int NO;
  struct watchpoint *next;
  char expression[32];
  unsigned pre_val;
  
  /* TODO: Add more members if necessary */

} WP;


word_t expr(char *e, bool *success);

#endif

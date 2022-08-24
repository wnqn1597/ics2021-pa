#include <stdint.h>

typedef struct ringbufnode {
    int NO;
    uint32_t instr;
    uint32_t pc;
    struct ringbufnode *next;
} RBN;

void init_pool();
void insert(uint32_t instr, uint32_t pc);
void display_pool();

#include <isa.h>
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly

//static const uint32_t img [] = {
//  0x800002b7,  // lui t0,0x80000
//  0x0002a023,  // sw  zero,0(t0)
//  0x0002a503,  // lw  a0,0(t0)
//  0x0000006b,  // nemu_trap
//};

//static const uint32_t img [] = {
//  0x80006f,
//  0x100006f,
//  0x800000b7,
//  0x408067,
//  0x0,
//  0x117,
//  0x110193,
//  0x0000006b,
//};

static const uint32_t img [] = {
  0x00001037,
  0x000010b7,
  0x00001137,
  0x000011b7,
  0x00001237,
  0x000012b7,
  0x00001337,
  0x000013b7,
  0x00001437,
  0x000014b7,
  0x00001537,
  0x000015b7,
  0x00001637,
  0x000016b7,
  0x00001737,
  0x000017b7,
  0x00001837,
  0x000018b7,
  0x00001937,
  0x000019b7,
  0x00001a37,
  0x00001ab7,
  0x00001b37,
  0x00001bb7,
  0x00001c37,
  0x00001cb7,
  0x00001d37,
  0x00001db7,
  0x00001e37,
  0x00001eb7,
  0x00001f37,
  0x00001fb7,
  0x6b
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;

  /* The zero register is always 0. */
  cpu.gpr[0]._32 = 0;
}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}

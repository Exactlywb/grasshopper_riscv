#include "grasshopper.h"
#include <string.h>

#include "vector_inst.h"

extern Block Coefs[NUM_ROUNDS / 2 - 1][BIT_IN_BYTES];
extern unsigned char Muls[256][16];
extern unsigned char LS_tbl[16][256][16];

void apply_x(Block *block, const Block *key) {
  // XOR two 16-bytes arrays block->data and key->data.
  // The result puts into block->data
  unsigned vl = __riscv_vsetvl_e8m1(BLOCK_SIZE);
  vuint8m1_t v1 = __riscv_vle8_v_u8m1(block->data, vl);
  vuint8m1_t v2 = __riscv_vle8_v_u8m1(key->data, vl);
  vuint8m1_t vd;

  vd = __riscv_vxor_vv_u8m1(v1, v2, vl);
  __riscv_vse8_v_u8m1(block->data, vd, vl);
}

void apply_l(Block *block) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    unsigned char tmp = 0;
    for (int j = 0; j < BLOCK_SIZE; ++j)
      tmp ^= Muls[block->data[j]][j];

    /* Copy backward.  */
    for (int rev = BLOCK_SIZE - 1; rev > 0; --rev)
      block->data[rev] = block->data[rev - 1];

    block->data[0] = tmp;
  }
}

void apply_ls(Block *block) {
  unsigned char tmp[16] = {0};
  for (int i = 0; i < BLOCK_SIZE; ++i)
    for (int j = 0; j < BLOCK_SIZE; ++j)
      tmp[j] ^= LS_tbl[i][block->data[i]][j];

  // memcpy pattern (it's better than libc's implementation for RISCV)
  unsigned vl = __riscv_vsetvl_e8m1(BLOCK_SIZE);
  vuint8m1_t v_tmp = __riscv_vle8_v_u8m1(tmp, vl);
  __riscv_vse8_v_u8m1(block->data, v_tmp, vl);
}

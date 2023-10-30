#include "grasshopper.h"
#include <string.h>

extern Block Coefs[NUM_ROUNDS / 2 - 1][BIT_IN_BYTES];
extern unsigned char Muls[256][16];
extern unsigned char LS_tbl[16][256][16];

void apply_x(Block *block, const Block *key) {
  for (int i = 0; i < BLOCK_SIZE; ++i)
    block->data[i] = block->data[i] ^ key->data[i];
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

  memcpy(block->data, tmp, BLOCK_SIZE);
}

void apply_inv_l(Block *block) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    unsigned char elem = block->data[0];
    memmove(block->data, block->data + 1, BLOCK_SIZE - 1);
    block->data[BLOCK_SIZE - 1] = elem;

    unsigned char tmp = 0;
    for (int j = 0; j < BLOCK_SIZE; ++j)
      tmp ^= Muls[block->data[j]][j];
    block->data[BLOCK_SIZE - 1] = tmp;
  }
}
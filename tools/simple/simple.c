#include "grasshopper.h"
#include <string.h>

void apply_x(Block *block, const Block *key) {
  for (int i = 0; i < BLOCK_SIZE; ++i)
    block->data[i] = block->data[i] ^ key->data[i];
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

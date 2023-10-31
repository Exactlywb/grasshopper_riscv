#include <string.h>
#include "grasshopper.h"
#include "vector_inst.h"

void apply_x(Block *block, const Block *key) {
  /*
    for (i = 0; i < BLOCK_SIZE; ++i)
      block->data[i] ^= key->data[i];
  */
  unsigned vl = __riscv_vsetvl_e8m1(BLOCK_SIZE);
  vuint8m1_t v1 = __riscv_vle8_v_u8m1(block->data, vl);
  vuint8m1_t v2 = __riscv_vle8_v_u8m1(key->data, vl);
  vuint8m1_t vd;

  vd = __riscv_vxor_vv_u8m1(v1, v2, vl);
  __riscv_vse8_v_u8m1(block->data, vd, vl);
}

void apply_ls(Block *block) {
  unsigned char tmp[16] = {0};
  unsigned vl = __riscv_vsetvl_e8m1(BLOCK_SIZE);
  vuint8m1_t vd;
  /*
    for (i = 0; i < BLOCK_SIZE; ++i)
      for (j = 0; j < BLOCK_SIZE; ++j)
        tmp[j] ^= LS_tbl[i][block->data[i]][j];

    memcpy(block->data, tmp, BLOCK_SIZE);
  */
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    unsigned char *tbl = LS_tbl[i][block->data[i]];
    vuint8m1_t v1 = __riscv_vle8_v_u8m1(tmp, vl);
    vuint8m1_t v2 = __riscv_vle8_v_u8m1(tbl, vl);

    vd = __riscv_vxor_vv_u8m1(v1, v2, vl);
    __riscv_vse8_v_u8m1(tmp, vd, vl);
  }

  __riscv_vse8_v_u8m1(block->data, vd, vl);
}

void apply_inv_l(Block *block) {
  unsigned vl = __riscv_vsetvl_e8m1(BLOCK_SIZE);
  vuint16m2_t vec_j = __riscv_vid_v_u16m2(vl);
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    unsigned char elem = block->data[0];

    /*
      memmove(block->data, block->data + 1, BLOCK_SIZE - 1);
    */
    vuint8m1_t vs = __riscv_vle8_v_u8m1(block->data + 1, vl - 1);
    __riscv_vse8_v_u8m1(block->data, vs, vl - 1);

    block->data[BLOCK_SIZE - 1] = elem;

    /*
      tmp = 0;
      for (j = 0; j < BLOCK_SIZE; ++j)
        tmp ^= Muls[block->data[j]][j];
      block->data[BLOCK_SIZE - 1] = tmp;
    */

    vuint8m1_t vtmp = __riscv_vmv_v_x_u8m1(0, vl);
    vuint8m1_t vidxs = __riscv_vle8_v_u8m1(block->data, vl);
    vuint16m2_t vext = __riscv_vwcvtu_x_x_v_u16m2(vidxs, vl);
    vext = __riscv_vmadd_vx_u16m2(vext, BLOCK_SIZE, vec_j, vl);
    vuint8m1_t v1 = __riscv_vloxei16_v_u8m1(*Muls, vext, vl);
    vtmp = __riscv_vredxor_vs_u8m1_u8m1(v1, vtmp, vl);

    block->data[BLOCK_SIZE - 1] = __riscv_vmv_x_s_u8m1_u8(vtmp);
  }
}

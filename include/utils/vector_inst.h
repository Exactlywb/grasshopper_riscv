#ifndef _RISCV_GRASSHOPPER_H__
#define _RISCV_GRASSHOPPER_H__

#if defined __has_include
#if __has_include(<riscv_vector.h>)
#include <riscv_vector.h>
#else

void perror(const char *s);
void exit(int code);

typedef unsigned int vuint8m1_t;
typedef unsigned int vuint16m2_t;

#include <errno.h>

#define NO_RISCV_SIMD_ABORT                                                    \
  do {                                                                         \
    errno = ENOENT;                                                            \
    perror("riscv_vector.h is not found");                                     \
    exit(1);                                                                   \
  } while (0)

vuint8m1_t __riscv_vsetvl_e8m1(unsigned size) { NO_RISCV_SIMD_ABORT; }
vuint8m1_t __riscv_vle8_v_u8m1(const void *p, vuint8m1_t v1) {
  NO_RISCV_SIMD_ABORT;
}
vuint8m1_t __riscv_vxor_vv_u8m1(vuint8m1_t v1, vuint8m1_t v2, vuint8m1_t v3) {
  NO_RISCV_SIMD_ABORT;
}
void __riscv_vse8_v_u8m1(const void *p, vuint8m1_t v1, vuint8m1_t v2) {
  NO_RISCV_SIMD_ABORT;
}

vuint16m2_t __riscv_vid_v_u16m2 (unsigned size) {
  NO_RISCV_SIMD_ABORT;
}

vuint8m1_t __riscv_vmv_v_x_u8m1 (unsigned a, unsigned b)
{
  NO_RISCV_SIMD_ABORT;
}

vuint16m2_t __riscv_vwcvtu_x_x_v_u16m2 (vuint8m1_t v, unsigned b)
{
  NO_RISCV_SIMD_ABORT;
}

vuint16m2_t __riscv_vmadd_vx_u16m2 (unsigned a, vuint16m2_t b, vuint16m2_t c, unsigned d)
{
  NO_RISCV_SIMD_ABORT;
}

vuint8m1_t __riscv_vloxei16_v_u8m1 (void *p, vuint16m2_t v, unsigned s)
{
  NO_RISCV_SIMD_ABORT;
}

vuint8m1_t __riscv_vredxor_vs_u8m1_u8m1 (vuint8m1_t v1, vuint8m1_t v2, unsigned l)
{
  NO_RISCV_SIMD_ABORT;
}

unsigned char __riscv_vmv_x_s_u8m1_u8 (vuint8m1_t v)
{
  NO_RISCV_SIMD_ABORT;
}

#endif
#else
#error "__has_include is not defined"
#endif

#endif

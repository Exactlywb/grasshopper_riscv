#ifndef _RISCV_GRASSHOPPER_H__
#define _RISCV_GRASSHOPPER_H__

#if defined __has_include
#if __has_include(<riscv_vector.h>)
#include <riscv_vector.h>
#else

void perror(const char *s);
void exit(int code);

typedef unsigned int vuint8m1_t;

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

#endif
#else
#error "__has_include is not defined"
#endif

#endif

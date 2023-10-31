/* This file provides interface for tables generation.
 *
 * Generate tables:
 * > Coefs[NUM_ROUNDS / 2 - 1][BIT_IN_BYTES]
 * > Muls[256][16]
 * > LS_tbl[16][256][16]
 *
 * These tables are used in grasshopper encrypt algorithm.  Instead of
 * generate it every time in runtime, we do it once by this file.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#define BLOCK_SIZE 16
#define NUM_ROUNDS 10

#define BIT_IN_BYTES 8

static unsigned char S[0x100] = {
    0xFC, 0xEE, 0xDD, 0x11, 0xCF, 0x6E, 0x31, 0x16, // 00..07
    0xFB, 0xC4, 0xFA, 0xDA, 0x23, 0xC5, 0x04, 0x4D, // 08..0F
    0xE9, 0x77, 0xF0, 0xDB, 0x93, 0x2E, 0x99, 0xBA, // 10..17
    0x17, 0x36, 0xF1, 0xBB, 0x14, 0xCD, 0x5F, 0xC1, // 18..1F
    0xF9, 0x18, 0x65, 0x5A, 0xE2, 0x5C, 0xEF, 0x21, // 20..27
    0x81, 0x1C, 0x3C, 0x42, 0x8B, 0x01, 0x8E, 0x4F, // 28..2F
    0x05, 0x84, 0x02, 0xAE, 0xE3, 0x6A, 0x8F, 0xA0, // 30..37
    0x06, 0x0B, 0xED, 0x98, 0x7F, 0xD4, 0xD3, 0x1F, // 38..3F
    0xEB, 0x34, 0x2C, 0x51, 0xEA, 0xC8, 0x48, 0xAB, // 40..47
    0xF2, 0x2A, 0x68, 0xA2, 0xFD, 0x3A, 0xCE, 0xCC, // 48..4F
    0xB5, 0x70, 0x0E, 0x56, 0x08, 0x0C, 0x76, 0x12, // 50..57
    0xBF, 0x72, 0x13, 0x47, 0x9C, 0xB7, 0x5D, 0x87, // 58..5F
    0x15, 0xA1, 0x96, 0x29, 0x10, 0x7B, 0x9A, 0xC7, // 60..67
    0xF3, 0x91, 0x78, 0x6F, 0x9D, 0x9E, 0xB2, 0xB1, // 68..6F
    0x32, 0x75, 0x19, 0x3D, 0xFF, 0x35, 0x8A, 0x7E, // 70..77
    0x6D, 0x54, 0xC6, 0x80, 0xC3, 0xBD, 0x0D, 0x57, // 78..7F
    0xDF, 0xF5, 0x24, 0xA9, 0x3E, 0xA8, 0x43, 0xC9, // 80..87
    0xD7, 0x79, 0xD6, 0xF6, 0x7C, 0x22, 0xB9, 0x03, // 88..8F
    0xE0, 0x0F, 0xEC, 0xDE, 0x7A, 0x94, 0xB0, 0xBC, // 90..97
    0xDC, 0xE8, 0x28, 0x50, 0x4E, 0x33, 0x0A, 0x4A, // 98..9F
    0xA7, 0x97, 0x60, 0x73, 0x1E, 0x00, 0x62, 0x44, // A0..A7
    0x1A, 0xB8, 0x38, 0x82, 0x64, 0x9F, 0x26, 0x41, // A8..AF
    0xAD, 0x45, 0x46, 0x92, 0x27, 0x5E, 0x55, 0x2F, // B0..B7
    0x8C, 0xA3, 0xA5, 0x7D, 0x69, 0xD5, 0x95, 0x3B, // B8..BF
    0x07, 0x58, 0xB3, 0x40, 0x86, 0xAC, 0x1D, 0xF7, // C0..C7
    0x30, 0x37, 0x6B, 0xE4, 0x88, 0xD9, 0xE7, 0x89, // C8..CF
    0xE1, 0x1B, 0x83, 0x49, 0x4C, 0x3F, 0xF8, 0xFE, // D0..D7
    0x8D, 0x53, 0xAA, 0x90, 0xCA, 0xD8, 0x85, 0x61, // D8..DF
    0x20, 0x71, 0x67, 0xA4, 0x2D, 0x2B, 0x09, 0x5B, // E0..E7
    0xCB, 0x9B, 0x25, 0xD0, 0xBE, 0xE5, 0x6C, 0x52, // E8..EF
    0x59, 0xA6, 0x74, 0xD2, 0xE6, 0xF4, 0xB4, 0xC0, // F0..F7
    0xD1, 0x66, 0xAF, 0xC2, 0x39, 0x4B, 0x63, 0xB6  // F8..FF
};

static unsigned char Lin[0x10] = {0x94, 0x20, 0x85, 0x10, 0xC2, 0xC0,
                                  0x01, 0xFB, 0x01, 0xC0, 0xC2, 0x10,
                                  0x85, 0x20, 0x94, 0x01};

typedef struct Block {
  unsigned char data[BLOCK_SIZE];
} Block;

void block_dump_to_file(const Block *block, FILE* fd) {
  fprintf(fd, "{");
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    if (i == BLOCK_SIZE-1) {
      fprintf(fd, "%#x} ", block->data[i]);
    } else {
      fprintf(fd, "%#x, ", block->data[i]);
    }
  }
}

void apply_l(Block *block, unsigned char Muls[][16]) {
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

/* Work-horse for generate_mul_tbl.  */
static unsigned char poly_mul(unsigned char left, unsigned char right) {
  /* p(x) = x**8 + x**7 + x**6 + x + 1:
       0b111000011 = 0x1C3

     with no MSB: 0xC3.  */
  unsigned char ret = 0;
  while (left && right) {
    if (right & 1)
      ret ^= left;

    left = (left << 1) ^ (left & 0x80 ? 0xC3 : 0x00);
    right >>= 1;
  }

  return ret;
}

/* Pregenerations.  */
void generate_mul_tbl(unsigned char Muls[][16]) {
  for (int i = 0; i < 256; ++i)
    for (int j = 0; j < 16; ++j)
      Muls[i][j] = poly_mul(i, Lin[j]);
}

void generate_coef_tbl(Block Coefs[][BIT_IN_BYTES], unsigned char Muls[][16]) {
  for (int i = 0; i < NUM_ROUNDS / 2 - 1; ++i)
    for (int j = 0; j < BIT_IN_BYTES; ++j) {
      Block *c = &Coefs[i][j];
      memset(c, 0, BLOCK_SIZE);
      c->data[BLOCK_SIZE - 1] = i * 8 + j + 1;
      apply_l(c, Muls);
    }
}

void generate_LS_tbl(unsigned char LS_tbl[][256][16]) {
#define MATR_POWER 4
  unsigned char l_tbl[BLOCK_SIZE][BLOCK_SIZE] = {0};

  /* Init matrix.  */
  for (int i = 0; i < BLOCK_SIZE; ++i)
    for (int j = 0; j < BLOCK_SIZE; ++j)
      if (i - 1 == j)
        l_tbl[i][j] = 1;
      else if (i == 0)
        l_tbl[i][j] = Lin[j];

  /* Powering the matrix.  */
  unsigned char tmp[BLOCK_SIZE][BLOCK_SIZE] = {0};
  for (int p = 0; p < MATR_POWER; ++p) {
    for (int i = 0; i < BLOCK_SIZE; ++i)
      for (int j = 0; j < BLOCK_SIZE; ++j)
        for (int k = 0; k < BLOCK_SIZE; ++k)
          tmp[i][j] ^= poly_mul(l_tbl[i][k], l_tbl[k][j]);

    memcpy(l_tbl, tmp, 256 * sizeof(unsigned char));
    memset(tmp, 0, 256 * sizeof(unsigned char));
  }

  for (int i = 0; i < BLOCK_SIZE; ++i)
    for (int j = 0; j < 256; ++j)
      for (int k = 0; k < BLOCK_SIZE; ++k)
        LS_tbl[i][j][k] ^= poly_mul(S[j], l_tbl[k][i]);
}

void write_muls_to_file(FILE* fd, unsigned char Muls[][16]) {
  if (fd == NULL) {
    perror("[-] Error bad file descriptor. Report write_muls_to_file");
    return;
  }

  fprintf(fd, "unsigned char Muls[256][16] = {\n");
  for (size_t i = 0; i < 256; ++i) {
    fprintf(fd, "{\n");
      for (size_t j = 0; j < 16; ++j) {
        if (i == 255 && j == 15) {
          fprintf(fd, "%#x}\n};", Muls[i][j]);
        } else if (j == 15) {
          fprintf(fd, "%#x},\n", Muls[i][j]);
        } else {
          fprintf(fd, "%#x, ", Muls[i][j]);
        }
      }
  }
  fprintf(fd, "\n");
}

void write_ld_tbl_to_file(FILE* fd, unsigned char LS_tbl[][256][16]) {
  if (fd == NULL) {
    perror("[-] Error bad file descriptor. Report write_ld_tbl_to_file");
    return;
  }

  fprintf(fd, "unsigned char LS_tbl[16][256][16] = {\n");
  for (size_t i = 0; i < 16; i++) {
    fprintf(fd, "{\n");
      for (size_t j = 0; j < 256; j++) {
        fprintf(fd, "{");
        for (size_t k = 0; k < 16; k++) {
          if (i == 15 && j == 255 && k == 15) {
            fprintf(fd, "%#x}\n}\n};", LS_tbl[i][j][k]);
          } else if (j == 255 && k == 15) {
            fprintf(fd, "%#x}\n},\n", LS_tbl[i][j][k]);
          } else if (k == 15) {
            fprintf(fd, "%#x},\n", LS_tbl[i][j][k]);
          } else {
            fprintf(fd, "%#x, ", LS_tbl[i][j][k]);
          }
        }
      }
  }
  fprintf(fd, "\n");
}

void write_coefs_to_file(FILE* fd, Block Coefs[][BIT_IN_BYTES]) {
  if (fd == NULL) {
    perror("[-] Error bad file descriptor. Report write_coefs_to_file");
    return;
  }

  fprintf(fd, "Block Coefs[NUM_ROUNDS / 2 - 1][BIT_IN_BYTES] = {\n");
  for (size_t i = 0; i < NUM_ROUNDS / 2 - 1; ++i) {
    fprintf(fd, "{\n");
      for (size_t j = 0; j < BIT_IN_BYTES; ++j) {
        if (i == NUM_ROUNDS / 2 - 2 && j == BIT_IN_BYTES - 1) {
          block_dump_to_file(&Coefs[i][j], fd);
          fprintf(fd, "}\n};");
        } else if (j == BIT_IN_BYTES - 1) {
          block_dump_to_file(&Coefs[i][j], fd);
          fprintf(fd, "},\n");
        } else {
          block_dump_to_file(&Coefs[i][j], fd);
          fprintf(fd, ",\n");
        }
      }
      fprintf(fd, "\n");
  }

  fprintf(fd, "\n");
}

/**
 * This function generates final content of .h file.
 * After that .h file could be included in any other sources you want.
*/
void fill_header_with_content(const char* path_to_header,
                              Block Coefs[][BIT_IN_BYTES],
                              unsigned char Muls[][16],
                              unsigned char LS_tbl[][256][16]) {
  FILE* fd = fopen(path_to_header, "w");
  if (fd == NULL) {
    fprintf(stderr, "[-] Error with open file %s\n", path_to_header);
    exit(1);
  }

  fprintf(fd, "#ifndef _TABLES_GEN_H__\n#define _TABLES_GEN_H__\n\n");
  fprintf(fd, "#include \"config.h\"\n#include \"grasshopper/grasshopper.h\"\n\n");

  write_muls_to_file(fd, Muls);
  write_ld_tbl_to_file(fd, LS_tbl);
  write_coefs_to_file(fd, Coefs);

  fprintf(fd, "#endif\n\n");

  fclose(fd);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage %s path_to_file.h\n", argv[0]);
    return -1;
  }

  const char* path_to_header = argv[1];

  Block Coefs[NUM_ROUNDS / 2 - 1][BIT_IN_BYTES];
  unsigned char Muls[256][16];
  unsigned char LS_tbl[16][256][16];

  memset (Coefs, 0, BLOCK_SIZE * (NUM_ROUNDS / 2 - 1) * (BIT_IN_BYTES));
  memset (Muls, 0, 256 * 16);
  memset (LS_tbl, 0, 16 * 256 * 16);

  generate_mul_tbl(Muls);
  generate_coef_tbl(Coefs, Muls);
  generate_LS_tbl(LS_tbl);

  fill_header_with_content(path_to_header, Coefs, Muls, LS_tbl);
}

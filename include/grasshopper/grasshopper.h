#ifndef _GRASSHOPPER_DRIVER_H__
#define _GRASSHOPPER_DRIVER_H__

#include "utils/config.h"

/* This structure is used to truncate necessary buffer.  This is not
   strict aliasing violation cause all buffers in functions is char *.  */
typedef struct Block {
  unsigned char data[BLOCK_SIZE];
} Block;

typedef struct Key {
  Block first;
  Block second;
} Key;

extern Block Coefs [NUM_ROUNDS / 2 - 1] [BIT_IN_BYTES];
extern unsigned char Muls [256] [16];
extern unsigned char LS_tbl [16] [256] [16];

enum cipher_mode {
  ECB,
  CBC,
  PCBC,
  CFB,
  OFB,
};

typedef Block Keys_Array[NUM_ROUNDS];

// TODO: maybe driver should return pointer on encrypted info?
void grasshopper_encrypt(Block *block, Key *key);
void grasshopper_decrypt(Block *block, Key *key);
void block_dump(const Block *block);

void ECB_encryption(Block *text, Key *key, size_t NBlocks);
void ECB_decryption(Block *text, Key *key, size_t NBlocks);

void CBC_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks);
void CBC_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks);

void PCBC_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks);
void PCBC_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks);

void CFB_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks);
void CFB_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks);

void OFB_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks);
void OFB_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks);

void encryption_driver(enum cipher_mode mode, Block *text, Key *key,
                       const Block *iv, size_t NBlocks);
void decryption_driver(enum cipher_mode mode, Block *text, Key *key,
                       const Block *iv, size_t NBlocks);

/* Interface.  */
void encrypt_block(Block *block, Key *key);
void decrypt_block(Block *block, Key *key);

void generate_keys(Keys_Array keys, Key *key);

void apply_f(Block *first, Block *second, const Block *key);
void apply_x(Block *block, const Block *key);
void apply_ls(Block *block);
void apply_xls(Block *block, const Block *key);
void apply_inv_xls(Block *block, const Block *key);
void apply_inv_l(Block *block);
void apply_s(Block *block, const unsigned char *tbl);

#endif

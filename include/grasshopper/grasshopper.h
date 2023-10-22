#ifndef _GRASSHOPPER_DRIVER_H__
#define _GRASSHOPPER_DRIVER_H__

#include <config.h>

/* This structure is used to truncate necessary buffer.  This is not
   strict aliasing violation cause all buffers in functions is char *.  */
typedef struct Block
{
  unsigned char data [BLOCK_SIZE];
} Block;

typedef struct Key
{
  Block first;
  Block second;
} Key;

typedef Block Keys_Array[NUM_ROUNDS];

//TODO: maybe driver should return pointer on encrypted info?
void grasshopper_encrypt (Block *block, Key *key);
void grasshopper_decrypt (Block *block, Key *key);
void block_dump (const Block *block);

void ECB_encryption(Block *text, const Key* key, size_t NBlocks);
void ECB_decryption(Block *text, const Key* key, size_t NBlocks);

void CBC_encryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);
void CBC_decryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);

void PCBC_encryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);
void PCBC_decryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);

void CFB_encryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);
void CFB_decryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);

void OFB_encryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);
void OFB_decryption(Block *text, const Key* key, const Block* iv, size_t NBlocks);

#endif


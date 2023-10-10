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
void grasshoper_encrypt (Block *block, Key *key);
void block_dump (const Block *block);

#endif


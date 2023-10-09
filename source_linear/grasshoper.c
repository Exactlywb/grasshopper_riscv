#include "config.h"
#include "grasshoper.h"
#include <string.h>

/* This structure is used to truncate necessary buffer.  This is not
   strict aliasing violation cause all buffers in functions is char *.  */
typedef struct Block
{
  unsigned char data [BLOCK_SIZE];
} Block;

static void apply_x (Block *, Key);
static void encrypt_block (Block *, Key, Key);

void
grasshoper_encrypt (char *buf, size_t size, Key key)
{
  Key first_x_key;
  memcpy (first_x_key, key, BLOCK_SIZE);
  Key second_x_key;
  memcpy (second_x_key, key + BLOCK_SIZE, BLOCK_SIZE);

  for (int block_ind = 0; block_ind < size; block_ind += BLOCK_SIZE)
  {
    Block *block = (Block *)(buf + block_ind);
    encrypt_block (block, first_x_key, second_x_key);
    apply_x (block, first_x_key);
    apply_x (block, second_x_key);
  }
}

static void
apply_x (Block *block, Key key)
{
}

static void
encrypt_block (Block *block, Key first, Key second)
{
}


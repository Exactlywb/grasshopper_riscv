#include "grasshopper/grasshopper.h"

#include <string.h>

static void apply_x(Block *block, Block *key);
static void encrypt_block(Block *, Key key);

static void generate_keys (Keys_Array keys, Key key);
static void apply_f (Block *first, Block *second, const Block *key);

static void apply_xsl (Block *block, const Block *key);

//TODO: remove it with autogenerated tables
static void generate_mul_tbl ();
static void generate_coef_tbl ();
static void generate_LS_tbl ();

//TODO: autogenerate this and move to config.h
Block Coefs [NUM_ROUNDS / 2 - 1] [BIT_IN_BYTES];
char Muls [256] [16];
char LS_tbl [16] [256] [16];

void grasshoper_encrypt(Block *block, Key key)
{
  //TODO: remove it with autogenerating for tables
  static int generate = 1;
  if (generate)
  {
    generate_mul_tbl ();
    generate_coef_tbl ();
    generate_LS_tbl ();
    generate = 0;
  }

  encrypt_block (block, key);
  apply_x (block, (Block *)key);
  apply_x (block, (Block *)(key + BLOCK_SIZE));
}

static void
apply_x(Block *block, Block *key)
{
  char *block_byte = (char *)block;
  char *key_byte = (char *)key;
  for (int i = 0; i < BLOCK_SIZE; ++i)
    block_byte [i] = block_byte [i] ^ key_byte [i];
}

static void encrypt_block(Block *block, Key key)
{
  Keys_Array keys;
  generate_keys (keys, key);

  for (int i = 0; i < NUM_ROUNDS - 1; ++i)
    apply_xsl (block, &keys [i]);

  apply_x (block, &keys [NUM_ROUNDS - 1]);
}

static void generate_keys (Keys_Array keys, Key key)
{
  keys [0] = *((Block *)((char *)key + 0 * BLOCK_SIZE));
  keys [1] = *((Block *)((char *)key + 1 * BLOCK_SIZE));

  for (int i = 1; i < NUM_ROUNDS / 2; ++i)
  {
    keys [2 * i + 0] = keys [2 * i - 2];
    keys [2 * i + 1] = keys [2 * i - 1];
    for (int j = 0; j < BIT_IN_BYTES; ++j)
      apply_f (&keys [2 * i], &keys [2 * i + 1], &coef_table [i - 1] [j]);
  }
}

static void apply_f (Block *first, Block *second, const Block *key)
{
  Block tmp = *first;
  apply_xsl (&tmp, key);
  apply_x (&tmp, second);

  /* Swap them.  */
  *second = *first;
  *first = tmp;
}

static void apply_xsl (Block *block, const Block *key)
{
}

static void generate_mul_tbl ()
{
}

static void generate_coef_tbl ()
{
}

static void generate_LS_tbl ()
{
}

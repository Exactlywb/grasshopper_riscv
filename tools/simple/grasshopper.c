#include "grasshopper/grasshopper.h"

#include <string.h>
#include <stdio.h>

static void encrypt_block (Block *block, Key *key);
static void decrypt_block (Block *block, Key *key);

static void generate_keys (Keys_Array keys, Key *key);

static void apply_f (Block *first, Block *second, const Block *key);
static void apply_l (Block *block);
static void apply_x (Block *block, const Block *key);
static void apply_ls (Block *block);
static void apply_xls (Block *block, const Block *key);
static void apply_inv_xls (Block *block, const Block *key);
static void apply_inv_l (Block *block);
static void apply_s (Block *block, const unsigned char *tbl);

//TODO: remove it with autogenerated tables
static void generate_mul_tbl ();
static void generate_coef_tbl ();
static void generate_LS_tbl ();

int get_cipher_block(Block *text, const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file)
    return 0;

  return fread(text->data, sizeof(char), BLOCK_SIZE, file);
}

void encryption_driver (enum cipher_mode mode, Block *text, Key* key, const Block* iv, size_t NBlocks) {
  switch (mode)
  {
    case ECB: ECB_encryption(text, key, NBlocks); break;
    case CBC: CBC_encryption(text, key, iv, NBlocks); break;
    case PCBC: PCBC_encryption(text, key, iv, NBlocks); break;
    case CFB: CFB_encryption(text, key, iv, NBlocks); break;
    case OFB: OFB_encryption(text, key, iv, NBlocks); break;
  }
}

void decryption_driver (enum cipher_mode mode, Block *text, Key* key, const Block* iv, size_t NBlocks) {
  switch (mode)
  {
    case ECB: ECB_decryption(text, key, NBlocks); break;
    case CBC: CBC_decryption(text, key, iv, NBlocks); break;
    case PCBC: PCBC_decryption(text, key, iv, NBlocks); break;
    case CFB: CFB_decryption(text, key, iv, NBlocks); break;
    case OFB: OFB_decryption(text, key, iv, NBlocks); break;
  }
}

// Insure that all 
void ECB_encryption(Block *text, Key* key, size_t NBlocks) {
  for(int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(text + i, key);
  }
}

void ECB_decryption(Block *text, Key* key, size_t NBlocks) {
  for(int i = 0; i < NBlocks; ++i) {
    grasshopper_decrypt(text + i, key);
  }
}

void CBC_encryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for(int i = 0; i < NBlocks; ++i) {
    apply_x(text + i, &result_block);
    grasshopper_encrypt(text + i, key);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
  }
}

void CBC_decryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  Block encrypt_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    memcpy(encrypt_block.data, text[i].data, BLOCK_SIZE);
    grasshopper_decrypt(text + i, key);
    apply_x(text + i, &result_block);
    memcpy(result_block.data, encrypt_block.data, BLOCK_SIZE);
  }
}

void PCBC_encryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  Block open_text;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for(int i = 0; i < NBlocks; ++i) {
    memcpy(open_text.data, text[i].data, BLOCK_SIZE);
    apply_x(text + i, &result_block);
    grasshopper_encrypt(text + i, key);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
    apply_x(&result_block, &open_text);
  }
}

void PCBC_decryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  Block cipher_text;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for(int i = 0; i < NBlocks; ++i) {
    memcpy(cipher_text.data, text[i].data, BLOCK_SIZE);
    grasshopper_decrypt(text + i, key);
    apply_x(text + i, &result_block);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
    apply_x(&result_block, &cipher_text);
  }
}

void CFB_encryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    apply_x(text + i, &result_block);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
  }
}

void CFB_decryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  Block cipher_text;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    memcpy(cipher_text.data, text[i].data, BLOCK_SIZE);
    apply_x(text + i, &result_block);
    memcpy(result_block.data, cipher_text.data, BLOCK_SIZE);
  }
}

// TODO: it is possible to vectorize OFB decryption, encryption. aplly_x can be vectorize.
void OFB_encryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    apply_x(text + i, &result_block);
  }
}

void OFB_decryption(Block *text, Key* key, const Block* iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    apply_x(text + i, &result_block);
  }
}



//TODO: autogenerate this and move to config.h
Block Coefs [NUM_ROUNDS / 2 - 1] [BIT_IN_BYTES];
unsigned char Muls [256] [16];
unsigned char LS_tbl [16] [256] [16];

void grasshopper_encrypt(Block *block, Key *key)
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
  apply_x (&key->first, block);
  apply_x (&key->second, block);
}

static void encrypt_block (Block *block, Key *key)
{
  Keys_Array keys;
  generate_keys (keys, key);

  for (int i = 0; i < NUM_ROUNDS - 1; ++i)
    apply_xls (block, &keys [i]);

  apply_x (block, &keys [NUM_ROUNDS - 1]);
}

void grasshopper_decrypt (Block *block, Key *key)
{
  Key next_key;
  memcpy (&next_key, key, BLOCK_SIZE * 2);

  apply_x (&next_key.first, block);
  apply_x (&next_key.second, block);
  decrypt_block (block, key);

  memcpy (key, &next_key, BLOCK_SIZE * 2);
}

static void decrypt_block (Block *block, Key *key)
{
  Keys_Array keys;
  generate_keys (keys, key);

  for (int i = 0; i < NUM_ROUNDS - 1; ++i)
    apply_inv_xls (block, &keys [NUM_ROUNDS - 1 - i]);

  apply_x (block, &keys [0]);
}

static void generate_keys (Keys_Array keys, Key *key)
{
  keys [0] = key->first;
  keys [1] = key->second;

  for (int i = 1; i < NUM_ROUNDS / 2; ++i)
  {
    keys [2 * i + 0] = keys [2 * i - 2];
    keys [2 * i + 1] = keys [2 * i - 1];
    for (int j = 0; j < BIT_IN_BYTES; ++j)
      apply_f (&keys [2 * i], &keys [2 * i + 1], &Coefs [i - 1] [j]);
  }
}

static void
apply_x (Block *block, const Block *key)
{
  for (int i = 0; i < BLOCK_SIZE; ++i)
    block->data [i] = block->data [i] ^ key->data [i];
}

static void
apply_s (Block *block, const unsigned char *tbl)
{
  for (int i = 0; i < BLOCK_SIZE; ++i)
    block->data [i] = tbl [block->data [i]];
}

static void apply_l (Block *block)
{
  for (int i = 0; i < BLOCK_SIZE; ++i)
  {
    unsigned char tmp = 0;
    for (int j = 0; j < BLOCK_SIZE; ++j)
      tmp ^= Muls [block->data [j]] [j];

    /* Copy backward.  */
    for (int rev = BLOCK_SIZE - 1; rev > 0; --rev)
      block->data [rev] = block->data [rev - 1];

    block->data [0] = tmp;
  }
}

static void apply_f (Block *first, Block *second, const Block *key)
{
  Block tmp = *first;
  apply_xls (&tmp, key);
  apply_x (&tmp, second);

  /* Shift them.  */
  *second = *first;
  *first = tmp;
}

static void apply_ls (Block *block)
{
  unsigned char tmp [16] = {0};
  for (int i = 0; i < BLOCK_SIZE; ++i)
    for (int j = 0; j < BLOCK_SIZE; ++j)
      tmp [j] ^= LS_tbl [i] [block->data [i]] [j];

  memcpy (block->data, tmp, BLOCK_SIZE);
}

static void apply_xls (Block *block, const Block *key)
{
  apply_x (block, key);
  apply_ls (block);
}

static void apply_inv_xls (Block *block, const Block *key)
{
  apply_x (block, key);
  apply_inv_l (block);
  apply_s (block, InvS);
}

static void apply_inv_l (Block *block)
{
  for (int i = 0; i < BLOCK_SIZE; ++i)
  {
    unsigned char elem = block->data [0];
    memcpy (block->data, block->data + 1, BLOCK_SIZE - 1);
    block->data [BLOCK_SIZE - 1] = elem;

    unsigned char tmp = 0;
    for (int j = 0; j < BLOCK_SIZE; ++j)
      tmp ^= Muls [block->data [j]] [j];
    block->data [BLOCK_SIZE - 1] = tmp;
  }
}

/* Work-horse for generate_mul_tbl.  */
static unsigned char poly_mul (unsigned char left, unsigned char right)
{
  /* p(x) = x**8 + x**7 + x**6 + x + 1:
       0b111000011 = 0x1C3

     with no MSB: 0xC3.  */
  unsigned char ret = 0;
  while (left && right)
  {
    if (right & 1)
      ret ^= left;

    left = (left << 1) ^ (left & 0x80 ? 0xC3 : 0x00);
    right >>= 1;
  }

  return ret;
}

/* Pregenerations.  */
static __attribute__((noinline)) void generate_mul_tbl ()
{
  for (int i = 0; i < 256; ++i)
    for (int j = 0; j < 16; ++j)
      Muls [i] [j] = poly_mul (i, Lin [j]);
}

static __attribute__((noinline)) void generate_coef_tbl ()
{
  for (int i = 0; i < NUM_ROUNDS / 2 - 1; ++i)
    for (int j = 0; j < BIT_IN_BYTES; ++j)
    {
      Block *c = &Coefs [i] [j];
      memset (c, 0, BLOCK_SIZE);
      c->data [BLOCK_SIZE - 1] = i * 8 + j + 1;
      apply_l (c);
    }
}

static __attribute__((noinline)) void generate_LS_tbl ()
{
#define MATR_POWER 4
  unsigned char l_tbl [BLOCK_SIZE] [BLOCK_SIZE] = {0};

  /* Init matrix.  */
  for (int i = 0; i < BLOCK_SIZE; ++i)
    for (int j = 0; j < BLOCK_SIZE; ++j)
      if (i - 1 == j)
	l_tbl [i] [j] = 1;
      else if (i == 0)
	l_tbl [i] [j] = Lin [j];

  /* Powering the matrix.  */
  unsigned char tmp [BLOCK_SIZE] [BLOCK_SIZE] = {0};
  for (int p = 0; p < MATR_POWER; ++p)
  {
    for (int i = 0; i < BLOCK_SIZE; ++i)
      for (int j = 0; j < BLOCK_SIZE; ++j)
	for (int k = 0; k < BLOCK_SIZE; ++k)
	  tmp [i] [j] ^= poly_mul (l_tbl [i] [k], l_tbl [k] [j]);

    memcpy (l_tbl, tmp, 256 * sizeof (unsigned char));
    memset (tmp, 0, 256 * sizeof (unsigned char));
  }

  for (int i = 0; i < BLOCK_SIZE; ++i)
    for (int j = 0; j < 256; ++j)
      for (int k = 0; k < BLOCK_SIZE; ++k)
	LS_tbl [i] [j] [k] ^= poly_mul (S [j], l_tbl [k] [i]);
}


void block_dump (const Block *block)
{
  for (int i = 0; i < BLOCK_SIZE; ++i)
    printf ("%c", block->data [i]);

  printf ("\n");
}


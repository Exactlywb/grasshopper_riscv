#include "grasshopper.h"
#include <stdio.h>
#include <string.h>

void encryption_driver(enum cipher_mode mode, Block *text, Key *key,
                       const Block *iv, size_t NBlocks) {
  switch (mode) {
  case ECB:
    ECB_encryption(text, key, NBlocks);
    break;
  case CBC:
    CBC_encryption(text, key, iv, NBlocks);
    break;
  case PCBC:
    PCBC_encryption(text, key, iv, NBlocks);
    break;
  case CFB:
    CFB_encryption(text, key, iv, NBlocks);
    break;
  case OFB:
    OFB_encryption(text, key, iv, NBlocks);
    break;
  }
}

void decryption_driver(enum cipher_mode mode, Block *text, Key *key,
                       const Block *iv, size_t NBlocks) {
  switch (mode) {
  case ECB:
    ECB_decryption(text, key, NBlocks);
    break;
  case CBC:
    CBC_decryption(text, key, iv, NBlocks);
    break;
  case PCBC:
    PCBC_decryption(text, key, iv, NBlocks);
    break;
  case CFB:
    CFB_decryption(text, key, iv, NBlocks);
    break;
  case OFB:
    OFB_decryption(text, key, iv, NBlocks);
    break;
  }
}

// Insure that all
void ECB_encryption(Block *text, Key *key, size_t NBlocks) {
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(text + i, key);
  }
}

void ECB_decryption(Block *text, Key *key, size_t NBlocks) {
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_decrypt(text + i, key);
  }
}

void CBC_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    apply_x(text + i, &result_block);
    grasshopper_encrypt(text + i, key);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
  }
}

void CBC_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
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

void PCBC_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
  Block result_block;
  Block open_text;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    memcpy(open_text.data, text[i].data, BLOCK_SIZE);
    apply_x(text + i, &result_block);
    grasshopper_encrypt(text + i, key);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
    apply_x(&result_block, &open_text);
  }
}

void PCBC_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
  Block result_block;
  Block cipher_text;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    memcpy(cipher_text.data, text[i].data, BLOCK_SIZE);
    grasshopper_decrypt(text + i, key);
    apply_x(text + i, &result_block);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
    apply_x(&result_block, &cipher_text);
  }
}

void CFB_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    apply_x(text + i, &result_block);
    memcpy(result_block.data, text[i].data, BLOCK_SIZE);
  }
}

void CFB_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
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

// TODO: it is possible to vectorize OFB decryption, encryption. aplly_x can be
// vectorize.
void OFB_encryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    apply_x(text + i, &result_block);
  }
}

void OFB_decryption(Block *text, Key *key, const Block *iv, size_t NBlocks) {
  Block result_block;
  memcpy(result_block.data, iv->data, BLOCK_SIZE);
  for (int i = 0; i < NBlocks; ++i) {
    grasshopper_encrypt(&result_block, key);
    apply_x(text + i, &result_block);
  }
}

void grasshopper_encrypt(Block *block, Key *key) {
  encrypt_block(block, key);
  apply_x(&key->first, block);
  apply_x(&key->second, block);
}

void encrypt_block(Block *block, Key *key) {
  Keys_Array keys;
  generate_keys(keys, key);

  for (int i = 0; i < NUM_ROUNDS - 1; ++i)
    apply_xls(block, &keys[i]);

  apply_x(block, &keys[NUM_ROUNDS - 1]);
}

void grasshopper_decrypt(Block *block, Key *key) {
  Key next_key;
  memcpy(&next_key, key, BLOCK_SIZE * 2);

  apply_x(&next_key.first, block);
  apply_x(&next_key.second, block);
  decrypt_block(block, key);

  memcpy(key, &next_key, BLOCK_SIZE * 2);
}

void decrypt_block(Block *block, Key *key) {
  Keys_Array keys;
  generate_keys(keys, key);

  for (int i = 0; i < NUM_ROUNDS - 1; ++i)
    apply_inv_xls(block, &keys[NUM_ROUNDS - 1 - i]);

  apply_x(block, &keys[0]);
}

void generate_keys(Keys_Array keys, Key *key) {
  keys[0] = key->first;
  keys[1] = key->second;

  for (int i = 1; i < NUM_ROUNDS / 2; ++i) {
    keys[2 * i + 0] = keys[2 * i - 2];
    keys[2 * i + 1] = keys[2 * i - 1];
    for (int j = 0; j < BIT_IN_BYTES; ++j)
      apply_f(&keys[2 * i], &keys[2 * i + 1], &Coefs[i - 1][j]);
  }
}

void apply_s(Block *block, const unsigned char *tbl) {
  for (int i = 0; i < BLOCK_SIZE; ++i)
    block->data[i] = tbl[block->data[i]];
}

void apply_f(Block *first, Block *second, const Block *key) {
  Block tmp = *first;
  apply_xls(&tmp, key);
  apply_x(&tmp, second);

  /* Shift them.  */
  *second = *first;
  *first = tmp;
}

void apply_xls(Block *block, const Block *key) {
  apply_x(block, key);
  apply_ls(block);
}

void apply_inv_xls(Block *block, const Block *key) {
  apply_x(block, key);
  apply_inv_l(block);
  apply_s(block, InvS);
}

void block_dump(const Block *block) {
  for (int i = 0; i < BLOCK_SIZE; ++i)
    printf("%c", block->data[i]);

  printf("\n");
}

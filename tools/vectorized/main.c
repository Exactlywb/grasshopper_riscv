#include "config.h"
#include "file_common.h"
#include "grasshopper/grasshopper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool import_key(Key *key, const char *key_file) {
  FILE *input;
  if (!(input = fopen(key_file, "r"))) {
    fprintf(stderr, "Can't open %s test\n", key_file);
    return false;
  }

  // Firstly we read first 16 bytes that will formed first key block
  fread(key->first.data, sizeof(char), BLOCK_SIZE, input);
  // Now we need to read las 16 bytes to forme second key block.
  // To do it we make an offset and set file pointer.
  fseek(input, BLOCK_SIZE, SEEK_SET);
  fread(key->second.data, sizeof(char), BLOCK_SIZE, input);
  fclose(input);
  return true;
}

int main(int argc, char **argv) {
  // Now we should also pass key file, that could be generated by
  // generation/KeyGeneration.py
  if (argc != 3) {
    fprintf(stderr, "Usage: ./grasshopper test_file key_file\n");
    return _EXIT_CODE;
  }

  FILE *input;
  if (!(input = fopen(argv[1], "r"))) {
    fprintf(stderr, "Can't open %s test\n", argv[1]);
    return _EXIT_CODE;
  }

  size_t file_size = get_file_size(input);
  Block *buffer =
      (Block *)allocate_with_padding(file_size, BLOCK_SIZE, &file_size);
  fread(buffer, sizeof(char), file_size, input);

  Key key;
  if (!import_key(&key, argv[2])) {
    return _EXIT_CODE;
  }

  for (size_t block_ind = 0; block_ind < file_size / BLOCK_SIZE; ++block_ind) {
    grasshopper_encrypt(buffer + block_ind, &key);
#if 1
    fprintf(stdout, "Running %ld/%ld block: ", block_ind,
            file_size / BLOCK_SIZE);
    for (int i = 0; i < BLOCK_SIZE; ++i)
      fprintf(stdout, "%c", (buffer + block_ind)->data[i]);
    fprintf(stdout, "\n");
#endif
  }

  fclose(input);
  free(buffer);
  return 0;
}

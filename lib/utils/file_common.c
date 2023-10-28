#include "file_common.h"
#include "config.h"

size_t get_file_size(FILE *f) {
  fseek(f, 0, SEEK_END);
  size_t ret = ftell(f);
  fseek(f, 0, SEEK_SET);

  return ret;
}

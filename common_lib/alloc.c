#include "config.h"
#include <stdlib.h>

void *
allocate_with_padding (size_t size, size_t padding, size_t *res_size)
{
  *res_size = size + padding - (size % padding);
  return calloc (*res_size, sizeof (char));
}


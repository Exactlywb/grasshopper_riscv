#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "grasshoper.h"
#include "file_common.h"

int main (int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf (stderr, "Usage: ./grasshoper test_file\n");
    return _EXIT_CODE;
  }

  FILE *input;
  if (!(input = fopen (argv [1], "r")))
  {
    fprintf (stderr, "Can't open %s test\n", argv [1]);
    return _EXIT_CODE;
  }

  size_t file_size = get_file_size (input);
  char *buffer = (char *)allocate_with_padding (file_size, 128, &file_size);
  fread (buffer, sizeof (char), file_size, input);

  Key key; //TODO: fill the key

  grasshoper_encrypt (buffer, file_size, key);

  fclose (input);

  return 0;
}

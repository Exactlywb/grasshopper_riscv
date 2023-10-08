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
  char *buffer = (char *)allocate_with_padding
		   (file_size, PADDING, &file_size);
  fread (buffer, sizeof (char), file_size, input);

  Key key = {0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11, 0x22,
	     0x33, 0x44, 0x55, 0x66, 0x77, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54,
	     0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

  grasshoper_encrypt (buffer, file_size, key);

  fclose (input);
  free (buffer);
  return 0;
}

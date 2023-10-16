#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "grasshopper/grasshopper.h"
#include "file_common.h"

int main (int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf (stderr, "Usage: ./grasshopper test_file\n");
    return _EXIT_CODE;
  }

  FILE *input;
  if (!(input = fopen (argv [1], "r")))
  {
    fprintf (stderr, "Can't open %s test\n", argv [1]);
    return _EXIT_CODE;
  }

  size_t file_size = get_file_size (input);
  Block *buffer = (Block *)allocate_with_padding
		    (file_size, BLOCK_SIZE, &file_size);
  fread (buffer, sizeof (char), file_size, input);

  Key key = {{0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11, 0x22,
	      0x33, 0x44, 0x55, 0x66, 0x77},
	     {0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45,
	      0x67, 0x89, 0xab, 0xcd, 0xef}};

  for (size_t block_ind = 0; block_ind < file_size / BLOCK_SIZE; ++block_ind)
  {
    grasshopper_encrypt (buffer + block_ind, &key);
#if 1
    fprintf (stdout, "Running %ld/%ld block: ", block_ind, file_size / BLOCK_SIZE);
    for (int i = 0; i < BLOCK_SIZE; ++i)
      fprintf (stdout, "%c", (buffer + block_ind)->data [i]);
    fprintf (stdout, "\n");
#endif
  }

  fclose (input);
  free (buffer);
  return 0;
}

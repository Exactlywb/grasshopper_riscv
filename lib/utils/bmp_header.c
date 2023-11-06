#include "bmp_header.h"

FILE* read_bmp_header(BMPHeader* head1, FILE* file) {
  fread(head1, sizeof(BMPHeader), 1, file);
  return file;
}

FILE* write_bmp_header(BMPHeader* head1, FILE* file) {
  fwrite(head1, sizeof(BMPHeader), 1, file);
  return file;
}

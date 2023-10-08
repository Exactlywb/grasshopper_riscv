#ifndef _GRASSHOPER_CONFIG_H__
#define _GRASSHOPER_CONFIG_H__

#include <stddef.h>

#ifndef _EXIT_CODE
# define _EXIT_CODE 255
#endif

typedef unsigned char Key [32];

void *allocate_with_padding (size_t, size_t, size_t *);

#endif


#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "why.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <complex.h>
#include <math.h>
#include <assert.h>

void*       mem_expand(void* memory, Uint size, Uint extra_size);
ssize_t     BlockReadFromFile(Block* block, Uint index, int file, Uint n_bytes);

#endif
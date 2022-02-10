#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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

#define Q_SORT_THRESHOLD    (1 << 4)
#define FLT_EPSILON         (((Float)1) / (1 << 20))
#define BLOCK_CAPACITY      (1 << 10)
#define HASH_VALUE          (5381)

#endif
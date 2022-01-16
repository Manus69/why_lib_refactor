#ifndef MACRO_H
#define MACRO_H

#include "why.h"

#define GET_FORMAT(x, space) _Generic((x), Uint : "%zu"space, Int : "%ld"space, char * : "%s"space)
#define ABS(x) ((x) > 0 ? (x) : (-x))

#define SWAP(x, y) __extension__ ({__typeof(x) tmp = x; x = y; y = tmp;})
#define SET(mem, index, value) __extension__ (((__typeof(value)*)(mem))[index] = value)
#define GET(target, mem, index) __extension__ (((__typeof(target) *)mem)[index])

#define MEM_GET(mem, index, type) (((type *)mem)[index])
#define MEM_SET(mem, index, value, type) (((type *)mem)[index] = value)
#define MEM_SWAP(mem, j, k, type) (SWAP(((type *)mem)[j], ((type *)mem)[k]))
#define TARGET_SET(target, mem, index, type) (*(type *)target = ((type *)mem)[index])

#endif
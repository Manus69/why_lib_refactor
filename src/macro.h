#ifndef MACRO_H
#define MACRO_H

#include "why.h"

#define COMPARE_NUMERIC(lhs, rhs, type) (*(type *)lhs > *(type *)rhs ? -1 : *(type *)rhs > *(type *)lhs)
#define GET_FORMAT(x, space) _Generic((x), Uint : "%zu"space, Int : "%ld"space, char * : "%s"space)
#define ABS(x) ((x) > 0 ? (x) : (-x))

#define MEM_GET(mem, index, type) (((type *)mem)[index])
#define MEM_SET(mem, index, value, type) (((type *)mem)[index] = value)
#define MEM_SET_FP(mem, index, ptr, type) (((type *)mem)[index] = *(type *)ptr)
#define MEM_SWAP(mem, j, k, type) (SWAP(((type *)mem)[j], ((type *)mem)[k]))
#define TARGET_SET(target, mem, index, type) (*(type *)target = ((type *)mem)[index])

#define SWAP(x, y) __extension__ ({__typeof(x) tmp = x; x = y; y = tmp;})
#define SET(mem, index, value) __extension__ (((__typeof(value)*)(mem))[index] = value)
#define GET(target, mem, index) __extension__ (((__typeof(target) *)mem)[index])

#endif
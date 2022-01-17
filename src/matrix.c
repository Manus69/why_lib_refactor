#include "why.h"
#include "declarations.h"
#include "block.h"

struct Matrix
{
    Block   block;
    Uint    n_rows;
    Uint    n_cols;
};


void MatrixDot(void* target, const Matrix* matrix, Uint row, Uint col);
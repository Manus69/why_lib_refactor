#ifndef MATRIX_H
#define MATRIX_H

#define MATRIX_COL_MAX  (1 << 7)
#define R0_SIZE         (1 << 6)
#define R1_SIZE         (1 << 6)
#define ROW_REG_SIZE    (1 << 12)

struct Matrix
{
    Block*                  block;
    const ArInterface*      interface;
    Uint                    n_rows;
    Uint                    n_cols;
};

#endif
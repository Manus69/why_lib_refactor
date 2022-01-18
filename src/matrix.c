#include "why.h"
#include "declarations.h"
#include "block.h"
#include "matrix.h"

static void* _register0 = NULL;
static void* _register1 = NULL;
static void* _row_register = NULL;

Int MatrixUnitInit(Uint r0_size, Uint r1_size, Uint row_reg_size)
{
    _register0 = malloc(r0_size);
    _register1 = malloc(r1_size);
    _row_register = malloc(row_reg_size);

    if (!_register0 || !_register1 || !_row_register)
    {
        free(_register0);
        free(_register1);
        free(_row_register);

        return WHY_ERROR;
    }

    return WHY_OK;
}

void MatrixUnitTerminate(void)
{
    free(_register0);
    free(_register1);
}

static Matrix* _create(Uint n_rows, Uint n_cols, Block* (block_create)(Uint), const ArInterface* interface)
{
    Matrix* matrix;
    Uint    n_items;

    assert(n_cols < MATRIX_COL_MAX);
    n_items = n_rows * n_cols;

    if ((matrix = malloc(sizeof(*matrix))))
    {
        if ((matrix->block = block_create(n_items)))
        {
            matrix->interface = interface;
            matrix->n_rows = n_rows;
            matrix->n_cols = n_cols;

            return matrix;
        }
        free(matrix);
    }
    
    return NULL;
}

Matrix* MatrixCreateFloat(Uint n_rows, Uint n_cols)
{
    return _create(n_rows, n_cols, BlockCreateFloat, &FloatArInterface);
}

void MatrixDestroy(Matrix* matrix)
{
    BlockDestroy(matrix->block);
    free(matrix);
}

bool MatrixDimEqual(const Matrix* lhs, const Matrix* rhs)
{
    return (lhs->n_rows == rhs->n_rows) && (lhs->n_cols == rhs->n_cols);
}

bool MatrixMultipliable(const Matrix* lhs, const Matrix* rhs)
{
    return lhs->n_cols == rhs->n_rows;
}

static Uint _row_col_to_index(const Matrix* matrix, Uint row, Uint col)
{
    return row * matrix->n_cols + col;
}

void MatrixGet(void* target, const Matrix* matrix, Uint row, Uint col)
{
    BlockGet(target, matrix->block, _row_col_to_index(matrix, row, col));
}

void MatrixSet(Matrix* matrix, Uint row, Uint col, const void* item)
{
    BlockSet(matrix->block, _row_col_to_index(matrix, row, col), item);
}

void MatrixSetNth(Matrix* matrix, Uint n, const void* item)
{
    BlockSet(matrix->block, n, item);
}

void* MatrixPointAt(const Matrix* matrix, Uint row, Uint col)
{
    return BlockPointAt(matrix->block, _row_col_to_index(matrix, row, col));
}

void MatrixDot(void* target, const Matrix* lhs, const Matrix* rhs, Uint row, Uint col)
{
    Uint    n;
    void*   _lhs;
    void*   _rhs;

    assert(MatrixMultipliable(lhs, rhs));

    lhs->interface->zero(_register1);
    n = 0;
    while (n < rhs->n_rows)
    {
        _lhs = MatrixPointAt(lhs, row, n);
        _rhs = MatrixPointAt(rhs, n, col);
        lhs->interface->mult(_register0, _lhs, _rhs);
        lhs->interface->add(_register1, _register1, _register0);

        ++ n;
    }
    lhs->block->interface->get(target, _register1, 0);
}

static void _get_product_row(Uint row, const Matrix* lhs, const Matrix* rhs)
{
    Uint n;

    n = 0;
    while (n < lhs->n_cols)
    {
        MatrixDot(_register0, lhs, rhs, row, n);
        lhs->block->interface->set(_row_register, n, _register0);

        ++ n;
    }
}

static void _copy_row(Matrix* matrix, Uint row)
{
    ;
}

void MatrixMult(Matrix* target, const Matrix* lhs, const Matrix* rhs)
{
    ;
}
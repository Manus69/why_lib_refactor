#include "why.h"
#include "declarations.h"
#include "block.h"
#include "matrix.h"
//
#include "rational.h"
//

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
    free(_row_register);
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

Matrix* MatrixCreateRational(Uint n_rows, Uint n_cols)
{
    return _create(n_rows, n_cols, BlockCreateRational, &RationalArInterface);
}

void MatrixInitFromArray(Matrix* matrix, const void* array)
{
    Uint n;
    Uint n_items;

    n_items = MatrixNItems(matrix);
    n = 0;

    while (n < n_items)
    {
        MatrixSetNth(matrix, n, array);
        array += matrix->block->interface->size;

        ++ n;
    }
}

void MatrixDestroy(Matrix* matrix)
{
    BlockDestroy(matrix->block);
    free(matrix);
}

Uint MatrixNRows(const Matrix* matrix)
{
    return matrix->n_rows;
}

Uint MatrixNCols(const Matrix* matrix)
{
    return matrix->n_cols;
}

Uint MatrixNItems(const Matrix* matrix)
{
    return matrix->n_rows * matrix->n_cols;
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
    while (n < rhs->n_cols)
    {
        MatrixDot(_register0, lhs, rhs, row, n);
        lhs->block->interface->set(_row_register, n, _register0);

        ++ n;
    }
}

static void _copy_row(Matrix* matrix, Uint row)
{
    Uint    n;
    Uint    index;
    void*   item;

    n = 0;
    index = row * matrix->n_cols;

    while (n < matrix->n_cols)
    {
        item = _row_register + (n * matrix->block->interface->size);
        MatrixSetNth(matrix, index, item);

        ++ n;
        ++ index;
    }
}

static Uint _row_size(const Matrix* matrix)
{
    return matrix->n_cols * matrix->block->interface->size;
}

static void _copy_register_to_row(Matrix* matrix, Uint row)
{
    void*   ptr;

    ptr = MatrixPointAt(matrix, row, 0);
    memcpy(ptr, _row_register, _row_size(matrix));
}

static void _copy_row_to_register(const Matrix* matrix, Uint row)
{
    void* ptr;

    ptr = MatrixPointAt(matrix, row, 0);
    memcpy(_row_register, ptr, _row_size(matrix));
}

Int MatrixMult(Matrix* target, const Matrix* lhs, const Matrix* rhs)
{
    Uint row;

    if (!MatrixMultipliable(lhs, rhs))
        return WHY_ERROR;

    row = 0;
    while (row < target->n_rows)
    {
        _get_product_row(row, lhs, rhs);
        // _copy_row(target, row);
        _copy_register_to_row(target, row);
        ++ row;
    }

    return WHY_OK;
}

Int MatrixAdd(Matrix* target, const Matrix* lhs, const Matrix* rhs)
{
    void*   _lhs;
    void*   _rhs;
    void*   _cell;
    Uint    n;

    if (!MatrixDimEqual(lhs, rhs))
        return WHY_ERROR;

    n = 0;
    while (n < lhs->block->n_items)
    {
        _lhs = BlockPointAt(lhs->block, n);
        _rhs = BlockPointAt(rhs->block, n);
        _cell = BlockPointAt(target->block, n);
        lhs->interface->add(_cell, _lhs, _rhs);

        ++ n;
    }

    return WHY_OK;
}

void MatrixSwapRows(Matrix* matrix, Uint j, Uint k)
{
    void* j_ptr;
    void* k_ptr;

    j_ptr = MatrixPointAt(matrix, j, 0);
    k_ptr = MatrixPointAt(matrix, k, 0);
    _copy_row_to_register(matrix, j);
    memcpy(j_ptr, k_ptr, _row_size(matrix));
    _copy_register_to_row(matrix, k);
}

void MatrixScaleRow(Matrix* matrix, Uint row, const void* value)
{
    void*   ptr;
    Uint    n;

    n = 0;
    while (n < matrix->n_cols)
    {
        ptr = MatrixPointAt(matrix, row, n);
        matrix->interface->mult(ptr, ptr, value);
        ++ n;
    }
}

void MatrixAddRows(Matrix* matrix, Uint j, Uint k)
{
    void*   lhs;
    void*   rhs;
    Uint    n;

    n = 0;
    while (n < matrix->n_cols)
    {
        lhs = MatrixPointAt(matrix, j, n);
        rhs = MatrixPointAt(matrix, k, n);
        matrix->interface->add(lhs, lhs, rhs);

        ++ n;
    }
}

void MatrixAddScaledRows(Matrix* matrix, Uint target, Uint source, const void* factor)
{
    void*   lhs;
    void*   rhs;
    Uint    n;

    if (matrix->interface->is_zero(factor))
        return ;
    
    n = 0;
    while (n < matrix->n_cols)
    {
        lhs = MatrixPointAt(matrix, target, n);
        rhs = MatrixPointAt(matrix, source, n);

        //
        // Rational p = RationalCopy(lhs);
        // p = RationalCopy(factor);
        //
        
        matrix->interface->mult(_register0, rhs, factor);
        matrix->interface->add(lhs, lhs, _register0);

        ++ n;
    }
}

void MatrixMapRow(Matrix* matrix, Uint row, void (*function)(void* ))
{
    Uint    n;
    void*   item;

    n = 0;
    while (n < matrix->n_cols)
    {
        item = MatrixPointAt(matrix, row, n);
        function(item);

        ++ n;
    }
}

static bool _is_suitable(const Matrix* matrix, Uint row, Uint col)
{
    void* item;

    item = MatrixPointAt(matrix, row, col);

    return !matrix->interface->is_zero(item);
}

static Int _find_suitable_element(const Matrix* matrix, Uint row, Uint col)
{
    while (row < matrix->n_rows)
    {
        if (_is_suitable(matrix, row, col))
            return row;
        ++ row;
    }

    return NOT_FOUND;
}

static Int _find_pivot_row(const Matrix* matrix, Uint row, Uint col)
{
    Int _row;

    _row = row;
    while (col < matrix->n_cols)
    {
        _row = _find_suitable_element(matrix, _row, col);
        if (_row != NOT_FOUND)
            return _row;
        
        ++ col;
    }

    return NOT_FOUND;
}

Int MatrixFindPivotRow(const Matrix* matrix)
{
    return _find_pivot_row(matrix, 0, 0);
}

static Int _find_nonzero_col(const Matrix* matrix, Uint row)
{
    Uint    n;
    void*   item;

    n = 0;
    while (n < matrix->n_cols)
    {
        item = MatrixPointAt(matrix, row, n);
        if (!matrix->interface->is_zero(item))
            return n;
        
        ++ n;
    }

    return NOT_FOUND;
}

static void* _compute_multiplier(const Matrix* matrix, Uint pivot_row, Uint pivot_col, Uint target_row)
{
    void* pivot;
    void* target;

    pivot = MatrixPointAt(matrix, pivot_row, pivot_col);
    target = MatrixPointAt(matrix, target_row, pivot_col);

    matrix->interface->div(_register1, target, pivot);
    matrix->interface->negate(_register1, _register1);

    return _register1;
}

static void _eliminate(Matrix* matrix, Uint pivot_row, Uint pivot_col, Uint target_row)
{
    void* multiplier;

    multiplier = _compute_multiplier(matrix, pivot_row, pivot_col, target_row);
    MatrixAddScaledRows(matrix, target_row, pivot_row, multiplier);
}

void MatrixRowEliminate(Matrix* matrix, Uint pivot_row, Uint target_row)
{
    Uint col;

    col = _find_nonzero_col(matrix, pivot_row);
    _eliminate(matrix, pivot_row, col, target_row);
}

void MatrixEliminateBelow(Matrix* matrix, Uint pivot_row)
{
    Uint target_row;

    target_row = pivot_row + 1;
    while (target_row < matrix->n_rows)
    {
        MatrixRowEliminate(matrix, pivot_row, target_row);
        ++ target_row;
    }    
}

static void _echelon_form(Matrix* matrix, Uint index)
{
    Int pivot_row;

    while (index < matrix->n_cols && index < matrix->n_rows)
    {
        //
        // PrintMatrix(matrix, PrintRationalP);
        //
        pivot_row = _find_pivot_row(matrix, index, index);
        if (pivot_row == NOT_FOUND)
            return ;
        
        if (pivot_row != (Int)index)
            MatrixSwapRows(matrix, index, pivot_row);

        MatrixEliminateBelow(matrix, pivot_row);
        index = pivot_row + 1;
    }
}

void MatrixEchelonForm(Matrix* matrix)
{
    _echelon_form(matrix, 0);
}

static void _normalize_row(Matrix* matrix, Uint row, Uint pivot_col)
{
    void* pivot;

    pivot = MatrixPointAt(matrix, row, pivot_col);
    matrix->interface->invert(_register0, pivot);
    MatrixScaleRow(matrix, row, _register0);
}

void _eliminate_up(Matrix* matrix, Uint pivot_row, Uint pivot_col)
{
    Int row;

    row = pivot_row - 1;
    while (row >= 0)
    {
        _eliminate(matrix, pivot_row, pivot_col, row);
        -- row;
    }
}

void MatrixEliminateUp(Matrix* matrix)
{
    Int row;
    Int col;

    row = matrix->n_rows - 1;
    while (row >= 0)
    {
        col = _find_nonzero_col(matrix, row);

        if (col == NOT_FOUND)
        {
            -- row;
            continue ;
        }
        _normalize_row(matrix, row, col);
        _eliminate_up(matrix, row, col);

        -- row;
    }
}

#include "declarations.h"
#include "matrix.h"
#include "block.h"

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

Matrix* MatrixCreateUint(Uint n_rows, Uint n_cols)
{
    return _create(n_rows, n_cols, BlockCreateUint, NULL); //this is spooky
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

static Int _init_from_table(Matrix* matrix, const Table* table, 
                                Int (*parse_element)(void * , const char *))
{
    Uint    row;
    Uint    col;
    Int     status;
    void*   target;
    void*   dest;

    row = 0;
    while (row < matrix->n_rows)
    {
        col = 0;
        while (col < matrix->n_cols)
        {
            dest = TablePointAt(table, row, col);
            target = MatrixPointAt(matrix, row, col);
            status = parse_element(target, *(char **)dest);

            if (status <= 0)
                return status;
            
            ++ col;
        }
        ++ row;
    }

    return 1;
}

static Int _check_col_consistency(const Table* table)
{
    Uint    row;
    Uint    n_cols;
    Uint    n_rows;
    Deck*   current_row;

    current_row = TablePointAtRow(table, 0);
    n_cols = DeckNItems(current_row);
    n_rows = TableNRows(table);
    row = 1;

    while (row < n_rows)
    {
        current_row = TablePointAtRow(table, row);
        if (DeckNItems(current_row) != n_cols)
            return WHY_ERROR;
        
        ++ row;
    }

    return n_cols;
}

static Matrix* _create_from_table(const Table* table, Matrix* (*constructor)(Uint, Uint),
                                Int (*parser)(void *, const char *))
{
    Uint    n_rows;
    Int     n_cols;
    Matrix* matrix;
    Int     status;

    if ((n_rows = TableNRows(table)) < 1)
        return NULL;
    
    if ((n_cols = _check_col_consistency(table)) < 1)
        return NULL;
    
    if (!(matrix = constructor(n_rows, n_cols)))
        return NULL;
    
    if ((status = _init_from_table(matrix, table, parser)) <= 0)
    {
        MatrixDestroy(matrix);
        return NULL;
    }

    return matrix;
}

Matrix* MatrixCreateRationalFromTable(const Table* table)
{
    return _create_from_table(table, MatrixCreateRational, ParseRationalWRAP);
}

Matrix* MatrixCreateUintFromTable(const Table* table)
{
    return _create_from_table(table, MatrixCreateUint, ParseUintWRAP);
}

Matrix* MatrixCreateUintFromTableWithParser(const Table* table, Int (*parser)(void *, const char *))
{
    return _create_from_table(table, MatrixCreateUint, parser);
}

void MatrixDestroy(Matrix* matrix)
{
    BlockDestroy(matrix->block);
    free(matrix);
}
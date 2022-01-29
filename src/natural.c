#include "declarations.h"
#include "natural.h"

#define STRING_CHARS_PER_DIGIT (9)

static Natural* _create(Uint capacity)
{
    Natural* natural;

    if ((natural = malloc(sizeof(*natural))))
    {
        if ((natural->digits = malloc(sizeof(*natural->digits) * capacity)))
        {
            natural->capacity = capacity;
            natural->n_digits = 0;

            return natural;
        }
        free(natural);
    }

    return NULL;
}

static void _set_digit(Natural* n, Uint value, Uint index)
{
    n->digits[index] = value;
}

static Int _expand_size(Natural* n, Uint extra_size)
{
    void* new;

    if (!(new = realloc(n->digits, n->n_digits * sizeof(*n->digits) + extra_size)))
        return WHY_ERROR;
    
    n->digits = new;

    return WHY_OK;
}

static Int _expand_n_digits(Natural* n, Uint n_digits)
{
    return _expand_size(n, n_digits * sizeof(*n->digits));
}

static Int _parse_string(Natural* n, const char* string, Uint length)
{
    uint32_t    digit;
    Int         left_index;
    Int         right_index;

    right_index = length - 1;
    left_index = right_index - STRING_CHARS_PER_DIGIT;

    if (left_index < 0)
        left_index = 0;
    
    while (true)
    {
        if ((ParseUint32(&digit, string + left_index, right_index - left_index + 1)) <= 0)
            return WHY_ERROR;
        
        

    }
}

Natural* NaturalCreate(const char* string)
{
    Uint        length;
    Uint        capacity;
    Natural*    n;

    if (!length)
        return 0;
    
    capacity = length / STRING_CHARS_PER_DIGIT + (length % STRING_CHARS_PER_DIGIT != 0);
    if (!(n = _create(capacity)))
        return NULL;


}
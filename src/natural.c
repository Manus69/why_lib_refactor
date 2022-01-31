#include "declarations.h"
#include "natural.h"

#define STRING_CHARS_PER_DIGIT (18)
#define DIGIT_MAX_VALUE (999999999999999999ULL)

static Natural* _create(Uint capacity)
{
    Natural* natural;

    if ((natural = malloc(sizeof(*natural))))
    {
        if ((natural->digits = malloc(sizeof(*natural->digits) * capacity)))
        {
            natural->capacity = capacity;
            natural->n_digits = 0;
            memset(natural->digits, 0, sizeof(*natural->digits) * capacity);

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
    void*   new;

    new = MemExpandZero(n->digits, n->capacity * sizeof(*n->digits), extra_size);

    if (!new)
        return WHY_ERROR;

    free(n->digits);
    n->digits = new;

    return WHY_OK;
}

static Int _expand_n_digits(Natural* n, Uint n_digits)
{
    if ((_expand_size(n, n_digits * sizeof(*n->digits)) != WHY_OK))
        return WHY_ERROR;
    
    n->capacity += n_digits;

    return WHY_OK;
}

static Int _parse_string(Natural* natural, const char* string)
{
    const char* current;
    Deck*       substrings;
    Uint        result;
    Uint        carry;
    Uint        n;

    substrings = StringCutFromEnd(string, STRING_CHARS_PER_DIGIT);
    DeckReverse(substrings);
    n = 0;
    carry = 0;

    while (n < DeckNItems(substrings))
    {
        current = *(const char **)DeckPointAt(substrings, n);
        if ((ParseUint(&result, current)) <= 0)
        {
            DeckDestroy(substrings);
            return WHY_ERROR;
        }

        result += carry;
        if (result  <= DIGIT_MAX_VALUE)
            _set_digit(natural, result, n);
        else
        {
            _set_digit(natural, DIGIT_MAX_VALUE, n);
            carry = result - DIGIT_MAX_VALUE;
        }

        ++ natural->n_digits;
        ++ n;
    }
    DeckDestroy(substrings);

    return TOKEN_PARSE_SUCCESS;
}

Natural* NaturalCreate(const char* string)
{
    Uint        length;
    Uint        capacity;
    Natural*    n;

    if ((length = strlen(string)) == 0)
        return NULL;
    
    capacity = length / STRING_CHARS_PER_DIGIT + (length % STRING_CHARS_PER_DIGIT != 0);
    if (!(n = _create(capacity)))
        return NULL;

    if ((_parse_string(n, string) <= 0))
    {
        NaturalDestroy(n);
        return NULL;
    }

    return n;
}

void NaturalDestroy(Natural* n)
{
    if (n)
    {
        free(n->digits);
        free(n);
    }
}

void NaturalDestroyWRAP(void* n)
{
    NaturalDestroy(*(Natural **)n);
}

static void _add_digits(Natural* target, const Natural* lhs, const Natural* rhs, Uint index, Uint carry)
{
    Uint _lhs;
    Uint _rhs;
    Uint result;

    _lhs = lhs->n_digits > index ? lhs->digits[index] : 0;
    _rhs = rhs->n_digits > index ? rhs->digits[index] : 0;
    result = _lhs + _rhs + carry;

    if (result <= DIGIT_MAX_VALUE)
    {
        _set_digit(target, result, index);
    }
    else
    {
        carry = result - DIGIT_MAX_VALUE;
        return _add_digits(target, lhs, rhs, ++ index, carry);
    }
}

Int NaturalAdd(Natural* target, const Natural* lhs, const Natural* rhs)
{
    Int     diff;
    Uint    n;
    Uint    max_order;
    
    max_order = MAX(lhs->n_digits, rhs->n_digits);
    diff = (Int)target->capacity - (Int)(max_order + 1);
    if (diff < 0)
    {
        if (!(_expand_n_digits(target, -diff) <= 0))
            return WHY_ERROR;

    }

    n = 0;
    while (n < max_order)
    {
        PrintNaturalN(&lhs);
        PrintNaturalN(&rhs);
        _add_digits(target, lhs, rhs, n, 0);
        ++ n;
    }

    return WHY_OK;
}
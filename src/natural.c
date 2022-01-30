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

static Int _parse_string(Natural* natural, const char* string)
{
    const char* current;
    Deck*       substrings;
    Uint        result;
    Uint        carry;
    Uint        n;

    substrings = StringCut(string, STRING_CHARS_PER_DIGIT);
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
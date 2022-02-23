#include "declarations.h"
#include "natural.h"

static Int _expand(Natural* number, Uint extra_digits)
{
    void* new;

    new = MemExpandZero(number->digits, number->capacity + 1, extra_digits);
    if (!new)
        return WHY_ERROR;
    
    free(number->digits);
    number->digits = new;
    number->capacity += extra_digits;

    return WHY_OK;
}

void NaturalInit(Natural* number, Uint n)
{
    Uint index;
    Uint n_digits;

    n_digits = MathCountDigits(n);

    if (n_digits > number->capacity)
    {
        if (_expand(number, n_digits) != WHY_OK)
            return ;
    }

    memset(number->digits, 0, number->capacity);
    if (n == 0)
    {
        number->digits[0] = '0';
    }

    index = 0;
    while (n)
    {
        number->digits[index] = (n % 10) + '0';
        n = n / 10;
        ++ index;
    }
}

void NaturalInitStrLength(Natural* number, const char* string, Uint length)
{
    Uint n;

    if (number->capacity < length)
    {
        if (_expand(number, length) != WHY_OK)
            return ;
    }

    memset(number->digits, 0, number->capacity);

    n = 0;
    while (n < length)
    {
        number->digits[n] = string[length - n - 1];
        ++ n;
    }
}

void NaturalInitStr(Natural* number, const char* string)
{
    return NaturalInitStrLength(number, string, strlen(string));
}

char* NaturalGetDigits(const Natural* number)
{
    return number->digits;
}

static Natural* _create(Uint n_digits)
{
    Natural* number;

    if ((number = malloc(sizeof(*number))))
    {
        if ((number->digits = MemZero(n_digits + 1)))
        {
            number->capacity = n_digits;
            return number;
        }
        free(number);
    }

    return number;
}

Natural* NaturalCreateZero(Uint n_digits)
{
    return _create(n_digits);
}

Natural* NaturalCreate(const char* digit_string)
{
    Natural*    number;
    Uint        length;
    const char* current;
    
    if (*digit_string == '0')
        return NULL;
    
    current = digit_string;
    while (IsDigit(*current))
        ++ current;
    
    if (*current)
        return NULL;
    
    length = current - digit_string;
    if (!(number = _create(length)))
        return NULL;
    
    NaturalInitStrLength(number, digit_string, length);

    return number;
}

Natural* NaturalCreateFromUint(Uint n)
{
    Uint        length;
    Natural*    number;

    length = MathCountDigits(n);
    if (!(number = _create(length)))
        return NULL;

    NaturalInit(number, n);

    return number;
}

void NaturalDestroy(Natural* number)
{
    if (!number)
        return ;
    
    free(number->digits);
    free(number);
}

void NaturalDestroyWRAP(void* number)
{
    NaturalDestroy(*(Natural **)number);
}

Int NaturalCompare(const Natural* lhs, const Natural* rhs)
{
    Uint _lhs_length;
    Uint _rhs_length;
    Int  n;

    _lhs_length = strlen(lhs->digits);
    _rhs_length = strlen(rhs->digits);

    if (_lhs_length > _rhs_length)
        return -1;
    if (_lhs_length < _rhs_length)
        return 1;
    if (!_lhs_length)
        return 0;

    n = _lhs_length - 1;
    while (n >= 0)
    {
        if (lhs->digits[n] > rhs->digits[n])
            return -1;
        else if (lhs->digits[n] < rhs->digits[n])
            return 1;
        
        -- n;
    }
    
    return 0;
}

Int NaturalCompareWRAP(const void* lhs, const void* rhs)
{
    return NaturalCompare(*(const Natural **)lhs, *(const Natural **)rhs);
}

static char _value(char c)
{
    return c > 0 ? c - '0' : 0;
}

Uint NaturalNDIgits(const Natural* n)
{
    return strlen(n->digits);
}

void NaturalSet(Natural* number, const Natural* rhs)
{
    if (number->capacity < rhs->capacity)
    {
        if (_expand(number, rhs->capacity - number->capacity) != WHY_OK)
            return ;
    }

    memmove(number->digits, rhs->digits, rhs->capacity);
}

static Int _check_capacity(Natural* target, const Natural* lhs, const Natural* rhs)
{
    Uint max;
    Uint _lhs;
    Uint _rhs;

    _lhs = NaturalNDIgits(lhs);
    _rhs = NaturalNDIgits(rhs);
    max = MAX(_lhs, _rhs);

    if (target->capacity >= max * 2)
        return WHY_OK;

    return _expand(target, 2 * max);
}

void NaturalAdd(Natural* target, const Natural* lhs, const Natural* rhs)
{
    char    carry;
    char    result;
    Uint    _lhs;
    Uint    _rhs;
    Uint    index;

    // memset(target->digits, 0, target->capacity);
    if (_check_capacity(target, lhs, rhs) != WHY_OK)
        return ;

    carry = 0;
    _lhs = 0;
    _rhs = 0;
    index = 0;

    while (lhs->digits[_lhs] || rhs->digits[_rhs] || carry)
    {
        result = _value(lhs->digits[_lhs]) + _value(rhs->digits[_rhs]) + carry;

        if (result >= 10)
        {
            result = result % 10;
            carry = 1;
        }
        else
            carry = 0;
        
        target->digits[index] = result + '0';

        ++ index;
        _lhs = lhs->digits[_lhs] ? _lhs + 1 : _lhs;
        _rhs = rhs->digits[_rhs] ? _rhs + 1 : _rhs;
    }
}

static void _mult_by_digit(char* target, const char* lhs, Uint start, char digit)
{
    char result;
    char carry;

    carry = 0;
    while (true)
    {
        if (!*lhs && !carry)
            return ;

        result = _value(*lhs) * _value(digit) + _value(target[start]) + carry;

        if (result >= 10)
        {
            carry = result / 10;
            result = result % 10;
        }
        else
        {
            carry = 0;
        }

        target[start] = result + '0';

        ++ start;
        ++ lhs;
    }
}

void NaturalMult(Natural* target, const Natural* lhs, const Natural* rhs)
{
    Uint        n;  
    Natural*    buffer;

    if (_check_capacity(target, lhs, rhs) != WHY_OK)
        return ;

    buffer = NaturalCreateZero(target->capacity);
    n = 0;
    memset(buffer->digits, 0, target->capacity);

    while (rhs->digits[n])
    {
        _mult_by_digit(buffer->digits, lhs->digits, n, rhs->digits[n]);
        ++ n;
    }

    memcpy(target->digits, buffer->digits, target->capacity);
    NaturalDestroy(buffer);
}

// void NaturalPower(Natural* target, const Natural* number, Uint exponent)
// {
//     Natural* buffer;

//     if (exponent == 0)
//         return NaturalInit(target, 1);
    
//     if (exponent == 1)
//         return NaturalSet(target, number);

//     buffer = NaturalCreateZero(target->capacity);
//     NaturalSet(buffer, number);
//     -- exponent;

//     while (exponent)
//     {
//         NaturalMult(buffer, buffer, number);
//         -- exponent;
//     }

//     NaturalSet(target, buffer);
//     NaturalDestroy(buffer);
// }

void NaturalPower(Natural* target, const Natural* number, Uint exponent)
{
    Natural* buffer;

    if (exponent == 0)
        return NaturalInit(target, 1);
    
    if (exponent == 1)
        return NaturalSet(target, number);

    if (!(buffer = NaturalCreateZero(target->capacity)))
        return ;
    
    if (exponent % 2 == 0)
    {
        NaturalMult(buffer, number, number);
        NaturalPower(target, buffer, exponent / 2);
    }
    else
    {
        NaturalMult(buffer, number, number);
        NaturalPower(buffer, buffer, (exponent - 1) / 2);
        NaturalMult(target, buffer, number);
    }

    NaturalDestroy(buffer);
}
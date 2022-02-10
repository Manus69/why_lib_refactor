#include "declarations.h"

void NaturalInit(char* target, Uint n)
{
    if (n == 0)
    {
        *target = '0';
        return ;
    }

    while (n)
    {
        *target = ((n % 10)) + '0';
        ++ target;
        n = n / 10;
    }
}

char* NaturalCreate(const char* string)
{
    Uint        length;
    char*       result;
    const char* current;
    
    if (*string == '0')
        return NULL;
    
    current = string;
    while (IsDigit(*current))
        ++ current;
    
    if (*current)
        return NULL;
    
    length = current - string;
    result = StringNCopy(string, length);
    StringReverseLength(result, length);

    return result;
}

static char _value(char c)
{
    return c > 0 ? c - '0' : 0;
}

Uint NaturalAddRetDigits(char* target, const char* lhs, const char* rhs)
{
    char    carry;
    char    result;
    char*   start;

    carry = 0;
    start = target;
    while (*lhs || *rhs || carry)
    {
        result = _value(*lhs) + _value(*rhs) + carry;

        if (result >= 10)
        {
            result = result % 10;
            carry = 1;
        }
        else
            carry = 0;
        
        *target = result + '0';

        lhs = *lhs ? lhs + 1 : lhs;
        rhs = *rhs ? rhs + 1 : rhs;
        ++ target;
    }

    return target - start;
}

void NaturalAdd(char* target, const char* lhs, const char* rhs)
{
    NaturalAddRetDigits(target, lhs, rhs);
}

void NaturalSetLength(char* target, const char* number, Uint n_digits)
{
    memcpy(target, number, n_digits);
}

void NaturalSet(char* target, const char* number)
{
    Uint length;

    length = strlen(number);

    NaturalSetLength(target, number, length);
}
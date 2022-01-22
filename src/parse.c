#include "declarations.h"

bool IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool IsSpace(char c)
{
    return (c == ' ') || (c == '\t');
}

Int ParseUint(Uint* target, const char* string)
{
    Uint            result;
    const char*     current;

    if (!string)
        return WHY_ERROR;

    if (*string == '0')
    {
        UintInit(target, 0);
        return 1;
    }

    current = string;
    result = 0;

    while (*current && IsDigit(*current))
    {
        result = result * 10 + (*current - '0');
        ++ current;

        if (result > WHY_MAX)
            return WHY_ERROR;
    }

    UintInit(target, result);

    return current - string;
}

Int ParseInt(Int* target, const char* string)
{
    Int     status;
    Uint    n;

    if (!string)
        return WHY_ERROR;
    
    if (*string == '-')
    {
        if ((status = ParseUint(&n, ++ string)) > 0)
        {
            IntInit(target, -1 * (Int)n);
            return 1 + status;
        }

        return WHY_ERROR;
    }

    if ((status = ParseUint(&n, string)) > 0)
        IntInit(target, (Int)n);
    
    return status;
}

Int ParseRational(Rational* target, const char* string)
{
    Int         top;
    Int         bot;
    Int         status;
    const char* current;

    current = string;
    if ((status = ParseInt(&top, string)) <= 0)
        return status;
    
    while (*current && IsSpace(*current))
        ++ current;
    
    if (*current == '/')
        ++ current;
    else return 0;

    while (*current && IsSpace(*current))
        ++ current;

    if ((status = ParseInt(&bot, current)) <= 0)
        return status;
    
    RationalInit(target, top, bot);

    return current - string;
}
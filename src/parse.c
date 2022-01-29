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

bool IsWSpace(char c)
{
    return c == ' ';
}

Int ParseUint32(uint32_t* target, const char* string, Uint length)
{
    Uint        result;
    const char* current;

    if (!string)
        return WHY_ERROR;
    
    if (!*string || !length)
        return 0;
    
    if (*string == '0')
    {
        *target = 0;
        return 1;
    }

    current = string;
    result = 0;

    while (IsDigit(*current) && length)
    {
        result = result * 10 + (*current - '0');
        ++ current;
        -- length;
        
        if (result > UINT32_MAX)
            return WHY_ERROR;
    }

    *target = (uint32_t)result;

    return current - string;
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

Int ParseUintWRAP(void* target, const char* string)
{
    return ParseUint(target, string);
}

Int ParseDigitSequence(Uint* target, const char* string)
{
    const char* start;

    start = string;
    while (*string == '0')
        ++ string;

    return (string - start) + ParseUint(target, string);
}

Int ParseDigitSequenceWRAP(void* target, const char* string)
{
    return ParseDigitSequence(target, string);
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

Int ParseIntWRAP(void* target, const char* string)
{
    return ParseInt(target, string);
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
    
    current += status;
    if (*current && IsSpace(*current))
        ++ current;
    
    if (*current == '/')
        ++ current;
    else
    {
        RationalInit(target, top, 1);
        return current - string;
    }

    if (*current && IsSpace(*current))
        ++ current;

    if ((status = ParseInt(&bot, current)) <= 0)
        return status;
    
    if (bot == 0)
        return WHY_ERROR;
    
    current += status;
    RationalInit(target, top, bot);

    return current - string;
}

Int ParseRationalWRAP(void* target, const char* string)
{
    return ParseRational(target, string);
}

#include "why.h"
#include "declarations.h"

struct Why
{
    clock_t start;
    clock_t end;
};

struct Why _why;

Int WhyStart(void)
{
    _why = (struct Why){0};
    _why.start = clock();
    srandom(_why.start);

    return WHY_OK;
}

void WhyEnd(void)
{
    _why.end = clock();
    PrintTimeDiff(_why.start, _why.end);
}
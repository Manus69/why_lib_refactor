#include "why.h"
#include "declarations.h"
#include "matrix.h"

struct Why
{
    clock_t start;
    clock_t end;
    Deck*   pointers;
};

struct Why _why;

Int WhyStart(void)
{
    _why = (struct Why){0};
    _why.start = clock();
    _why.pointers = DeckCreatePtr(NULL, MemDestroy);
    srandom(_why.start);

    if (MatrixUnitInit(R0_SIZE, R1_SIZE, ROW_REG_SIZE) != WHY_OK)
        return WHY_ERROR;

    return WHY_OK;
}

void WhyEnd(void)
{
    MatrixUnitTerminate();
    DeckDestroy(_why.pointers);
    _why.end = clock();
    PrintTimeDiff(_why.start, _why.end);
}

Int WhySavePtr(const void* ptr)
{
    return DeckPushBack(_why.pointers, ptr);
}
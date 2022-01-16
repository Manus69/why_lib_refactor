#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <complex.h>

#include "why.h"
//
#include "block.h"
#include "macro.h"
#include "declarations.h"
#include "rational.h"
//

void block_test()
{
    Block block;

    BlockInit(&block, 10, Ptr);

    BlockSet(&block, 1, "ass");
    char* item;
    char* item2;

    BlockGet(&item, &block, 1);
    printf("%s\n", item);

    item = "cock";
    BlockSet(&block, 0, item);
    BlockSwap(&block, 0, 1);
    BlockGet(&item2, &block, 1);
    printf("%s\n", item2);

    BlockDestroy(&block);

}

int main()
{
    block_test();    

    return EXIT_SUCCESS;
}
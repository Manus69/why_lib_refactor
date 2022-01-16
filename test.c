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
#include "macro.h"
#include "declarations.h"
#include "rational.h"
//

void block_test()
{
    Block* block;

    Uint limit = 1000000;
    block = BlockCreate(limit, Float);
    Uint n = 0;
    Float x = 3.14159;

    while (n < limit)
    {
        BlockSet(block, n, x);
        x = x + 1;
        ++ n;
    }

    BlockGet(&x, block, limit - 1);
    printf("%f\n", x);
    Float* p = BlockPointAt(block, 100);
    printf("%f\n", *p);
    printf("%ld\n", ComapreFloat(BlockPointAt(block, 1), BlockPointAt(block, 1)));
    p = BlockPointAt(block, 0);
    *p = -1;
    BlockGet(p, block, 0);
    printf("%f\n", *p);
    BlockDestroy(block);

    block = BlockCreatePtr(1000);
    BlockSet(block, 0, "cock");
    BlockSet(block, 1, "this is a test");
    BlockSet(block, 2, "0123456789");
    BlockSwap(block, 0, 2);

    char** str = BlockPointAt(block, 1);
    printf("%s\n", *str);
    printf("%ld\n", ComapreFloat(BlockPointAt(block, 1), BlockPointAt(block, 2)));


    BlockDestroy(block);
}

void input_test()
{
    Byte* bytes;

    // bytes = ReadFile("text_file.txt");
    bytes = ReadFile("text_file.txt");

    printf("%s\n", bytes);
    free(bytes);
}

void array_test()
{
    ;
}

int main()
{
    // block_test();
    input_test();

    // Uint size = 4096;
    // void* memory = malloc(size);
    // int file = open("text_file.txt", O_RDONLY);
    // ssize_t read_size = read(file, memory, size);
    // free(memory);

    return EXIT_SUCCESS;
}
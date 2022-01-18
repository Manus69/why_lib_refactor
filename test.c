#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <complex.h>
#include <time.h>

#include "why.h"
//
#include "macro.h"
#include "declarations.h"
#include "rational.h"
//

void block_test()
{
    Block* block;

    block = BlockCreate(10, Ptr);
    char* str0 = "ass";
    char* str1 = "ka;flgjaiovbgja";

    BlockSet(block, 1, str0);
    BlockSet(block, 2, str1);
    BlockSwap(block, 1, 2);

    char* str;
    BlockGet(&str, block, 1);
    printf("%s\n", str);
    BlockDestroy(block);

    block = BlockCreate(100, Float);
    Float x, y, z;
    x = M_PI;
    y = -1;
    BlockSet(block, 30, &x);
    BlockSet(block, 40, &y);
    BlockSwap(block, 30, 40);
    BlockGet(&z, block, 30);
    printf("%f\n", z);
    BlockDestroy(block);
}

void input_test()
{
    Byte* bytes;

    bytes = ReadFile("text_file.txt");
    // printf("%s", (char *)bytes);

    Uint size = 30000;
    Block* block = BlockCreate(size, Ptr);
    Uint n = 0;
    Byte* start = bytes;

    while (true)
    {
        while (*start == '\n')
        {
            *start = '\0';
            ++ start;
        }
        
        BlockSet(block, n, start);
        ++ n;

        while (*start != '\n' && *start)
            ++ start;

        if (!*start)
            break ;
    }

    // PrintBlock(block, 0, n, PrintCstrN);
    QuickSort(block, 0, n - 1, CompareCstr);
    PrintBlock(block, 0, n, PrintCstrN);
    BlockDestroy(block);
    free(bytes);
}

void sort_test()
{
    Block* block;

    Uint size = (1 << 20);
    block = BlockCreate(size, Uint);
    Uint n = 0;
    Uint x;

    while (n < size)
    {
        x = size - n;
        BlockSet(block, n, &x);

        ++ n;
    }

    // PrintBlock(block, 0, size, PrintUintN);
    QuickSort(block, 0, size - 1, CompareUint);
    // PrintBlock(block, 0, size, PrintUintN);
    BlockDestroy(block);
}

void tt_generate2(Uint size)
{
    Uint n;
    Uint limit;

    n = 0;
    limit = ((Uint)1 << size);
    while (n < limit)
    {
        PrintNBits(n, size);
        printf("\n");
        ++ n;
    }
}

void ar_interface_test()
{
    Rational p;
    Rational q;

    RationalArInterface.one(&p);
    PrintRationalN(&p);
    RationalArInterface.add(&q, &p, &p);
    PrintRationalN(&q);
    RationalArInterface.mult(&p, &q, &q);
    PrintRationalN(&p);

}

void type_interface_test()
{
    void* mem = malloc(256);
    char* str0 = "thisi s a test";
    char* str1 = "cock";
    char* str;

    PtrInterface.set(mem, 1, str0);
    PtrInterface.set(mem, 2, str1);

    PtrInterface.get(&str, mem, 1);
    PrintCstrN(str);

    memset(mem, 0, 256);
    
    Rational p, q;
    Uint n = 0;
    Uint limit = (1 << 20);
    // Float x = 3.14;

    while (n < limit)
    {
        RationalInit(&p, n, 1337);
        // RationalAdd(&p, &p, &p);
        RationalAddWRAP(&p, &p, &p);
        // SetRational(mem, 10, &p);
        // RationalInterface.set(mem, 10, &p);
        // GetRational(&q, mem, 10);
        // RationalInterface.get(&q, mem, 10);
        // x += 1.01;


        ++ n;
    }
    // printf("%f\n", x);
    // RationalInit(&p, -1, 100);
    // RationalInterface.set(mem, 11, &p);
    // RationalInterface.swap(mem, 10, 11);
    // RationalInterface.get(&p, mem, 10);
    PrintRational(&p);
    // PrintRationalN(&q);
    free(mem);
}

void matrix_test()
{
    Matrix* A;
    Matrix* B;

    A = MatrixCreateFloat(2, 2);
    B = MatrixCreateFloat(2, 2);

    Float _A[4] = {1.0, 2.0, 0.0, 1.0};
    Float _B[4] = {1.0, 2.0, 0.0, 1.0};

    Uint n = 0;
    while (n < 4)
    {
        MatrixSetNth(A, n, &_A[n]);
        MatrixSetNth(B, n, &_B[n]);

        ++ n;
    }

    Float x;
    MatrixDot(&x, A, B, 0, 1);
    printf("%f\n", x);

    MatrixDestroy(A);
    MatrixDestroy(B);
}

int main()
{
    WhyStart();

    // ar_interface_test();
    // block_test();
    // input_test();
    // sort_test();
    matrix_test();


    WhyEnd();
    return EXIT_SUCCESS;
}
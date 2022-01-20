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
    Deck* lines;

    lines = ReadFileAllLines("text_file.txt");

    // PrintDeck(lines, PrintCstrN);
    SortDeck(lines, CompareCstr);
    // PrintDeck(lines, PrintCstrN);

    DeckDestroy(lines);
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
    // InsertionSort(block, 0, size - 1, CompareUint);
    // PrintBlock(block, 0, size, PrintUintN);
    BlockGet(&x, block, size - 1);
    PrintUintN(&x);

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
    ;
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
    
    Rational p;
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
    Uint size = 10; 
    
    Matrix* A = MatrixCreateFloat(size, size);
    Matrix* B = MatrixCreateFloat(size, size);

    Float x = 1.0;
    Uint n = 0;

    size = size * size;
    while (n < size)
    {
        x += n;
        MatrixSetNth(B, n, &x);
        ++ n;
    }

    PrintMatrix(B, PrintFloatS);
    printf("\n");
    // MatrixAddRows(B, 0, 1);
    Float f = -1;
    MatrixAddScaledRows(B, 0, 1, &f);
    PrintMatrix(B, PrintFloatS);
    //
    // Float factor = -1;
    // MatrixScaleRow(B, 0, &factor);
    // PrintMatrix(B, PrintFloatS);
    // printf("\n");
    //
    // MatrixSwapRows(B, 0, 2);
    // PrintMatrix(B, PrintFloatS);
    // MatrixMult(A, B, B);
    // PrintMatrix(A, PrintFloatS);

    MatrixDestroy(A);
    MatrixDestroy(B);
}

void matrix_rational_test()
{
    Matrix* A = MatrixCreateRational(2, 3);
    Rational array[] = {{1, 1}, {1, 1}, {3, 1}, {3, 1}, {-2, 1}, {4, 1}};
    MatrixInitFromArray(A, array);
    PrintMatrix(A, PrintRationalP);
    MatrixEchelonForm(A);
    PrintMatrix(A, PrintRationalP);

    MatrixDestroy(A);
}

void matrix_add_test()
{
    Uint size = 100;
    Matrix* A = MatrixCreateFloat(size, size);
    Matrix* B = MatrixCreateFloat(size, size);

    Uint n = 0;
    Float x;
    size = size * size;
    while (n < size)
    {
        x = n;
        MatrixSetNth(A, n, &x);
        ++ n;
    }

    // PrintMatrix(A, PrintFloatS);
    MatrixAdd(A, A, A);
    PrintMatrix(A, PrintFloatS);

    MatrixDestroy(A);
    MatrixDestroy(B);
}

void deck_test()
{
    Deck* deck;

    deck = DeckCreatePtr(NULL, NULL);
    DeckPushFront(deck, "first");
    DeckPushBack(deck, "ass");
    DeckPushBack(deck, "thsi is a test");
    DeckPushBack(deck, "cock");
    DeckPushBack(deck, "what?");
    DeckPushBack(deck, "does it work");
    PrintDeck(deck, PrintCstrN);
    
    DeckDestroy(deck);
}

int main()
{
    WhyStart();

    // ar_interface_test();
    // block_test();
    // input_test();
    // sort_test();
    // matrix_test();
    // matrix_add_test();
    matrix_rational_test();
    // deck_test();

    WhyEnd();
    return EXIT_SUCCESS;
}
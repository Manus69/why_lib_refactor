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

    // lines = ReadFileAllLines("text_file.txt");
    lines = ReadFileAllLines2("text_file.txt");
    // PrintDeck(lines, PrintCstrN);
    // SortDeck(lines, CompareCstr);
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
    // Matrix* A = MatrixCreateRational(2, 3);
    // Rational array[] = {{0, 1}, {1, 1}, {3, 1}, {3, 1}, {-2, 1}, {4, 1}};

    // Matrix* A = MatrixCreateRational(3, 4);
    // Rational array[] = {{1, 1}, {-2, 1}, {1, 1}, {0, 1},
    //                     {2, 1}, {1, 1}, {-3, 1}, {5, 1},
    //                     {4, 1}, {-7, 1}, {1, 1}, {-1, 1}};

    // Matrix* A = MatrixCreateRational(3, 5);
    // Rational array[] = {{1, 1}, {-1, 1}, {1, 1}, {-1, 1}, {1, 1},
    //                     {2, 1}, {1, 1}, {-3, 1}, {0, 1}, {2, 1},
    //                     {5, 1}, {-2, 1}, {0, 1}, {-3, 1}, {5, 1}};


    Matrix* A = MatrixCreateRational(3, 1);
    Rational array[] = {{2, 1}, {-2, 1}, {1, 1}};

    MatrixInitFromArray(A, array);
    PrintMatrix(A, PrintRationalP);
    MatrixEchelonForm(A);
    PrintMatrix(A, PrintRationalP);
    MatrixEliminateUp(A);
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

    deck = DeckCreateUint();
    Uint n;

    n = 11;
    DeckPushBack(deck, &n);
    n = 661;
    DeckPushBack(deck, &n);
    PrintDeck(deck, PrintUintN);

    DeckDestroy(deck);
}

void parse_test()
{
    Uint n;
    Int status;

    status = ParseUint(&n, "0");
    PrintIntS(&status);
    PrintUintN(&n);

    status = ParseUint(&n, "137x");
    PrintIntS(&status);
    PrintUintN(&n);

    status = ParseUint(&n, "999999999999999999999999999999999999999999");
    PrintIntS(&status);
    PrintUintN(&n);

    Int m;
    status = ParseInt(&m, "-1");
    PrintIntS(&status);
    PrintIntN(&m);

    status = ParseInt(&m, "-99999999999999999999999999999999999");
    PrintIntS(&status);
    PrintIntN(&m);

    Rational p;
    status = ParseRational(&p, "2/4 ");
    PrintIntS(&status);
    PrintRationalN(&p);

    status = ParseRational(&p, "-137 / 661");
    PrintIntS(&status);
    PrintRationalN(&p);

    status = ParseRational(&p, "-2/0");
    PrintIntS(&status);
    PrintRationalN(&p);
}

void string_test()
{
    // char* str0 = strdup("this is a   test");
    char* str0 = strdup("a,");

    Deck* deck = StringSplitDestructive(str0, ',');
    PrintDeck(deck, PrintCstrN);
    printf("---\n%zu\n", DeckNItems(deck));

    free(str0);
    DeckDestroy(deck);

    char* str = " this, is,,a test";
    deck = StringSplit(str, ',');
    // deck = StringSplitLength(str, ',', 2);
    PrintDeck(deck, PrintCstrN);
    DeckDestroy(deck);
}

static void _table_test(const char* string, char table_sep_left, char table_sep_right,
                        char row_sep_left, char row_sep_right, char col_sep)
{
    Table* table;
    Int status;

    table = TableCreatePtr(NULL, NULL);
    status = ParseTable(table, string, table_sep_left, table_sep_right, row_sep_left, row_sep_right, col_sep);
    PrintCstrN(&string);
    PrintTable(table, PrintCstrS);
    printf("status = %ld\n---------\n", status);

    TableDestroy(table);
}

void table_test()
{
    char* string;

    string = "x;";
    _table_test(string, 0, 0, 0, ';', ' ');

    string = "a, b, c";
    _table_test(string, 0, 0, 0, 0, ',');

    string = "a, ,c";
    _table_test(string, 0, 0, 0, 0, ',');

    string = "[a, b][x, y]";
    _table_test(string, 0, 0, '[', ']', ',');

    string = "[a, b, c; x, y]";
    _table_test(string, '[', ']', 0, ';', ',');

    string = "a]";
    _table_test(string, 0, ']', 0, ';', ',');
}

void matrix_table_test()
{
    Matrix* matrix;
    Table* table;

    table = TableCreatePtr(NULL, NULL);
    Int status = ParseTable(table, "0 0 1;0 1 1;1 1 1", 0, 0, 0, ';', ' ');
    PrintTable(table, PrintCstrS);

    if (status > 0)
    {
        matrix = MatrixCreateRationalFromTable(table);
        MatrixEchelonForm(matrix);
        PrintMatrix(matrix, PrintRationalP);
        MatrixDestroy(matrix);
    }

    TableDestroy(table);
    char* system;
    /*
                00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 rhs
                00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33 rhs
                |        |       |       |       |
    */
    system =    "0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 6\n"
                "0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 8\n"
                "0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 7\n"
                "0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 9\n"
                "0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 3\n"
                "0 1 0 0 0 1 0 0 0 0 1 0 0 1 0 0 10\n"
                "0 0 1 0 0 0 1 1 0 0 1 0 0 0 0 0 10\n"
                "0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 7\n"
                "0 1 0 0 1 1 0 0 0 0 0 0 0 0 0 0 6\n"
                "0 0 1 1 0 0 1 0 0 0 0 0 0 0 0 0 8\n"
                "0 0 0 0 0 0 0 0 0 1 0 0 1 1 0 0 7\n"
                "0 0 0 0 0 0 0 0 0 0 1 1 0 0 1 0 9";

    table = TableCreatePtr(NULL, NULL);
    status = ParseTable(table, system, 0, 0, 0, 0, 0);
    PrintTable(table, PrintCstrS);

    if (status > 0)
    {
        matrix = MatrixCreateRationalFromTable(table);
        PrintMatrix(matrix, PrintRationalP);
        // MatrixDiagonalize(matrix);
        MatrixEchelonForm(matrix);
        PrintMatrix(matrix, PrintRationalP);
        MatrixEliminateUp(matrix);
        PrintMatrix(matrix, PrintRationalP);
        MatrixDestroy(matrix);
    }
    TableDestroy(table);    
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
    // matrix_rational_test();
    deck_test();
    // parse_test();
    // table_test();
    // string_test();
    // matrix_table_test();

    WhyEnd();
    return EXIT_SUCCESS;
}
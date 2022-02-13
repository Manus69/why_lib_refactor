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

#define _FILE_NAME ("text_file.txt")

void block_test()
{
    Block* block;
    void* ptr;

    block = BlockCreateUint(4);
    Uint numbers[] = {1, 10, 100, 1000};
    BlockInitFromArray(block, numbers);

    // PrintBlock(block, PrintUintN);


    Uint n = 0;
    BlockFold(&n, block, UintAddWRAP);
    PrintUintN(&n);

    BlockDestroy(block);
    block = BlockCreatePtr(3);
    char* str = "this is a test";
    BlockSet(block, 0, &str);
    BlockGet(&ptr, block, 0);
    PrintCstrN(&ptr);
    BlockDestroy(block);

    block = BlockCreateUint(3);
    n = INT32_MAX;
    BlockSet(block, 1, &n);
    BlockGet(&n, block, 1);
    PrintUintN(&n);

    BlockDestroy(block);
}

void input_test()
{
    Deck* lines;

    // lines = ReadFileAllLines("text_file.txt");
    lines = ReadFileAllLines2("text_file.txt");
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
    PrintCstrN(&str);

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
    Deck* new_deck;

    // deck = DeckCreatePtr(NULL, NULL);
    // DeckPushFront(deck, "first");
    // DeckPushBack(deck, "ass");
    // DeckPushBack(deck, "thsi is a test");
    // DeckPushBack(deck, "cock");
    // DeckPushBack(deck, "what?");
    // DeckPushBack(deck, "does it work");
    // PrintDeck(deck, PrintCstrN);
    
    // DeckDestroy(deck);

    // deck = DeckCreateUint();
    // Uint n;

    // n = 11;
    // DeckPushBack(deck, &n);
    // n = 661;
    // DeckPushBack(deck, &n);
    // PrintDeck(deck, PrintUintN);
    // DeckDestroy(deck);

    void* ptr;
    deck = StringSplit("11 22 33 44 11", ' ');
    Int r = DeckCompare(deck, 2, 1, CompareCstr);
    PrintIntN(&r);
    DeckGet(&ptr, deck, 1);
    PrintCstrN(&ptr);
    ptr = strdup("ass");
    DeckPushBack(deck, &ptr);
    PrintDeck(deck, PrintCstrP);
    // new_deck = DeckUnique(deck, CompareCstr);
    // PrintDeck(new_deck, PrintCstrP);
    free(ptr);
    DeckDestroy(deck);
    // DeckDestroy(new_deck);

    deck = MathFactor(INT32_MAX - 1);
    PrintDeck(deck, PrintUintS);
    r = DeckCompare(deck, 1, 2, CompareUint);
    PrintUintN(&r);
    new_deck = DeckUnique(deck, CompareInt);
    PrintDeck(new_deck, PrintUintS);
    DeckDestroy(deck);
    DeckDestroy(new_deck);

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
    Deck* strings;
    char* str;

    str = strdup("  a  b");
    strings = StringSplitStr(str, "  ");

    PrintDeck(strings, PrintCstrP);

    DeckDestroy(strings);
    free(str);

    printf("\n------\n");
    str = strdup(" a b");
    strings = StringSplitDestructive(str, ' ');

    PrintDeck(strings, PrintCstrP);

    DeckDestroy(strings);
    free(str);

    // str = (char *)strdup("a--b--c");
    // str = (char *)ReadFile(_FILE_NAME);
    // Byte* another_str = StringSplitSplice(str, "\n");

    // PrintCstrN(&another_str);

    // free(str);
    // free(another_str);

    printf("\n");
    strings = StringCutFromEnd("0123456789", 3);
    PrintDeck(strings, PrintCstrN);

    DeckDestroy(strings);
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

void math_test()
{
    // Uint p = MathGetNthPrime(1000000);
    // PrintUintN(&p);


    Deck* divisors;
    Uint n;

    n = 284;
    divisors = MathFactor(n);
    // PrintDeck(divisors, PrintUintS);
    DeckDestroy(divisors);

    divisors = MathComputeDivisors(n);
    SortDeck(divisors, CompareUint);
    PrintDeck(divisors, PrintUintS);

    DeckDestroy(divisors);

    n = 360 / 100;
    PrintUintN(&n);
}

void natural_test()
{
    Natural* lhs = NaturalCreateFromUint(1);
    Natural* rhs = NaturalCreateFromUint(1);
    Natural* result = NaturalCreateZero(1000);

    NaturalAdd(result, lhs, rhs);
    PrintNaturalN(&result);

    NaturalInit(lhs, 64);
    NaturalInit(rhs, 16);
    NaturalMult(result, lhs, rhs);
    PrintNaturalN(&result);

    NaturalDestroy(lhs);
    NaturalDestroy(rhs);
    NaturalDestroy(result);
}

static Int _find_pivot(const char* str, Uint length)
{
    Int n;

    n = length - 2;
    while (n >= 0)
    {
        if (str[n] < str[n + 1])
        {
            return n;
        }

        -- n;
    }

    return NOT_FOUND;
}

static Int _find_index(const char* str, Int pivot, Uint length)
{
    Int n;

    n = length - 1;
    while (n > pivot)
    {
        if (str[n] > str[pivot])
            return n;
        -- n;
    }

    return NOT_FOUND;
}

static Int _permute(char* str, Uint length)
{
    Int pivot;
    Int index;

    if (length < 2)
        return NOT_FOUND;
    
    pivot = _find_pivot(str, length);

    if (pivot == NOT_FOUND)
    {
        StringReverseLength(str, length);

        return pivot;
    }

    index = _find_index(str, pivot, length);

    SWAP(str[pivot], str[index]);
    StringReverseLength(str + pivot + 1, length - pivot - 1);

    return pivot;
}

void permutation_test()
{
    char* str;

    str = strdup("0123");
    // str = strdup("0123456789");
    Uint length = strlen(str);

    while (_permute(str, length) != NOT_FOUND)
    {
        // PrintCstrN(&str);
    }

    free(str);

    Block* block = BlockCreateInt(5);
    Int array[] = {0, 1, 2, 2, 3};
    BlockInitFromArray(block, array);
    PrintBlockN(block, PrintIntS);
    
    while (BlockPermuteLexical(block, CompareInt))
    {
        PrintBlockN(block, PrintIntS);
    }

    BlockDestroy(block);
}

void hash_table_test()
{
    HashTable* table;
    Deck* strings;

    strings = ReadFileAllLines2(_FILE_NAME);
    table = HashDeck(strings, 3, StringHashWRAP, NULL, NULL, CompareCstr);

    PrintHashTable(table, PrintCstrS);

    char* item = "OPTIONS";
    char* str = *(char **)HashTableFind(table, &item, CompareCstr);
    printf("%s\n", str);

    DeckDestroy(strings);
    HashTableDestroy(table);
    
}

int main()
{
    WhyStart();
    MathUnitInit(1 << 10);

    // ar_interface_test();
    // block_test();
    // input_test();
    // sort_test();
    // matrix_test();
    // matrix_add_test();
    // matrix_rational_test();
    // deck_test();
    // parse_test();
    // table_test();
    // string_test();
    // matrix_table_test();
    // math_test();
    natural_test();
    // permutation_test();
    // hash_table_test();

    // char* str = "ass";
    // PrintCstrN(&str);

    MathUnitTerminate();
    WhyEnd();
    return EXIT_SUCCESS;
}
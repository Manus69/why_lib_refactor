// #include "why.h"
// #include "declarations.h"
// #include "array.h"
// #include "rational.h"

// static void* _copy_default(void* item)
// {
//     return item;
// }

// static void _destroy_default(void* item)
// {
//     (void)item;
// }

// static Uint _index_to_abs(const Array* array, Uint index)
// {
//     return index + array->left_insert_index + 1;
// }

// static void _init(Array* array)
// {
//     array->copy = NULL;
//     array->destroy = NULL;
//     array->left_insert_index = 0;
//     array->right_insert_index = 1;
// }

// static Array* _create(Uint n_items, Int (*_block_init)(Block* , Uint))
// {
//     Array* array;

//     if ((array = malloc(sizeof(*array))))
//     {
//         if (_block_init(&array->block, n_items) == WHY_OK)
//         {
//             _init(array);
//             return array;
//         }
//         free(array);
//     }

//     return NULL;
// }

// Array* ArrayCreateInt(Uint n_items)
// {
//     return _create(n_items, BlockInitInt);
// }

// Array* ArrayCreateUint(Uint n_items)
// {
//     return _create(n_items, BlockInitUint);
// }

// Array* ArrayCreateFloat(Uint n_items)
// {
//     return _create(n_items, BlockInitFloat);
// }

// Array* ArrayCreateRational(Uint n_items)
// {
//     return _create(n_items, BlockInitRational);
// }

// Array* ArrayCreateComplex(Uint n_items)
// {
//     return _create(n_items, BlockInitComplex);
// }

// Array* ArrayCreatePtr(Uint n_items)
// {
//     Array* array;

//     array = _create(n_items, BlockInitPtr);
//     array->copy = _copy_default;
//     array->destroy = _destroy_default;

//     return array;
// }

// Uint ArrayGetSize(const Array* array)
// {
//     return array->right_insert_index - array->left_insert_index - 1;
// }

// void ArraySwap(Array* array, Uint j, Uint k)
// {
//     return array->block.swap(&array->block, _index_to_abs(array, j), _index_to_abs(array, k));
// }

// void ArraySetByte(Array* array, Uint index, Byte value)
// {
//     return BlockSetByte(&array->block, _index_to_abs(array, index), value);
// }

// void ArraySetInt(Array* array, Uint index, Int n)
// {
//     return BlockSetInt(&array->block, _index_to_abs(array, index), n);
// }

// void ArraySetUint(Array* array, Uint index, Uint n)
// {
//     return BlockSetUint(&array->block, _index_to_abs(array, index), n);
// }

// void ArraySetFloat(Array* array, Uint index, Float x)
// {
//     return BlockSetFloat(&array->block, _index_to_abs(array, index), x);
// }

// void ArraySetRational(Array* array, Uint index, Rational p)
// {
//     return BlockSetRational(&array->block, _index_to_abs(array, index), p);
// }

// void ArraySetComplex(Array* array, Uint index, Complex z)
// {
//     return BlockSetComplex(&array->block, _index_to_abs(array, index), z);
// }

// void ArraySetPtr(Array* array, Uint index, void* item)
// {
//     void* copy;

//     copy = array->copy(item);

//     return BlockSetPtr(&array->block, _index_to_abs(array, index), copy);
// }

// static bool _has_right_capacity(const Array* array)
// {
//     return array->block.n_items > array->right_insert_index;
// }

// Int ArrayExpand(Array* array, Uint n_items)
// {
//     if (BlockRealloc(&array->block, n_items) != WHY_OK)
//         return WHY_ERROR;
    
//     return WHY_OK;
// }

// #define _BOUND_CHECK(array) ({if (!_has_right_capacity(array) && (ArrayExpand(array, array->block.n_items) != WHY_OK)) return ;})

// #define _SET(array, value, type) (ArraySet ## type(array, _index_to_abs(array, array->right_insert_index ++), value))

// void ArrayPushByte(Array* array, Byte value)
// {
//     _BOUND_CHECK(array);
//     _SET(array, value, Byte);
// }

// void ArrayPushInt(Array* array, Int n)
// {
//     _BOUND_CHECK(array);
//     _SET(array, n, Int);
// }

// void ArrayPushUint(Array* array, Uint n)
// {
//     _BOUND_CHECK(array);
//     _SET(array, n, Uint);
// }

// void ArrayPushFloat(Array* array, Float x)
// {
//     _BOUND_CHECK(array);
//     _SET(array, x, Float);
// }

// void ArrayPushRational(Array* array, Rational p)
// {
//     _BOUND_CHECK(array);
//     _SET(array, p, Rational);
// }

// void ArrayPushComplex(Array* array, Complex z)
// {
//     _BOUND_CHECK(array);
//     _SET(array, z, Complex);
// }

// void ArrayPushPtr(Array* array, void* ptr)
// {
//     _BOUND_CHECK(array);
//     _SET(array, ptr, Ptr);
// }

// void ArrayGet(void* target, const Array* array, Uint index)
// {
//     return BlockGet(target, &array->block, _index_to_abs(array, index));
// }

// // void ArrayGet(void* target, const Array* array, Uint index)
// // {
// //     return array->block.get(target, &array->block, _index_to_abs(array, index));
// // }

// void ArrayPop(void* target, Array* array)
// {
//     if (ArrayGetSize(array))
//         BlockGet(target, &array->block, _index_to_abs(array, -- array->right_insert_index));
// }

// void ArrayMap(Array* array, void (*function)(void *))
// {
//     Uint    n;
//     void*   item;

//     n = array->left_insert_index + 1;
//     while (n < array->right_insert_index)
//     {
//         item = BlockPointAt(&array->block, n);
//         function(item);

//         ++ n;
//     }
// }

// void ArrayDestroyItems(Array* array)
// {
//     return ArrayMap(array, array->destroy);
// }

// void ArrayDestroy(Array* array)
// {
//     if (array->destroy != _destroy_default)
//         ArrayDestroyItems(array);
    
//     BlockDestroy(&array->block);
//     free(array);
// }
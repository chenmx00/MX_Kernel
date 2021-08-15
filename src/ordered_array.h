//ordered_array.h --Interface for creating, inserting and deleting from ordered arrays.
//Ian C.
#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H
#include "common.h"
typedef void* type_t; //anything that can be casted to void
typedef s8int (*lessthan_predicate_t)(type_t, type_t);
typedef struct 
{
    type_t *array;
    u32int size;
    u32int max_size;
    lessthan_predicate_t lessthan;

}ordered_array_t;


#endif
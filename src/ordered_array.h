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

s8int standard_less_than_predicate(type_t a, type_t b);
ordered_array_t create_ordered_array(u32int max_size, lessthan_predicate_t less_than);
ordered_array_t place_ordered_array(void *addr, u32int max_size, lessthan_predicate_t less_than);
void destroy_ordered_array(ordered_array_t *array);
void insert_ordered_array(type_t item, ordered_array_t *array);
type_t lookup_ordered_array(u32int index, ordered_array_t *array);
void remove_ordered_array(u32int index, ordered_array_t *array);
#endif


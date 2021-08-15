//ordered_array.h --Implementation for creating, inserting and deleting from ordered arrays.
//Ian C.
#include "ordered_array.h"
s8int standard_less_than_predicate(type_t a, type_t b){
    return (a < b)? 1 : 0;
}

ordered_array_t create_ordered_array(s32int max_size, lessthan_predicate_t lessthan){
    ordered_array_t ret;
    ret.array = (void*) kmalloc(max_size * sizeof(type_t));
    memset(ret.array, 0, max_size * sizeof(type_t));
    ret.size = 0;
    ret.max_size = max_size;
    ret.lessthan = lessthan;
    return ret;
}
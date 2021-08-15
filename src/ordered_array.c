//ordered_array.h --Implementation for creating, inserting and deleting from ordered arrays.
//Ian C.
#include "ordered_array.h"
#include "kheap.h"
s8int standard_less_than_predicate(type_t a, type_t b){
    return (a < b)? 1 : 0;
}

ordered_array_t create_ordered_array(u32int max_size, lessthan_predicate_t lessthan){
    ordered_array_t ret;
    ret.array = (void*) kmalloc(max_size * sizeof(type_t));
    memset(ret.array, 0, max_size * sizeof(type_t));
    ret.size = 0;
    ret.max_size = max_size;
    ret.lessthan = lessthan;
    return ret;
}

ordered_array_t place_ordered_array(void *address, u32int max_size, lessthan_predicate_t lessthan){
    ordered_array_t ret;
    ret.array = (type_t*) address;
    memset(ret.array, 0, max_size * sizeof(type_t));
    ret.size = 0;
    ret.max_size = max_size;
    ret.lessthan = lessthan;
    return ret;
}

void destroy_ordered_array(ordered_array_t *array){
    //kfree(array->array);
}

void insert_ordered_array(type_t item, ordered_array_t *array){
    if (array->lessthan){
        u32int iterator = 0;
        while(iterator < array->size && array->lessthan(array->array[iterator], item)){
            iterator++;
        }
        if (array->size == iterator){
            array->array[array->size++] = item;
        } else {
            type_t tmp = array->array[iterator];
            array->array[iterator] = item;
            while (iterator < array->size)
            {
                iterator++;
                type_t tmp2 = array->array[iterator];
                array->array[iterator] = tmp;
                tmp = tmp2;
            }
            array->size++;
        }
    }
}

type_t lookup_ordered_array(u32int index, ordered_array_t *array){
    if (index < array->size){
        return array->array[index];
    }
}

void remove_ordered_array(u32int index, ordered_array_t *array){
    while (index < array->size){
        array->array[index] = array->array[++index];
    }
    array->size--;
}
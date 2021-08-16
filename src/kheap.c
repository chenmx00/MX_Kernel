//kheap.c - Implement
#include "common.h" 
#include "kheap.h"
extern u32int end;
u32int placement_address = (u32int) &end;
static u32int kmalloc_helper(u32int size, int align, u32int *phys){
    if(align && (placement_address & 0xFFFFF000)){
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if(phys){
        *phys = placement_address;
    }
    u32int start = placement_address;
    placement_address += size;
    return start;
}

u32int kmalloc(u32int size){
    return kmalloc_helper(size, 0, 0);
}

u32int kmalloc_aligned(u32int size){
    return kmalloc_helper(size, 1, 0);
}

u32int kmalloc_physical(u32int size, u32int *phys){
    return kmalloc_helper(size, 0, phys);
}

u32int kmalloc_aligned_physical(u32int size, u32int *phys){
    return kmalloc_helper(size, 1, phys);
}

static s32int find_smallest_hole(u32int size, u8int page_align, heap_t *heap){
    //Find the smallest hole that fits.
    u32int iterator = 0;
    while(iterator < heap ->index.size){
        header_t *header = (header_t *) lookup_ordered_array(iterator, &heap->index);
        if (page_align > 0){
            u32int location = (u32int)header;
            s32int offset = 0;
            if ((location+sizeof(header_t)) & 0xFFFFF000 != 0){
                offset = 0x1000 - (location + sizeof(header_t)) % 0x1000;
            }
            s32int hole_size = (s32int) header->size - offset;
            if (hole_size >= (s32int)size){
                break; //found it
            }
        } else if (header->size > size){
            break; //found it
        }
        iterator++; //next block
    }
    if (iterator == heap->index.size){
        return -1; //no hole left
    } else {
        return iterator;
    }
}

static s8int header_t_less_than(void* a, void* b){
    return (((header_t *)a)->size < ((header_t *)b)->size) ? 1 : 0;
}

heap_t *create_heap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly){
    heap_t *heap = (heap_t*) kmalloc(sizeof(header_t));
    if ((start & 0x1000 != 0) || (end & 0x1000 != 0)){
        return;
    }
    heap->index = place_ordered_array((void *)start, HEAP_INDEX_SIZE, &header_t_less_than);
    start += sizeof(type_t) * HEAP_INDEX_SIZE;
    if((start  & 0xFFFFF000) != 0) {
        start += 0x1000 - (start % 0x1000); //question mark 
    }
    heap->start_address = start;
    heap->end_address = end;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    header_t * hole = (header_t *) start;
    hole->size = end - start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    insert_ordered_array((type_t) hole, &heap->index);
    return heap;
}







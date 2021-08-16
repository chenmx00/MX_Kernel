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







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



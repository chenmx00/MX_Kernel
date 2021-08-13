//kheap.c - Implement
#include "common.h" 
#include "kheap.h"
u32int placement_address;
u32int kmalloc_aligned_physical(u32int size, int align, u32int *phys){
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

u32int kmalloc_aligned(u32int size, int align){
    if (align && (placement_address & 0xFFFFF000)){
        placement_address &= 0xFFFFF000;
        placement_address += 1000;
        u32int start = placement_address;
        placement_address += size;
        return start;
    }
}

u32int kmalloc_physical(u32int size, u32int *phys){
    if(phys){
        *phys = placement_address;
    }
    u32int start = placement_address;
    placement_address += size;
    return start;
}

u32int kmalloc(u32int size){
    u32int start = placement_address;
    placement_address += size;
    return start;

}

//kheap.h - Declares interfaces for kernel space allocation.
//Ian C.
#include "common.h"
u32int kmalloc(u32int size);
u32int kmalloc_aligned(u32int size);
u32int kmalloc_physical(u32int size, u32int *phys);
u32int kmalloc_aligned_physical(u32int size, u32int *phys);


//kheap.c - Implement
#include "common.h" 
#include "kheap.h"
#include "paging.h"
extern u32int end;
extern page_directory_t *kernel_directory;
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
        return 0;
    }
    heap->index = place_ordered_array((void *)start, HEAP_INDEX_SIZE, &header_t_less_than);
    start += sizeof(type_t) * HEAP_INDEX_SIZE;
    if((start  & 0xFFFFF000) != 0) 
        start = (start & 0xFFFFF000) + 0x1000; //page_align
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

void expand(heap_t * heap, u32int new_size){
    if (new_size < heap->end_address - heap->start_address)
        return; //sanity check
    if (new_size & 0xFFFFF000 != 0)
        new_size = (new_size & 0xFFFFF000) + 0x1000; //page align
    if (heap->start_address + new_size > heap->max_address)
        return; //sanity check
    u32int i = heap->end_address - heap->start_address; //old size
    while (i < new_size)
    {
        alloc_frame(get_page(heap->start_address+i, 1, kernel_directory), (heap->supervisor) ? 1 : 0, (heap->readonly) ? 0 : 1);
        i += 0x1000;
    }
    heap->end_address = heap->start_address + new_size;
}

static s32int contract(heap_t * heap, u32int new_size){
    if (new_size > heap->end_address - heap->start_address)
        return -1; //sanity check
    if (new_size & 0x1000)
        new_size = (new_size & 0x1000) + 0x1000; //page align
    if (new_size <  HEAP_MIN_SIZE)
        return -1; //sanity check
    u32int i = heap->end_address - heap->start_address - 0x1000;
    while(new_size < i){
        free_frame(get_page(heap->start_address+i, 0, kernel_directory));
        i -= 0x1000;
    }
    heap->end_address = heap->start_address + new_size;
    return new_size;
}

void *alloc(u32int size, u8int page_align, heap_t* heap){
    u32int total_request_size = size + sizeof(header_t) + sizeof(footer_t);
    s32int iterator = find_smallest_hole(size, page_align, heap);
    if (iterator == -1) //make a new hole
    {
        
    }
    header_t *og_hole_header = (header_t *) lookup_ordered_array((u32int) iterator, &heap->index);
    s32int og_hole_size = og_hole_header->size;
    if ((og_hole_size - total_request_size) < (sizeof(header_t) + sizeof(footer_t))){
        size += og_hole_size - total_request_size;
        total_request_size = og_hole_size;
    }
    



    
}








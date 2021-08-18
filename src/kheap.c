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
        u32int old_length = heap->end_address - heap->start_address;
        u32int old_end_address = heap->end_address;
        expand(heap, old_length + total_request_size);
        u32int new_length = heap->end_address - heap->start_address;
        iterator = 0;
        u32int idx = -1;
        u32int value = 0;
        while (iterator < heap->index.size){
            u32int tmp = (u32int)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value){
                tmp = value;
                idx = iterator;
            } 
            iterator++;
        }
        if (idx == -1){ //need to make a new header
        header_t* hole_header = (header_t*) old_end_address;
        hole_header->magic = HEAP_MAGIC;
        hole_header->is_hole = 1;
        hole_header->size = new_length - old_length;
        footer_t hole_footer = (footer_t*) (old_end_address + hole_header->size - sizeof(footer_t));

        }

    }
    header_t *og_hole_header = (header_t *) lookup_ordered_array((u32int) iterator, &heap->index);
    u32int og_hole_pos = (u32int) og_hole_header;
    s32int og_hole_size = og_hole_header->size;
    if ((og_hole_size - total_request_size) < (sizeof(header_t) + sizeof(footer_t))){
        size += og_hole_size - total_request_size;
        total_request_size = og_hole_size;
    }
    if(page_align && og_hole_pos & 0xFFFFF000){
       u32int new_pos = og_hole_pos + 0x1000 -(og_hole_pos) & 0xFFF - sizeof(header_t);
       header_t *hole_header = (header_t*) og_hole_header;
       hole_header->is_hole = 1;
       hole_header->magic = HEAP_MAGIC;
       hole_header->size = 0x1000;
       footer_t *hole_footer = (footer_t*) ((u32int)new_pos - sizeof(footer_t));
       hole_footer->magic = HEAP_MAGIC;
       hole_footer->header = hole_header;
       og_hole_pos = new_pos;
       og_hole_size = og_hole_size - hole_header->size;
    } else {
        remove_ordered_array(iterator, &heap->index);
    }
    header_t *block_header  = (header_t *)og_hole_pos;
    block_header->magic = HEAP_MAGIC;
    block_header->is_hole = 0;
    block_header->size = total_request_size;
    footer_t *block_footer = (footer_t *)og_hole_pos + size + sizeof(block_header);
    block_footer->header = block_header;
    block_footer->magic = HEAP_MAGIC;

    if (og_hole_size - total_request_size > 0) //why not count for overheads -- cuz if the overhead is greater than this must equates to false
    {
        header_t* hole_header = (header_t*) og_hole_pos + sizeof(size) + sizeof(header_t) + sizeof(footer_t);
        hole_header->magic = HEAP_MAGIC;
        hole_header->is_hole = 1;
        hole_header->size = og_hole_size - total_request_size;
        footer_t* hole_footer = (footer_t*) (u32int)hole_header + hole_header->size - sizeof(footer_t);
        if((u32int) hole_footer < heap->end_address){
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        insert_ordered_array(hole_header, &heap->index);
    }
    return (void*)((u32int)block_header+sizeof(header_t));
    









    
}








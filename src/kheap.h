//kheap.h - Declares interfaces for kernel space allocation.
//Ian C.
#include "common.h"
#include "ordered_array.h"
#define HEAP_START 0XC0000000
#define HEAP_INITIAL_SIZE 0X100000
#define HEAP_INDEX_SIZE 0X20000
#define HEAP_MAGIC 0X5465255C
#define HEAP_MIN_SIZE 0X70000
u32int kmalloc(u32int size);
u32int kmalloc_aligned(u32int size);
u32int kmalloc_physical(u32int size, u32int *phys);
u32int kmalloc_aligned_physical(u32int size, u32int *phys);
u8int kfree(void *p);

typedef struct {
    u32int magic;
    u8int is_hole;
    u32int size;
}header_t;

typedef struct{
    u32int magic;
    header_t *header;
}footer_t;

typedef struct {
    ordered_array_t index;
    u32int start_address;
    u32int end_address;
    u32int max_address;
    u8int supervisor;
    u8int readonly;
}heap_t;

heap_t *create_heap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly);
void *alloc(u32int size, u8int page_align, heap_t *heap);
u8int free(void *p, heap_t *heap);





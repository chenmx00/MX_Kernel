#ifndef PAGING_H
#define PAGING_N
#endif
#include "common.h" 
#include "isr.h"

typedef struct page{
    u32int present :1; //page present in memory
    u32int rw :1; //readonly if clear, writeable if set
    u32int user :1; //supervisor level only if clear
    u32int accessed :1; //has the page been accessed since last refresh
    u32int dirty :1; //has the page been written since the last refresh
    u32int unused :7; //amalgamation of unused and reserved bits
    u32int frame :20; //frame address
} page_t;

typedef struct page_table
{
    page_t pages[1024];
}page_table_t;

typedef struct page_directory{
    page_table_t *tables[1024];
    u32int tablesPhysical[1024];
    u32int physicalAddress;
}page_directory_t;

void initiliaze_paging();
void switch_page_directory(page_directory_t *new);
page_t *get_page(u32int address, int make, page_directory_t *dir);
void page_fault(registers_t regs);

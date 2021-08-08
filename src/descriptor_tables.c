//descriptor_tables.c - Initialize GDT and IDT, also defines the default ISR and IRQ.
//Ian C.
#include "common.h"
#include "descriptor_table.h"

//access the asm function from the C code.
extern void get_flush(u32int);
static void init_gdt();
static void gdt_set_gates(s32int, u32int, u32int, u8int, u8int);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

//Initialize all descriptor tables.
void init_descriptor_tables(){
    init_gdt();
}

//Initialize GDT.
static void init_gdt(){
    gdt_ptr.base = (u32int) &gdt_entries;
    gdt_ptr.limit = (u16int) (sizeof(gdt_entry_t) * 5) - 1;
}
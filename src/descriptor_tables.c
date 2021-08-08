//descriptor_tables.c - Initialize GDT and IDT, also defines the default ISR and IRQ.
//Ian C.
#include "common.h"
#include "descriptor_table.h"

//access the asm function from the C code.
extern void gdt_flush(u32int);
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
    gdt_set_gates(0, 0, 0, 0, 0); //Null Segment
    gdt_set_gates(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //Kernel Code Segment, access: 10011010->0x9A
    gdt_set_gates(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //Kernel Data Segment, access: 10010010->0x92
    gdt_set_gates(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //User Code Segment, access: 11111010->0xFA
    gdt_set_gates(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //User Data segment, access: 11110010->0xF2
    gdt_flush((u32int)&gdt_ptr);


}

static void gdt_set_gates(s32int num, u32int base, u32int limit, u8int access, u8int granularity){
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    gdt_entries[num].access = access;
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= (granularity & 0xF0);
}


//descriptor_tables.c - Initialize GDT and IDT, also defines the default ISR and IRQ.
//Ian C.
#include "common.h"
#include "descriptor_tables.h"

//access the asm function from the C code.
extern void gdt_flush(u32int);
extern void idt_flush(u32int);

static void init_gdt();
static void init_idt();
static void gdt_set_gates(s32int, u32int, u32int, u8int, u8int);
static void idt_set_gates(u8int, u32int, u16int, u8int);

gdt_entry_t gdt_entries[GDT_SIZE];
idt_entry_t idt_entires[IDT_SIZE];
gdt_ptr_t gdt_ptr;
idt_ptr_t idt_ptr;

//Initialize all descriptor tables.
void init_descriptor_tables(){
    init_gdt();
    inti_idt();
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


//Initialize IDT.
static void init_idt(){
    idt_ptr.base = (u32int) & idt_entires;
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    memset(&idt_entires, 0, sizeof(idt_entires) * 256); //Initialize IDT entries.
    for (int i = 0; i < 32; i++){
        idt_set_gates(i, fetch_isr(i), 0x08, 0x8E);
    }
    idt_flush((u32int)&idt_ptr);
}

//Helper functions
static void gdt_set_gates(s32int num, u32int base, u32int limit, u8int access, u8int granularity){
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    gdt_entries[num].access = access;
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= (granularity & 0xF0);
}

static void idt_set_gates(u8int num, u32int base, u16int selector, u8int attribute){
    idt_entires[num].base_low = base & 0xFFFF;
    idt_entires[num].selector = selector;
    idt_entires[num].zero = (u8int) 0;
    idt_entires[num].type_attr = attribute;
    idt_entires[num].base_high = (base >> 16) & 0xFFFF; 
}

static u32int fetch_isr(u8int num){
    u32int isr_table[32];
    isr_table[0] = isr0;
    isr_table[1] = isr1;
    isr_table[2] = isr2;
    isr_table[3] = isr3;
    isr_table[4] = isr4;
    isr_table[5] = isr5;
    isr_table[6] = isr6;
    isr_table[7] = isr7;
    isr_table[8] = isr8;
    isr_table[9] = isr9;
    isr_table[10] = isr10;
    isr_table[11] = isr11;
    isr_table[12] = isr12;
    isr_table[13] = isr13;
    isr_table[14] = isr14;
    isr_table[15] = isr15;
    isr_table[16] = isr16;
    isr_table[17] = isr17;
    isr_table[18] = isr18;
    isr_table[19] = isr19;
    isr_table[20] = isr20;
    isr_table[21] = isr21;
    isr_table[22] = isr22;
    isr_table[23] = isr23;
    isr_table[24] = isr24;
    isr_table[25] = isr25;
    isr_table[26] = isr26;
    isr_table[27] = isr27;
    isr_table[28] = isr28;
    isr_table[29] = isr29;
    isr_table[30] = isr30;
    isr_table[31] = isr31;
    return isr_table[num];
}


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
static u32int fetch_isr(u8int num);
static u32int fetch_irq(u8int num);
void * memset (void *dest, int val, u32int len);

gdt_entry_t gdt_entries[GDT_SIZE];
idt_entry_t idt_entires[IDT_SIZE];
gdt_ptr_t gdt_ptr;
idt_ptr_t idt_ptr;

//Initialize all descriptor tables.
void init_descriptor_tables(){
    init_gdt();
    init_idt();
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
    idt_ptr.base = (u32int) &idt_entires;
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    memset(&idt_entires, 0, sizeof(idt_entry_t) * 256); //Initialize IDT entries.
    out_byte(0x20, 0x11); //ICW1 -- begins Initialize PICs, flush 0x11 to the command port to start PIC initialization.
    out_byte(0xA0, 0x11); //ICW1 -- same process for PIC2.
    out_byte(0x21, 0x20); //ICW2 -- Remap offset address of IDT. In x86 protected mode, we have to offset 0x20 because the first 32 interrupts are reserved for CPU exceptions.
    out_byte(0xA1, 0X28); //ICW2 -- same process for PIC2.
    out_byte(0x21, 0x04); //ICW3 -- Cascading mode enable, PIC1 for the master.
    out_byte(0xA1, 0x02); //ICW3 -- Cascading mode enable, PIC2 for the slavery.
    out_byte(0x21, 0x01); //ICW4 -- Environment info, set the last bit to tell PIC we are running on 80x86 mode.
    out_byte(0xA1, 0x01); //ICW4 -- same process for PIC2.
    out_byte(0x21, 0xFF); //Mask(Enable) interrupts for PIC1.
    out_byte(0xA1, 0xFF); //Mask(Enable) interrupts for PIC2.

    for (int i = 0; i < 32; i++){
        idt_set_gates(i, (u32int)fetch_isr(i), 0x08, 0x8E);
    }
    for (int i = 32; i < 48; i++){
        idt_set_gates(i, (u32int)fetch_irq(i-32), 0x08, 0x8E);
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
    isr_table[0] = (u32int)isr0;
    isr_table[1] = (u32int)isr1;
    isr_table[2] = (u32int)isr2;
    isr_table[3] = (u32int)isr3;
    isr_table[4] = (u32int)isr4;
    isr_table[5] = (u32int)isr5;
    isr_table[6] = (u32int)isr6;
    isr_table[7] = (u32int)isr7;
    isr_table[8] = (u32int)isr8;
    isr_table[9] = (u32int)isr9;
    isr_table[10] = (u32int)isr10;
    isr_table[11] = (u32int)isr11;
    isr_table[12] = (u32int)isr12;
    isr_table[13] = (u32int)isr13;
    isr_table[14] = (u32int)isr14;
    isr_table[15] = (u32int)isr15;
    isr_table[16] = (u32int)isr16;
    isr_table[17] = (u32int)isr17;
    isr_table[18] = (u32int)isr18;
    isr_table[19] = (u32int)isr19;
    isr_table[20] = (u32int)isr20;
    isr_table[21] = (u32int)isr21;
    isr_table[22] = (u32int)isr22;
    isr_table[23] = (u32int)isr23;
    isr_table[24] = (u32int)isr24;
    isr_table[25] = (u32int)isr25;
    isr_table[26] = (u32int)isr26;
    isr_table[27] = (u32int)isr27;
    isr_table[28] = (u32int)isr28;
    isr_table[29] = (u32int)isr29;
    isr_table[30] = (u32int)isr30;
    isr_table[31] = (u32int)isr31;
    return isr_table[num];
}

static u32int fetch_irq(u8int num){
    u32int irq_table[16];
    irq_table[0] = (u32int)irq0;
    irq_table[1] = (u32int)irq1;
    irq_table[2] = (u32int)irq2;
    irq_table[3] = (u32int)irq3;
    irq_table[4] = (u32int)irq4;
    irq_table[5] = (u32int)irq5;
    irq_table[6] = (u32int)irq6;
    irq_table[7] = (u32int)irq7;
    irq_table[8] = (u32int)irq8;
    irq_table[9] = (u32int)irq9;
    irq_table[10] = (u32int)irq10;
    irq_table[11] = (u32int)irq11;
    irq_table[12] = (u32int)irq12;
    irq_table[13] = (u32int)irq13;
    irq_table[14] = (u32int)irq14;
    irq_table[15] = (u32int)irq15;
    return irq_table[num];
}

void * memset (void *dest, int val, u32int len)
{
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}


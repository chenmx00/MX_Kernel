;descriptor_tables.s - contains setup code for global descriptor table and interrupt descriptor table.
[GLOBAL gdt_flush]
gdt_flush:
  mov eax, [esp + 4] ;Get the pointer of GDT, passed as parameter.
  lgdt [eax] ;Call instruction lgdt to load the GDT pointer.
  mov ax, 0x10 ;0x10 is the offset in GDT to data segment
  mov ds, ax ; Flush 0x10 into all segment registers (exclude cs)
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:.flush ;Perform far jump into eip(.flush) and set offset(cs) to 0x08
.flush:
  ret

[GLOBAL idt_flush]
idt_flush:
  mov edx, [esp + 4] ;Loads the first param (idt_ptr)
  lidt [edx] ;Load idt
  sti ;Turns on interruption
  ret
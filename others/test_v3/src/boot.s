;
; boot.s -- Kernel start location.
;

MBOOT_PAGE_ALIGN equ 1<<0 ;Load Kernel and Module on a page boundary.
MBOOT_MEM_INFO equ 1<<1 ;Provide your kernel with memory info.
MBOOT_HEADER_MAGIC equ 0xBADB002 ;Multiboot magic value.
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOG_CHECKSUM  equ - (MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]   ; All instructions are 32 bits wide.
[GLOBAL mboot]  ; Make 'mboot' accessible from C.
[EXTERN code]   ; Start of the '.text' section.
[EXTERN bss]    ; Start of the '.bss' section.
[EXTERN end]    ; End of the last loadable section.

mboot:
    dd MBOOT_HEADER_MAGIC ;Grub will search this value on every 4-bytes boundary in the kernel file.
    dd MBOOT_HEADER_FLAGS ;How Grub will load my settings.
    dd MBOOG_CHECKSUM ;To ensure that the above values are correct.
    dd mboot ; Location of this descriptor.
    dd code ; Start of kernel's code section.
    dd bss ; Start of kernel's bss section.
    dd end ; End of the kernel.
    dd start ; Kernel Entry Point(Inital EIP)

[GLOBAL start]
[EXTERN kmain]

start:
    push ebx    ;Load multiboot header location.
    cli ;Disable interrupts.
    call kmain ;call our main() function.
    jmp $   ; jump into the infinite loop.

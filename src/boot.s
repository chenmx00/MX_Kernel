;;kernel.asm
;;Ian C.
;;06/06/2021
;;credit from Arjun Sreedharan
bits 32  ;;nasm directive - 32 bit
section .text
        ;multiboot header spec for grub
        align 4
        dd 0x1BADB002 ;magic
        dd 0x00 ;flags
        dd - (0x1BADB002 + 0x00) ;checksum

global start
global keyboard_handler
global read_port
global write_port
global load_idt
global gdt_flush

extern kmain  ;kmain is defined in the c file
extern keyboard_handler_main ; keyboard handler is definred in the c file for convinience.

read_port:
  mov edx, [esp + 4] ;Read 4 bytes in memory starting at current stack pointer + 4 bytes (next address, which is the first parameter)
  in al, dx ;dx is the lower 16 bits for edx and instruction in loads it into al, which is the lower 8 bits of eax.
  ret

write_port:
  mov edx, [esp + 4] ;Loads the first param(port number) into edx
  mov al, [esp + 4 + 4] ;Loads the second param(actual data) into eax(lower 8 bits)
  out dx, al ;Loads al(actual data) into dx(port number)
  ret

load_idt:
  mov edx, [esp + 4] ;Loads the first param (idt_ptr)
  lidt [edx] ;Load idt
  sti ;Turns on interruption
  ret

keyboard_handler:
  call keyboard_handler_main
  iretd

gdt_flush:
  mov eax, [esp + 4] ;Get the pointer of GDT, passed as parameter.
  lgdt [eax] ;Call instruction lgdt to load the GDT pointer.
  mov ax, 0x10 ;0x10 is the offset in GDT to data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:.flush
.flush:
  ret


start:
  cli  ;block interrupts by calling clear interrupt
  mov esp,stack_space  ;set esp as stack_space
  call kmain
  hlt  ;halt the CPU

section .bss
resb 8192  ;8kib for stack
stack_space:

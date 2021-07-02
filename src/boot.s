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
extern kmain  ;kmain is defined in the c file


start:
  cli  ;block interrupts by calling clear interrupt
  mov esp,stack_space  ;set esp as stack_space
  call kmain
  hlt  ;halt the CPU

section .bss
resb 8192  ;8kib for stack
stack_space:

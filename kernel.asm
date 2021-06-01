;;kernel.asm
bits 32  ;;nasm directive - 32 bit
section .text

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

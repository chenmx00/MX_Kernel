[org 0x7c00] ; tells the assembler the offset of the bootloder
;The main routine makes sure the parameters are already when called.

mov bx, HELLO
call print
call print_nl
mov bx, DAME
call print
call print_nl

jmp $

%include "boot_sect_print.asm"

;data
HELLO:
  db 'Hello, World', 0

GOODBYE:
  db 'Goodbye', 0

;padding and magic numbers
times 510 - ($-$$) db 0
dw 0xaa55

print:
  pusha


start:
  mov al, [bx] ;//put the data in lower eax
  cmp al,  0
  je done

  mov ah, 0x0e ;print using bios's help
  int 0x10; //Fire intterupt

  add bx, 1 ;//increment pointer and do next loop.
  jmp start

done:
  popa
  ret

print_nl:
  pusha

  mov ah, 0x0e ;
  mov al, 0x0a ; //newline char
  int 0x10 ; //fire the interrupt
  mov al, 0x0d ; //carriage return
  int 0x10 ; //fire the interrupt

  popa
  ret

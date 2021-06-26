//common.c -- Defines more global functions.
//Ian C. -- Credit taken from James M's kernel tutorial.

#include "common.h"

//write a byte to a specified port.
void out_byte(u16int port, u8int value)
{
    asm volatile("outb %1, $0" : : "dN" (port), "a" (value));
}

u8int in_byte(u16int port){
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int in_word(u16int port){
    u16int ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

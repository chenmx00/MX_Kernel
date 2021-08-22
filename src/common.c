//common.c -- Defines more global functions.
//Ian C. -- Credit taken from James M's kernel tutorial.

#include "common.h"

//write a byte to a specified port.
void out_byte(u16int port, u8int value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
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

void* memset (void *dest, int val, u32int len)
{
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}
void* memcpy (void *dest, const void *src, u32int len)
{
  char *d = dest;
  const char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}

u32int strlen(const char *s) {
    u32int i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}

char *strcpy (char *dest, const char *src)
{
  return memcpy (dest, src, strlen (src) + 1);
}

int strcmp(const char *X, const char *Y)
{
    while (*X)
    {
        // if characters differ, or end of the second string is reached
        if (*X != *Y) {
            break;
        }
 
        // move to the next pair of characters
        X++;
        Y++;
    }
 
    // return the ASCII difference after converting `char*` to `unsigned char*`
    return *(const unsigned char*)X - *(const unsigned char*)Y;
}

char *strncpy(char *dest, const char *src, u32int n)
{
    char *ret = dest;
    do {
        if (!n--)
            return ret;
    } while (*dest++ = *src++);
    while (n--)
        *dest++ = 0;
    return ret;
}
 

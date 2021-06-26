//common.h -- Defines typedefs and more global functions.
//Ian C. -- Credit taken from James M's kernel tutorial.

#ifndef COMMON_H
#define COMMON_H

//some typedefs to standalize sizes cross platforms for x86-32.
typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

void out_byte(u16int port, u8int value);
u8int in_byte(u16int port);
u16int in_word(u16int port);
#endif
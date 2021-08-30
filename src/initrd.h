#ifndef INITRD_H
#define INITRD_H
#include "common.h"
#include "fs.h"
typedef struct
{
    u32int file_quantity;
}initrd_header_t;
typedef struct 
{
    u8int magic;
    s8int name[64]; //file name
    u32int offset; // offset of that file in the initrd that the file starts
    u32int length; // size of that file
}initrd_file_header_t;

fs_node_t* initialize_initrd(u32int location);
#endif

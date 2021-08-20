#include "common.h"
#include "initrd.h"
initrd_file_header_t* file_header;
initrd_header_t* initrd_header;
fs_node_t* initrd_root;
fs_node_t* initrd_dev;
fs_node_t* root_nodes;
u32int file_quantity;
struct dirent dirent;

static u32int initrd_read(fs_node_t* node, u32int offset, u32int size, u8int* buffer){
    initrd_file_header_t header = file_header[node->inode];
    if (offset > header.length)
        return 0;
    if (offset + size > header.length)
        size = header.length - offset; //crop the tail
    memcpy(buffer, (u8int*)(header.offset + offset), size);
    return size;
}
static struct dirent* initrd_readdir(fs_node_t* node, u32int index){
    if (node == initrd_root && index == 0){
        strcpy(dirent.name, "dev");
        dirent.name[3];
        dirent.inode = 0;
        return &dirent;
    }

    if (index-1 >= file_quantity)
        return 0;
    strcpy(dirent.name, root_nodes[index - 1].name);
    dirent.name[strlen(root_nodes[index - 1].name)] = 0;
    dirent.inode = root_nodes[index - 1].inode;
    return &dirent;
}









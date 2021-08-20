#include "common.h"
#include "initrd.h"
#include "kheap.h"
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
static fs_node_t* initrd_finddir(fs_node_t *node, char* name){
    if (node == initrd_root && !strcmp(name, "dev"))
        return initrd_dev;
    for (int i = 0; i < file_quantity; i++){
        if (!strcmp(name, root_nodes[i].name))
            return &root_nodes[i];
    }
    return 0;
}

fs_node_t* initialize_initrd(u32int locaiton){
    initrd_header = (initrd_header_t *) locaiton;
    file_header = (initrd_file_header_t *) (locaiton + sizeof(initrd_header_t));
    initrd_root = (fs_node_t*) kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root, "/");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = initrd_root->read = initrd_root->write = initrd_root->open = initrd_root->close = initrd_root->ptr =  initrd_root->impl = 0;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_dev = (fs_node_t*) kmalloc(sizeof(fs_node_t));
    strcpy(initrd_dev, "dev");
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = initrd_dev->read = initrd_dev->write = initrd_dev->open = initrd_dev->close = initrd_dev->ptr =  initrd_dev->impl = 0;
    initrd_dev->flags = FS_DIRECTORY;
    initrd_dev->readdir = &initrd_readdir;
    initrd_dev->finddir = &initrd_finddir;
    root_nodes = (fs_node_t *) kmalloc(sizeof(fs_node_t) * initrd_header->file_quantity);
    file_quantity = initrd_header->file_quantity;
    for(ini i = 0; i < initrd_header->file_quantity; i++){
        
    }
}










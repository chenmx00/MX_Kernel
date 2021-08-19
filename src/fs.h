#ifndef FS_H
#define FS_H
#include "common.h"
#define FS_FILE 0X01
#define FS_DIRECTORY 0X02
#define FS_CHARDEVICE 0X03
#define FS_BLOCKDEVICE 0X04
#define FS_PIPE 0X05
#define FS_SYMLINK 0X06
#define FS_MOUNTPOINT 0X08
struct fs_node;
typedef u32int (*read_type_t) (struct fs_node*, u32int, u32int, u8int*);
typedef u32int (*write_type_t) (struct fs_node*, u32int, u32int, u8int*);
typedef void (*open_type_t) (struct fs_node*);
typedef void (*close_type_t) (struct fs_node*);
typedef struct dirent *(*readdir_type_t) (struct fs_node*, u32int);
typedef struct fs_node * (*finddir_type_t) (struct fs_node*, char *name);
typedef struct fs_node{
    char name[128];
    u32int mask;
    u32int uid;
    u32int gid;
    u32int flags;
    u32int inode;
    u32int length;
    u32int impl;
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
} fs_node_t;
struct dirent{
    char name[128];
    u32int inode;
}; 
extern fs_node_t *fs_root;

u32int read_fs(fs_node_t* node, u32int offset, u32int size, u8int* buffer);
u32int write_fs(fs_node_t* node, u32int offset, u32int size, u8int* buffer);
void open_fs(fs_node_t* node, u8int read, u8int write);
void close_fs(fs_node_t* node);
struct dirent* readdir_fs(fs_node_t* node, u32int index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);




#endif
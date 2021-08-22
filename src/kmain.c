/*
 * kernel.c
 * Ian C.
 * 06/06/2021
 */
#include "multiboot.h"
#include "descriptor_tables.h"
#include "monitor.h"
#include "keyboard.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "initrd.h"
#include "fs.h"
extern u32int placement_address;
int initrd_load = 0;
u32int initrd_location = 0;
void kmain(struct multiboot *mboot_ptr) {
  monitor_clear();
  monitor_write("Kernel version 0.08\n");
  monitor_write("Minxing's kernel\n");
  monitor_write("-Initializing Global Descriptor Table...\n");
  monitor_write("-Initializing Interrupt Descriptor Table...\n");
  init_descriptor_tables();
  monitor_write("-Done Initializing\n");
  monitor_write("-Initializing Keyboards...\n");
  keyboard_init();
  monitor_write("-Done Initializing\n");
  if (mboot_ptr->mods_count > 0){
    initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    placement_address = initrd_end;
    initrd_load = 1;
  }
  
  monitor_write("-Initializing Segmentation and Paging...\n");
  initialize_paging();
  monitor_write("-Done Initializing\n");
  monitor_write("-Ready To Use\r");
  if(initrd_load){
    monitor_write("-Initializing Inital Ram Disk...\n");
    fs_root = initialize_initrd(initrd_location);
    monitor_write("-Done Initializing\n");
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
      monitor_write("Found file ");
      monitor_write(node->name);
      fs_node_t *fsnode = finddir_fs(fs_root, node->name);

      if ((fsnode->flags&0x7) == FS_DIRECTORY)
        monitor_write("\n\t(directory)\n");
      else
      {
        monitor_write("\n\t contents: \"");
        char buf[512];
        u32int sz = read_fs(fsnode, 0, 512, buf);
        int j;
        for (j = 0; j < sz; j++)
          monitor_put(buf[j]);

        monitor_write("\"\n");
      }
      i++;
    }
  }

 




  //init_timer(50);
  //asm volatile( "int $0x5");
  //asm volatile( "int $0x6");
  //asm volatile ("sti");



  while(1);
}

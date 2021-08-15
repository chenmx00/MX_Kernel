/*
 * kernel.c
 * Ian C.
 * 06/06/2021
 */
#include "descriptor_tables.h"
#include "monitor.h"
#include "keyboard.h"
#include "timer.h"
#include "paging.h"
void kmain(void) {
  char *kernel_version = "Kernel version 0.05";
  char *kernel_intro = "Minxing's kernel\n";

  monitor_clear();
  monitor_write(kernel_version);
  newline_add();
  monitor_write(kernel_intro);
  
  init_descriptor_tables();
  keyboard_init();
  //init_timer(50);

  asm volatile( "int $0x5");
  asm volatile( "int $0x6");
  asm volatile ("sti");
  initialize_paging();
  u32int *ptr = (u32int*)0x0000FFFF;
  *ptr = 20;
  u32int do_page_fault = *ptr;
  monitor_write_dec(do_page_fault);
  //init_timer(50);


  while(1);
}

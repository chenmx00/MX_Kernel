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

  monitor_clear();
  monitor_write("Kernel version 0.07\n");
  monitor_write("Minxing's kernel\n");
  monitor_write("-Initializing Global Descriptor Table...\n");
  monitor_write("-Initializing Interrupt Descriptor Table...\n");
  init_descriptor_tables();
  monitor_write("-Done Initializing\n");
  monitor_write("-Initializing Keyboards...\n");
  keyboard_init();
  monitor_write("-Done Initializing\n");
  monitor_write("-Initializing Segmentation and Paging...\n");
  initialize_paging();
  monitor_write("-Done Initializing\n");
  monitor_write("-Ready To Use\r");

  //init_timer(50);

  //asm volatile( "int $0x5");
  //asm volatile( "int $0x6");
  //asm volatile ("sti");



  while(1);
}

/*
 * kernel.c
 * Ian C.
 * 06/06/2021
 */
#include "descriptor_tables.h"
#include "monitor.h"
#include "keyboard.h"
#include "timer.h"
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
  while(1);
}

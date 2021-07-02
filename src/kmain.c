/*
 * kernel.c
 * Ian C.
 * 06/06/2021
 */
#include "monitor.h"
#include "keyboard.h"
void kmain(void) {
  char *kernel_version = "Kernel version 0.0.3";
  char *kernel_intro = "Minxing's first kernel\n";

  monitor_clear();
  monitor_write(kernel_version);
  newline_add();
  monitor_write(kernel_intro);
  keyboard_init();
  while(1);
}

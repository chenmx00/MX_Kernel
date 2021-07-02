/*
 * kernel.c
 * Ian C.
 * 06/06/2021
 */
#include "monitor.h"

void kmain(void) {
  char *str1 = "Kernel version 0.0.1\n";
  char *str2 = "Minxing's first kernel";

  monitor_clear();
  monitor_write(str1);
  monitor_write(str2);
  
  while(1);
}

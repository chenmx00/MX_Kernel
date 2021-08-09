//monitor.h -- Defines the interface for monitor.h
//Ian C. -- Credit from JamesM.

#ifndef MONITOR_H
#define MONITOR_H
#include "common.h"
void monitor_put(char c);
void monitor_clear();
void monitor_write(char *c);
void monitor_write_dec(u32int n);
void entry_output();
void newline_add();
#endif
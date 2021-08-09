#include "keyboard.h"
#include "monitor.h"
#include "keyboard_map.h"
#include "isr.h"

static void keyboard_callback(registers_t regs);

void keyboard_init(){
  /*0xFD is 11111101 - enables IRQ1(the keyboard)*/
  write_port(0x21, 0xFD);
  register_interrupt_handler(IRQ1,&keyboard_callback);
}

static void keyboard_callback(registers_t regs){
  unsigned char status;
  char keycode;
  write_port(0x20, 0x20); //write EOF into PIC's command port to initiate
  status = read_port(KEYBOARD_STATUS_PORT); //Lowest bit of status will be set if buffer is not empty
  if (status & 0x01){
    keycode = read_port(KEYBOARD_DATA_PORT);
    if (keycode < 0){
      return;
    } else {
      monitor_put(keyboard_map[keycode]);
    }
  }
}
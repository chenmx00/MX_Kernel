#include "keyboard.h"
#include "monitor.h"
#include "keyboard_map.h"

static void idt_init(void){
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];
    
    /*populate IDT entry of keyboard's interrupt*/
    //For 286 compatibility, we need to store lower 16 bits and higher 16 bits separately
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff; //stores lower 16 bits here
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET; //stores the second entry segment after 8 bytes GDT entry
    IDT[0x21].zero = 0x00;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16; //stores higher 16 bits here

    /*Ports
     *         PIC1   PIC2
     *Command  0x20   0xA0
     *Data     0x21   0xA1
     */
  
    /* ICW1 -- Begins Initialization*/
    write_port(0x20, 0x11); // flush 0x11 to the command port to start PIC initialization.
    write_port(0xA0, 0x11);

    /* ICW2 -- Remap offset address of IDT */
    /* In x86 protected mode, we have to offset 0x20 because the first 32 interrupts are reserved for CPU exceptions.*/
    write_port(0x21, 0x20);
    write_port(0xA1, 0x20);

    /* ICW3 -- Setup Cascading, Master-Slavery mode*/
    /* We set it to zero to disable it */
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);

    /*ICW4 -- Environment info */
    write_port(0x21, 0x01); //We set the last bit to tell PIC we are running on 80x86 mode.
    write_port(0xA1, 0x01);
    /* Finished  PICs Initialization */

    /*Mask interrupts*/
    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    /* fill the IDT descriptor */
    idt_address = (unsigned long) IDT;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16); //lower 16 bits
    idt_ptr[1] = idt_address >> 16; // higher 16 bits
    load_idt(idt_ptr);
}

static void kb_init(void){
  /*0xFD is 11111101 - enables IRQ1(the keyboard)*/
  write_port(0x21, 0xFD);
}

void keyboard_init(){
    idt_init();
    kb_init();
}

void keyboard_handler_main(void){
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
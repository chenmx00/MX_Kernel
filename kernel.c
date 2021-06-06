/*
 * kernel.c
 * Ian C.
 * 06/06/2021
 */
#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES
#define VIDEO_MEM 0xb8000
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTRY_KEY_CODE 0x1C

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

/* current cursor location */
unsigned int current_loc = 0;
char *vidptr = (char*) VIDEO_MEM; //beginning of video mem

struct IDT_entry {
  unsigned short int offset_lowerbits; //2 bytes
  unsigned short int selector; //2 bytes
  unsigned char zero; // 1 bytes
  unsigned char type_attr; //1 bytes
  unsigned short int offset_higherbits; //2 bytes
}; // each IDT_entry is 64 bits or 8 bytes in total.

struct IDT_entry IDT[IDT_SIZE]; // it has 256 * 8 = 2048 bytes

void idt_init(void){
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
}




void kmain(void) {
  const char *str1 = "Kernel version 0.0.1";
  const char *str2 = "Minxing's first kernel";
  unsigned int i = 0;
  unsigned int j = 0;

  /** this loop clears the screen(framebuffer)
   * there are 25 lines of 80 columns; each element takes 2bytes */

  while(j < 80 * 25 * 2){
    vidptr[j] = ' ';
    vidptr[j+1] = 0x07;
    j = j + 2;
  }

  j = 0;
  
  /* this loop writes the string to video memory
   */
  while(str1[j] != '\0'){
    vidptr[i] = str1[j];
    vidptr[i+1] = 0x00 + (i) % 15 + 1;
    ++j;
    i = i + 2;
  }
  
  j = 0;
  i = 160;
  while(str2[j] != '\0'){
    vidptr[i] = str2[j];
    vidptr[i+1] = 0x00 + (i) % 15 + 1;
    ++j;
    i = i + 2;
  }
  return;
}

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTRY_KEY_CODE 0x1C
#define EMPTY_LINES_TOP 8

extern void keyboard_handler(void);
extern char read_port(unsigned short);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
void keyboard_init();

struct IDT_entry {
  unsigned short int offset_lowerbits; //2 bytes
  unsigned short int selector; //2 bytes
  unsigned char zero; // 1 bytes
  unsigned char type_attr; //1 bytes
  unsigned short int offset_higherbits; //2 bytes
}; // each IDT_entry is 64 bits or 8 bytes in total.

struct IDT_entry IDT[IDT_SIZE]; // it has 256 * 8 = 2048 bytes

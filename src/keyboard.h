#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTRY_KEY_CODE 0x1C
#define EMPTY_LINES_TOP 8


void keyboard_init();
extern char read_port(unsigned short);
extern void write_port(unsigned short port, unsigned char data);


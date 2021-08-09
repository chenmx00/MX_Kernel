#include "common.h"

typedef struct registers
{
    u32int ds; //Data segment selector.
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; //Pushed by pusha.
    u32int int_no, err_code; //Interrupt number and error code.
    u32int eip, cs, eflags, usersp, ss; //Pushed by processor automatically.
} registers_t;

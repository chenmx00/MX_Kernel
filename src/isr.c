#include "common.h"
#include "isr.h"
#include "monitor.h"
isr_t interrupt_handlers[256];

void isr_handler(registers_t regs){
    if (interrupt_handlers[regs.int_no] != 0){
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    } else {
    monitor_write("received interrupts: ");
    monitor_write_dec(regs.int_no);
    monitor_put('\n');
    }
}

void irq_handler(registers_t regs){
    if(regs.int_no >= 40){
        out_byte(0xA0, 0X20); //Send the reset signal to slave.
    }
    out_byte(0x20, 0x20); //Send the reset signal to master.
    if (interrupt_handlers[regs.int_no] != 0){
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}


void register_interrupt_handler(u8int n, isr_t handler){
    interrupt_handlers[n] = handler;
}
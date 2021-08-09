//timer.c - Initialize PIT, and set the interrupt handler.
#include "common.h"
#include "monitor.h"
#include "isr.h"
u32int tick = 0;

static void timer_callback(registers_t regs){
    tick++;
    monitor_write("tick: ");
    monitor_write_dec(tick);
    monitor_put('\n');
}
void init_timer(u32int frequency){
    register_interrupt_handler(IRQ0, &timer_callback);
    u32int divisor = 1193180 / frequency;
    out_byte(0x43, 0x36);
    out_byte(0x40,(u8int) (divisor & 0xFF));
    out_byte(0x40, (u8int) ((divisor >> 8) & 0xFF));
}
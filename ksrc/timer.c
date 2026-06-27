#include "timer.h"
#include "printk.h"
#include "drivers/82489DX.h"

uint_t lapic_ticks_per_ms = 0;
uint_t lapic_ticks_per_us = 0;

void timer_delay_us(uint32_t us)
{

    uint_t wait_ticks = lapic_ticks_per_us * us;;
    uint_t initial_count = LAPIC_REG(LAPIC_ICR);
    uint_t last_count = LAPIC_REG(LAPIC_CCR);
    
    while (wait_ticks > 0) {
        uint_t delta = 0;
        uint_t current_count = LAPIC_REG(LAPIC_CCR);

        if (current_count <= last_count) {
            delta = last_count - current_count;
        } else {
            delta = last_count + (initial_count - current_count);
        }

        if (delta >= wait_ticks) {
            wait_ticks = 0;
        } else {
            wait_ticks -= delta;
        }

        last_count = current_count;

        asm volatile("pause");
    }
}

void timer_delay_ms(uint32_t ms)
{

    uint_t wait_ticks = lapic_ticks_per_ms * ms;
    printk("wait_ticks: %u %u\n", ms,lapic_ticks_per_ms);
    uint_t initial_count = LAPIC_REG(LAPIC_ICR);
    uint_t last_count = LAPIC_REG(LAPIC_CCR);
    
    while (wait_ticks > 0) {
        uint_t delta = 0;
        uint_t current_count = LAPIC_REG(LAPIC_CCR);

        if (current_count <= last_count) {
            delta = last_count - current_count;
        } else {
            delta = last_count + (initial_count - current_count);
        }

        if (delta >= wait_ticks) {
            wait_ticks = 0;
        } else {
            wait_ticks -= delta;
        }

        last_count = current_count;

        asm volatile("pause");
    }
}
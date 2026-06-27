#ifndef KERNEL_TIMER_H_
#define KERNEL_TIMER_H_
#include "types.h"

extern void timer_delay_us(uint32_t us);
extern void timer_delay_ms(uint32_t ms);

#endif /* TIMER_H_ */
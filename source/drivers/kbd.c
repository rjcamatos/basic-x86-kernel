#include "kbd.h"
#include "i8042.h"
//#include "8259A.h"
#include "82489DX.h"
#include "i8042.h"
#include "../idt.h"

static void irq_handler(uint32_t vector, idt_hargs_t args, uint32_t error)
{

	byte_t in = i8042_read();
	if( in != KBD_IN_COMMAND_ACK )
	{
		printk("%u|",(uint_t)in);
	}
	lapic_eio(vector);
	//pic_eio((uint8_t)vector);
}

void kbd_init()
{
	i8042_init();

	idt_set_interrupt_handler(33,&irq_handler);

	i8042_write(KBD_OUT_COMMAND_ENABLE);
	while( i8042_read() != 0xFA ) continue;

	i8042_write(KBD_OUT_COMMAND_ECHO);
	while( i8042_read() != 0xEE ) continue;

	i8042_write(KBD_OUT_COMMAND_SET_INDICATOR);
	while( i8042_read() != 0xFA ) continue;

	i8042_write(0b00000111);
	while( i8042_read() != 0xFA ) continue;
}

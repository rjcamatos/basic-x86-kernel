#include "i8042.h"

#include "8259A.h"
#include "82489DX.h"


//http://www.computer-engineering.org/ps2keyboard/
//http://wiki.osdev.org/%228042%22_PS/2_Controller



inline i8042_status_t i8042_status()
{
	return (i8042_status_t)ioport_inb(0x64);
}

inline void i8042_command(i8042_command_t command)
{
	ioport_outb(0x64,command);
}

inline byte_t i8042_read()
{
	uint_t retry = 500;
	while( retry-- )
	{
		i8042_status_t s = i8042_status();
		if( s.ps2_mode.output_buffer_full || s.ps2_mode.mouse_output_buffer_full ) break;
	}
	if( ! retry ) return 0x00;
	return ioport_inb(0x60);
}

inline void i8042_write(byte_t data)
{
	uint_t retry = 500;
	while( retry-- )
	{
		i8042_status_t s = i8042_status();
		if( ! s.ps2_mode.input_buffer_full ) break;
	}
	if( ! retry ) return;
	ioport_outb(0x60,data);
}

inline void i8042_command_set(i8042_command_t command, byte_t value)
{
	i8042_command(command);
	i8042_write(value);
}

inline byte_t i8042_command_get(i8042_command_t command)
{
	i8042_command(command);
	return i8042_read();
}

static void irq_handler(uint32_t vector, idt_hargs_t args, uint32_t error)
{
	byte_t c = i8042_read();
	lapic_eio(vector);
	//pic_eio((uint8_t)vector);
	printk("i8042: %u|",(uint_t)c);
}

void i8042_init()
{
	printk("INITIALIZING i8042 !\n");

	bool_t dual_channel = FALSE;

	//disable devices
	i8042_command(I8042_COMMAND_DISABLE_KEYBOARD_INTERFACE);
	i8042_command(I8042_COMMAND_DISABLE_MOUSE_INTERFACE);

	//flush controller output buffer
	i8042_read();

	//set command byte
	i8042_command_byte_t cb = (i8042_command_byte_t)i8042_command_get(I8042_COMMAND_READ_COMMAND_BYTE);
	cb.ps2_mode.keyboard_interrupt = 0;
	cb.ps2_mode.mouse_interrupt = 0;
	cb.ps2_mode.keyboard_translation = 0;

	if( cb.ps2_mode.mouse_port_clock )
	{
		printk("    detected dual channel controller\n");
		dual_channel = TRUE;
	}
	i8042_command_set(I8042_COMMAND_WRITE_COMMAND_BYTE,cb.raw_flags);

	//test controller
	printk("    controller self test: ");
	if( i8042_command_get(I8042_COMMAND_CONTROLLER_SELF_TEST) == 0x55 )
	{
		printk("pass\n");

	} else {
		printk("fail\n");
		return;
	}

	printk("    keyboard interface test: ");
	if( i8042_command_get(I8042_COMMAND_KEYBOARD_INTERFACE_TEST) == 0x00 )
	{
		printk("pass\n");
		cb.ps2_mode.keyboard_interrupt = 1;
		idt_set_interrupt_handler(33,&irq_handler);
	} else {
		printk("fail\n");
	}

	printk("    mouse interface test: ");
	if( dual_channel && i8042_command_get(I8042_COMMAND_MOUSE_INTERFACE_TEST) == 0x00 )
	{
		printk("pass\n");
		cb.ps2_mode.mouse_interrupt = 1;
		idt_set_interrupt_handler(44,&irq_handler);
	} else {
		printk("fail\n");
	}

	i8042_command_set(I8042_COMMAND_WRITE_COMMAND_BYTE,cb.raw_flags);

	if( cb.ps2_mode.keyboard_interrupt )
	{
		printk("    enabling keyboard interface\n");
		i8042_command(I8042_COMMAND_ENABLE_KEYBOARD_INTERFACE);
	}

	if( cb.ps2_mode.mouse_interrupt )
	{
		printk("    enabling mouse interface\n");
		i8042_command(I8042_COMMAND_ENABLE_MOUSE_INTERFACE);
	}

}




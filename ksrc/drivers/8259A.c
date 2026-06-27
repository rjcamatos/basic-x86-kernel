#include "8259A.h"

void pic_init(void)
{
	printk("Initializing PIC 8259A !\n");

	io_port_outb(PIC_MC,PIC_ICW1_IC4);			// need icw4
	io_port_outb(PIC_MD,PIC_ICW2_VEC(32));		// master PIC start vector
	io_port_outb(PIC_MD,PIC_ICW3_MASTER(2));	// slave on pin 2 of master
	io_port_outb(PIC_MD,PIC_ICW4_PM);			// 8086/8088 mode

	io_port_outb(PIC_SC,PIC_ICW1_IC4);			// need ICW4
	io_port_outb(PIC_SD,PIC_ICW2_VEC(40));		// slave PIC start vector
	io_port_outb(PIC_SD,PIC_ICW3_SLAVE(2));		// slave identity
	io_port_outb(PIC_SD,PIC_ICW4_PM);			// 8086/8088 mode

	pic_unmask_all();
}

void pic_mask_all()
{
	io_port_outb(PIC_MD,0xFF);
	io_port_outb(PIC_SD,0xFF);
}

void pic_unmask_all()
{
	io_port_outb(PIC_MD ,0x00);
	io_port_outb(PIC_SD ,0x00);
}



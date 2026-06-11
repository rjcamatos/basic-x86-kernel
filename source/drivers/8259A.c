#include "8259A.h"

#include "../lib/ioport.h"
#include "../lib/printk.h"

void pic_init(void)
{
	printk("Initializing PIC 8259A !\n");

	ioport_outb(PIC_MC,PIC_ICW1_IC4);			// need icw4
	ioport_outb(PIC_MD,PIC_ICW2_VEC(32));		// master PIC start vector
	ioport_outb(PIC_MD,PIC_ICW3_MASTER(2));	// slave on pin 2 of master
	ioport_outb(PIC_MD,PIC_ICW4_PM);			// 8086/8088 mode

	ioport_outb(PIC_SC,PIC_ICW1_IC4);			// need ICW4
	ioport_outb(PIC_SD,PIC_ICW2_VEC(40));		// slave PIC start vector
	ioport_outb(PIC_SD,PIC_ICW3_SLAVE(2));		// slave identity
	ioport_outb(PIC_SD,PIC_ICW4_PM);			// 8086/8088 mode

	pic_unmask_all();
}

void pic_mask_all()
{
	ioport_outb(PIC_MD,0xFF);
	ioport_outb(PIC_SD,0xFF);
}

void pic_unmask_all()
{
	ioport_outb(PIC_MD ,0x00);
	ioport_outb(PIC_SD ,0x00);
}

void pic_eio(uint8_t vector)
{
	if( vector >= 40) ioport_outb(PIC_SC,0x20); // write eio to slave PIC
	ioport_outb(PIC_MC,0x20);
}

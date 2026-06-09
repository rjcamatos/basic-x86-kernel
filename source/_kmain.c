
#include "bios/bda.h"
#include "lib/types.h"

#include "gdt.h"
#include "idt.h"

#include "drivers/8259A.h"		// A PIC - Controlador de programaca de Interrupções, para vir a ser substituido pela APIC (modelos antigos)
#include "drivers/82489DX.h"	// A APIC - 82489DX.h é a APIC, tem definicoes da LAPIC integrando o 82093AA.h que tem a IOAPIC.
#include "drivers/MC146818.h"	// A CMOS - Relógio de tempo Real
#include "drivers/i8042.h"		
#include "drivers/kbd.h"

#include "lib/printk.h"
#include "lib/stdio.h"
#include "lib/string.h"

#include "multiboot/multiboot.h"

void kmain()
{
	cmos_rtc_t rtc;
	cmos_read_rtc(&rtc);
	printk("Today: %u/%u/%u %u:%u:%u\n",rtc.date_of_month,rtc.month,rtc.year,rtc.hours,rtc.minutes,rtc.seconds);

	gdt_init();
	idt_init();

	pic_init();
	pic_mask_all();

	lapic_init();
	
	kbd_init();

	//acpi_find();

	__asm__ __volatile__("sti"); //Enable interrupts

	while(TRUE) {
		__asm__ __volatile__("hlt"); //Stop CPU exetcution.
		//CPU CONTINUES TO SERVE INTERRUPTS)
	}
}


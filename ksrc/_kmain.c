#include "types.h"

#include "multiboot/multiboot.h"	// The Multiboot

#include "cpu.h"		// The CPU

#include "bios/bda.h"	// The BIOS Information

#include "gdt.h"		// The Global Descriptor Table
#include "idt.h"		// The Interrupt Descriptor Table
#include "mmu/paging.h"	// The Memory Pagging	

#include "drivers/8259A.h"		// A PIC
#include "drivers/82489DX.h"	// A APIC
#include "drivers/MC146818.h"	// A CMOS
#include "drivers/i8042.h"		// Mouse & Keyboard Driver
#include "drivers/kbd.h"		// Mouse & Keyboard User Interface

#include "printk.h"	// The function to Fancy Print on Screen

#include "acpi.h"	// The ACPI, for now Justs Enable POWER BUTTONS

#include "timer.h"	// The Timer

void kmain(uint32_t magic, multiboot_info_t *mbi) 
{
	cmos_rtc_t rtc;
	cmos_read_rtc(&rtc);
	printk("KERNEL Init Today at: %u/%u/%u %u:%u:%u !\n",rtc.date_of_month,rtc.month,rtc.year,rtc.hours,rtc.minutes,rtc.seconds);

	//gdt_init();		//called from entry.s

	//idt_init();		//called from entry.s
	
	//paging_init();	//called from entry.s

	pic_init();		// Initialize PIC
	pic_mask_all();	// Mask All Interrupts from PIC
	
	lapic_init();	// Inititalize Local Local APIC

	ioapic_init();	// Initialize Local APIC

	// We are ready to Wait for Hardware Interrupts !
	__asm__ __volatile__("sti"); // Enable the interrupts

	lapic_timer_config(); // Configure the LAPIC Timer

	acpi_init(); // Initialize ACPI

	i8042_init(); 	// Enable Mouse and Keyboard

	kbd_init();		// Initialize Keyboard
	mouse_init();	// initialize Mouse

	//(Bootloader Magic): É o número que o QEMU coloca no registrador
	if ( magic == 0x2BADB002 ) {
		// Verificar se o bit 2 (Command Line) está ativo nas flags
		if ((mbi->flags & (1 << 2))) {
			char_t *cmdline = (char_t *)(mbi->cmdline+KERNEL_CONFIG_VMA); // Tente direto se tiver identity mapping
			printk("Multiboot Args Found !\n");
			printk("    Args = %s\n", cmdline); // Certifique-se de que o seu printk suporta %s
		}
	}

	char_t vendor[13];
	printk("CPU Vendor: %s\n", cpu_cpuid_manufacturer(vendor));
	
	// 3. Force na General Protection Fault (Vetor 13)
    // Load 0x9999 value (don´t exists in the vector GDT) to DS (Data Segment)
	/*
	__asm__ volatile(
        "mov $0x9999, %ax\n\t"
        "mov %ax, %ds"
    );
	*/
	
	while(1) {
		//timer_delay_us(2000000); // Delay for 5 second
		//printk("TESTE 2\n");
		__asm__ volatile("hlt;");
	}
	
}

/**
* This as basic .ld script, extended by .S extension
*  to reference that it passes first by co-processor
*
* The default entry is the: "start" symbol
* Multiboot Header can be configured to use
* another entry point, regardless of this
* ENTRY setting.
**/

ENTRY(entry)

SECTIONS
{
	// KERNEL_CONFIG_LMA = 0x00100000;	/* The Load memory address AT 1MB */
	// KERNEL_CONFIG_VMA = 0xC0000000;	/* The Virtual memory address, address used by the jmp far AT 3GB */
	// LMA and VMA are defined in CMakeLists.txt for Global Use
	KERNEL_ENTRY = entry - KERNEL_CONFIG_VMA;

	. = KERNEL_CONFIG_LMA + KERNEL_CONFIG_VMA;

	.multiboot ALIGN(4) : AT( ADDR(.multiboot) - KERNEL_CONFIG_VMA )
	{
		__multiboot_start = . - KERNEL_CONFIG_VMA;
		*(.multiboot)
		__multiboot_end = . - KERNEL_CONFIG_VMA;
	}

	.text ALIGN(0x1000) : AT( ADDR(.text) - KERNEL_CONFIG_VMA )
	{	
		__text_start = . - KERNEL_CONFIG_VMA;
		*(.text)
		__text_end = . - KERNEL_CONFIG_VMA;
	}

	.data ALIGN(0x1000) : AT( ADDR(.data) - KERNEL_CONFIG_VMA )
	{
   		__data_start = . - KERNEL_CONFIG_VMA;
		*(.data)
		__data_end = . - KERNEL_CONFIG_VMA;
	}

	.rodata ALIGN(0x1000) : AT( ADDR(.rodata) - KERNEL_CONFIG_VMA )
	{
   		__rodata_start = . - KERNEL_CONFIG_VMA;
		*(.rodata)
		__rodata_end = . - KERNEL_CONFIG_VMA;
	}

   .bss ALIGN(0x1000) : AT( ADDR(.bss) - KERNEL_CONFIG_VMA )
   {
		__bss_start = . - KERNEL_CONFIG_VMA;
		*(.bss)
		__bss_end = . - KERNEL_CONFIG_VMA;
   }

	/*
		Force a real section to mark the end of the runtime kernel execution
		This is used for example inside paging.c, to automaticly map the
		Kernel Memory
	*/
    .kernel_end_marker ALIGN(4) : {
        __kernel_virtual_end = .;
        __kernel_physical_end = . - KERNEL_CONFIG_VMA;
        LONG(0); /* Injects 4 physical bytes to ensure it exists before debug sections */
    }
    /* The compiler's .debug_* sections will automatically be appended here, */
    /* but the symbols above have already captured the end of the runtime code. */
   
}
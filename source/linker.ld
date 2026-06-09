/**
* The default entry is the: "start" symbol
* Multiboot Header can be configured to use
* another entry point, regardless of this
* ENTRY setting.
**/

ENTRY(entry)

SECTIONS
{
	KERNEL_LMA = 0x00100000;	/* The Load memory address AT 1MB */
	KERNEL_VMA = 0xC0000000;	/* The Virtual memory address, address used by the jmp far AT 3GB */
	KERNEL_ENTRY = entry - KERNEL_VMA;

	. = KERNEL_LMA + KERNEL_VMA;

	.multiboot ALIGN(4) : AT( ADDR(.multiboot) - KERNEL_VMA )
	{
		__multiboot_start = . - KERNEL_VMA;
		*(.multiboot)
		__multiboot_end = . - KERNEL_VMA;
	}

	.text ALIGN(0x1000) : AT( ADDR(.text) - KERNEL_VMA )
	{	
		__text_start = . - KERNEL_VMA;
		*(.text)
		__text_end = . - KERNEL_VMA;
	}

	.data ALIGN(0x1000) : AT( ADDR(.data) - KERNEL_VMA )
	{
   		__data_start = . - KERNEL_VMA;
		*(.data)
		__data_end = . - KERNEL_VMA;
	}

	.rodata ALIGN(0x1000) : AT( ADDR(.rodata) - KERNEL_VMA )
	{
   		__rodata_start = . - KERNEL_VMA;
		*(.rodata)
		__rodata_end = . - KERNEL_VMA;
	}

   .bss ALIGN(0x1000) : AT( ADDR(.bss) - KERNEL_VMA )
   {
		__bss_start = . - KERNEL_VMA;
		*(.bss)
		__bss_end = . - KERNEL_VMA;
   }
   
}
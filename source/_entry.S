.set KERNEL_VMA,0xC0000000	//the kernel Virtual Memory Address
.global entry

.section .text

	entry: //the kernel LMA entry POINT
		mov $(kernel_stack_bottom - KERNEL_VMA),%esp	//set the kernel stack
		mov %ebx,(multiboot_info - KERNEL_VMA)			//set the multiboot_info structure pointer (declared inside multiboot/multiboot.c)

		push %ebp
		mov %esp,%ebp

		call paging_init_from_lma //when this call return we are in VMA
		call kmain

		leave
		ret

	entry_init_paging: //supressed by paging_init_from_lma()
		push %ebp
		mov %esp,%ebp
		//set the page directory
		mov %cr3,%eax
		or $(entry_page_directory - KERNEL_VMA),%eax
		mov %eax,%cr3
		//enable 4MB pages
		mov %cr4,%eax
		or $0x10,%eax
		mov %eax,%cr4
		//enable paging
		mov %cr0,%eax
		or $0x80000000,%eax
		mov %eax,%cr0
		//return
		leave
		ret

.section .data

	.align 4
	kernel_stack_top:
		.rept 4096;
			.long 0x00000000;
		.endr;
	kernel_stack_bottom:
		.byte 0x00

	.align 0x1000
	entry_page_directory: //supressed by paging_init_from_lma()
		.set addr,0x00000000
		.rept 1024;
			.if addr == KERNEL_VMA
				.set addr,0x00000000
			.endif
			.long addr|0x87 //Present,ReadWrite,Supervisor
			.set addr,addr+0x400000 //Next 4MB of memory
		.endr;

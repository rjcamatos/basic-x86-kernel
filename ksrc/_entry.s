
.extern gdt_init
.extern idt_init
.extern paging_init

.extern kmain

.global entry

.section .text

	entry: # The Kernel Entry Point

		# Initialize Stack for Logical Address
		mov $(kernel_stack_top - KERNEL_CONFIG_VMA),%esp

		# Initialize entry Proceudure
		push %ebp
		mov %esp,%ebp

		add $KERNEL_CONFIG_VMA,%ebx # Add Virtual Addr to Info Structure
		push %ebx # Arg 2: Recived Pointer to Multiboot Info Structure
    	push %eax # Arg 1: Multiboot v1 Magic Number (0x2BADB002)

		 # Initialize paging at VMA (Virtual Memory Address)
		 # configure a basic page directory to allow jump to higher half
		call entry_init_paging	

		 # Initialize Global Descriptor Table, to configure
		 # flat memory model for the kernel and user space
		call gdt_init

 		# Initialize Interrupt Descriptor Table	
		# allow us to catch exceptions and interrupts as soon as possible
		call idt_init

		pop %eax # Restore Magic Number
		pop %ebx # Restore Multiboot Info Structure

		# Long Jump to Segment Selector 0x08, and go to entry_higher_half
		ljmp $0x08,$entry_higher_half

	entry_higher_half:
		add $KERNEL_CONFIG_VMA,%esp # Add Virtual Addr to Stack Pointer

		 # Initialize Paging
		 # We are in Higher Half Memory, ready to configure final page tables
		 # discarding the low memory 1:1 mapping for Kenrel
		 call paging_init

		push %ebx # Arg 2: Recived Pointer to Multiboot Info Structure
		push %eax # Arg 1: The Multiboot Magic

		# Call the Kernel Main Function
		call kmain

		ret

	entry_init_paging: # Initialize Page Driectory, to allow jump to Higher Half
		push %ebp
		mov %esp,%ebp
		//set the page directory
		mov %cr3,%eax
		or $(entry_page_directory - KERNEL_CONFIG_VMA),%eax
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
	kernel_stack_bottom:
		.rept 1024 * 4 - 1;  # The Kernel Stack of 18KB
			.long 0x00000000;
		.endr;
	kernel_stack_top:
		.long 0x00000000;

	.align 4096
	entry_page_directory: # A Page Driectory, to allow jump to Higher Half
		.set addr,0x00000000
		.rept 1024;
			.if addr >= KERNEL_CONFIG_VMA
				.set addr,0x00000000
			.endif
			.long addr|0x87 #Present,ReadWrite,Supervisor
			.set addr,addr+0x400000 #Next 4MB of memory
		.endr;



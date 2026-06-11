#include "idt.h"
#include "lib/mem.h"
#include "lib/printk.h"

#include "drivers/8259A.h"
#include "drivers/82489DX.h"

// The Interrupt Descriptor Table, aligned for best performance
idt_gate_t __attribute__((aligned(8))) idt_gate[255];

// Used To Load The Interrupt Descriptor Table
idt_lidt_t lidt;

// Callback handlers provided to ASM gate handlers
voidptr_t idt_gate_callback[255];


/* Table 6-1. Protected-Mode Exceptions and Interrupts */
void idt_exception_handler(uint32_t vector, idt_hargs_t args, uint32_t error)
{
	const char* vinfo[255] = {
		"#DE|Divide Error|DIV and IDIV instructions.",
		"#DB|Debug Exception|Hardware breakpoints and single-step execution.",
		"#NMI|Interrupt|Nonmaskable external interrupt.",
		"#BP|Breakpoint|INT 3 instruction.",
		"#OF|Overflow|INTO instruction.",
		"#BR|BOUND Range Exceeded|BOUND instruction.",
		"#UD|Invalid Opcode (Undefined Opcode)|UD2 instruction or reserved opcode.(1)",
		"#NM|Device Not Available (No MathCoprocessor)|Floating-point or WAIT/FWAIT instruction.",
		"#DF|Double Fault|Any instruction that can generate anexception, an NMI, or an INTR.",
		"#MP|Coprocessor Segment Overrun (reserved)|Floating-point instruction.(2)",
		"#TS|Invalid TSS|Task switch or TSS access.",
		"#NP|Segment Not Present|Loading segment registers or accessing system segments.",
		"#SS|Stack-Segment Fault|Stack operations and SS register loads.",
		"#GP|General Protection|Any memory reference and other protection checks.",
		"#PF|Page Fault|Any memory reference.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#MF|x87 FPU Floating-Point Error (Math Fault)|x87 FPU floating-point or WAIT/FWAIT instruction.",
		"#AC|Alignment Check|Any data reference in memory.(3)",
		"#MC|Machine Check|Error codes (if any) and source are model dependent.(4)",
		"#XM|SIMD Floating-Point Exception|SSE/SSE2/SSE3 floating-point instructions(5)",
		"#VE|Virtualization Exception|EPT violations(6)",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
		"#??|Intel reserved, do not use.|Intel reserved, do not use.",
    	/* GEMENI: As posições de 32 a 255 serão inicializadas automaticamente como NULL */
	};


	// O bloco nativo do CPU começa exatamente após o argumento 'error' na stack
    idt_cpu_frame_t *cpu = (idt_cpu_frame_t *)((uint32_t*)&error + 1);

    if (vector == 13) { // General Protection Fault
        printk("Kernel Panic! Erro no endereco EIP: %u\n", cpu->eip);
        printk("Segmento de codigo CS: %u\n", cpu->cs);
		// --- PARAR O PROCESSADOR NA TOTALIDADE ---
		// 1. Desativa todas as interrupções de hardware (evita que o timer ou teclado acordem o CPU)
		// 2. Coloca o CPU em estado de suspensão (Halt)
		// 3. Mantém num loop caso uma NMI tente acordar o processador
		while(1) {
			__asm__ volatile("cli; hlt");
		}
    }

	printk("Exp %s|",vinfo[vector]);
	printk("Exp %u|",vector);
}

void idt_interrupt_handler(uint32_t vector, idt_hargs_t args, uint32_t error)
{	
	printk("Int %u || ",vector);
	// END-OF-INTERRUPT
	pic_eio((uint8_t)vector); // required when pic is unmasked
	lapic_eio(vector); // required when lapic is enabled
}

void idt_init(void)
{
	printk("Initializing IDT !\n");

	memset(idt_gate,0x00,sizeof(idt_gate));
	memset(idt_gate_callback,0x00,sizeof(idt_gate_callback));

	extern uint32_t idt_gate_handler[]; //inside idt_asm.S
	uint32_t idx;
	for( idx=0; idx < 255; idx++ )
	{
		if( idx == 15 || (idx > 20 && idx < 32) ) {
			// Intel reserved, don't use it
			continue;
		} else if( idx < 32 ) {
			// Exceptions handlers
			idt_gate_callback[idx] = &idt_exception_handler;
			idt_set_gate_trap(&idt_gate[idx],0x8,idt_gate_handler[idx],IDT_FLAG_GATE_D|IDT_FLAG_GATE_DPL0|IDT_FLAG_GATE_P);
			continue;
		} else {
			// User defined interrupts
			idt_gate_callback[idx] = &idt_interrupt_handler;
			idt_set_gate_interrupt(&idt_gate[idx],0x8,idt_gate_handler[idx],IDT_FLAG_GATE_D|IDT_FLAG_GATE_DPL0|IDT_FLAG_GATE_P);
		}
	}
	lidt.limit = sizeof(idt_gate)-1;
	lidt.base = (uint32_t)idt_gate;
	idt_set_table(&lidt);
}

void idt_set_table(idt_lidt_t *lidt)
{
	__asm__ __volatile__("lidt (%0)"::"r"(lidt):);
}

void idt_set_gate_task(idt_gate_t *gate, uint16_t tss, uint16_t flags)
{
	*(uint64_t*)gate =	((uint64_t)tss<<16) |
						((uint64_t)0b00101<<40) |
						((uint64_t)flags<<32);
}

void idt_set_gate_interrupt(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags)
{
	*(uint64_t*)gate = 	((uint64_t)offset&0x0000FFFF)|
						((uint64_t)ss<<16) |
						((uint64_t)0b00110<<40) |
						((uint64_t)flags<<32) |
						((uint64_t)(offset&0xFFFF0000)<<32);
}

void idt_set_gate_trap(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags)
{
	*(uint64_t*)gate = 	((uint64_t)offset&0x0000FFFF)|
						((uint64_t)ss<<16) |
						((uint64_t)0b00111<<40) |
						((uint64_t)flags<<32) |
						((uint64_t)(offset&0xFFFF0000)<<32);
}

void idt_set_interrupt_handler(uint8_t interrupt,voidptr_t handler)
{
	idt_gate_callback[interrupt] = handler;
}



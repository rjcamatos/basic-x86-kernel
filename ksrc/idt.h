/*
 * IDT - Interrupt Descriptor Table
 *
 *  Created on: 04/06/2014
 *      Author: myself
 */

/** COMMENTS By GEMINI */

/**
 * IDT - Interrupt Descriptor Table
 * 
 * Manages interrupts and exceptions by mapping vectors to their
 * corresponding Interrupt Service Routines (ISRs).
 */

#ifndef KERNEL_IDT_H_
#define KERNEL_IDT_H_

#include "types.h"

// IDTR register structure for the native LIDT instruction
typedef struct {
	uint16_t limit; // Total IDT size in bytes minus 1
	uint32_t base;  // Linear base address of the IDT
} __attribute__((packed)) idt_lidt_t;

// IDT entry structure (Gate) for 32-bit x86 architecture
typedef struct {
	uint32_t offset_low:16;  // ISR offset bits 0-15
	uint32_t ss:16;          // Code Segment selector (GDT)
	uint32_t flags:16;       // Attributes and Gate type flags
	uint32_t offset_high:16; // ISR offset bits 16-31
} __attribute__((packed)) idt_gate_t;

/* Access and Type Flags for IDT Gates */
#define IDT_FLAG_GATE_DPL0	(0<<13) // Kernel privilege level
#define IDT_FLAG_GATE_DPL1	(1<<13)	// Driver privilege level 1
#define IDT_FLAG_GATE_DPL2	(2<<13)	// Driver privilege level 2
#define IDT_FLAG_GATE_DPL3	(3<<13)	// User application privilege level
#define IDT_FLAG_GATE_P		(1<<15)	// Present bit
#define IDT_FLAG_GATE_D		(1<<11)	// Size bit (1 = 32-bit, 0 = 16-bit)

// General-purpose registers saved during an interrupt
typedef struct {
		int32_t edi; // Destination Index register
		int32_t esi; // Source Index register
		int32_t ebp; // Base Pointer register
		int32_t esp; // Original Stack Pointer register
		int32_t ebx; // Base register
		int32_t edx; // Data register
		int32_t ecx; // Counter register
		int32_t eax; // Accumulator register
} __attribute__((packed)) idt_registers_t;

// Error code structure pushed by the CPU on exceptions
typedef union {
    uint32_t raw_bits;
    struct {
        uint32_t ext:1;   // External event flag
        uint32_t idt:1;   // IDT indicator flag
        uint32_t ti:1;    // Table Indicator flag
        uint32_t ssi:16;  // Segment Selector Index
        uint32_t _r0:13;  // Reserved bits
    } __attribute__((packed));
} __attribute__((packed)) idt_error_t;

// Execution frame pushed automatically by CPU hardware
typedef struct {
	uint32_t eip;    // Instruction Pointer
	uint32_t cs;     // Code Segment selector
	uint32_t eflags; // Flags register
} __attribute__((packed)) idt_cpu_frame_t;

/* Function Prototypes */

// Initializes and loads the IDT
extern void idt_init(void);

// Loads the IDTR register using native 'lidt' instruction
extern void idt_set_table(idt_lidt_t *lidt);

// Configures a Task Gate in the IDT
extern void idt_set_gate_task(idt_gate_t *gate, uint16_t tss, uint16_t flags);

// Configures an Interrupt Gate (disables hardware interrupts)
extern void idt_set_gate_interrupt(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags);

// Configures a Trap Gate (keeps hardware interrupts enabled)
extern void idt_set_gate_trap(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags);

// Registers a C handler function for an interrupt vector
extern void idt_set_interrupt_handler(uint8_t interrupt, voidptr_t handler);

// Unmasks a specific interrupt vector
extern void idt_unmask_interrupt(uint8_t vector);

// Masks a specific interrupt vector
extern void idt_mask_interrupt(uint8_t vector);

#endif /* IDT_H_ */

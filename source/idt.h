/*
 * IDT - Interrupt Descriptor Table
 *
 * A Interrupt Descriptor Table (IDT) é uma estrutura de dados binária usada
 * pelas arquiteturas x86 e x86-64 para gerir interrupções e exceções.Quando o 
 * processador deteta um evento — como um erro de divisão por zero, uma tecla 
 * pressionada ou um sinal do temporizador —, ele suspende temporariamente o 
 * código atual, consulta a IDT para encontrar a função responsável por lidar 
 * com esse evento (chamada de Interrupt Service Routine - ISR) e executa-a.
 * ~
 * By GEMINI
 * 
 * */

#ifndef IDT_H_
#define IDT_H_

#include "lib/types.h"

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_lidt_t;

typedef struct {
	uint32_t offset_low:16;
	uint32_t ss:16;
	uint32_t flags:16;
	uint32_t offset_high:16;
} __attribute__((packed)) idt_gate_t;

// Flags for idt_gate_t
#define IDT_FLAG_GATE_DPL0	(0<<13) // Privilege Level 0
#define IDT_FLAG_GATE_DPL1	(1<<13)	// Privilege Level 1
#define IDT_FLAG_GATE_DPL2	(2<<13)	// Privilege Level 2
#define IDT_FLAG_GATE_DPL3	(3<<13)	// Privilege Level 3
#define IDT_FLAG_GATE_P		(1<<15)	// Present
#define IDT_FLAG_GATE_D		(1<<11)	// Size of gate 32bits, otherwise is 16bits, ONLY FOR "TRAP" AND "INTERRUPT" GATES

/* Arguments for Interrupt Handlers */
typedef struct {
	struct {
		int32_t EDI;
		int32_t ESI;
		int32_t EBP;
		int32_t ESP;
		int32_t EBX;
		int32_t EDX;
		int32_t ECX;
		int32_t EAX;
	} registers;
	/*
	//Figure 6-4. Stack Usage on Transfers to Interrupt and Exception-Handling Routines
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	//6.13 ERROR CODE
	struct {
		uint32_t EXT:1;		//when EXT=1 is external event
		uint32_t IDT:1;		//when IDT=1 SSI is gate descriptor in IDT
		uint32_t TI:1;		//when IDT=0 and TI=1 SSI is code segment or gate descriptor in LDT
							//when IDT=0 and TI=0 SSI is descriptor in current GDT
		uint32_t SSI:16;	//segment selector index
		uint32_t _r0:13;	//reserved
	} idt_error;*/
} __attribute__((packed)) idt_hargs_t;

extern void idt_init();
extern void idt_set_table(idt_lidt_t *lidt);
extern void idt_set_gate_task(idt_gate_t *gate, uint16_t tss, uint16_t flags);
extern void idt_set_gate_interrupt(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags);
extern void idt_set_gate_trap(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags);
extern void idt_interrupt_handler(uint32_t vector, idt_hargs_t args, uint32_t error);
extern void idt_set_interrupt_handler(uint8_t interrupt, voidptr_t handler);


#endif /* IDT_H_ */

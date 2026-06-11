/**
 * 8259A PIC - Programmable Interrupt Controller
 * 
 * Um Programmable Interrupt Controller (PIC) é um chip de hardware que 
 * gere os pedidos de interrupção (IRQs) de múltiplos dispositivos periféricos, 
 * encaminhando-os para o processador por ordem de prioridade. Ele liberta 
 * a CPU de monitorizar constantemente os dispositivos, tornando o sistema 
 * muito mais eficiente.
 * 
 * By GEMINI
 * 
 */

#ifndef DRIVERS_8259A_H_
#define DRIVERS_8259A_H_

#include "../lib/types.h"

//PIC PORTS
#define PIC_MC 0x0020	// PIC Master Command PORT
#define PIC_MD 0x0021	// PIC Master Data PORT
#define PIC_SC 0x00A0	// PIC Slave Command PORT
#define PIC_SD 0x00A1	// PIC Slave Data PORT

// Initialization Command Word 1
#define PIC_ICW1_IC4	(0b1<<0|1<<4)	// Need initialization command word 4
#define PIC_ICW1_SNGL	(0b1<<1|1<<4)	// Single mode otherwise  Cascade
#define PIC_ICW1_ADI	(0b1<<2|1<<4)	// Call address interval of 4
#define PIC_ICW1_LTIM	(0b1<<3|1<<4)	// Level triggered mode

// Initialization Command Word 2
#define PIC_ICW2_VEC(INDEX)	(INDEX)	//Interrupt vector address

// Initialization Command Word 3 (Only if ~PIC_ICW1_SNGL)
#define PIC_ICW3_MASTER(IR)	(1<<IR)		// IR input has a slave
#define PIC_ICW3_SLAVE(SID)	(SID&0b111)	// Slave ID

// Initialization Command Word 4 (Only if PIC_ICW1_IC4)
#define PIC_ICW4_PM		(0b1<<0)		// 8086/8088 Mode
#define PIC_ICW4_AEIO	(0b1<<1)		// Auto end-of-interrupt
#define PIC_ICW4_SBUF	(0b1<<3)		// Buffered Mode/Slave
#define PIC_ICW4_MBUF	(0b11<<2)		// Buffered Mode/Master
#define PIC_ICW4_SFNM	(0b1<<4)		// Special Fully Nested Mode

extern void pic_init();
extern void pic_mask_all();
extern void pic_unmask_all();
extern void pic_eio(uint8_t vector);

#endif /* DRIVERS_8259A_H_ */

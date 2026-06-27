/** COMMENTS By GEMINI */

/**
 * 8259A PIC - Programmable Interrupt Controller
 * 
 * Routes peripheral IRQs to CPU by priority.
 */

#ifndef KERNEL_DRIVERS_8259A_H_
#define KERNEL_DRIVERS_8259A_H_

#include "types.h"
#include "io.h"
#include "printk.h"

/* PIC I/O Ports */
#define PIC_MC 0x0020    // Master PIC Command
#define PIC_MD 0x0021    // Master PIC Data
#define PIC_SC 0x00A0    // Slave PIC Command
#define PIC_SD 0x00A1    // Slave PIC Data

/* ICW1 - Initial Setup */
#define PIC_ICW1_IC4    ((1<<0) | (1<<4)) // ICW4 needed
#define PIC_ICW1_SNGL   ((1<<1) | (1<<4)) // Single mode (else Cascade)
#define PIC_ICW1_ADI    ((1<<2) | (1<<4)) // Call address interval 4
#define PIC_ICW1_LTIM   ((1<<3) | (1<<4)) // Level triggered mode

/* ICW2 - Vector Mapping */
#define PIC_ICW2_VEC(INDEX) (INDEX)       // Interrupt vector base address

/* ICW3 - Cascade Setup (if !PIC_ICW1_SNGL) */
#define PIC_ICW3_MASTER(IR)  (1<<(IR))    // Master IR line to Slave
#define PIC_ICW3_SLAVE(SID)  ((SID) & 0x07) // Slave ID (binary line ID)

/* ICW4 - Operating Mode (if PIC_ICW1_IC4) */
#define PIC_ICW4_PM     (1<<0)           // 8086/8088 mode (for x86)
#define PIC_ICW4_AEOI   (1<<1)           // Auto End-Of-Interrupt
#define PIC_ICW4_SBUF   (1<<3)           // Buffered mode / Slave
#define PIC_ICW4_MBUF   (3<<2)           // Buffered mode / Master
#define PIC_ICW4_SFNM   (1<<4)           // Special Fully Nested Mode

/* Function Prototypes */

// Initializes and remaps Master/Slave PICs to prevent CPU vector conflicts
extern void pic_init(void);

// Masks all interrupt lines to disable IRQs
extern void pic_mask_all(void);

// Unmasks all interrupt lines to enable IRQs
extern void pic_unmask_all(void);

// Sends an End of Interrupt (EOI) signal to the PIC
static inline void pic_eio(uint8_t vector)
{
	if( vector >= 40) io_port_outb(PIC_SC,0x20); // write eio to slave PIC
	io_port_outb(PIC_MC,0x20);
}

#endif /* DRIVERS_8259A_H_ */

/** COMMENTS By GEMINI */

/**
 * 82489DX APIC (LAPIC + IOAPIC) - Advanced Programmable Interrupt Controller
 * 
 * Replaces 8259A PIC with LAPIC and IOAPIC for multi-core systems.
 * IOAPIC routes external hardware IRQs to specific CPU cores.
 * LAPIC (one per core) handles local interrupts and inter-core communication (IPIs).
 * 
 * See 8259A and 82093AA headers for context.
 */

#ifndef KERNEL_DRIVERS_82489DX_H_
#define KERNEL_DRIVERS_82489DX_H_

#include "types.h"
#include "82093AA.h"

/* Local LAPIC Registers */
#define LAPIC_ID			0x0020	// Local LAPIC ID (R/W)
#define LAPIC_VER			0x0030	// Local LAPIC Version (RO)
#define LAPIC_TPR			0x0080	// Task Priority Register (R/W)
#define LAPIC_APR			0x0090	// Arbitration Priority Register (RO)
#define LAPIC_PPR			0x00A0	// Processor Priority Register (RO)
#define LAPIC_EIO			0x00B0	// End of Interrupt Register (WO)
#define LAPIC_RRD			0x00C0	// Remote Read Register (RO)
#define LAPIC_LDR			0x00D0	// Logical Destination Register (R/W)
#define LAPIC_DFR			0x00E0	// Destination Format Register (R/W)
#define LAPIC_SIVR			0x00F0	// Spurious Interrupt Vector Register (R/W)
#define LAPIC_ISR0			0x0100	// In-Service Register bits 31:0 (RO)
#define LAPIC_ISR1			0x0110	// In-Service Register bits 63:32 (RO)
#define LAPIC_ISR2			0x0120	// In-Service Register bits 95:64 (RO)
#define LAPIC_ISR3			0x0130	// In-Service Register bits 127:96 (RO)
#define LAPIC_ISR4			0x0140	// In-Service Register bits 159:128 (RO)
#define LAPIC_ISR5			0x0150	// In-Service Register bits 191:160 (RO)
#define LAPIC_ISR6			0x0160	// In-Service Register bits 223:192 (RO)
#define LAPIC_ISR7			0x0170	// In-Service Register bits 255:224 (RO)
#define LAPIC_TMR0			0x0180	// Trigger Mode Register bits 31:0 (RO)
#define LAPIC_TMR1			0x0190	// Trigger Mode Register bits 63:32 (RO)
#define LAPIC_TMR2			0x01A0	// Trigger Mode Register bits 95:64 (RO)
#define LAPIC_TMR3			0x01B0	// Trigger Mode Register bits 127:96 (RO)
#define LAPIC_TMR4			0x01C0	// Trigger Mode Register bits 159:128 (RO)
#define LAPIC_TMR5			0x01D0	// Trigger Mode Register bits 191:160 (RO)
#define LAPIC_TMR6			0x01E0	// Trigger Mode Register bits 223:192 (RO)
#define LAPIC_TMR7			0x01F0	// Trigger Mode Register bits 255:224 (RO)
#define LAPIC_IRR0			0x0200	// Interrupt Request Register bits 31:0 (RO)
#define LAPIC_IRR1			0x0210	// Interrupt Request Register bits 63:32 (RO)
#define LAPIC_IRR2			0x0220	// Interrupt Request Register bits 95:64 (RO)
#define LAPIC_IRR3			0x0230	// Interrupt Request Register bits 127:96 (RO)
#define LAPIC_IRR4			0x0240	// Interrupt Request Register bits 159:128 (RO)
#define LAPIC_IRR5			0x0250	// Interrupt Request Register bits 191:160 (RO)
#define LAPIC_IRR6			0x0260	// Interrupt Request Register bits 223:192 (RO)
#define LAPIC_IRR7			0x0270	// Interrupt Request Register bits 255:224 (RO)
#define LAPIC_ESR			0x0280	// Error Status Register (RO)
#define LAPIC_LVTCMCIR		0x02F0	// LVT CMCI Register (R/W)
#define LAPIC_ICR0			0x0300	// Interrupt Command Register bits 0-31 (R/W)
#define LAPIC_ICR1			0x0310	// Interrupt Command Register bits 32-63 (R/W)
#define LAPIC_LVTTR			0x0320	// LVT Timer Register (R/W)
#define LAPIC_LVTTSR		0x0330	// LVT Thermal Sensor Register (R/W)
#define LAPIC_LVTPMCR		0x0340	// LVT Performance Monitoring Counters (R/W)
#define LAPIC_LVTLINT0		0x0350	// LVT LINT0 Register (R/W)
#define LAPIC_LVTLINT1		0x0360	// LVT LINT1 Register (R/W)
#define LAPIC_LVTERROR		0x0370	// LVT Error Register (R/W)
#define LAPIC_ICR			0x0380	// Initial Count Register for Timer (R/W)
#define LAPIC_CCR			0x0390	// Current Count Register for Timer (RO)
#define LAPIC_DCR			0x03E0	// Divide Configuration Register for Timer (R/W)


// Local LAPIC ID Register
typedef struct {
	union {
		struct {
			uint_t _r0:24;
			uint_t id:4;
			uint_t _r1:4;
		} pp6;				// P6 and Pentium layout
		struct {
			uint_t _r0:24;
			uint_t id:8;
		} p4xl;				// Pentium 4, Xeon, and later layout
		struct {
			uint32_t id;
		} xapic;			// x2APIC mode layout
	};
} lapic_idr_t;

// Local LAPIC Version Register
typedef struct {
	uint_t ver:8;		// Version
	uint_t _r0:8;		// Reserved
	uint_t mlvte:8;		// Max LVT Entry
	uint_t seiobs:1;	// EOI-broadcast suppression support
	uint_t _r1:7;		// Reserved
} lapic_ver_t;

// Task-Priority Register (TPR)
typedef struct {
	uint_t tps:4;		// Task Priority Sub-Class
	uint_t tpc:4;		// Task Priority Class
	uint_t _r0:24;		// Reserved
} lapic_tpr_t;

// Arbitration Priority Register (APR)
typedef struct {
	uint_t aps:4;		// Arbitration Priority Sub-Class
	uint_t apc:4;		// Arbitration Priority Class
	uint_t _r0:24;		// Reserved
} lapic_apr_t;

// Processor-Priority Register (PPR)
typedef struct {
	uint_t pps:4;		// Processor Priority Sub-Class
	uint_t ppc:4;		// Processor Priority Class
	uint_t _r0:24;		// Reserved
} lapic_ppr_t;

// End-Of-Interrupt Register (EOI)
typedef struct {
	uint32_t eio;		// End-Of-Interrupt
} lapic_eio_t;

// Remote Read Register (RRD)
typedef struct {
	uint32_t rrd;		// Register data field
} lapic_rrd_t;

// Logical Destination Register (LDR)
typedef struct {
	uint_t _r0:24;		// Reserved
	uint_t id:8;		// Logical LAPIC ID
} lapic_ldr_t;

// Destination Format Register (DFR)
typedef struct {
	uint_t _r0:28;		// Reserved
	uint_t mod:4;		// Destination model format
} lapic_dfr_t;

// Spurious-Interrupt Vector Register (SVR)
typedef struct {
	uint_t sv:8;		// Spurious Vector
	uint_t sed:1;		// LAPIC Software Enable/Disable
	uint_t fpc:3;		// Focus Processor Checking
	uint_t ebs:1;		// EOI-Broadcast Suppression
	uint_t _r0:19;		// Reserved
} lapic_sivr_t;

// ISR, TMR, and IRR Registers
typedef struct {
	union {
		struct {
			uint_t _r0:16;		// LAPIC Reserved
			uint_t vec:16;
		} fv;					// Vectors 16 to 31
		struct {
			uint32_t vec;
		} nv;					// Vectors 32 to 255
	};
} lapic_isr_t, lapic_tmr_t, lapic_irr_t;

// Error Status Register (ESR)
typedef struct {
	uint_t sc:1;		// Send Checksum Error
	uint_t rc:1;		// Receive Checksum Error
	uint_t sa:1;		// Send Accept Error
	uint_t ra:1;		// Receive Accept Error
	uint_t ripi:1;		// Redirectable IPI Error
	uint_t siv:1;		// Send Illegal Vector
	uint_t riv:1;		// Received Illegal Vector
	uint_t ira:1;		// Illegal Register Address
	uint_t _r0:24;		// Reserved
} lapic_esr_t;

// LVT CMCI Register
typedef struct {
	uint_t vec:8;		// Vector
	uint_t dm:3;		// Delivery Mode
	uint_t _r0:1;		// Reserved
	uint_t ds:1;		// Delivery Status
	uint_t _r1:3;		// Reserved
	uint_t ma:1;		// Mask
	uint_t _r2:15;		// Reserved
} lapic_lvt_cmci_t;

// LVT Timer Register
typedef struct {
	uint_t vec:8;		// Vector
	uint_t _r0:4;		// Reserved
	uint_t ds:1;		// Delivery Status
	uint_t _r1:3;		// Reserved
	uint_t ma:1;		// Mask
	uint_t tm:2;		// Timer Mode
	uint_t _r2:13;		// Reserved
} lapic_lvt_tr_t;

/* LVT Timer Register Flag Masks */
#define LAPIC_LVTTR_VECTOR(v)      ((v) & 0xFF)   // Prevents vector overflow
#define LAPIC_LVTTR_MASK           (1 << 16)      // Disable Timer interrupts
#define LAPIC_LVTTR_ONESHOT        (0 << 17)      // Count-once execution mode
#define LAPIC_LVTTR_PERIODIC       (1 << 17)      // Automatic reload execution mode

// LVT Thermal Sensor and Performance Monitoring Counters Registers
typedef struct {
	uint_t vec:8;		// Vector
	uint_t dm:3;		// Delivery Mode
	uint_t _r0:1;		// Reserved
	uint_t ds:1;		// Delivery Status
	uint_t _r1:3;		// Reserved
	uint_t ma:1;		// Mask
	uint_t _r2:15;		// Reserved
} lapic_lvt_ts_t, lapic_lvt_pmc_t;

// LVT LINT0 and LINT1 Registers
typedef struct {
	uint_t vec:8;		// Vector
	uint_t dm:3;		// Delivery Mode
	uint_t _r0:1;		// Reserved
	uint_t ds:1;		// Delivery Status
	uint_t iipp:1;		// Pin Polarity
	uint_t rirr:1;		// Remote IRR
	uint_t tm:1;		// Trigger Mode
	uint_t ma:1;		// Mask
	uint_t _r2:15;		// Reserved
} lapic_lvt_lint0_t, lapic_lvt_lint1_t ;

#define LAPIC_LINT_VECTOR(v)	((v)&0xFF)	// Isolates 8-bit vector
#define LAPIC_LINT_FIXED		(0b000<<8)	// Fixed vector mode
#define LAPIC_LINT_SMI			(0b010<<8)	// System Management Interrupt
#define LAPIC_LINT_NMI			(0b100<<8)	// Non-Maskable Interrupt
#define LAPIC_LINT_INIT			(0b101<<8)	// INIT signal soft-reset
#define LAPIC_LINT_EXTINT		(0b111<<8)	// External 8259 PIC fallback mode

#define LAPIC_LINT_IIPP			(0b1<<13)	// Active Low pin polarity
#define LAPIC_LINT_RIRR			(0b1<<14)	// Remote IRR status flag
#define LAPIC_LINT_LEVEL		(0b1<<15)	// Level-triggered indicator bit
#define LAPIC_LINT_MASK			(0b1<<16)	// Disable pin interrupt flag

// LVT Error Register
typedef struct {
	uint_t vec:8;		// Vector
	uint_t _r0:4;		// Reserved
	uint_t ds:1;		// Delivery Status
	uint_t _r1:3;		// Reserved
	uint_t ma:1;		// Mask
	uint_t _r2:15;		// Reserved
} lapic_lvt_error_t;

// Initial Count and Current Count Registers
typedef struct {
	uint32_t count;
} lapic_icr_t, lapic_ccr_t;
#define LAPIC_ICR_COUNT(n)		(n)

// Divide Configuration Register
typedef struct {
	uint_t div:4;		// Divisor selector bits
	uint_t _r1:28;		// Reserved
} lapic_dcr_t;
#define LAPIC_DCR_DIV1			0b1011
#define LAPIC_DCR_DIV2			0b0000
#define LAPIC_DCR_DIV4			0b0001
#define LAPIC_DCR_DIV8			0b0010
#define LAPIC_DCR_DIV16			0b0011
#define LAPIC_DCR_DIV32			0b1000
#define LAPIC_DCR_DIV64			0b1001
#define LAPIC_DCR_DIV128		0b1010

extern volatile voidptr_t lapic_base;

// Map memory access offset directly to LAPIC base address
#define LAPIC_REG(REG)		*((uint32_t*)((uint32_t)lapic_base|REG))

/* Function Prototypes */

// Initializes the Local APIC
extern void lapic_init(void);

// Remaps the MMIO base address
extern void lapic_remap(uint32_t addr);

// Gets the current MMIO base pointer
extern uint32_t* lapic_get_base(void);

// Enables the LAPIC via the Spurious Vector Register
extern void lapic_enable(void);


extern uint_t lapic_ticks_per_ms;
extern uint_t lapic_ticks_per_us;

// Configures internal timer rates and execution modes
void lapic_timer_config(void);

// Sends an End of Interrupt (EOI) signal
static inline void lapic_eio(uint32_t val)
{
	LAPIC_REG(LAPIC_EIO) = val;
}



#endif /* DRIVERS_82489DX_H_ */

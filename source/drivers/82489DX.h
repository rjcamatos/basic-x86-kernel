/*
	Local LAPIC - Local Advanced Interrupt Controller

	Is included in the P6 family, Pentium 4, Intel Xeon processors, and other
	more recent Intel 64 and IA-32 processor families.

	The 82489DX It's composed by the IOAPIC and this one.
	It's integrated in the CPU in modern computers.
 */

#ifndef DRIVERS_82489DX_H_
#define DRIVERS_82489DX_H_

#include "../lib/types.h"
#include "82093AA.h"

/* Local LAPIC Registers */
#define LAPIC_ID				0x0020	//	Local LAPIC ID Register Read/Write.
#define LAPIC_VER			0x0030	//	Local LAPIC Version Register Read Only.
#define LAPIC_TPR			0x0080	//	Task Priority Register (TPR) Read/Write.
#define LAPIC_APR			0x0090	//	Arbitration Priority Register1 (APR) Read Only.
#define LAPIC_PPR			0x00A0	//	Processor Priority Register (PPR) Read Only.
#define LAPIC_EIO			0x00B0	//	EOI Register Write Only.
#define LAPIC_RRD			0x00C0	//	Remote Read Register1 (RRD) Read Only
#define LAPIC_LDR			0x00D0	//	Logical Destination Register Read/Write.
#define LAPIC_DFR			0x00E0	//	Destination Format Register Read/Write (see Section10.6.2.2).
#define LAPIC_SIVR			0x00F0	//	Spurious Interrupt Vector Register Read/Write (see Section 10.9.
#define LAPIC_ISR0			0x0100	//	In-Service Register (ISR); bits 31:0 Read Only.
#define LAPIC_ISR1			0x0110	//	In-Service Register (ISR); bits 63:32 Read Only.
#define LAPIC_ISR2			0x0120	//	In-Service Register (ISR); bits 95:64 Read Only.
#define LAPIC_ISR3			0x0130	//	In-Service Register (ISR); bits 127:96 Read Only.
#define LAPIC_ISR4			0x0140	//	In-Service Register (ISR); bits 159:128 Read Only.
#define LAPIC_ISR5			0x0150	//	In-Service Register (ISR); bits 191:160 Read Only.
#define LAPIC_ISR6			0x0160	//	In-Service Register (ISR); bits 223:192 Read Only.
#define LAPIC_ISR7			0x0170	//	In-Service Register (ISR); bits 255:224 Read Only.
#define LAPIC_TMR0			0x0180	//	Trigger Mode Register (TMR); bits 31:0 Read Only.
#define LAPIC_TMR1			0x0190	//	Trigger Mode Register (TMR); bits 63:32 Read Only.
#define LAPIC_TMR2			0x01A0	//	Trigger Mode Register (TMR); bits 95:64 Read Only.
#define LAPIC_TMR3			0x01B0	//	Trigger Mode Register (TMR); bits 127:96 Read Only.
#define LAPIC_TMR4			0x01C0	//	Trigger Mode Register (TMR); bits 159:128 Read Only.
#define LAPIC_TMR5			0x01D0	//	Trigger Mode Register (TMR); bits 191:160 Read Only.
#define LAPIC_TMR6			0x01E0	//	Trigger Mode Register (TMR); bits 223:192 Read Only.
#define LAPIC_TMR7			0x01F0	//	Trigger Mode Register (TMR); bits 255:224 Read Only.
#define LAPIC_IRR0			0x0200	//	Interrupt Request Register (IRR); bits 31:0 Read Only.
#define LAPIC_IRR1			0x0210	//	Interrupt Request Register (IRR); bits 63:32 Read Only.
#define LAPIC_IRR2			0x0220	//	Interrupt Request Register (IRR); bits 95:64 Read Only.
#define LAPIC_IRR3			0x0230	//	Interrupt Request Register (IRR); bits 127:96 Read Only.
#define LAPIC_IRR4			0x0240	//	Interrupt Request Register (IRR); bits 159:128 Read Only.
#define LAPIC_IRR5			0x0250	//	Interrupt Request Register (IRR); bits 191:160 Read Only.
#define LAPIC_IRR6			0x0260	//	Interrupt Request Register (IRR); bits 223:192 Read Only.
#define LAPIC_IRR7			0x0270	//	Interrupt Request Register (IRR); bits 255:224 Read Only.
#define LAPIC_ESR			0x0280	//	Error Status Register Read Only.
#define LAPIC_LVTCMCIR		0x02F0	//	LVT CMCI Register Read/Write.
#define LAPIC_ICR0			0x0300	//	Interrupt Command Register (ICR); bits 0-31 Read/Write.
#define LAPIC_ICR1			0x0310	//	Interrupt Command Register (ICR); bits 32-63 Read/Write.
#define LAPIC_LVTTR			0x0320	//	LVT Timer Register Read/Write.
#define LAPIC_LVTTSR			0x0330	//	LVT Thermal Sensor Register2 Read/Write.
#define LAPIC_LVTPMCR		0x0340	//	LVT Performance Monitoring Counters Register3 Read/Write.
#define LAPIC_LVTLINT0		0x0350	//	LVT LINT0 Register Read/Write.
#define LAPIC_LVTLINT1		0x0360	//	LVT LINT1 Register Read/Write.
#define LAPIC_LVTERROR		0x0370	//	LVT Error Register Read/Write.
#define LAPIC_ICR			0x0380	//	Initial Count Register (for Timer) Read/Write.
#define LAPIC_CCR			0x0390	//	Current Count Register (for Timer) Read Only.
#define LAPIC_DCR			0x03E0	//	Divide Configuration Register (for Timer) Read/Write.


// Local LAPIC ID Register
typedef struct {
	union {
		struct {
			uint_t _r0:24;
			uint_t id:4;
			uint_t _r1:4;
		} pp6;				//Format on: P6 family and Pentium processors
		struct {
			uint_t _r0:24;
			uint_t id:8;
		} p4xl;				//Format on: Pentium 4 processors, Xeon processors, and later processors
		struct {
			uint32_t id;
		} xapic;			//Format on: x2APIC Mode
	};
} lapic_idr_t;

// Local LAPIC Version Register
typedef struct {
	uint_t ver:8;		// Version
	uint_t _r0:8;		// Reserved
	uint_t mlvte:8;		// Max LVT Entry
	uint_t seiobs:1;	// Support for EOI-broadcast suppression
	uint_t _r1:7;		// Reserved
} lapic_ver_t;

// Task-Priority Register (TPR)
typedef struct {
	uint_t tps:4;		// Task-Priority Sub-Class
	uint_t tpc:4;		// Task-Priority Class
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
	uint_t pps:4;		// Processor-Priority Sub-Class
	uint_t ppc:4;		// Processor-Priority Class
	uint_t _r0:24;		// Reserved
} lapic_ppr_t;

// End-Of-Interrupt Register (EOI)
typedef struct {
	uint32_t eio;		// End-Of-Interrupt
} lapic_eio_t;

// Remote Read Register (RRD)  Can't find bit field in manual
typedef struct {
	uint32_t rrd;		// ????
} lapic_rrd_t;

//Logical Destination Register (LDR)
typedef struct {
	uint_t _r0:24;		// Reserved
	uint_t id:8;		// Logical LAPIC ID
} lapic_ldr_t;

// Destination Format Register (DFR)
typedef struct {
	uint_t _r0:28;		// Reserved
	uint_t mod:4;		// Model
} lapic_dfr_t;

// Spurious-Interrupt Vector Register (SVR)
typedef struct {
	uint_t sv:8;		// Spurious Vector
	uint_t sed:1;		// LAPIC Software Enable/Disable
	uint_t fpc:3;		// Focus Processor Checking
	uint_t ebs:1;		// EOI-Broadcast Suppression
	uint_t _r0:19;		// Reserved
} lapic_sivr_t;

// In-Service (ISR), Trigger Mode (TMR) and Interrupt Request (IRR) Registers
typedef struct {
	union {
		struct {
			uint_t _r0:16;		// Reserved by LAPIC
			uint_t vec:16;
		} fv;					// For vector from 16 until 31
		struct {
			uint32_t vec;
		} nv;					// For vector from 32 until 255
	};
} lapic_isr_t, lapic_tmr_t, lapic_irr_t;

// Error Status Register (ESR)
typedef struct {
	uint_t sc:1;		// Send Checksum Error
	uint_t rc:1;		// Receive Checksum Error
	uint_t sa:1;		// Send Accept Error
	uint_t ra:1;		// Receive Accept Error
	uint_t ripi:1;		// Redirectable IPI
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
#define LAPIC_LVTTR_VECTOR(v)	(v)
#define LAPIC_LVTTR_MASK			(0b1<<16)
#define LAPIC_LVTTR_ONESHOT		(0b00<<17)
#define LAPIC_LVTTR_PERIODIC		(0b01<<17)
#define LAPIC_LVTTR_TSCDEADLINE	(0b10<<17)


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
	uint_t iipp:1;		// Interrupt Input Pin Polarity
	uint_t rirr:1;		// Remote IRR
	uint_t tm:1;		// Trigger Mode
	uint_t ma:1;		// Mask
	uint_t _r2:15;		// Reserved
} lapic_lvt_lint0_t, lapic_lvt_lint1_t ;
#define LAPIC_LINT_VECTOR(v)	(v)
#define LAPIC_LINT_FIXED		(0b000<<8)
#define LAPIC_LINT_SMI		(0b010<<8)
#define LAPIC_LINT_NMI		(0b100<<8)
#define LAPIC_LINT_EXTINT	(0b111<<8)
#define LAPIC_LINT_INIT		(0b101<<8)
#define LAPIC_LINT_IIPP		(0b1<<13)	// Interrupt Input Pin Polarity
#define LAPIC_LINT_RIRR		(0b1<<14)	// Remote IRR
#define LAPIC_LINT_LEVEL		(0b1<<15)	// Level Trigger Mode otherwise Edge
#define LAPIC_LINT_MASK		(0b1<<16)	// Mask the interrupt

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
	uint_t div:4;		// Bits 0, 1 and 3 the 2nd bit is reserved
	uint_t _r1:28;		// Reserved
} lapic_dcr_t;
#define LAPIC_DCR_DIV1			0b1011
#define LAPIC_DCR_DIV2			0b0000
#define LAPIC_DCR_DIV4			0b0001
#define LAPIC_DCR_DIV8			0b0010
#define LAPIC_DCR_DIV16			0b0011
#define LAPIC_DCR_DIV32			0b1000
#define LAPIC_DCR_DIV64			0b1001
#define LAPIC_DCR_DIV128			0b1010


extern volatile voidptr_t lapic_base;
#define LAPIC_REG(REG)		*((uint32_t*)((uint32_t)lapic_base|REG))
void lapic_init();
void lapic_remap(uint32_t addr);
uint32_t* lapic_get_base();
void lapic_enable();
void lapic_timer_set(uint32_t lvttr_flags, uint32_t dcr_flags, uint32_t icr_flags);
void lapic_eio(uint32_t val);

#endif /* DRIVERS_82489DX_H_ */

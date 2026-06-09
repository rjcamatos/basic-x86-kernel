/*
	82093AA IOAPIC - I/O ADVANCED PROGRAMMABLE INTERRUPT CONTROLLER
 */

#ifndef DRIVERS_82093AA_H_
#define DRIVERS_82093AA_H_

#include "../lib/types.h"

/* IOAPIC Registers */
#define IOAPIC_IOAPICID				0x00
#define IOAPIC_IOAPICVER			0x01
#define IOAPIC_IOAPICARB			0x02
#define IOAPIC_IOREDTBL_L(INDEX)	(0x10 + (INDEX * 2) )
#define IOAPIC_IOREDTBL_H(INDEX)	(0x10 + (INDEX * 2) + 1 )

/* IOREGSEL�I/O REGISTER SELECT REGISTER (used to select the LAPIC register)*/
typedef struct {
	uint_t sel:8;		//0:7 LAPIC Register Address�R/W. Bits [7:0] specify the IOAPIC register to be read/written via the IOWIN Register.
	uint_t _res0:24;	//31:8 Reserved
} ioapic_regsel_t;

/* IOWIN�I/O WINDOW REGISTER (used to read/write an LAPIC register)*/
typedef struct {
	uint32_t data;		//31:0 LAPIC Register Data�R/W. Memory references to this register are mapped to the LAPIC
						// register specified by the contents of the IOREGSEL Register.
} ioapic_win_t;

/* IOAPICID�IOAPIC IDENTIFICATION REGISTER (required for lapic message transmission )*/
typedef struct {
	uint_t _res0:24;	//23:0 Reserved.
	uint_t id:4;		//27:24 IOAPIC Identification�R/W. This 4 bit field contains the IOAPIC identification.
	uint_t _res1:4;		//31:28 Reserved.
} ioapic_id_t;

/* IOAPICVER�IOAPIC VERSION REGISTER */
typedef struct {
	uint_t ver:8;		//7:0 LAPIC VERSION�RO. This 8 bit field identifies the implementation version. The version number assigned to the IOAPIC is 11h.
	uint_t _res0:8;		//15:8 Reserved.
	uint_t mre:8;		//23:16 Maximum Redirection Entry�RO. This field contains the entry number (0 being the lowest
						// entry) of the highest entry in the I/O Redirection Table. The value is equal to the number of
						// interrupt input pins for the IOAPIC minus one. The range of values is 0 through 239. For this
						// IOAPIC, the value is 17h.*/
	uint_t _res1:8;		//31:24 Reserved.
} ioapic_ver_t;

/* IOAPICARB�IOAPIC ARBITRATION REGISTER */
typedef struct {
	uint_t _res0:24;	//23:0 Reserved.
	uint_t id:4;		//27:24 IOAPIC Identification�R/W. This 4 bit field contains the IOAPIC Arbitration ID.
	uint_t _res1:4;		//31:28 Reserved.
}  ioapic_arb_t;

/* IOREDTBL[23:0]�I/O REDIRECTION TABLE REGISTERS */
typedef struct {
	uint_t intvec:8;	//7:0 Interrupt Vector (INTVEC)�R/W:
	uint_t delmod:3;	//10:8 Delivery Mode (DELMOD)�R/W.
	uint_t destmod:1;	//11 Destination Mode (DESTMOD)�R/W.
	uint_t delivs:1;	//12 Delivery Status (DELIVS)�RO.
	uint_t intpol:1;	//13 Interrupt Input Pin Polarity (INTPOL)�R/W.
	uint_t riir:1;		//14 Remote IRR�RO.
	uint_t tm:1;		//15 Trigger Mode�R/W.;
	uint_t mask:1;		//16 Interrupt Mask�R/W.
	uint_t _res0:15;	//55:17 Reserved.
}  ioapic_redtbl_L_t;

typedef struct {
	uint_t _res0:24;	//55:17 Reserved.
	uint_t des:8;		//63:56 Destination Field�R/W.
}  ioapic_redtbl_H_t;


#define ioapic_cast(VAR) *((uint32_t*)&(VAR))
extern uint32_t ioapic_read(uint8_t reg);
extern void ioapic_write(uint8_t reg, uint32_t value);
extern void ioapic_init();


#endif /* DRIVERS_82093AA_H_ */

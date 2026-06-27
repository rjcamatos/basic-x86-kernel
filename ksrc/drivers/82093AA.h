/** COMMENTS By GEMINI */

/**
 * 82093AA IOAPIC - I/O Advanced Programmable Interrupt Controller
 * 
 * Routes hardware IRQs to processors in multi-core systems.
 * Replaces the obsolete 8259A PIC on x86 platforms.
 */

#ifndef KERNEL_DRIVERS_82093AA_H_
#define KERNEL_DRIVERS_82093AA_H_

#include "types.h"

/* The VirtualMachines Address of IOAPIC, this is relocated in Real Systems */
// The attribute unused tels gcc to not Warning that is not in use
static volatile uint32_t __attribute__((unused)) *ioapic_base = (uint32_t*)0xFEC00000;

/* IOAPIC Registers (Index Offsets) */
#define IOAPIC_IOAPICID             0x00    // IOAPIC Identification Register
#define IOAPIC_IOAPICVER            0x01    // IOAPIC Version Register
#define IOAPIC_IOAPICARB            0x02    // IOAPIC Arbitration Register
#define IOAPIC_IOREDTBL_L(INDEX)    (0x10 + ((INDEX) * 2))     // Redirection Table Low Entry (Bits 0-31)
#define IOAPIC_IOREDTBL_H(INDEX)    (0x10 + ((INDEX) * 2) + 1) // Redirection Table High Entry (Bits 32-63)

/* IOREGSEL - I/O Register Select Register */
typedef struct {
    uint32_t sel:8;     // Internal Register Address (R/W)
    uint32_t _res0:24;  // Reserved
} __attribute__((packed)) ioapic_regsel_t;

/* IOWIN - I/O Window Register */
typedef struct {
    uint32_t data;      // Internal Register Data (R/W)
} __attribute__((packed)) ioapic_win_t;

/* IOAPICID - IOAPIC Identification Register */
typedef struct {
    uint32_t _res0:24;  // Reserved
    uint32_t id:4;      // Unique chip ID on APIC bus (R/W)
    uint32_t _res1:4;   // Reserved
} __attribute__((packed)) ioapic_id_t;

/* IOAPICVER - IOAPIC Version Register */
typedef struct {
    uint32_t ver:8;     // IOAPIC Version (RO)
    uint32_t _res0:8;   // Reserved
    uint32_t mre:8;     // Max Redirection Entries (RO)
    uint32_t _res1:8;   // Reserved
} __attribute__((packed)) ioapic_ver_t;

/* IOAPICARB - IOAPIC Arbitration Register */
typedef struct {
    uint32_t _res0:24;  // Reserved
    uint32_t id:4;      // IOAPIC Arbitration ID (RO)
    uint32_t _res1:4;   // Reserved
} __attribute__((packed)) ioapic_arb_t;

/* IOREDTBL[23:0] - Low Part of Redirection Table Registers (Bits 0-31) */
typedef struct {
    uint32_t intvec:8;  // IDT mapped vector (R/W)
    uint32_t delmod:3;  // Delivery Mode (R/W)
    uint32_t destmod:1; // Destination Mode (0 = Physical, 1 = Logical) (R/W)
    uint32_t delivs:1;  // Delivery Status (0 = Idle, 1 = Pending) (RO)
    uint32_t intpol:1;  // Pin Polarity (0 = High, 1 = Low) (R/W)
    uint32_t riir:1;    // Remote IRR for level interrupts (RO)
    uint32_t tm:1;      // Trigger Mode (0 = Edge, 1 = Level) (R/W)
    uint32_t mask:1;    // Interrupt Mask (0 = Active, 1 = Masked) (R/W)
    uint32_t _res0:15;  // Reserved
} __attribute__((packed)) ioapic_redtbl_L_t;

/* IOREDTBL[23:0] - High Part of Redirection Table Registers (Bits 32-63) */
typedef struct {
    uint32_t _res0:24;  // Reserved
    uint32_t des:8;     // Target Local APIC ID or mask (R/W)
} __attribute__((packed)) ioapic_redtbl_H_t;

// Safely casts driver structures to 32-bit integers
#define ioapic_cast(VAR) (*((uint32_t*)&(VAR)))

/* Function Prototypes */

// Reads a 32-bit internal IOAPIC register
#define ioapic_read(BASE,REG) (*(BASE) = (REG), *(BASE + 4))

// Writes a 32-bit value to an internal IOAPIC register
#define ioapic_write(BASE, REG, VALUE) (*(BASE) = (uint32_t)(REG), *(BASE + 4) = (VALUE))

// Initializes the IOAPIC subsystem
extern void ioapic_init(void);

#endif /* DRIVERS_82093AA_H_ */

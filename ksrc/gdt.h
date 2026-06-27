/*
 * gdt.h
 *
 *  Created on: 04/06/2014
 *      Author: myself
 */

/** COMMENTS By GEMINI */

/**
 * GDT - Global Descriptor Table
 * 
 * Configures the x86 32-bit segmentation mechanism to divide RAM,
 * enforce privilege levels (DPL), and define memory access rules.
 */

#ifndef KERNEL_GDT_H_
#define KERNEL_GDT_H_

#include "mem.h"

#define GDT_LIMIT 10    // Max entries

// Data Segment Types (Bits 8-11)
#define GDT_TYPE_DATA_RO    (0<<8)  // Read-Only
#define GDT_TYPE_DATA_ROA   (1<<8)  // Read-Only, Accessed
#define GDT_TYPE_DATA_RW    (2<<8)  // Read/Write
#define GDT_TYPE_DATA_RWA   (3<<8)  // Read/Write, Accessed
#define GDT_TYPE_DATA_ROD   (4<<8)  // Read-Only, Expand-Down
#define GDT_TYPE_DATA_RODA  (5<<8)  // Read-Only, Expand-Down, Accessed
#define GDT_TYPE_DATA_RWD   (6<<8)  // Read/Write, Expand-Down
#define GDT_TYPE_DATA_RWDA  (7<<8)  // Read/Write, Expand-Down, Accessed

// Code Segment Types (Bits 8-11)
#define GDT_TYPE_CODE_EO    (8<<8)  // Execute-Only
#define GDT_TYPE_CODE_EOA   (9<<8)  // Execute-Only, Accessed
#define GDT_TYPE_CODE_ER    (10<<8) // Execute/Read
#define GDT_TYPE_CODE_ERA   (11<<8) // Execute/Read, Accessed
#define GDT_TYPE_CODE_EC    (12<<8) // Execute-Only, Conforming
#define GDT_TYPE_CODE_EOCA  (13<<8) // Execute-Only, Conforming, Accessed
#define GDT_TYPE_CODE_ERC   (14<<8) // Execute/Read, Conforming
#define GDT_TYPE_CODE_ERCA  (15<<8) // Execute/Read, Conforming, Accessed

// Flags
#define GDT_FLAG_S          (1<<12) // 1 = Code/Data, 0 = System
#define GDT_FLAG_DPL0       (0<<13) // Ring 0: Kernel
#define GDT_FLAG_DPL1       (1<<13) // Ring 1: Drivers
#define GDT_FLAG_DPL2       (2<<13) // Ring 2: Drivers
#define GDT_FLAG_DPL3       (3<<13) // Ring 3: User
#define GDT_FLAG_P          (1<<15) // Present bit
#define GDT_FLAG_DB         (1<<22) // 0 = 16-bit, 1 = 32-bit
#define GDT_FLAG_G          (1<<23) // Granularity: 0 = 1B, 1 = 4KiB

// GDT Entry (8 bytes)
typedef struct {
    uint16_t limit_low;             // Limit 0-15
    uint16_t base_low;              // Base 0-15
    uint8_t base_middle;            // Base 16-23
    
    // Access Byte
    uint8_t ac:1;                   // Accessed
    uint8_t rw:1;                   // Read/Write
    uint8_t dc:1;                   // Conforming/Direction
    uint8_t ex:1;                   // Executable
    uint8_t _r1:1;                  // S bit
    uint8_t privilege:2;            // DPL
    uint8_t present:1;              // Present

    // Flags and Upper Limit
    uint8_t limit_high:4;           // Limit 16-19
    uint8_t _r2:1;                  // AVL
    uint8_t _r3:1;                  // Long mode
    uint8_t size:1;                 // D/B
    uint8_t granularity:1;          // G bit

    uint8_t base_high;              // Base 24-31
} __attribute__((packed)) gdt_entry_t;

// GDT Pointer for LGDT (6 bytes)
typedef struct {
    uint16_t limit;                 // Size - 1
    uint32_t base;                  // Base address
} __attribute__((packed)) gdt_lgdt_t;

// Segment Selector (16 bits)
typedef struct {
    uint16_t privilege:2;           // RPL
    uint16_t table:1;               // TI
    uint16_t index:13;              // Index
} __attribute__((packed)) gdt_segment_t;

// Prototypes

// Initializes the 32-bit GDT with default flat segments
extern void gdt_init(void);

// Loads the GDTR register using native 'lgdt' instruction
extern void gdt_set_table(gdt_lgdt_t *lgdt);

// Configures and packs the values of a specific GDT entry
extern void gdt_set_gate(gdt_entry_t *entry, uint32_t limit, uint32_t base, uint32_t type, uint32_t flags);

// Reloads all CPU segment registers via an assembly far jump
extern void gdt_flush(gdt_segment_t cs, gdt_segment_t ss, gdt_segment_t ds, gdt_segment_t es, gdt_segment_t fs, gdt_segment_t gs);

// Returns the 32-bit base address from a segment selector
extern uint32_t gdt_get_segment_vma(gdt_segment_t s);

// Returns the 32-bit base address of the active data segment
extern uint32_t gdt_get_current_segment_vma(void);

#endif /* GDT_H_ */

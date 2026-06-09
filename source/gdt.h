/*
 * Global Descriptor Table
 * */

#include "lib/mem.h"

#ifndef GDT_H_
#define GDT_H_

#define GDT_LIMIT 10	//Global Descriptor table max limit is: 65535/8=8191

#define GDT_TYPE_DATA_RO	(0<<8)	//Read-Only
#define GDT_TYPE_DATA_ROA	(1<<8)	//Data Read-Only, accessed
#define GDT_TYPE_DATA_RW	(2<<8)	//Data Read/Write
#define GDT_TYPE_DATA_RWA	(3<<8)	//Data Read/Write, accessed
#define GDT_TYPE_DATA_ROD	(4<<8)	//Data Read-Only, expand-down
#define GDT_TYPE_DATA_RODA	(5<<8)	//Data Read-Only, expand-down, accessed
#define GDT_TYPE_DATA_RWD	(6<<8)	//Data Read/Write, expand-down
#define GDT_TYPE_DATA_RWDA	(7<<8)	//Data Read/Write, expand-down, accessed
#define GDT_TYPE_CODE_EO	(8<<8)	//Code Execute-Only
#define GDT_TYPE_CODE_EOA	(9<<8)	//Code Execute-Only, accessed
#define GDT_TYPE_CODE_ER	(10<<8)	//Code Execute/Read
#define GDT_TYPE_CODE_ERA	(11<<8)	//Code Execute/Read, accessed
#define GDT_TYPE_CODE_EC	(12<<8)	//Code Execute-Only, conforming
#define GDT_TYPE_CODE_EOCA	(13<<8)	//Code Execute-Only, conforming, accessed
#define GDT_TYPE_CODE_ERC	(14<<8)	//Code Execute/Read, conforming
#define GDT_TYPE_CODE_ERCA	(15<<8)	//Code Execute/Read, conforming, accessed

#define GDT_FLAG_S			(1<<12) //Segment is code or data, if not set this flag is system
#define GDT_FLAG_DPL0		(0<<13)	//Privilege level 0 - MOST (this is the default)
#define GDT_FLAG_DPL1		(1<<13) //Privilege level 1
#define GDT_FLAG_DPL2		(2<<13) //Privilege level 2
#define GDT_FLAG_DPL3		(3<<13) //Privilege level 3 - LESS
#define GDT_FLAG_P			(1<<15)	//Segment is present
#define GDT_FLAG_L			(1<<21) //For use in IA32E only (x64)
#define GDT_FLAG_DB			(1<<22) //Warning, always use this flag, read the manual
#define GDT_FLAG_G			(1<<23) //Use 4Kb granularity

typedef struct {
	uint32_t limit_low:16;			// Low bits of segment size limit

	uint32_t base_low:16;			// Low bits of segment base address

	uint32_t base_middle:8;			// Middle bits of segment base address
	uint32_t ac:1;					// The CPU sets this to 1 when the segment is accessed. (Just set to 0)
	uint32_t rw:1;					// Readable bit for code selectors, Writable bit for data selectors
	uint32_t dc:1;					// Direction bit/Conforming bit.
	uint32_t ex:1;					// Executable bit. If 1 code in this segment can be executed, ie. a code selector. If 0 it is a data selector
	uint32_t _r1:1;					// reserved, always set to 1
	uint32_t privilege:2;			// Privilege, Contains the ring level, 0 = highest (kernel), 3 = lowest (user applications).
	uint32_t present:1;				// This must be 1 for all valid selectors.

	uint32_t limit_high:4;		 	// High bits of segment size limit
	uint32_t _r2:1;					// reserved
	uint32_t _r3:1;					// reserved
	uint32_t size:1;				// If 0 the selector defines 16 bit protected mode. If 1 it defines 32 bit protected mode. You can have both 16 bit and 32 bit selectors at once
	uint32_t granularity:1;			// If 0 the limit is in 1 B blocks (byte granularity), if 1 the limit is in 4 KiB blocks (page granularity)

	uint32_t base_high:8;			// High bits of segment base address
} __attribute__((packed)) gdt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_lgdt_t;

typedef struct {
	uint32_t privilege:2;
	uint32_t table:1;
	uint32_t index:13;
} __attribute__((packed)) gdt_segment_t;

void gdt_init();
void gdt_set_table(gdt_lgdt_t *lgdt);
void gdt_set_gate(gdt_entry_t *entry, uint32_t limit, uint32_t base, uint32_t type, uint32_t flags);
void gdt_flush(gdt_segment_t cs, gdt_segment_t ss, gdt_segment_t ds, gdt_segment_t es, gdt_segment_t fs, gdt_segment_t gs);
uint32_t gdt_get_segment_vma(gdt_segment_t s);

#endif /* GDT_H_ */

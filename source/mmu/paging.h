/*
 * paging.h
 *
 *  Created on: 18/05/2014
 *      Author: myself
 */

#ifndef PAGING_H_
#define PAGING_H_

#include "../lib/types.h"

/* Use of CR3 with 32-Bit Paging */
typedef struct  __attribute__((packed)) {
	uint_t _i0:3;
	uint_t page_level_write_through:1;
	uint_t page_level_cache_disable:1;
	uint_t _i1:7;
	uint_t physical_address:20;
} paging_cr3_t;

/* Format of a 32-Bit Page-Directory Entry */
typedef union __attribute__((packed)) {

	/* Format of a 32-Bit Page-Directory Entry that Maps a 4-MByte Page */
	struct {
		uint_t present:1;
		uint_t read_write:1;
		uint_t supervisor:1;
		uint_t page_level_write_through:1;
		uint_t page_level_cache_disable:1;
		uint_t accessed:1;
		uint_t dirty:1;
		uint_t page_size:1;
		uint_t global:1;
		uint_t _i0:3;
		uint_t pat:1;
		uint_t m:8;
		uint_t _r0:1;
		uint_t physical_address:10;
	} map_4mb; // structure when page_size = 1

	/* Format of a 32-Bit Page-Directory Entry that References a Page Table */
	struct {
		uint_t present:1;
		uint_t read_write:1;
		uint_t supervisor:1;
		uint_t page_level_write_through:1;
		uint_t page_level_cache_disable:1;
		uint_t accessed:1;
		uint_t _i0:1;
		uint_t page_size:1;
		uint_t _i1:4;
		uint_t physical_address:20;
	} reference_table; // structure when page_size = 0

} paging_page_directory_entry_t;

typedef paging_page_directory_entry_t __attribute__((aligned(4096))) paging_page_directory_t[1024];

#define PAGING_PDE_PRESENT		(1<<0)
#define PAGING_PDE_READWRITE	(1<<1)
#define PAGING_PDE_SUPERVISOR	(1<<2)
#define PAGING_PDE_4MB			(1<<7)

/* Format of a 32-Bit Page-Table Entry that Maps a 4-KByte Page */
typedef struct __attribute__((packed)) {
	uint_t present:1;
	uint_t read_write:1;
	uint_t supervisor:1;
	uint_t page_level_write_through:1;
	uint_t page_level_cache_disable:1;
	uint_t accessed:1;
	uint_t dirty:1;
	uint_t pat:1;
	uint_t global:1;
	uint_t _i0:3;
	uint_t physical_address:20;
} paging_page_table_entry_t;

typedef paging_page_table_entry_t __attribute__((aligned(4096))) paging_page_table_t[1024];
typedef paging_page_table_t paging_page_tables_t[1024];

/*
 * Sets a page directory to CR3 register
 * */
extern inline void paging_set_page_directory(paging_page_directory_t *pd);

/*
 * Returns the current page directory at CR3 register
 * */
extern inline paging_page_directory_t* paging_get_page_directory();

/*
 * Maps 4MB of physical memory at [phyaddr] to a given linear address [linaddr]
 * */
extern inline void paging_map_4mb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags);

/*
 * Maps 4KB of physical memory at [phyaddr] to a given linear address [linaddr]
 * */
extern inline void paging_map_4kb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags);

/*
 *  Returns the physical memory address of a give linear address [linaddr]
 * */
extern inline uint32_t paging_get_phyaddr(uint32_t linaddr);

/*
 * Initialize the 32 bit paging from LMA (load memory address) returning to VMA
 * */
extern void paging_init_from_lma();

#endif /* PAGING_H_ */

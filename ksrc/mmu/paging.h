/*
 * paging.h
 *
 *  Created on: 18/05/2014
 *      Author: myself
 */

#ifndef KERNEL_PAGING_H_
#define KERNEL_PAGING_H_

#include "types.h"

#include "_config.h"

/* Use of CR3 with 32-Bit Paging */
typedef struct {
    uint32_t _i0:3;
    uint32_t page_level_write_through:1;
    uint32_t page_level_cache_disable:1;
    uint32_t _i1:7;
    uint32_t physical_address:20;
} paging_cr3_t; // Removido packed (bitfields de 32 bits alinham naturalmente)

/* Format of a 32-Bit Page-Directory Entry */
typedef union {

    /* Format of a 32-Bit Page-Directory Entry that Maps a 4-MByte Page */
    struct {
        uint32_t present:1;
        uint32_t read_write:1;
        uint32_t supervisor:1;
        uint32_t page_level_write_through:1;
        uint32_t page_level_cache_disable:1;
        uint32_t accessed:1;
        uint32_t dirty:1;
        uint32_t page_size:1;
        uint32_t global:1;
        uint32_t _i0:3;
        uint32_t pat:1;
        uint32_t m:8;
        uint32_t _r0:1;
        uint32_t physical_address:10;
    } __attribute__((packed)) map_4mb; // Mantido packed aqui para os bitfields internos

    /* Format of a 32-Bit Page-Directory Entry that References a Page Table */
    struct {
        uint32_t present:1;
        uint32_t read_write:1;
        uint32_t supervisor:1;
        uint32_t page_level_write_through:1;
        uint32_t page_level_cache_disable:1;
        uint32_t accessed:1;
        uint32_t _i0:1;
        uint32_t page_size:1;
        uint32_t _i1:4;
        uint32_t physical_address:20;
    } __attribute__((packed)) reference_table; // Mantido packed aqui

} paging_page_directory_entry_t; // REMOVIDO o packed global da união

/* Definição correta do Array de 1024 entradas alinhado a 4KB */
typedef paging_page_directory_entry_t __attribute__((aligned(4096))) paging_page_directory_t[1024];

// Page Directory Entry
#define PAGING_PDE_PRESENT          0x001
#define PAGING_PDE_READWRITE        0x002
#define PAGING_PDE_USER            0x004
#define PAGING_PDE_SUPERVISOR      0x000
#define PAGING_PDE_WRITE_THROUGH    0x008
#define PAGING_PDE_CACHE_DISABLE    0x010
#define PAGING_PDE_ACCESSED         0x020
#define PAGING_PDE_SIZE            0x080
#define PAGING_PDE_GLOBAL           0x100
#define PAGING_PDE_AVAILABLE_MASK   0xE00
//
#define PAGING_PDE_TABLE_MASK       0xFFFFF000
#define PAGING_PDE_4MB_FRAME_MASK   0xFFC00000
#define PAGING_PDE_FLAGS_MASK       0x00000FFF

//Page Table Entry 
#define PAGING_PTE_PRESENT          0x001 
#define PAGING_PTE_READWRITE        0x002 
#define PAGING_PTE_USER            0x004 
#define PAGING_PTE_SUPERVISOR      0x000 
#define PAGING_PTE_WRITE_THROUGH    0x008 
#define PAGING_PTE_CACHE_DISABLE    0x010 
#define PAGING_PTE_ACCESSED         0x020 
#define PAGING_PTE_DIRTY            0x040 
#define PAGING_PTE_PAT              0x080 
#define PAGING_PTE_GLOBAL           0x100 
#define PAGING_PTE_AVAILABLE_MASK   0xE00 
//
#define PAGING_PTE_FRAME_MASK       0xFFFFF000
#define PAGING_PTE_FLAGS_MASK       0x00000FFF

/* Format of a 32-Bit Page-Table Entry that Maps a 4-KByte Page */
typedef struct {
    uint32_t present:1;
    uint32_t read_write:1;
    uint32_t supervisor:1;
    uint32_t page_level_write_through:1;
    uint32_t page_level_cache_disable:1;
    uint32_t accessed:1;
    uint32_t dirty:1;
    uint32_t pat:1;
    uint32_t global:1;
    uint32_t _i0:3;
    uint32_t physical_address:20;
} paging_page_table_entry_t; // Removido packed (alinha naturalmente a 4 bytes)


/* Cada Page Table é uma array de 1024 entradas, alinhada em 4KB */
typedef paging_page_table_entry_t __attribute__((aligned(4096))) paging_page_table_t[1024];

/* Um conjunto que pode conter até 1024 Page Tables */
/* For full memory 4KB maping, the array should be 1024 */
/* Limit to 10, 10*4KB = 40KB */
typedef paging_page_table_t paging_page_tables_t[KERNEL_CONFIG_PAGING_PAGE_TABLES];


/*
 * Sets a page directory to CR3 register
 * */
static inline void paging_set_page_directory(paging_page_directory_t *pd)
{
	__asm__ __volatile__("mov %0,%%cr3"::"r"(pd):"memory");
}


/*
 * Returns the current page directory at CR3 register
 * */
static inline paging_page_directory_t* paging_get_page_directory()
{
	//Note: inline function only works in optimizing compilation
	voidptr_t rtv = NULL;
	__asm__ __volatile__("mov %%cr3,%0":"=r"(rtv)::"memory");

	return rtv;
}

/**
 * Flush the paging Page Table Entry
 */
static inline void paging_flush_ptbl_entry(uint32_t linaddr)
{
    asm volatile("invlpg (%0)" :: "r"(linaddr) : "memory");
}

/* 
 * Returns an unreferenced page table
 * */
extern paging_page_table_t* paging_get_page_table(void);

/*
 * Maps 4MB of physical memory at [phyaddr] to a given linear address [linaddr]
 * */
extern void paging_map_4mb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags);

/*
 * Maps 4KB of physical memory at [phyaddr] to a given linear address [linaddr]
 * */
extern void paging_map_4kb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags);

/*
 *  Returns the physical memory address of a give linear address [linaddr]
 * */
extern uint32_t paging_get_phyaddr(uint32_t linaddr);

/*
 * Initialize the 32 bit paging VMA ( virtual memory address)
 * */
extern void paging_init(void);


#endif /* PAGING_H_ */

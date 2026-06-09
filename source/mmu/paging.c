#include "paging.h"
#include "../lib/mem.h"
#include "../lib/printk.h"
#include "../multiboot/multiboot.h"

static paging_page_directory_t _pgdir;	//the kernel page directory

static paging_page_tables_t _pgtbls;	//the page tables

//Notes:
//	inline function only works in optimizing compilation
//	static function is only visible inside current .c file
//	global static variable is only visible inside current .c file


//the unreferenced page tables
typedef voidptr_t paging_page_tables_unref_t[1024];
static paging_page_tables_unref_t _pgtbls_unref;

inline void paging_set_page_directory(paging_page_directory_t *pd)
{

	__asm__ __volatile__("mov %0,%%cr3"::"r"(pd):);
}

inline paging_page_directory_t* paging_get_page_directory()
{
	//Note: inline function only works in optimizing compilation
	voidptr_t pd = NULL;
	__asm__ __volatile__("mov %%cr3,%0":"=r"(pd)::);
	return pd;
}

/* returns an unreferenced page table */
static inline paging_page_table_t* paging_get_page_table()
{
	paging_page_tables_unref_t *pgtbls_unref = (paging_page_tables_unref_t*)((uint_t)_pgtbls_unref - (uint_t)KERNEL_VMA);
	voidptr_t retval = NULL;
	uint_t idx = 0;
	for( ;idx < sizeof(paging_page_tables_t) / sizeof(paging_page_table_t); idx++ )
	{
		if( (*pgtbls_unref)[idx] )
		{
			retval = (*pgtbls_unref)[idx];
			(*pgtbls_unref)[idx] = NULL;
			break;
		}
	}
	return retval;
}

inline void paging_map_4mb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags)
{
	paging_page_directory_t *pgdir = paging_get_page_directory();
	*(uint32_t*)&(*pgdir)[linaddr>>22] = (phyaddr & 0xFFC00000)|PAGING_PDE_4MB|flags;
}

inline void paging_map_4kb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags)
{
	paging_page_directory_t *pgdir = paging_get_page_directory();
	paging_page_table_t *pgtbl = paging_get_page_table();
	*(uint32_t*)&(*pgdir)[linaddr>>22] = (uint32_t)(*pgtbl)|PAGING_PDE_PRESENT;
	*(uint32_t*)&(*pgtbl)[(linaddr>>12)&0x3FF] = (phyaddr & 0xFFFFF000)|flags;
}

inline uint32_t paging_get_phyaddr(uint32_t linaddr)
{
	paging_page_directory_t *pgdir = paging_get_page_directory();
	uint32_t entry = *(uint32_t*)&(*pgdir)[linaddr>>22];

	if( entry & PAGING_PDE_4MB )
	{
		return (entry & 0xFFC00000);
	} else {
		entry = *(uint32_t*)(entry & 0xFFFFF000);
		return (entry & 0xFFFFF000);
	}
}

void paging_init_from_lma()
{
	paging_page_directory_t *pgdir = (paging_page_directory_t*)((uint_t)_pgdir - (uint_t)KERNEL_VMA);
	paging_page_tables_t *pgtbls = (paging_page_tables_t*)((uint_t)_pgtbls - (uint_t)KERNEL_VMA);
	paging_page_tables_unref_t *pgtbls_unref = (paging_page_tables_unref_t*)((uint_t)_pgtbls_unref - (uint_t)KERNEL_VMA);

	memset(*pgdir,0x00,sizeof(paging_page_directory_t));
	memset(*pgtbls,0x00,sizeof(paging_page_tables_t));

	uint_t idx = 0;
	for( ;idx < sizeof(paging_page_tables_t) / sizeof(paging_page_table_t); idx++ )
	{
		(*pgtbls_unref)[idx] = (*pgtbls)[idx];
	}

	paging_set_page_directory(pgdir);

	uint_t kernel_end = 0x400000 * 4; // Map the first 16MB of memory
	uint_t addr = 0;
	while( addr <= kernel_end )
	{
		// 1:1 kernel mapping
		paging_map_4mb(addr,addr,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);

		// high memory kernel mapping
		paging_map_4mb(addr,((uint32_t)KERNEL_VMA|addr),PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);

		addr += 0x400000;	//Increment 4MB
	}

	// IOAPIC mapping SIZE IS 4KB (1:1 mapping) (AT 0xFEC00000)
	// 	 AND
	// LAPIC mapping SIZE IS 4KB (1:1 mapping) (AT 0xFEE00000)
	paging_map_4mb(0xFEC00000,0xFEC00000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);

	// ACPI mapping SIZE IS 4KB (1:1 mapping)
	paging_map_4mb(0x7FE0000,0x7FE0000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
		//this bellongs to acpi to
	paging_map_4mb(0x5ffe1853,0x5ffe1853,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);


	//enable 4Mbi pages
	__asm__ __volatile__("mov %cr4,%eax");
	__asm__ __volatile__("or $0x00000010,%eax");
	__asm__ __volatile__("mov %eax,%cr4");

	//enable paging
	__asm__ __volatile__("mov %cr0,%eax");
	__asm__ __volatile__("or $0x80000000,%eax");
	__asm__ __volatile__("mov %eax,%cr0");

	//Recursively walk the stack
	uint32_t *ebp = NULL;
	__asm__ __volatile__("add $%0,%%ebp"::"m"(KERNEL_VMA):);
	__asm__ __volatile__("add $%0,%%esp"::"m"(KERNEL_VMA):);
	__asm__ __volatile__("mov %%ebp,%0":"=m"(ebp)::);
	while( *ebp ) //While there is a "Previous Stack Frame"
	{
		*(ebp) += (uint32_t)KERNEL_VMA;		// add kernel VMA to "Saved %ebp"
		*(ebp+1) += (uint32_t)KERNEL_VMA;	// add kernel VMA to "Return Address"
		ebp = (uint32_t*)*ebp;				// move to "Previous Stack Frame"
	}

	/* SUPRESSED
	//RECURSIVE WALK THE STACK, AND SET RET TO KERNEL VIRTUAL MEMEORY ADDRESS
	__asm__ __volatile__("mov $%0,%%ecx"::"m"(KERNEL_VMA):); //THIS IS THE VMA
	__asm__ __volatile__("add %ecx, %ebp");
	__asm__ __volatile__("add %ecx, %esp");
	//back trace the stack and fix it
	__asm__ __volatile__("mov %ebp, %eax");
	__asm__ __volatile__("__loop:");
		//fix ebp+0 (address of previous stack frame)
		__asm__ __volatile__("add %ecx, (%eax)");
		//fix ebp+4 (eip is here, ret opcode use this to return from functions)
		__asm__ __volatile__("add %ecx, 4(%eax)");
		//got to previous stack frame
		__asm__ __volatile__("mov (%eax), %eax");
	//if this is the last stack frame end the loop
	__asm__ __volatile__("cmp $0, (%eax)");
	__asm__ __volatile__("jne __loop");
	//END FIX THE STACK
	 */

	//DOWN HERE FOR TESTING

	/*
	//map 20MB to 20MB with 4KB size
	paging_map_4kb(0x1400000,0x1400000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE);
	//map 20MB to 24MB with 4KB size
	paging_map_4kb(0x1400000,0x1800000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE);

	//map 40MB to 40MB with 4KB size
	paging_map_4kb(0x2800000,0x2800000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE);
	//map 40MB to 48MB with 4KB size
	paging_map_4kb(0x2800000,0x3000000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE);

	uint_t xxx = paging_get_phyaddr(0x1800000);
	uint_t yyyy = paging_get_phyaddr(0x3000000);
	uint_t zzz = paging_get_phyaddr(0xFEC00000);*/
}

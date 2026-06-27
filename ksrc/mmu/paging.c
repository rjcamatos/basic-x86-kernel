#include "paging.h"
#include "mem.h"
#include "printk.h"

static paging_page_directory_t _pgdir;	//The Page Directory
static paging_page_tables_t _pgtbls;	//The Page Tables

void paging_set_page_directory(paging_page_directory_t *pd)
{
	__asm__ __volatile__("mov %0,%%cr3"::"r"(pd):);
}

paging_page_directory_t* paging_get_page_directory()
{
	//Note: inline function only works in optimizing compilation
	voidptr_t rtv = NULL;
	__asm__ __volatile__("mov %%cr3,%0":"=r"(rtv)::);
	return rtv;
}

paging_page_table_t* paging_get_page_table()
{
    uint32_t idx = 0;
    uint32_t entry_idx = 0;
    uint32_t num_tables = sizeof(paging_page_tables_t) / sizeof(paging_page_table_t);

	printk("    number of kernel page tables: %d\n",num_tables);
    for( ; idx < num_tables; idx++ ) {
        uint32_t used = 0;
        for(entry_idx = 0; entry_idx < 1024; entry_idx++) {
            if (_pgtbls[idx][entry_idx].present != 0) {
                used = 1;
                break;
            }
        }
        if (!used) {
            // Retorna o endereço físico da tabela limpa
            paging_page_tables_t *pgtbls_phys = (paging_page_tables_t*)((uint_t)_pgtbls-KERNEL_CONFIG_VMA);
            return &(*pgtbls_phys)[idx];
        }
    }
    return NULL;
}

void paging_map_4mb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags)
{
	paging_page_directory_t *pgdir = (paging_page_directory_t*)((uint_t)_pgdir);
	*(uint32_t*)&(*pgdir)[linaddr>>22] = (phyaddr & 0xFFC00000)|PAGING_PDE_SIZE|flags;
}

void paging_map_4kb(uint32_t phyaddr, uint32_t linaddr, uint32_t flags)
{
    paging_page_directory_t *pgdir = (paging_page_directory_t*)((uint_t)_pgdir);
    uint32_t pd_index = linaddr >> 22;
    uint32_t current_pde = *(uint32_t*)&(*pgdir)[pd_index];
    paging_page_table_t *pgtbl = NULL;

    // Se o PDE não está presente, aloca uma nova Page Table
    if (!(current_pde & PAGING_PDE_PRESENT)) {
        pgtbl = paging_get_page_table();
        uint32_t pgtbl_physical = (uint32_t)pgtbl; 
        *(uint32_t*)&(*pgdir)[pd_index] = (pgtbl_physical & 0xFFFFF000) | PAGING_PDE_PRESENT | PAGING_PDE_READWRITE | PAGING_PDE_SUPERVISOR;
    } else {
        // Se já existe, reutiliza a Page Table existente obtendo o endereço físico dela do PDE
        pgtbl = (paging_page_table_t*)(current_pde & 0xFFFFF000);
    }

    uint32_t pt_index = (linaddr >> 12) & 0x3FF;
    *(uint32_t*)&(*pgtbl)[pt_index] = (phyaddr & 0xFFFFF000) | flags;
}

uint32_t paging_get_phyaddr(uint32_t linaddr)
{
	paging_page_directory_t *pgdir = paging_get_page_directory();
	uint32_t entry = *(uint32_t*)&(*pgdir)[linaddr>>22];

	if( !(entry & PAGING_PDE_PRESENT) ) return 0;

	if( entry & PAGING_PDE_SIZE ) {
        return (entry&0xFFC00000)|(linaddr&0x003FFFFF);
	} else { //IS 4KB Pages !
        uint32_t *pgtbl = (uint32_t*)(entry & 0xFFFFF000);
        uint32_t pt_index = (linaddr >> 12) & 0x3FF;
        uint32_t pte = pgtbl[pt_index];
        if( !(pte&PAGING_PTE_PRESENT) ) return 0;
        return (pte&0xFFFFF000)|(linaddr&0x00000FFF);
    }
	return 0;
}

void paging_init()
{
	printk("Initializing PAGING !\n");

	paging_page_directory_t *pgdir = (paging_page_directory_t*)((uint_t)_pgdir);
	paging_page_tables_t *pgtbls = (paging_page_tables_t*)((uint_t)_pgtbls);

	// For safe zero Page Directory and Page Tables
	memset(*pgdir,0x00,sizeof(paging_page_directory_t));
	memset(*pgtbls,0x00,sizeof(paging_page_tables_t));
	
	// Dynamically map Kenel Memory
	extern uint_t __kernel_physical_end;
	uint_t map_count = 0;
	uint_t map_addr = 0x00000000;
	while( map_addr <= (uint_t)&__kernel_physical_end) {
		printk("    mapping physadd: 0X%x, to: virtaddr 0X%x !\n",map_addr,KERNEL_CONFIG_VMA|map_addr);
		// One o One Mapping (1:1) for Kernel Memory
		//paging_map_4mb(map_addr,map_addr,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
		//High Half Mapping for Kernel Memory
		paging_map_4mb(map_addr,(KERNEL_CONFIG_VMA+map_addr),PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
		map_addr += 0x00400000;
		map_count += 4;
	}
	printk("    Kernel reserved dymamiclly: %dMiB !\n",map_count);

	// IOAPIC mapping SIZE IS 4KB (1:1 mapping) (AT 0xFEC00000)
	// AND LAPIC mapping SIZE IS 4KB (1:1 mapping) (AT 0xFEE00000)
	uint32_t apic_flags = PAGING_PTE_PRESENT|PAGING_PTE_READWRITE|PAGING_PTE_SUPERVISOR|PAGING_PTE_CACHE_DISABLE|PAGING_PTE_WRITE_THROUGH;
	paging_map_4kb(0xFEC00000,0xFEC00000,apic_flags);
	paging_map_4kb(0xFEE00000,0xFEE00000,apic_flags);
	

	// ACPI mapping SIZE IS 4KB (1:1 mapping)
	//paging_map_4mb(0x7FE0000,0x7FE0000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
	//this bellongs to acpi to
	//paging_map_4mb(0x5ffe1853,0x5ffe1853,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);


	paging_set_page_directory((paging_page_directory_t*)((uint_t)_pgdir-KERNEL_CONFIG_VMA));

	//enable 4MB pages
	__asm__ __volatile__("mov %%cr4, %%eax\n\t"
						"or $0x00000010, %%eax\n\t"
						"mov %%eax, %%cr4"
						: : : "eax");

	//enable paging
	__asm__ __volatile__("mov %%cr0, %%eax\n\t"
						"or $0x80000000, %%eax\n\t"
						"mov %%eax, %%cr0"
						: : : "eax");

}


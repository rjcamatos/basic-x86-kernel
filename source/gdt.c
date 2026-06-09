/*
 * we are going to use Basic Flat Model,
 * the GDT maps the full memory, and later we use paging to
 * implement protection.
 *
 * Reference: Intel Programing Guide, Volume 3 - 3.2.1, page 3-3
 * */


#include "gdt.h"
#include "lib/printk.h"

gdt_entry_t gdt[GDT_LIMIT];
gdt_lgdt_t  gdtl;

void gdt_init()
{
	printk("Initializing GDT !\n");

	gdt_entry_t *gdt_p = gdt;
	gdt_lgdt_t *gdtl_p = &gdtl;

	memset(gdt_p,0x00,sizeof(gdt));

	//entry 0 is the mandatory null descriptor
	gdt_set_gate(gdt_p+1,0xFFFFF,0,GDT_TYPE_CODE_ER,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL0); // the kernel code descriptor
	gdt_set_gate(gdt_p+2,0xFFFFF,0,GDT_TYPE_DATA_RW,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL0); // the kernel data descriptor
	gdt_set_gate(gdt_p+3,0xFFFFF,0,GDT_TYPE_CODE_ER,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL3); // the user code descriptor
	gdt_set_gate(gdt_p+4,0xFFFFF,0,GDT_TYPE_DATA_RW,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL3); // the user data descriptor

	gdtl_p->limit = 8*(GDT_LIMIT);
	gdtl_p->base = (uint32_t)gdt_p;

	gdt_set_table(gdtl_p);

	gdt_segment_t code;
	gdt_segment_t data;
	*(uint16_t*)&code = 0x0008;
	*(uint16_t*)&data = 0x0010;

	gdt_flush(code,data,data,data,data,data);
}

void gdt_set_table(gdt_lgdt_t *lgdt)
{
	__asm__ __volatile__("lgdt (%0)"::"r"(lgdt):);
}

void gdt_set_gate(gdt_entry_t *entry, uint32_t limit, uint32_t base, uint32_t type, uint32_t flags)
{
	*((uint32_t*)entry) = (0xffff&limit)|(base<<16);
	*((uint32_t*)entry+1) = (base>>16&0xff)|type|(base&0xff000000)|(limit&0xf0000)|flags;
}

void gdt_flush(gdt_segment_t cs, gdt_segment_t ss, gdt_segment_t ds, gdt_segment_t es, gdt_segment_t fs, gdt_segment_t gs)
{
	//load new data segments (WARNING do not change the order)
	__asm__ __volatile__("mov %0, %%fs"::"m"(fs):); //General Purpose Segment
	__asm__ __volatile__("mov %0, %%gs"::"m"(gs):); //General Purpose Segments
	__asm__ __volatile__("mov %0, %%es"::"m"(es):); //Extra Segment
	__asm__ __volatile__("mov %0, %%ds"::"m"(ds):); //Data Segment
	__asm__ __volatile__("mov %0, %%ss"::"m"(ss):); //Stack Segment

	//enter new code segment
	__asm__ __volatile__("ljmp $0x08, $gdt_far_jmp_here");
	__asm__ __volatile__("gdt_far_jmp_here:");

	/*
	__asm__ __volatile__(
        "ljmp $0x08, $1f\n\t"
        "1:\n\t"
    );*/
}

uint32_t gdt_get_segment_vma(gdt_segment_t s)
{
	uint32_t index = s.index;
	gdt_lgdt_t *gdtl_p;
	__asm__ __volatile__("sgdt (%0)":"=r"(gdtl_p)::);
	gdt_entry_t *entry = (gdt_entry_t*)gdtl_p->base;
	uint32_t base = (entry[index].base_low)|(entry[index].base_middle<<16)|(entry[index].base_high<<24);
	return ( 0xffffffff - base + 1 );
}




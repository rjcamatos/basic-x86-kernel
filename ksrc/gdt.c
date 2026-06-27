/*
 * we are going to use Basic Flat Model,
 * the GDT maps the full memory, and later we use paging to
 * implement protection.
 *
 * Reference: Intel Programing Guide, Volume 3 - 3.2.1, page 3-3
 * */

#include "_config.h"

#include "gdt.h"
#include "printk.h"

gdt_entry_t gdt[GDT_LIMIT];
gdt_lgdt_t  gdtl;

void gdt_init(void)
{
	printk("Initializing GDT !\n");

	gdt_entry_t *gdt_p = gdt;
	gdt_lgdt_t *gdtl_p = &gdtl;

	memset(gdt_p,0x00,sizeof(gdt));

	//entry 0 is the mandatory null descriptor
	//already set to 0 by memset, so we can skip it

	// If GDT_FLAG_G is set limit *= 4KByte, else limit *= 1Byte
	// limit 0xFFFFF for Flat Memory Model, all the 4GB
		// The Kernel Code descriptor:
		gdt_set_gate(gdt_p+1,0xFFFFF,0,GDT_TYPE_CODE_ER,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL0); 
	
		// The Kernel Data Descriptor:
		gdt_set_gate(gdt_p+2,0xFFFFF,0,GDT_TYPE_DATA_RW,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL0); 
	
	// We don`t have a user code or user data yet, so skip it for now!
		// the user code descriptor:
		//gdt_set_gate(gdt_p+3,0xFFFFF,0,GDT_TYPE_CODE_ER,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL3);
		// the user data descriptor :
		//gdt_set_gate(gdt_p+4,0xFFFFF,0,GDT_TYPE_DATA_RW,GDT_FLAG_S|GDT_FLAG_P|GDT_FLAG_DB|GDT_FLAG_G|GDT_FLAG_DPL3);

	// Define The Global Descriptor Table limit and base
	gdtl_p->limit = 8*(GDT_LIMIT);
	gdtl_p->base = (uint32_t)gdt_p;

	// Set the Global Descriptor Table
	gdt_set_table(gdtl_p);

	// 0x08: Point to 1st Segment of GDT (Usualy Kernel the Code Segment).
	gdt_segment_t code;
	*(uint16_t*)&code = 0x08;

	 // 0x0010: Point to 1st Segment of GDT (Ususlay the Kernel Data Segment).
	gdt_segment_t data;
	*(uint16_t*)&data = 0x10;

	// Initialize and Enter new Code Segment and Data Segment
	gdt_flush(code,data,data,data,data,data);
}

void gdt_set_table(gdt_lgdt_t *lgdt)
{
	__asm__ __volatile__("lgdt (%0)"::"r"(lgdt):);
}

void gdt_set_gate(gdt_entry_t *entry, uint32_t limit, uint32_t base, uint32_t type, uint32_t flags)
{
	*((uint32_t*)entry) = (limit & 0xFFFF) | ((base & 0xFFFF) << 16);
	*((uint32_t*)entry+1) = ((base >> 16) & 0xFF) | type | (((limit >> 16) & 0x0F) << 16) | flags | (base & 0xFF000000);
}


void gdt_flush(gdt_segment_t cs, gdt_segment_t ss, gdt_segment_t ds, gdt_segment_t es, gdt_segment_t fs, gdt_segment_t gs)
{
	/* O objetivo do far jump logo após carregar a GDT não é saltar para um programa
	 ou função diferente. O objetivo é fazer o processador recarregar os seus circuitos 
	 internos com as novas regras de privilégio que acabaste de definir na GDT, sem 
	 interromper o fluxo do código.Na prática, o código faz isto:Carrega a nova tabela 
	 na CPU (instrução lgdt).Executa o far jump para o novo índice de código da GDT, 
	 apontando para a linha imediatamente abaixo.O far jump força o processador a recarregar 
	 o CS (Code Segment) com o novo seletor, que tem as novas permissões e privilégios 
	 definidos na GDT. Depois disso, o código continua a ser executado normalmente, mas 
	 agora com as novas regras de acesso e privilégio em vigor. Portanto, o far jump é 
	 uma etapa necessária para garantir que as mudanças na GDT sejam efetivamente aplicadas 
	 ao código que está sendo executado.
	 */


	// Load new data segments (WARNING do not change the order)
	__asm__ __volatile__("mov %0, %%fs"::"m"(fs):); //General Purpose Segment
	__asm__ __volatile__("mov %0, %%gs"::"m"(gs):); //General Purpose Segments
	__asm__ __volatile__("mov %0, %%es"::"m"(es):); //Extra Segment
	__asm__ __volatile__("mov %0, %%ds"::"m"(ds):); //Data Segment
	__asm__ __volatile__("mov %0, %%ss"::"m"(ss):); //Stack Segment

	// Criar um seletor válido dinamicamente
	uint32_t cs_selector = (cs.index << 3) | cs.privilege;
	__asm__ __volatile__(
		"push %0\n\t"    // Coloca o seletor CS na pilha
		"push $1f\n\t"   // Coloca o endereço da etiqueta '1' (offset) na pilha
		"lret\n\t"       // Faz o "retorno longo", carregando o CS e saltando para '1'
		"1:\n\t"
		:
		: "r" ((uint32_t)cs_selector) // Passa o seletor através de um registo geral ("r")
		: "memory"
	);

}

uint32_t gdt_get_segment_vma(gdt_segment_t s)
{
	gdt_lgdt_t *gdtl_p;

	// 3. Obter a base da GDT (Mesmo código que já tinhas)
	__asm__ __volatile__("sgdt (%0)":"=r"(gdtl_p)::);
	
	gdt_entry_t *entry = (gdt_entry_t*)gdtl_p->base;

	// 4. Reconstruir o endereço base
	uint32_t base = (entry[s.index].base_low & 0xFFFF) | 			// Ocupa os primeiros 16 bits
                	((entry[s.index].base_middle & 0xFF) << 16) |	// Ocupa os segunintes 8 bits
                	((entry[s.index].base_high & 0xFF) << 24);		// Ocupa os últimos 8 bits

	printk("GDT Segment Base Address: 0X%x 0X%x\n", base,(base+KERNEL_CONFIG_VMA));
	return (base+KERNEL_CONFIG_VMA );
}

uint32_t gdt_get_current_segment_vma(void)
{
    // 1. Ler o Seletor de Segmento de Dados (DS) atual via Assembly Inline
    uint16_t ds_selector;
    __asm__ __volatile__("mov %%ds, %0" : "=r"(ds_selector));

    // 2. Extrair o índice da GDT do seletor (bits 3-15 contêm o índice)
    gdt_segment_t s;
    s.index = ds_selector >> 3;

    // 3. Obter a base da GDT (Mesmo código que já tinhas)
    gdt_lgdt_t *gdtl_p;
    __asm__ __volatile__("sgdt (%0)" : "=r"(gdtl_p) ::);

    gdt_entry_t *entry = (gdt_entry_t*)gdtl_p->base;

    // 4. Reconstruir o endereço base
    uint32_t base = (entry[s.index].base_low & 0xFFFF) |
                    ((entry[s.index].base_middle & 0xFF) << 16) |
                    ((entry[s.index].base_high & 0xFF) << 24);
	
	printk("GDT Segment Base Address: 0X%x 0X%x\n", base,(base+KERNEL_CONFIG_VMA));
	return (base+KERNEL_CONFIG_VMA );
}




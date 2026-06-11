/*
 * lapic.c
 *
 *  Created on: 04/06/2014
 *      Author: ricardo.matos
 */

#include "82489DX.h"
#include "../cpu.h"
#include "../idt.h"
#include "../lib/mem.h"
#include "../lib/printk.h"

// Endereção base da memória da LOCAL APIC. Comun nas máquinas Virtuais.
volatile voidptr_t lapic_base = (voidptr_t)0xFEE00000;

void lapic_init(void)
{
	ioapic_init(); //Inicia a IOAPIC, este driver, contem todas essas funcoes. por forma a nao voltar a defeni-las

	printk("Initializing LAPIC 82489DX !\n");

	lapic_base = (voidptr_t)lapic_get_base();

	lapic_idr_t id;
	*(uint32_t*)&id = LAPIC_REG(LAPIC_ID);

	lapic_ver_t ver;
	*(uint32_t*)&ver = LAPIC_REG(LAPIC_VER);

	LAPIC_REG(LAPIC_LVTLINT0) = LAPIC_LINT_VECTOR(40);
	LAPIC_REG(LAPIC_LVTLINT1) = LAPIC_LINT_VECTOR(41);

	//SETUP A TIMER
	LAPIC_REG(LAPIC_LVTTR) = LAPIC_LVTTR_VECTOR(32)|LAPIC_LVTTR_PERIODIC;
	LAPIC_REG(LAPIC_DCR) = LAPIC_DCR_DIV1;
	LAPIC_REG(LAPIC_ICR) = LAPIC_ICR_COUNT(999999999);

	//Handle SPURIOUS INTERRUPT And Enable LOCAL LAPIC
	LAPIC_REG(LAPIC_SIVR) = 39 | (0b1<<8);

	lapic_enable();
}


uint32_t* lapic_get_base(void)
{
	uint32_t eax, edx;
	cpu_msr_read(MSR_IA32_APIC_BASE,&edx,&eax);
	return (uint32_t*)(eax & 0xFFFFF000);
}

void lapic_enable(void)
{
	uint32_t eax,edx;
	cpu_msr_read(MSR_IA32_APIC_BASE,&edx,&eax);
	eax |= 0x800;
	cpu_msr_write(MSR_IA32_APIC_BASE,edx,eax);
}

void lapic_remap(uint32_t addr)
{
	uint32_t eax,edx;
	cpu_msr_read(MSR_IA32_APIC_BASE,&edx,&eax);
	eax &= 0x00000FFF;
	eax |= addr;
	cpu_msr_write(MSR_IA32_APIC_BASE,edx,eax);
}

void lapic_eio(uint32_t val)
{
	LAPIC_REG(LAPIC_EIO) = val;
}


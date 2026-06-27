/*
 * lapic.c
 *
 *  Created on: 04/06/2014
 *      Author: ricardo.matos
 */

#include "82489DX.h"
#include "_config.h"

#include "i8254.h"

#include "mem.h"
#include "printk.h"

#include "cpu.h"
#include "idt.h"


// Base address of the LOCAL APIC. Commun in Virtual Machines, 
// but can be remapped to a different address in physical memory.
volatile voidptr_t lapic_base = (voidptr_t)0xFEE00000;

// Calibrate LCAPIC timer using the PIT
void lapic_timer_calibrate(void) {

    LAPIC_REG(LAPIC_DCR) = LAPIC_DCR_DIV16;
    LAPIC_REG(LAPIC_LVTTR) = LAPIC_LVTTR_ONESHOT | LAPIC_LVTTR_MASK;

    LAPIC_REG(LAPIC_ICR) = 0xFFFFFFFF;

    pit_wait_ms(10);
    uint32_t current_ccr = LAPIC_REG(LAPIC_CCR);
    uint32_t ticks_in_10ms = 0xFFFFFFFF - current_ccr;

    LAPIC_REG(LAPIC_ICR) = 0;

    lapic_ticks_per_ms = ticks_in_10ms / 10;

	if( ! lapic_ticks_per_ms) {
		// This is a fallback if no calibrations was possible!
		lapic_ticks_per_ms = (KERNEL_CONFIG_TIMER_FALLBACK_FREQ/16)/1000;
	}
    
	lapic_ticks_per_us = lapic_ticks_per_ms / 1000;
	
	printk("    timer ticks set to: %d/ms, %d/us\n",
		lapic_ticks_per_ms,
		lapic_ticks_per_us
	);

}

// Configure LAPIC timer to generate at specific frequency (in milliseconds).
void lapic_timer_config(void)
{
	printk("LAPIC configure:\n");
	lapic_timer_calibrate();

	LAPIC_REG(LAPIC_DCR) = LAPIC_DCR_DIV16;

	LAPIC_REG(LAPIC_LVTTR) = LAPIC_LVTTR_VECTOR(32)|LAPIC_LVTTR_PERIODIC|LAPIC_LVTTR_MASK;
	LAPIC_REG(LAPIC_ICR) = lapic_ticks_per_ms * 1000; // Set initial count for 1ms interval
	LAPIC_REG(LAPIC_LVTTR) &= ~LAPIC_LVTTR_MASK;
}


// Initialize the Local APIC (LAPIC).
void lapic_init(void)
{

	printk("Initializing LAPIC 82489DX !\n");

	lapic_base = (voidptr_t)lapic_get_base();

	lapic_idr_t id;
	*(uint32_t*)&id = LAPIC_REG(LAPIC_ID);

	lapic_ver_t ver;
	*(uint32_t*)&ver = LAPIC_REG(LAPIC_VER);

	// 1. Configurar LINT0: Mascarar completamente (Ignorar o PIC obsoleto)
	// Como o PIC está fora de cena, esta linha não tem utilidade e deve ser desativada
	// para evitar interrupções flutuantes ou ruído de hardware.
	LAPIC_REG(LAPIC_LVTLINT0) = LAPIC_LINT_MASK; // Bit 16 = Masked (Desativado)

	// 2. Configurar LINT1: Definir como Linha Nativa de NMI
	// Modificamos o Delivery Mode para NMI (bits 10-8 = 0b100).
	// O vetor numérico (bits 0-7) DEVE ser obrigatoriamente 0 neste modo.
	LAPIC_REG(LAPIC_LVTLINT1) = LAPIC_LINT_NMI; // Delivery Mode: NMI, Vector: 0, Unmasked	

	//Activa a Local APIC
	lapic_enable();	// Enable Local APIC
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




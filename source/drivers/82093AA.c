#include "82093AA.h"
#include "../lib/printk.h"

// Endereção base da memória da IOAPIC. Comun nas máquinas Virtuais, Este endereço pode ser relocado.
volatile uint32_t* ioapic_base = (uint32_t*)0xFEC00000;

uint32_t ioapic_read(uint8_t reg)
{
	*ioapic_base = reg;
	return  *(ioapic_base +4);
}

void ioapic_write(uint8_t reg, uint32_t value)
{
	*(ioapic_base) = (uint32_t)reg;
	*(ioapic_base +4) = value;
}

void ioapic_init(void)
{
	printk("Initializing IOAPIC 82093AA !\n");

	ioapic_id_t ioid;
	ioapic_cast(ioid) = ioapic_read(IOAPIC_IOAPICID);

	ioapic_ver_t iov;
	ioapic_cast(iov) = ioapic_read(IOAPIC_IOAPICVER);

	int idx = 0;
	for( ;idx < iov.mre; idx++ )
	{
		ioapic_redtbl_L_t tbll;
		ioapic_redtbl_H_t tblh;

		ioapic_cast(tbll) = 0;
		tbll.intvec = idx + 32;
		if( tbll.intvec == 34 ) {
			//if originated from Master PIC to Slave PIC, ignore it
			tbll.mask = 1;
		}

		ioapic_cast(tblh) = 0;
		tblh.des = 0x0; //destination lapic id

		ioapic_write(IOAPIC_IOREDTBL_L(idx),ioapic_cast(tbll));
		ioapic_write(IOAPIC_IOREDTBL_H(idx),ioapic_cast(tblh));
	}
}

#include "cpu.h"


void cpu_msr_read(uint32_t msr, uint32_t *edx, uint32_t *eax)
{
   __asm__ __volatile__("rdmsr":"=d"(*edx),"=a"(*eax):"c"(msr));
}

void cpu_msr_write(uint32_t msr, uint32_t edx, uint32_t eax)
{
   __asm__ __volatile__("wrmsr"::"d"(edx),"a"(eax), "c"(msr));
}

bool_t cpu_has_apic()
{
	uint32_t edx = 0;
	__asm__ __volatile__("mov $1, %%eax; cpuid; ":"=d"(edx)::);
	return ( (edx>>9) & 1 );
}

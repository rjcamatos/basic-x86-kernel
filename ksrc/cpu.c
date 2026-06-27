#include "cpu.h"
#include "printk.h"
#include "mem.h"


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

// Função auxiliar nativa para executar a instrução assembly CPUID em 32 bits
void cpu_cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid"
                 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                 : "a"(leaf), "c"(0));
}

char_t* cpu_cpuid_manufacturer(char_t* buffer) {
    uint32_t eax, ebx, ecx, edx;
    cpu_cpuid(CPUID_LEAF_VENDOR_INFO, &eax, &ebx, &ecx, &edx);
    buffer[12] = '\0';
    memcpy(buffer, &ebx, 4);
    memcpy(buffer + 4, &edx, 4);
    memcpy(buffer + 8, &ecx, 4);
    return buffer;
}

uint_t cpu_cpuid_leafts() {
    uint32_t eax, ebx, ecx, edx;
    cpu_cpuid(CPUID_LEAF_VENDOR_INFO, &eax, &ebx, &ecx, &edx);
    return eax;
}

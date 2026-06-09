/*
 * cpu.h
 *
 *  Created on: 04/06/2014
 *      Author: myself
 */

#ifndef CPU_H_
#define CPU_H_

#include "lib/types.h"

//MODEL-SPECIFIC REGISTERS (MSRS)
//Table 35-2. IA-32 Architectural MSRs
#define MSR_IA32_P5_MC_ADDR				0x0
#define MSR_IA32_P5_MC_TYPE				0x1
#define MSR_IA32_MONITOR_FILTER_SIZE	0x6
#define MSR_IA32_TIME_STAMP_COUNTER		0x10
#define MSR_IA32_PLATFORM_ID			0x17
#define MSR_IA32_APIC_BASE				0x1B
#define MSR_IA32_FEATURE_CONTROL		0x3A
#define MSR_IA32_TSC_ADJUST				0x3B
#define MSR_IA32_BIOS_UPDT_TRIG			0x79
#define MSR_IA32_BIOS_SIGN_ID			0x8B
#define MSR_IA32_SMM_MONITOR_CTL		0X9B
#define MSR_IA32_SMBASE					0X9E
// etc ... PAGE: 35-2 Vol. 3C

//Read Model Specific Register into [EDX:EAX]
void cpu_msr_read(uint32_t msr, uint32_t *edx, uint32_t *eax);

//Write Model Specific Register from [EDX:EAX]
void cpu_msr_write(uint32_t msr, uint32_t edx, uint32_t eax);

//Check if there is an LAPIC chip ON >= P6 family processors
bool_t cpu_has_apic();

#endif /* CPU_H_ */

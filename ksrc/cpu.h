/** COMMENTS By GEMINI */

/*
 * cpu.h
 *
 *  Created on: 04/06/2014
 *      Author: myself
 */

/**
 * x86 CPU Control and Configuration
 * 
 * Provides low-level interfaces to interact with CPU registers (MSRs)
 * and query hardware capabilities.
 */

#ifndef KERNEL_CPU_H_
#define KERNEL_CPU_H_

#include "types.h"

/* MODEL-SPECIFIC REGISTERS (MSRS) */
/* Table 35-2. IA-32 Architectural MSRs (PAGE: 35-2 Vol. 3C) */
#define MSR_IA32_P5_MC_ADDR				0x0    // P5 Machine Check Address
#define MSR_IA32_P5_MC_TYPE				0x1    // P5 Machine Check Type
#define MSR_IA32_MONITOR_FILTER_SIZE	0x6    // Monitor line size range
#define MSR_IA32_TIME_STAMP_COUNTER		0x10   // Time Stamp Counter (TSC)
#define MSR_IA32_PLATFORM_ID			0x17   // Processor platform ID
#define MSR_IA32_APIC_BASE				0x1B   // Local APIC base address and status
#define MSR_IA32_FEATURE_CONTROL		0x3A   // Lock bits for VMX and SMX
#define MSR_IA32_TSC_ADJUST				0x3B   // TSC adjustment offset
#define MSR_IA32_BIOS_UPDT_TRIG			0x79   // BIOS Update Trigger
#define MSR_IA32_BIOS_SIGN_ID			0x8B   // BIOS Update Signature ID
#define MSR_IA32_SMM_MONITOR_CTL		0X9B   // SMM Monitor Control
#define MSR_IA32_SMBASE					0X9E   // SMM Base Address

/* Function Prototypes */

// Reads a Model Specific Register into EDX:EAX
extern void cpu_msr_read(uint32_t msr, uint32_t *edx, uint32_t *eax);

// Writes a Model Specific Register from EDX:EAX
extern void cpu_msr_write(uint32_t msr, uint32_t edx, uint32_t eax);

// Checks if a Local APIC is present
extern bool_t cpu_has_apic(void);


/* Standard CPUID Leaves (0x00000000 and up) */
#define CPUID_LEAF_VENDOR_INFO               0x00000000 // Returns maximum input value and vendor string (e.g., "GenuineIntel")
#define CPUID_LEAF_VERSION_FEATURES          0x00000001 // Returns processor signature (family/model/stepping) and basic feature flags (SSE, VMX, etc.)
#define CPUID_LEAF_CACHE_TLB_DESCRIPTORS     0x00000002 // Returns cache and TLB descriptor information (mainly used by Intel)
#define CPUID_LEAF_DETERMINISTIC_CACHE       0x00000004 // Returns deterministic cache parameters (size, topology). Requires ECX as sub-leaf.
#define CPUID_LEAF_MONITOR_MWAIT             0x00000005 // Returns MONITOR and MWAIT extensions support and layout
#define CPUID_LEAF_THERMAL_POWER             0x00000006 // Returns thermal management and digital power sensor capabilities
#define CPUID_LEAF_EXTENDED_FEATURES         0x00000007 // Returns extended feature flags (AVX2, AVX-512, SHA-NI). Requires ECX as sub-leaf.
#define CPUID_LEAF_ARCH_PERF_MONITORING      0x0000000A // Returns architectural performance monitoring capabilities
#define CPUID_LEAF_EXT_TOPOLOGY             0x0000000B // Returns extended topology enumeration (SMT, Core, and Socket levels)
#define CPUID_LEAF_SGX                       0x00000012 // Returns Software Guard Extensions (SGX) capability and enumeration
#define CPUID_LEAF_V2_EXT_TOPOLOGY          0x0000001F // Returns V2 extended topology enumeration (for newer multi-die/hybrid CPUs)
#define CPUID_LEAF_TSC_CRYSTAL_FREQ          0x00000015 // Returns TSC/Crystal ratio (EAX/EBX) and nominal Core Crystal Clock frequency (ECX, used for APIC base frequency)

/* Extended CPUID Leaves (0x80000000 and up) */
#define CPUID_EXT_LEAF_MAX_SUPPORTED         0x80000000 // Returns highest available extended function input value
#define CPUID_EXT_LEAF_FEATURES              0x80000001 // Returns extended signature and extended feature flags (e.g., NX bit, AMD-specifics)
#define CPUID_EXT_LEAF_BRAND_STRING_1        0x80000002 // Returns first 16 bytes of the CPU brand name string
#define CPUID_EXT_LEAF_BRAND_STRING_2        0x80000003 // Returns second 16 bytes of the CPU brand name string
#define CPUID_EXT_LEAF_BRAND_STRING_3        0x80000004 // Returns final 16 bytes of the CPU brand name string
#define CPUID_EXT_LEAF_L1_CACHE_TLB          0x80000005 // Returns L1 cache and TLB characteristics (mainly AMD)
#define CPUID_EXT_LEAF_L2_L3_CACHE           0x80000006 // Returns L2/L3 cache and TLB identifiers and characteristics
#define CPUID_EXT_LEAF_SVM_FEATURES          0x8000000A // Returns Secure Virtual Machine (SVM) features and revision (AMD)
#define CPUID_EXT_LEAF_ADDR_SIZE             0x80000008 // Returns linear and physical address sizes (e.g., 48-bit virtual, 40-bit physical)

extern void cpu_cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

extern char_t* cpu_cpuid_manufacturer(char_t* buffer);

extern uint_t cpu_cpuid_leafts(void);

#endif /* CPU_H_ */


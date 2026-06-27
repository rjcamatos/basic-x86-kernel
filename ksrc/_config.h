#ifndef KERNEL__CONFIG_H_
#define KERNEL__CONFIG_H_

/**
 * This is the Achiteture, ensure is not 64bits, but 32bits,
 * because this is an 32bits Kernel
 */
#undef ARCH_64_BIT
#define ARCH_32_BIT

/**
 * LMA and VMA are defined in CMakeLists.txt for Global Use
 * Just to Intellisens Editor's don't Loss Them
 */
/*
#ifndef KERNEL_CONFIG_LMA
    #define KERNEL_CONFIG_LMA 0x00100000
#endif
#ifndef KERNEL_CONFIG_VMA
    #define KERNEL_CONFIG_VMA 0xC0000000
#endif
*/

/**
 * The number of Kernel Page Tables, eache Page Table is 4KiB
 * If we full map the 4GiB with 4KiB pages, this need to be set
 * to 1024, like that it ocupies 4MiB
 */
#define KERNEL_CONFIG_PAGING_PAGE_TABLES 10

/**
 * This is used inside prink.c, is the buffer used by the
 * print to srceen printk
 */
#define KERNEL_CONFIG_PRINTK_BUFFER 4096

/**
 * This is used inside 82489DX.c (LAPIC driver), 
 * if the calibration freq. fails, this is the fallback freq.
 */
#define KERNEL_CONFIG_TIMER_FALLBACK_FREQ 1000000000 // 1GHz

#endif
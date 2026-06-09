/*
 * multiboot.h
 *
 *  Created on: 28/03/2015
 *      Author: myself
 */

#ifndef MULTIBOOT_MULTIBOOT_H_
#define MULTIBOOT_MULTIBOOT_H_


#include "../lib/types.h"
#include "multiboot_v1.h"


extern const multiboot_header_t multiboot_header;
extern multiboot_info_t *multiboot_info;

// Declarations inside linker.ld
extern const void (KERNEL_LMA)(void);
extern const void (KERNEL_VMA)(void);
extern const void (KERNEL_ENTRY)(void);

extern const void __multiboot_start;
extern const void __multiboot_end;

extern const void __text_start;
extern const void __text_end;

extern const void __data_start;
extern const void __data_end;

extern const void __rodata_start;
extern const void __rodata_end;

extern const void __bss_start;
extern const void __bss_end;


#endif /* MULTIBOOT_MULTIBOOT_H_ */

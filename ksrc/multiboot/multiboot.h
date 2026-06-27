/*
 * multiboot.h
 *
 *  Created on: 28/03/2015
 *      Author: myself
 */

#ifndef KERNEL_MULTIBOOT_MULTIBOOT_H_
#define KERNEL_MULTIBOOT_MULTIBOOT_H_


#include "types.h"
#include "multiboot_v1.h"


extern multiboot_header_t multiboot_header;
extern multiboot_info_t *multiboot_info;

extern void __multiboot_start;
extern void __multiboot_end;

extern void __text_start;
extern void __text_end;

extern void __data_start;
extern void __data_end;

extern void __rodata_start;
extern void __rodata_end;

extern void __bss_start;
extern void __bss_end;


#endif /* MULTIBOOT_MULTIBOOT_H_ */

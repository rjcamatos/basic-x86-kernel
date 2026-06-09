#include "multiboot.h"

#define MBOOT_HDR_FLAGS			( MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE )
#define MBOOT_HDR_CHECKSUM		-( MULTIBOOT_HEADER_MAGIC + MBOOT_HDR_FLAGS )



/*
 * This structure pointer will be set by _entry.S according to multiboot specification
 * */
multiboot_info_t *multiboot_info = NULL;

/*
 * According to GRUB 2 code this are the unsupported flags (0x0000fff8) :
 *		MULTIBOOT_AOUT_KLUDGE
 *
 * At boot Multiboot compliant Boot Loaders will search for this header
 * */
const multiboot_header_t __attribute__ ((section (".multiboot"),aligned(MULTIBOOT_HEADER_ALIGN))) multiboot_header  = {
		//the mandatory fields
		MULTIBOOT_HEADER_MAGIC,
		MBOOT_HDR_FLAGS,
		MBOOT_HDR_CHECKSUM,
		//the address files (not required for ELF, careful read the manual, WARNING UNSUPORTED BY GRUB 2)
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		//the video mode fields
		1,
		0,
		0,
		0
};

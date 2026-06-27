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
multiboot_header_t __attribute__((section(".multiboot"), aligned(MULTIBOOT_HEADER_ALIGN))) 
multiboot_header = {
    .magic = MULTIBOOT_HEADER_MAGIC,
    .flags = MBOOT_HDR_FLAGS,
    .checksum = MBOOT_HDR_CHECKSUM,
    
    // Como o GRUB lê o formato ELF nativamente, estes campos DEVEM ser zero
    // e o Bit 16 de MBOOT_HDR_FLAGS deve estar DESATIVADO (0)
    .header_addr = 0,
    .load_addr = 0,
    .load_end_addr = 0,
    .bss_end_addr = 0,
    .entry_addr = 0,
    
    // Se o Bit 2 de MBOOT_HDR_FLAGS estiver desativado, estes valores são ignorados pelo GRUB,
    // mas a estrutura precisa ter o tamanho correto se você decidir mantê-los.
    .mode_type = 0,
    .width = 0,
    .height = 0,
    .depth = 0
};

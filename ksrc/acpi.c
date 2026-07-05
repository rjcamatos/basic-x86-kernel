#include "_config.h"

#include "acpi.h"

#include "drivers/82489DX.h"

#include "mmu/paging.h"
#include "io.h"
#include "idt.h"
#include "mem.h"
#include "printk.h"

volatile uint_t power_off = 0;
struct acpi_FADT* global_fadt = NULL;

void acpi_interrupt_handler_power(uint32_t vector, idt_registers_t regs, idt_error_t error, idt_cpu_frame_t cpu)
{
    printk("\n *** %s(%d) POWER INTERRUPT *** VECTOR: %u!\n",__FILE_NAME__,__LINE__,vector);
    if( global_fadt ) {
        uint16_t status_a = io_port_inw((uint16_t)global_fadt->pm1a_evt_blk);
        if( status_a & (1<<8) ) {
            __asm__ __volatile__("cli; hlt;");
        }
    }
	// END-OF-INTERRUPT
	//pic_eio((uint8_t)vector); // required when pic is unmasked
	lapic_eio(vector); // required when lapic is enabled
}

void init_acpi_power_button(struct acpi_FADT* fadt) {
    
    global_fadt = fadt;

    // 1. Check if the power button is handled as a Fixed Feature or Control Method
    if (fadt->flags & (1 << 4)) {
        printk("    ACPI: Power button requires an AML interpreter. Direct FADT init aborted.\n");
        return;
    }

    // 2. Calculate the Enable Register address for PM1a
    // The register block is split in two halves: [Status Register] and [Enable Register]
    uint16_t pm1a_enable_reg = fadt->pm1a_evt_blk + (fadt->pm1_evt_len / 2);
    
    // 3. Enable the power button event interrupt in PM1a
    uint16_t en_val_a = io_port_indw(pm1a_enable_reg);
    en_val_a |= (1 << 8);
    io_port_outw(pm1a_enable_reg, en_val_a);

    // 4. Configure the PM1b block if it exists (non-zero address)
    if (fadt->pm1b_evt_blk != 0) {
        uint16_t pm1b_enable_reg = fadt->pm1b_evt_blk + (fadt->pm1_evt_len / 2);
        uint16_t en_val_b = io_port_inw(pm1b_enable_reg);
        en_val_b |= (1 << 8);
        io_port_outw(pm1b_enable_reg, en_val_b);
    }

    idt_set_interrupt_handler(32+(uint8_t)fadt->sci_interrupt,&acpi_interrupt_handler_power);
}

struct acpi_RSDPDescriptor* acpi_find_rsdp() {
    
    uint16_t ebda_segment = *(uint16_t*)(0x040E + KERNEL_CONFIG_VMA);
    uintptr_t ebda_address = (ebda_segment << 4) + KERNEL_CONFIG_VMA;
    // increment 16 in 16 bytes conform ACPI specification
    for (uintptr_t addr = ebda_address; addr < ebda_address + 1024; addr += 16) {
        if (memcmp((void*)addr, "RSD PTR ", 8) == 0) {
            return (struct acpi_RSDPDescriptor*)addr;
        }
    }

    uintptr_t bios_start = 0x000E0000 + KERNEL_CONFIG_VMA;
    uintptr_t bios_end   = 0x000FFFFF + KERNEL_CONFIG_VMA;
    // increment 16 in 16 bytes conform ACPI specification
    for (uintptr_t addr = bios_start; addr < bios_end; addr += 16) {
        if (memcmp((void*)addr, "RSD PTR ", 8) == 0) {
            return (struct acpi_RSDPDescriptor*)addr;
        }
    }
    return NULL; // ACPI Unsuported
}


void acpi_init(void) {
    
    printk("Initializing ACPI !\n");

    struct acpi_SDTHeader* rsdt = (struct acpi_SDTHeader*) (acpi_find_rsdp())->rsdt_address;
    
    uint32_t phys_base = (uint32_t)rsdt & 0xFFFFF000; 
    uint32_t virt_base = phys_base + KERNEL_CONFIG_ACPI_RSDP_VIRTUAL_ADDRESS;

    // Map the RSDT to the virtual address space to access its contents
    paging_map_4kb(phys_base,virt_base,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
    // For big RSDT map the next 4Kib 
    paging_map_4kb(phys_base+0x1000,virt_base+0x1000,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
    
    uint32_t offset = (uint32_t)rsdt & 0x00000FFF;
    rsdt = (struct acpi_SDTHeader*)(virt_base + offset);

    int entries = (rsdt->length - sizeof(struct acpi_SDTHeader)) / 4;
    uint32_t* pointers = (uint32_t*)((uintptr_t)rsdt + sizeof(struct acpi_SDTHeader));

    for (int i = 0; i < entries; i++) {
        struct acpi_SDTHeader* header = (struct acpi_SDTHeader*)((uintptr_t)(pointers[i]));
        header = (struct acpi_SDTHeader *)((uint32_t)header+KERNEL_CONFIG_ACPI_RSDP_VIRTUAL_ADDRESS);
        if( memcmp(header->signature, "FACP", 4) == 0) { // FACP é a assinatura da FADT
            struct acpi_FADT* fadt = (struct acpi_FADT*)header;
            init_acpi_power_button(fadt);
        }
    }

}



#include "_config.h"

#include "acpi.h"

#include "drivers/82489DX.h"

#include "mmu/paging.h"
#include "io.h"
#include "idt.h"
#include "mem.h"
#include "printk.h"




// Configura o pino mapeado para redirecionar ao Vetor 41 (IRQ do botão)
void ioapic_route_gsi(uint8_t pin) {
    uint8_t reg_low = 0x10 + (2 * pin);
    uint8_t reg_high = 0x11 + (2 * pin);

    // Vetor 41 (0x29), Fixed Mode, Active Low (1), Level Triggered (1), Unmasked (0)
    uint32_t low_bits = 0x29 | (0 << 8) | (0 << 11) | (1 << 13) | (1 << 15) | (0 << 16);
    uint32_t high_bits = (0x00 << 24); // Alvo: APIC ID 0

    ioapic_write(ioapic_base, reg_low, low_bits);
    ioapic_write(ioapic_base, reg_high, high_bits);
}

// ============================================================================
// 4. ATIVAÇÃO DO ACPI E BOTÃO DE ENERGIA
// ============================================================================
int init_acpi_power_button(struct acpi_FADT* fadt) {
    // 1. Ativa o subsistema ACPI no hardware
    if ((io_port_inw(fadt->pm1a_cnt_blk) & 0x01) == 0) {
        if (fadt->smi_cmd != 0) {
            io_port_outb(fadt->smi_cmd, fadt->acpi_enable);
            
            // Aguarda o hardware virar para o modo ACPI (SCI_EN torna-se 1)
            int timeout = 0;
            while ((io_port_inw(fadt->pm1a_cnt_blk) & 0x01) == 0) {
                io_wait();
                if (++timeout > 50000) return -1; // Falha de hardware
            }
        } else {
            return -1; // Sem suporte a SMI CMD
        }
    }

    // 2. Habilita o evento do botão de energia (Power Button Enable - PWRBTN_EN)
    // O registrador PM1_EN fica logo após o PM1_EVT_BLK (PM1_EVT_BLK + BlkLength / 2)
    // Em arquiteturas x86 padrão, ele está no deslocamento fixo +2 de PM1a_EVT_BLK.
    uint16_t pm1a_en_port = fadt->pm1a_evt_blk + 2;
    
    // Ler os eventos atuais configurados
    uint16_t pm1_en_value = io_port_inw(pm1a_en_port);
    
    // Bit 8 define o PWRBTN_EN (Power Button Event Enable)
    pm1_en_value |= (1 << 8); 
    
    // Salva a nova máscara de bits na porta do controlador
    io_port_outw(pm1a_en_port, pm1_en_value);

    // Repete o procedimento para o bloco B se ele existir na placa-mãe
    if (fadt->pm1b_evt_blk != 0) {
        uint16_t pm1b_en_port = fadt->pm1b_evt_blk + 2;
        io_port_outw(pm1b_en_port, io_port_inw(pm1b_en_port) | (1 << 8));
    }

    return 0; // ACPI ativo e monitorando o botão
}

volatile uint_t power_off = 0;
void acpi_interrupt_handler_power(uint32_t vector, idt_registers_t regs, idt_error_t error, idt_cpu_frame_t cpu)
{	
	printk("\n *** %s(%d) POWER INTERRUPT *** VECTOR: %u!\n",__FILE_NAME__,__LINE__,vector);
	// END-OF-INTERRUPT
	//pic_eio((uint8_t)vector); // required when pic is unmasked
	lapic_eio(vector); // required when lapic is enabled
    __asm__ __volatile__("cli; hlt;");
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
    
    uint32_t phys_base = (uint32_t)rsdt & 0xFFC00000; 
    uint32_t virt_base = phys_base;

    // Map the RSDT to the virtual address space to access its contents
    paging_map_4mb(phys_base,virt_base,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);
    
    uint32_t offset = (uint32_t)rsdt & 0x003FFFFF;
    rsdt = (struct acpi_SDTHeader*)(virt_base + offset);

    int entries = (rsdt->length - sizeof(struct acpi_SDTHeader)) / 4;
    uint32_t* pointers = (uint32_t*)((uintptr_t)rsdt + sizeof(struct acpi_SDTHeader));

    for (int i = 0; i < entries; i++) {
        struct acpi_SDTHeader* header = (struct acpi_SDTHeader*)((uintptr_t)(pointers[i]));
        if( memcmp(header->signature, "FACP", 4) == 0) { // FACP é a assinatura da FADT
            struct acpi_FADT* fadt = (struct acpi_FADT*)header;
            // Inicializa o botão de energia e configura o roteamento do I/O APIC
            init_acpi_power_button(fadt);
            uint8_t power_button_gsi = (uint8_t)fadt->sci_interrupt; 
            ioapic_route_gsi(power_button_gsi);
            idt_set_interrupt_handler(41,&acpi_interrupt_handler_power);
            break;
        }
    }

}



#include <types.h>
#include "_config.h"

#include "drivers/82489DX.h"
#include "drivers/8259A.h"

#include "mmu/paging.h"

#include "printk.h"

#include "io.h"
#include "idt.h"
#include "acpi.h"


// ============================================================================
// 2. ESTRUTURAS ACPI (RSDP, FADT e MADT)
// ============================================================================
struct rsdp_descriptor {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct acpi_sdt_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct fadt_table {
    struct acpi_sdt_header header;
    uint32_t firmware_ctrl;
    uint32_t dsdt;
    uint8_t  reserved;
    uint8_t  preferred_pm_profile;
    uint16_t sci_interrupt;
    uint32_t smi_cmd;
    uint8_t  acpi_enable;
    uint8_t  acpi_disable;
    uint8_t  s4bios_req;
    uint8_t  pstate_cnt;
    uint32_t pm1a_evt_blk;
    uint32_t pm1b_evt_blk;
    uint32_t pm1a_cnt_blk;
    uint32_t pm1b_cnt_blk;
    // Outros campos omitidos por brevidade
} __attribute__((packed));



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
int init_acpi_power_button(struct fadt_table* fadt) {
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


struct rsdp_descriptor* find_rsdp(void) {
    // 1. Procurar na região principal da BIOS (0xE0000 até 0xFFFFF)
    uintptr_t bios_start = 0x000E0000 | KERNEL_CONFIG_VMA;
    uintptr_t bios_end   = 0x000FFFFF | KERNEL_CONFIG_VMA;

    // Avança de 16 em 16 bytes conforme a especificação ACPI
    for (uintptr_t addr = bios_start; addr < bios_end; addr += 16) {
        char* signature = (char*)addr;
        
        // Verifica se os 8 bytes correspondem a "RSD PTR "
        if (signature[0] == 'R' && signature[1] == 'S' && 
            signature[2] == 'D' && signature[3] == ' ' &&
            signature[4] == 'P' && signature[5] == 'T' && 
            signature[6] == 'R' && signature[7] == ' ') {
            
            // Opcional: Você pode validar o checksum aqui se quiser mais segurança
            return (struct rsdp_descriptor*)addr;
        }
    }

    // 2. Se não achou na BIOS, tenta ler o endereço da EBDA no endereço fixo 0x40E
    uint16_t ebda_segment = *(volatile uint16_t*)0x0000040E;
    uintptr_t ebda_start = (uint_t)(ebda_segment) << 4;
    
    if (ebda_start > 0x400 && ebda_start < 0xA0000) { // Garante endereço válido
        for (uintptr_t addr = ebda_start; addr < ebda_start + 1024; addr += 16) {
            char* signature = (char*)addr;
            if (signature[0] == 'R' && signature[1] == 'S' && 
                signature[2] == 'D' && signature[3] == ' ' &&
                signature[4] == 'P' && signature[5] == 'T' && 
                signature[6] == 'R' && signature[7] == ' ') {
                return (struct rsdp_descriptor*)addr;
            }
        }
    }

    return NULL; // ACPI não encontrado (máquina muito antiga ou erro)
}

volatile uint_t power_off = 0;
void acpi_interrupt_handler_power(uint32_t vector, idt_registers_t regs, idt_error_t error, idt_cpu_frame_t cpu)
{	
	printk("\n *** %s(%d) POWER INTERRUPT *** VECTOR: %u!\n",__FILE_NAME__,__LINE__,vector);
	// END-OF-INTERRUPT
	pic_eio((uint8_t)vector); // required when pic is unmasked
	lapic_eio(vector); // required when lapic is enabled
    __asm__ __volatile__("cli; hlt;");
}


// ============================================================================
// 5. FUNÇÃO PRINCIPAL DO KERNEL (EXEMPLO DE CHAMADA)
// ============================================================================
void acpi_init(void) {
    
    printk("Initializing ACPI !\n");

    // Localizar a tabela FADT a partir do RSDP/RSDT
    struct acpi_sdt_header* rsdt = (struct acpi_sdt_header*) (find_rsdp())->rsdt_address;
    struct fadt_table* fadt = NULL;
    
    uint32_t rsdt_phys_base = (uint32_t)rsdt & 0xFFC00000; 
    paging_map_4mb(rsdt_phys_base,rsdt_phys_base,PAGING_PDE_PRESENT|PAGING_PDE_READWRITE|PAGING_PDE_SUPERVISOR);

    int entries = (rsdt->length - sizeof(struct acpi_sdt_header)) / 4;
    uint32_t* pointers = (uint32_t*)((uintptr_t)rsdt + sizeof(struct acpi_sdt_header));


    for (int i = 0; i < entries; i++) {
        struct acpi_sdt_header* header = (struct acpi_sdt_header*)(uintptr_t)pointers[i];
        if (header->signature[0] == 'F' && header->signature[1] == 'A' &&
            header->signature[2] == 'C' && header->signature[3] == 'P') { // FACP é a assinatura da FADT
            fadt = (struct fadt_table*)header;
            break;
        }
    }

    if (fadt != NULL) {
        // 1. Ativa o hardware ACPI e desmascara o botão de energia
        init_acpi_power_button(fadt);

        // 2. Descobre a GSI associada ao botão na FADT/MADT. 
        // Em grande parte das implementações x86, o SCI mapeia para a GSI da FADT.
        uint8_t power_button_gsi = (uint8_t)fadt->sci_interrupt; 

        // 3. Aplica o roteamento no I/O APIC para gerar o vetor de interrupção 41
        ioapic_route_gsi(power_button_gsi);

        idt_set_interrupt_handler(41,&acpi_interrupt_handler_power);
    }
}



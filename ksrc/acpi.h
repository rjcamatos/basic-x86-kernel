#ifndef KERNEL_ACPI_H_
#define KERNEL_ACPI_H_

#include "types.h"

// RSDP (Root System Description Pointer) - ACPI 1.0
struct acpi_RSDPDescriptor {
    char_t signature[8];      // Need to be "RSD PTR "
    uint8_t checksum;
    char_t oemid[6];
    uint8_t revision;
    uint32_t rsdt_address;   // 32-bits address to RSDT
} __attribute__((packed));

// SDT (System Description Table) Header - Common to all ACPI tables
struct acpi_SDTHeader {
    char_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char_t oem_id[6];
    char_t oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

// RSDT (Root System Description Table) - Contains pointers to other tables
struct acpi_RSDT {
    struct acpi_SDTHeader header;
    uint32_t pointer_to_othersdt[]; // Array dinâmico de ponteiros de 32-bits
} __attribute__((packed));

// FADT (Fixed ACPI Description Table) - For the Power Button
struct acpi_FADT {
    struct acpi_SDTHeader header;
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
    /* ... outros campos ... */
} __attribute__((packed));

// MADT (Multiple APIC Description Table) - For the LAPIC
struct acpi_MADT {
    struct acpi_SDTHeader header;
    uint32_t local_apic_address; // Physical Base Address of the LAPIC (32-bits)
    uint32_t flags;
    // Follow control structures of APIC (omited to keep it simple)
} __attribute__((packed));




extern volatile uint_t power_off;

extern void acpi_init(void);

#endif
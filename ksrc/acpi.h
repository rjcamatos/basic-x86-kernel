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
    
    /* === ACPI 1.0 EXTENSION FIELDS === */
    uint32_t pm2_cnt_blk;       // PM2 Control Block physical base address
    uint32_t pm_tmr_blk;        // Power Management Timer Block physical base address
    uint32_t gpe0_blk;          // General Purpose Event 0 Register Block base address
    uint32_t gpe1_blk;          // General Purpose Event 1 Register Block base address
    uint8_t  pm1_evt_len;       // Number of bytes decoded for PM1a_EVT_BLK and PM1b_EVT_BLK
    uint8_t  pm1_cnt_len;       // Number of bytes decoded for PM1a_CNT_BLK and PM1b_CNT_BLK
    uint8_t  pm2_cnt_len;       // Number of bytes decoded for PM2_CNT_BLK
    uint8_t  pm_tmr_len;        // Number of bytes decoded for PM_TMR_BLK
    uint8_t  gpe0_blk_len;      // Number of bytes decoded for GPE0_BLK
    uint8_t  gpe1_blk_len;      // Number of bytes decoded for GPE1_BLK
    uint8_t  gpe1_base;         // Offset at which GPE1 based events start
    uint8_t  cst_cnt;           // Support for C-state control
    uint16_t p_lvl2_lat;        // Worst-case latency to enter and exit C2 state (in microseconds)
    uint16_t p_lvl3_lat;        // Worst-case latency to enter and exit C3 state (in microseconds)
    uint16_t flush_size;        // Number of flush strides read to flush dirty cache lines
    uint16_t flush_stride;      // Cache line width in bytes
    uint8_t  duty_offset;       // Bit offset of the processor's duty cycle field in P_CNT
    uint8_t  duty_width;        // Bit width of the processor's duty cycle field in P_CNT
    uint8_t  day_alrm;          // RTC CMOS RAM index to day-of-month alarm
    uint8_t  mon_alrm;          // RTC CMOS RAM index to month-of-year alarm
    uint8_t  century;           // RTC CMOS RAM index to century data
    uint16_t iapc_boot_arch;    // IA-PC Boot Architecture Flags (e.g., legacy devices present)
    uint8_t  reserved2;         // Reserved (must be 0)
    uint32_t flags;             // Fixed Feature Flags (Bit 4: PWR_BUTTON)
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
/* ==========================================================================
 * PIT (Programmable Interval Timer - Intel 8253/8254) Driver
 * ========================================================================== */

 
#ifndef PIT_H
#define PIT_H

#include "types.h"

/* Base Frequency */
#define PIT_FREQUENCY            1193182  /* Internal oscillator base frequency in Hz */

/* I/O Ports */
#define PIT_PORT_CHANNEL0        0x40     /* Channel 0: System Timer / Interrupt (IRQ0) */
#define PIT_PORT_CHANNEL1        0x41     /* Channel 1: RAM Refresh (Obsolete) */
#define PIT_PORT_CHANNEL2        0x42     /* Channel 2: PC Speaker / Audio */
#define PIT_PORT_COMMAND         0x43     /* Command Register (Write-Only) */

/* Command Register: Channel Selection (Bits 6-7) */
#define PIT_SELECT_CHANNEL0      0x00     /* Select Channel 0 */
#define PIT_SELECT_CHANNEL1      0x40     /* Select Channel 1 */
#define PIT_SELECT_CHANNEL2      0x80     /* Select Channel 2 */
#define PIT_READ_BACK            0xC0     /* Read-Back Command (Intel 8254 only) */

/* Command Register: Access/Latch Mode (Bits 4-5) */
#define PIT_ACCESS_LATCH         0x00     /* Latch count value for stable reading */
#define PIT_ACCESS_LOW_BYTE      0x10     /* Read/Write least significant byte (LSB) only */
#define PIT_ACCESS_HIGH_BYTE     0x20     /* Read/Write most significant byte (MSB) only */
#define PIT_ACCESS_BOTH          0x30     /* Read/Write LSB first, then MSB (16-bit) */

/* Command Register: Operating Modes (Bits 1-3) */
#define PIT_MODE_0               0x00     /* Mode 0: Interrupt on Terminal Count */
#define PIT_MODE_1               0x02     /* Mode 1: Hardware Re-triggerable One-Shot */
#define PIT_MODE_2               0x04     /* Mode 2: Rate Generator (Frequency Divider) */
#define PIT_MODE_3               0x06     /* Mode 3: Square Wave Mode (Most Common) */
#define PIT_MODE_4               0x08     /* Mode 4: Software Triggered Strobe */
#define PIT_MODE_5               0x0A     /* Mode 5: Hardware Triggered Strobe */

/* Command Register: Counting Format (Bit 0) */
#define PIT_BINARY               0x00     /* 16-bit Binary Counter (Default) */
#define PIT_BCD                  0x01     /* Binary Coded Decimal Counter (4 digits) */

extern void pit_wait_ms(uint32_t ms);

#endif /* PIT_H */

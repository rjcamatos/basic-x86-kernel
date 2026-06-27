/**
 * x86 I/O Port Communication
 * 
 * Provides inline wrappers for native assembly instructions (in/out) 
 * to interact with hardware ports and peripherals.
 */

#ifndef KERNEL_IOPORT_H_
#define KERNEL_IOPORT_H_

#include "types.h"

// Writes an 8-bit byte to the port
static inline void io_port_outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Reads an 8-bit byte from the port
static inline uint8_t io_port_inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Writes a 16-bit word to the port
static inline void io_port_outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Reads a 16-bit word from the port
static inline uint16_t io_port_inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Writes a 32-bit double word to the port
static inline void io_port_outdw(uint16_t port, uint32_t value) {
    __asm__ __volatile__ (
        "outl %0, %1"
        : 
        : "a"(value), "Nd"(port)
    );
}

// Reads a 32-bit double word from the port
static inline uint32_t io_port_indw(uint16_t port) {
    uint32_t ret;
    __asm__ __volatile__ (
        "inl %1, %0"
        : "=a"(ret)      // EAX output constraint
        : "Nd"(port)     // DX input constraint
    );
    return ret;
}

// Forces a brief I/O delay (common for PIC)
static inline void io_wait(void) {
    io_port_outb(0x80, 0);
}

#endif /* IOPORT_H_ */

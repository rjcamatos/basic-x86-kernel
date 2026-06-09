#ifndef LIB_IOPORT_H_
#define LIB_IOPORT_H_

#include "types.h"

extern void ioport_outb(uint16_t port, byte_t value);
extern byte_t ioport_inb(uint16_t port);
extern word_t ioport_inw(uint16_t port);
extern dword_t ioport_indw(uint16_t port);

#endif /* LIB_IO_H_ */

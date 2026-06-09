#include "ioport.h"

void ioport_outb(uint16_t port, byte_t value)
{
	__asm__ __volatile__ ("out %1,%0"::"d"(port),"a"(value):);
}

byte_t ioport_inb(uint16_t port)
{
	byte_t value;
	__asm__ __volatile__ ("in %1,%0":"=a"(value):"d"(port):);
	return value;
}

word_t ioport_inw(uint16_t port)
{
	word_t value;
	__asm__ __volatile__ ("in %1,%0":"=a"(value):"d"(port):);
	return value;
}

dword_t ioport_indw(uint16_t port)
{
	dword_t value;
	__asm__ __volatile__ ("in %1,%0":"=a"(value):"d"(port):);
	return value;
}

#include "printk.h"
#include "stdio.h"
#include "mem.h"
#include "../bios/bda.h"


void console_putc(char_t c)
{
	volatile unsigned short *video = (volatile unsigned short*)0xB8000;
	static uint_t pos = 0;
	static uint_t column = 0;
	static uint_t row = 0;

	if( c == '\n' )
	{
		row++;
		column = 0;
		pos = bda_VideoControl1->NumberOfColumns * row;
	} else {
		*(video+pos) = 'A'<<8|c;
		pos++;
	}

	if( pos > bda_VideoControl1->NumberOfColumns * bda_VideoControl2->NumberOfRows ) pos = 0;

	if( ++column > bda_VideoControl1->NumberOfColumns )
	{
		column = 0;
		if( ++row > bda_VideoControl2->NumberOfRows ) row = 0;
	}
}

void console_puts(char_t *s)
{
	char_t *tmp = s;
	while( *tmp )
	{
		console_putc(*tmp++);
	}
}


void printk(char_t *fmt, ...)
{
	char buf[4096];
	memset(buf,0x00,sizeof(buf));
	va_list argp;
	va_start(argp,fmt);
	vsprintf(buf,fmt,argp);
	va_end(argp);
	console_puts(buf);
}



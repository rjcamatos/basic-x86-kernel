#include "printk.h"
#include "stdio.h"
#include "mem.h"
#include "bios/bda.h"
#include "_config.h"


void console_putc(char_t c)
{
	volatile uint16_t *video = (volatile uint16_t*)(0xB8000 + (uint_t)KERNEL_CONFIG_VMA);
	static uint_t column = 0;
	static uint_t row = 0;


	if( column >= bda_VideoControl1->NumberOfColumns ) {
		// Last column, start a new row, and set column to 0
		column = 0;
		row++;
	}

	if( row >= (uint_t)bda_VideoControl2->NumberOfRows+1 ) {
		//Scroll Up
		memcpy(
			(voidptr_t)&video[0],
			(const voidptr_t)&video[1*bda_VideoControl1->NumberOfColumns],
			(row*bda_VideoControl1->NumberOfColumns+column)*sizeof(uint16_t)
		);
		// Current row is the last, it will Scroll Up when
		// on next function reentry
		row = bda_VideoControl2->NumberOfRows;
		column = 0;

	}

	if( c == '\n' ) {
		// Next row start at column 0 when '\n' (new line)
		column = 0;
		row++;
		return;
	}

	if( column == 0 ) {
		// Clear old grabage when new row (column == 0)
		memset(
			(voidptr_t)&video[(row*bda_VideoControl1->NumberOfColumns)],
			0x00,
			(row*bda_VideoControl1->NumberOfColumns)*sizeof(uint16_t)
		);
	}
	
	// Write char to console
	video[row*bda_VideoControl1->NumberOfColumns+column] = 0xF100|c;

	// Next column
	column++;
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
	char buf[KERNEL_CONFIG_PRINTK_BUFFER];
	memset(buf,0x00,sizeof(buf));
	va_list argp;
	va_start(argp,fmt);
	vsprintf(buf,fmt,argp);
	va_end(argp);
	console_puts(buf);
}


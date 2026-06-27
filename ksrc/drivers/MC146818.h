/** COMMENTS By GEMINI */

/**
 * MC146818 RTC and CMOS Driver Header
 * 
 * Defines CMOS register addresses, status macros, and data structures 
 * for interacting with the hardware Real-Time Clock.
 */

// MC146818 RTC/CMOS driver header. Defines addresses and hardware clock structures.

#ifndef KERNEL_DRIVERS_MC146818_H_
#define KERNEL_DRIVERS_MC146818_H_

#include "types.h"
#include "io.h"

// RTC calendar data structure.
typedef struct {
	uint_t hours;
	uint_t minutes;
	uint_t seconds;

	uint_t date_of_month;
	uint_t month;
	uint_t year;

	uint_t century;
} cmos_rtc_t;

// CMOS addresses for RTC time, alarms, and status registers (A-D).
#define CMOS_RTC_SECONDS		0x00
#define CMOS_RTC_ALARM_SECOND	0x01
#define CMOS_RTC_MINUTES		0x02
#define CMOS_RTC_ALARM_MINUTE	0x03
#define CMOS_RTC_HOURS			0x04
#define CMOS_RTC_ALARM_HOUR		0x05
#define CMOS_RTC_DAY_OF_WEEK	0x06
#define CMOS_RTC_DATE_OF_MONTH	0x07
#define CMOS_RTC_MONTH			0x08
#define CMOS_RTC_YEAR			0x09
#define CMOS_RTC_RA				0x0A
#define CMOS_RTC_RB				0x0B
#define CMOS_RTC_RC				0x0C
#define CMOS_RTC_RD				0x0D

// CMOS address for the century byte.
#define CMOS_DATE_CENTURY_BYTE	0x32

// Converts a BCD byte to raw Decimal.
#define cmos_bcd2dec(CHAR) ( (CHAR & 0x0F) + ( (CHAR/16) * 10) )

// Selects the internal CMOS register index via port 0x70.
#define cmos_select(CMOS_ADDR) io_port_outb(0x70,CMOS_ADDR)

// Writes a byte to the selected CMOS register via port 0x71.
#define cmos_write(BYTE) io_port_outb(0x71,BYTE)

// Reads a byte from the selected CMOS register via port 0x71.
#define cmos_read() io_port_inb(0x71)

// Reads RTC data into the provided structure.
extern void cmos_read_rtc(cmos_rtc_t *rtc);

#endif /* DRIVERS_MC146818_H_ */

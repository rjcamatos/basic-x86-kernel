/*
 * Access function to CMOS (Complementary Metal Oxide Semiconductor )
 * References:
 *		http://wiki.osdev.org/CMOS
 *		IBM PT AT Technical Reference
 */

#ifndef DRIVERS_MC146818_H_
#define DRIVERS_MC146818_H_

#include "../lib/types.h"
#include "../lib/ioport.h"

typedef struct {
	uint_t hours;
	uint_t minutes;
	uint_t seconds;

	uint_t date_of_month;
	uint_t month;
	uint_t year;

	uint_t century;
} cmos_rtc_t;

/* CMOS memory address to access the Real-time clock information */
#define CMOS_RTC_SECONDS		0x00	// Seconds
#define CMOS_RTC_ALARM_SECOND	0x01	// Alarm second
#define CMOS_RTC_MINUTES		0x02	// Minutes
#define CMOS_RTC_ALARM_MINUTE	0x03	// Alarm minute
#define CMOS_RTC_HOURS			0x04	// Hours
#define CMOS_RTC_ALARM_HOUR		0x05	// Alarm hour
#define CMOS_RTC_DAY_OF_WEEK	0x06	// Day of week
#define CMOS_RTC_DATE_OF_MONTH	0x07	// Date of month
#define CMOS_RTC_MONTH			0x08	// Month
#define CMOS_RTC_YEAR			0x09	// Year
#define CMOS_RTC_RA				0x0A	// Status register A
#define CMOS_RTC_RB				0x0B	// Status register B
#define CMOS_RTC_RC				0x0C	// Status register C
#define CMOS_RTC_RD				0x0D	// Status register D

/*
 * CMOS memory address to access the Date century byte
 * 	Note: the century byte address is not equal to all platforms, use the ACPI to see the address
 * */
#define CMOS_DATE_CENTURY_BYTE	0x32

/* Convert a BCD byte to Decimal */
#define cmos_bcd2dec(CHAR) ( (CHAR & 0x0F) + ( (CHAR/16) * 10) )

/* Select the CMOS memory address to access */
#define cmos_select(CMOS_ADDR) ioport_outb(0x70,CMOS_ADDR)

/* Write a byte to previous selected CMOS memory address */
#define cmos_write(BYTE) ioport_outb(0x71,BYTE)

/* Read a byte from previous selected CMOS memory address */
#define cmos_read() ioport_inb(0x71)

/* Read the Real-time clock information into [rtc] argument */
extern void cmos_read_rtc(cmos_rtc_t *rtc);

#endif /* DRIVERS_MC146818_H_ */

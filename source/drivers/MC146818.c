#include "MC146818.h"

void cmos_read_rtc(cmos_rtc_t *rtc)
{

	cmos_select(CMOS_RTC_RA);
	while( (cmos_read() & 0x80) ); // Wait while RTC update is in progress

	cmos_select(CMOS_RTC_HOURS);
	rtc->hours = cmos_read();

	cmos_select(CMOS_RTC_MINUTES);
	rtc->minutes = cmos_read();

	cmos_select(CMOS_RTC_SECONDS);
	rtc->seconds = cmos_read();

	cmos_select(CMOS_RTC_DATE_OF_MONTH);
	rtc->date_of_month = cmos_read();

	cmos_select(CMOS_RTC_MONTH);
	rtc->month = cmos_read();

	cmos_select(CMOS_RTC_YEAR);
	rtc->year = cmos_read();

	cmos_select(CMOS_DATE_CENTURY_BYTE);
	rtc->century = cmos_bcd2dec(cmos_read());

	cmos_select(CMOS_RTC_RB);
	byte_t regb = cmos_read();

	if( ! (regb & 0x04) ) // Is date in BCD Format
	{
		rtc->hours = cmos_bcd2dec(rtc->hours);
		rtc->minutes = cmos_bcd2dec(rtc->minutes);
		rtc->seconds = cmos_bcd2dec(rtc->seconds);

		rtc->date_of_month = cmos_bcd2dec(rtc->date_of_month);
		rtc->month = cmos_bcd2dec(rtc->month);
		rtc->year = cmos_bcd2dec(rtc->year);
	}

	if( ! (regb & 0x02) ) // Is date in 12h Format
	{
		rtc->hours += 12;
	}

	if( rtc->century )
	{
		rtc->year += rtc->century * 100;
	}

}

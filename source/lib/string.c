#include "string.h"

/*
 * String from unsigned long
 * Convert Unsigned Long to String
 * */
uint32_t strfromul(uint32_t n, char_t *s)
{
	uint32_t num = 0;

	num = n;
	char_t *offset = s;
	while( num /= 10 ) offset++;
	*(offset+1) = '\0';

	num = n;
	do {
		*offset-- = (num%10) + 0x30;
	} while( num /=10 );

	return strlen(s);
}

/*
 * String from signed long
 * Convert Signed Long to String
 * */
uint32_t strfromsl(int32_t n, char_t *s)
{
	if( n > 0 ) {
		return strfromul((uint32_t)n,s);
	} else {
		*s = '-';
		return strfromul(~n+1,s+1);
	}
}

uint32_t strlen(const char_t *s)
{
	char_t *offset = (char_t*)s;
	while( *offset != '\0' ) offset++;
	return (offset - s);
}

char_t *strcpy(char_t *dst, const char_t *src)
{
	char_t *retv = dst;
	while( (*dst++ = *src++) != '\0' );
	return retv;
}


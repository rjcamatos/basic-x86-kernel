
#include "string.h"

/*
* String from unsigned
*/
uint_t strfromu(uint_t n, char_t *s)
{
	uint_t num = 0;

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
 * String from sgigned 
 * */
uint_t strfromd(int_t n, char_t *s)
{
	char_t *tmp = s;
	int_t inc = 0;
	if( n < 0 ) {
		*tmp++ = '-';
		n *= -1;
		inc = 1;
	}
	return inc + strfromu(n,tmp);
}



uint_t strlen(const char_t *s)
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

uint_t strhex(uint_t n, char_t *s)
{
	uint_t num = 0;

	num = n;
	char_t *offset = s;
	while( num /= 16 ) offset++;
	*(offset+1) = '\0';

	num = n;
	do {
		uint_t tmp = num%16;
		if( tmp < 10 ) *offset-- = tmp + 0x30;
		else *offset-- = (tmp-10) + 'A';
	} while( num /=16 );

	return strlen(s);
}




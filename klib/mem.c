#include "mem.h"

voidptr_t memset(voidptr_t m1, uint_t c, size_t n)
{
	if( ! n ) return m1;
	char_t *_m1 = m1;
	while( n-- )
	{
		*_m1++ = (char_t)c;
	}
	return m1;
}

int_t memcmp(const voidptr_t m1, const voidptr_t m2, size_t n)
{
	if( ! n ) return 0;
	char_t *_m1 = m1;
	char_t *_m2 = m2;
	while( --n && *_m1 == *_m2 )
	{
		_m1++;
		_m2++;
	}
	return (*_m1 - *_m2);
}

voidptr_t memcpy(voidptr_t dst, const voidptr_t src, size_t n)
{
	if( ! n ) return dst;
	char_t *_src = src;
	char_t *_dst = dst;
	while( n-- )
	{
		*_dst++ = *_src++;
	}
	return dst;
}

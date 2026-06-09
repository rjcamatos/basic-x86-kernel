#include "stdio.h"
#include "string.h"

int_t vsprintf( char_t *restrict dst, const char_t *restrict fmt, va_list ap)
{
	char_t *tmp = dst;
	while( *fmt != '\0' )
	{
		if( *fmt == '%' && *(fmt+1) )
		{
			switch(*++fmt)
			{
				case 'c':
				{
					*tmp++ = (char_t)va_arg(ap,uint32_t);
					break;
				}
				case 's':
				{
					strcpy(tmp,va_arg(ap,char_t*));
					tmp += strlen(tmp);
					break;
				}
				case 'u':
				{
					tmp += strfromul(va_arg(ap,uint32_t),tmp);
					break;
				}
			}
		} else {
			*tmp++ = *fmt;
		}
		fmt++;
	}
	*tmp = '\0';
	return (tmp - dst);
}

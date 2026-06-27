#include "stdio.h"
#include "string.h"

int_t vsprintf(char_t *restrict dst, const char_t *restrict fmt, va_list ap)
{
	char_t *tmp = dst;
	while( *fmt != '\0' )
	{
		if( *fmt == '%' && (*(fmt+1) != '\0') )
		{
			switch(*++fmt)
			{
				case 's':
				{
					char_t *src = va_arg(ap,char_t*);
					while( (*tmp++ = *src++) != '\0' );
					if( *tmp == '\0' ) tmp--;
					break;
				}
				case 'u':
				{
					tmp += strfromu(va_arg(ap,uint_t),tmp);
					break;
				}
				case 'd':
				{
					tmp += strfromd(va_arg(ap,int_t),tmp);
					break;
				}
				case 'x':
				{
					tmp += strhex(va_arg(ap,uint_t),tmp);
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

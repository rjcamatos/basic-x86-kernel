#ifndef LIB_STDIO_H_
#define LIB_STDIO_H_

#include "types.h"
#include "stdarg.h"

int_t vsprintf( char_t *restrict dst, const char_t *restrict fmt, va_list ap);

#endif /* LIB_STDIO_H_ */

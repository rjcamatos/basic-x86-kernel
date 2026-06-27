/*
 * string.h
 *
 *  Created on: 01/07/2014
 *      Author: ricardo.matos
 */

#ifndef KERNEL_LIB_STRING_H_
#define KERNEL_LIB_STRING_H_

#include "types.h"

extern uint_t strfromu(uint_t n, char_t *s);

extern uint_t strfromd(int_t n, char_t *s);

extern uint_t strlen(const char_t *s);

extern char_t *strcpy(char_t *dst, const char_t *src);

extern uint_t strhex(uint_t n, char_t *s);

#endif /* LIB_STRING_H_ */

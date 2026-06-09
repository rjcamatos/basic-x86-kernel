/*
 * string.h
 *
 *  Created on: 01/07/2014
 *      Author: ricardo.matos
 */

#ifndef LIB_STRING_H_
#define LIB_STRING_H_

#include "types.h"

uint32_t strfromul(uint32_t n, char_t *s);

uint32_t strfromsl(int32_t n, char_t *s);

uint32_t strlen(const char_t *s);

char_t *strcpy(char_t *dst, const char_t *src);

#endif /* LIB_STRING_H_ */

/*
 * mem.h
 *
 *  Created on: 28/05/2014
 *      Author: ricardo.matos
 */

#ifndef LIB_MEM_H_
#define LIB_MEM_H_

#include "types.h"

/*
Copy the unsigned char [c] to the first [n] bytes of [m1]
Return:
	pointer to [m1]
*/
extern voidptr_t memset(voidptr_t m1, uint_t c, size_t n);

/*
Compare the first [n] bytes of [m1] against [m2]
Return:
	< 0 [m1] is less than [m2]
	> 0 [m2] is less than [m1]
	= 0 [m1] is equal [m2]
*/
extern int_t memcmp(const voidptr_t m1, const voidptr_t m2, size_t n);

/*
Copy the first [n] bytes from [src] into [dst]
Return:
	pointer to [dst]
*/
extern voidptr_t memcpy(voidptr_t dst, const voidptr_t src, size_t n);

#endif /* LIB_MEM_H_ */

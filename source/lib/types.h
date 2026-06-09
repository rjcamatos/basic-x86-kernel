/*
 * types.h
 *
 *  Created on: 28/05/2014
 *      Author: ricardo.matos
 */

#ifndef LIB_TYPES_H_
#define LIB_TYPES_H_

typedef void* voidptr_t;
#define NULL ((voidptr_t)0)

typedef signed int int_t;
typedef unsigned int uint_t;
typedef unsigned int size_t;

//boolean data types
typedef unsigned char bool_t;
#define TRUE ((bool_t)1)
#define FALSE ((bool_t)0)

//8 bits data types
typedef char char_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef unsigned char byte_t;

//16 bits data types
typedef signed short wchar_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef unsigned short word_t;

//32 bits data types
typedef signed long int32_t;
typedef unsigned long uint32_t;
typedef unsigned long dword_t;

//64 bits data types
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long long qword_t;


#endif /* LIB_TYPES_H_ */

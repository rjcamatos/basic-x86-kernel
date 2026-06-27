/*
 * types.h
 *
 *  Created on: 28/05/2014
 *      Author: ricardo.matos
 */

#ifndef KERNEL_LIB_TYPES_H_
#define KERNEL_LIB_TYPES_H_
#include "_config.h"


typedef void* voidptr_t;
typedef unsigned int uintptr_t;
typedef int intptr_t;

#define NULL ((voidptr_t)0)

#ifdef ARCH_64_BIT
    typedef signed long long int_t;
    typedef unsigned long long uint_t;

    typedef unsigned long long size_t;
#else
    typedef signed int int_t;
    typedef unsigned int uint_t;
    
    typedef unsigned int size_t;
#endif

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

// Dados de 32 bits
typedef signed int int32_t; // 'int' garante 32-bits tanto em Windows como em Linux (32/64-bit)
typedef unsigned int uint32_t;
typedef unsigned int dword_t;

// Dados de 64 bits
typedef signed long long int64_t;    // 'long long' garante sempre 64-bits em qualquer sistema
typedef unsigned long long uint64_t;
typedef unsigned long long qword_t;


#endif /* LIB_TYPES_H_ */

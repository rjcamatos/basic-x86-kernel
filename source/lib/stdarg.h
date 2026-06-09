#ifndef LIB_STDARG_H_
#define LIB_STDARG_H_

#undef USE_BUILTIN_STDARG

#ifdef USE_BUILTIN_STDARG
	#define va_list __builtin_va_list
	#define va_start(v,f) __builtin_va_start(v, f)
	#define va_arg(v,a) __builtin_va_arg(v, a)
	#define va_end(v) __builtin_va_end(v)
#else
	typedef unsigned int va_list;
	#define va_start(list,last) list = (unsigned int)&last+sizeof(last)
	#define va_arg(list,type) ((type*)(list+=sizeof(type)))[-1]
	#define va_end(list) list = 0
	//#define va_sizeof(type) (sizeof(type)%sizeof(void*) ? sizeof(type)/sizeof(void*)*sizeof(void*)+sizeof(void*) : sizeof(type)) //align the size of type to void pointer
#endif

#endif /* LIB_STDARG_H_ */

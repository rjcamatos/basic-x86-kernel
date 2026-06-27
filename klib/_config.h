// Deteção da arquitetura (64-bit vs 32-bit)

/*

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__LP64__)
    #define ARCH_64_BIT
#else
    #define ARCH_32_BIT
#endif

*/

#undef ARCH_64_BIT
#define ARCH_32_BIT


#undef USE_BUILTIN_STDARG


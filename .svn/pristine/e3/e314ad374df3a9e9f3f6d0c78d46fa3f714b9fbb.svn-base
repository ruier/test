#ifndef SIMPLEPRINTF
#define SIMPLEPRINTF

#define DebugPrint

#ifdef DebugPrint
/*#define PORT3f8 0xbfd003f8*/
#define PORT0 (CCSBAR+0x4500)

/*
 * Macros to access the system control coprocessor
 */
 
#define __read_32bit_c0_register(source, sel)               \
({ int __res;                               \
    if (sel == 0)                           \
        __asm__ __volatile__(                   \
            "mfc0\t%0, " #source "\n\t"         \
            : "=r" (__res));                \
    else                                \
        __asm__ __volatile__(                   \
            ".set\tmips32\n\t"              \
            "mfc0\t%0, " #source ", " #sel "\n\t"       \
            ".set\tmips0\n\t"               \
            : "=r" (__res));                \
    __res;                              \
})
 
#define __read_64bit_c0_register(source, sel)               \
({ unsigned long long __res;                        \
    if (sizeof(unsigned long) == 4)                 \
        __res = __read_64bit_c0_split(source, sel);     \
    else if (sel == 0)                      \
        __asm__ __volatile__(                   \
            ".set\tmips3\n\t"               \
            "dmfc0\t%0, " #source "\n\t"            \
            ".set\tmips0"                   \
            : "=r" (__res));                \
    else                                \
        __asm__ __volatile__(                   \
            ".set\tmips64\n\t"              \
            "dmfc0\t%0, " #source ", " #sel "\n\t"      \
            ".set\tmips0"                   \
            : "=r" (__res));                \
    __res;                              \
})

#define __write_32bit_c0_register(register, sel, value)         \
do {                                    \
    if (sel == 0)                           \
        __asm__ __volatile__(                   \
            "mtc0\t%z0, " #register "\n\t"          \
            : : "Jr" ((unsigned int)(value)));      \
    else                                \
        __asm__ __volatile__(                   \
            ".set\tmips32\n\t"              \
            "mtc0\t%z0, " #register ", " #sel "\n\t"    \
            ".set\tmips0"                   \
            : : "Jr" ((unsigned int)(value)));      \
} while (0)
 
#define __write_64bit_c0_register(register, sel, value)         \
do {                                    \
    if (sizeof(unsigned long) == 4)                 \
        __write_64bit_c0_split(register, sel, value);       \
    else if (sel == 0)                      \
        __asm__ __volatile__(                   \
            ".set\tmips3\n\t"               \
            "dmtc0\t%z0, " #register "\n\t"         \
            ".set\tmips0"                   \
            : : "Jr" (value));              \
    else                                \
        __asm__ __volatile__(                   \
            ".set\tmips64\n\t"              \
            "dmtc0\t%z0, " #register ", " #sel "\n\t"   \
            ".set\tmips0"                   \
            : : "Jr" (value));              \
} while (0)

#define read_c0(x)   __read_32bit_c0_register(x, 0)
#define read_c0_status __read_32bit_c0_register($12, 0)
#define read_c0_cause __read_32bit_c0_register($13, 0)
#define write_c0(x, val)  __write_32bit_c0_register(x, 0, val)
#define write_c0_status(val)	write_c0($12, val)
#define write_c0_cause(val)		write_c0($13, val)

#endif

#endif

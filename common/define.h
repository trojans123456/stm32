#ifndef __DEFINE_H
#define __DEFINE_H

#include <stdint.h>

typedef unsigned char bool_t;
#define True    1
#define False   0

#if defined(__CC_ARM) /* arm compiler */
    #define DEF_SECTION(x)               __attribute__((section(x)))
    #define DEF_UNUSED                   __attribute__((unused))
    #define DEF_USED                     __attribute__((used))
    #define DEF_ALIGN(n)                 __attribute__((aligned(n)))
    #define DEF_WEAK                     __attribute__((weak))
    #define DEF_INLINE                   static __inline
#elif defined(__ICCARM) /* iar <__IAR_SYSTEM_ICC__> */
    #define DEF_SECTION(x)               @ x
    #define DEF_UNUSED
    #define DEF_USED                     __root
    #define DEF_PRAGMA(x)                _Pragma(#x)
    #define DEF_ALIGN(n)                 PRAGMA(data_alignment=n)
    #define DEF_WEAK                     __weak
    #define DEF_INLINE                   static inline
#elif defined(__GNUC__) /* gcc */
    #define DEF_SECTION(x)                 __attribute__((section(x)))
    #define DEF_UNUSED                     __attribute__((unused))
    #define DEF_USED                       __attribute__((used))
    #define DEF_ALIGN(n)                   __attribute__((aligned(n)))
    #define DEF_WEAK                       __attribute__((weak))
    #define DEF_INLINE                     static inline
#endif // defined

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])

/**
* ALIGN(13,4) = 16 align对齐
*/
#define ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * ALIGN_DOWN(13,4) = 12 align向下对齐
 */
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

typedef void (*init_fn_t)(void);
#define __INIT_EXPORT(fn,level) \
        DEF_USED const init_fn_t __board_init__##fn DEF_SECTION(".board__fn."level) = fn

#define stm32_board_init(fn)     __INIT_EXPORT(fn,"1")

#endif // __DEFINE_H

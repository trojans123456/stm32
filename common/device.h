#ifndef __DEVICE_H
#define __DEVICE_H

#include "define.h"

void device_init(void);

/* console output */
typedef void (*putc_ptr)(char ch);
void console_init(putc_ptr func);
void printk(const char *fmt,...);

/* get tick */
DEF_WEAK uint32_t get_tick(void);

#endif // __DEVICE_H

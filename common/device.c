#include <stdio.h>

#include "define.h"
#include "device.h"

static

static void stm32_board_start(void)
{
    return 0;
}
__INIT_EXPORT(stm32_board_start,"0.end");

static void stm32_board_end(void)
{
    return 0;
}
__INIT_EXPORT(stm32_board_end,"1.end");

void device_init(void)
{
    const init_fn_t *fn_ptr;
    for(fn_ptr = &__board_init__stm32_board_start; fn_ptr < &__board_init__stm32_board_end;fn_ptr++)
    {
        (*fn_ptr)();
    }
}

static putc_ptr putc;
static void console_puts(const char *str)
{
    if(NULL == putc)
        return ;

    while(*str)
    {
        if(*str == '\n')
            putc('\r');
        putc(*str);
        str++;
    }
}

void console_init(putc_ptr func)
{
    putc = func;
}
void printk(const char *fmt,...)
{
    va_list args;
    uint32_t i;
    char printbuffer[256];
    va_start(args,fmt);
    i = vsprintf(printbuffer,fmt,args);
    va_end(args);

    console_puts(printbuffer);
}

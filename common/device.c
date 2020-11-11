#include <stdio.h>
#include <stdarg.h>

#include "device.h"

static void stm32_board_start(void)
{
    return ;
}
__INIT_EXPORT(stm32_board_start,"0.end");

static void stm32_board_end(void)
{
    return ;
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

static putc_ptr putc_;
static void console_puts(const char *str)
{
    if(NULL == putc_)
        return ;

    while(*str)
    {
        if(*str == '\n')
            putc_('\r');
        putc_(*str);
        str++;
    }
}

void console_init(putc_ptr func)
{
    putc_ = func;
}
void printk(const char *fmt,...)
{
    va_list args;
   
    char printbuffer[256];
    va_start(args,fmt);
    vsprintf(printbuffer,fmt,args);
    va_end(args);

    console_puts(printbuffer);
}

/* get tick */
DEF_WEAK uint32_t get_tick(void)
{
    return 0;
}

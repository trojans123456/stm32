#include "device.h"

int main()
{
    device_init();

    int cnt = 0;
    while(1)
    {
        printk("xxx%d\n",cnt++);
    }
}

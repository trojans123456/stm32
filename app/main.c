#include "device.h"
#include "usart.h"

void delay_ms(uint16_t time)
{
   uint16_t i=0;
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;
   }
 }

int main()
{
    device_init();
		
    serial_t *serial = usart_find(UART_COM1);
    if(!serial)
        return 0;

    int cnt = 0;
    char buf[10] = "";
    while(1)
    {
       // printk("xxx%d\n",cnt++);	
        if(usart_read(serial,buf,sizeof(buf)) > 0)
            usart_write(serial,"xxx\n",9);
        delay_ms(2000);
    }
}

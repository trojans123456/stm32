#ifndef __SERIAL_H
#define __SERIAL_H

#include "board_config.h"

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

#define DATA_BITS_5                     5
#define DATA_BITS_6                     6
#define DATA_BITS_7                     7
#define DATA_BITS_8                     8
#define DATA_BITS_9                     9

#define STOP_BITS_1                     0
#define STOP_BITS_2                     1

#define PARITY_NONE                     0
#define PARITY_ODD                      1
#define PARITY_EVEN                     2

#define SERIAL_IOCTL_ENABLE_IRQ          0X01
#define SERIAL_IOCTL_DISABLE_IRQ         0x02
#define SERIAL_IOCTL_ENABLE_RX_DMA          0x03

typedef enum serial_port
{
    SERIAL_PORT_1 = 0,
    SERIAL_PORT_2,
    SERIAL_PORT_3,
    SERIAL_PORT_4,
    SERIAL_PORT_5,
    SERIAL_PORT_MAX
}serial_port_t;

struct serial_config
{
    uint32_t baud_rate;
    uint32_t data_bits:4;
    uint32_t stop_bits:2;
    uint32_t parity:2;
    uint32_t reserved:24;
};

#define SERIAL_CONFIG_DEFAULT   \
{   \
    BAUD_RATE_115200, \
    DATA_BITS_8, \
    STOP_BITS_1, \
    PARITY_NONE, \
    0 \
}

struct serial_rx_fifo
{
    uint8_t buffer;
    uint16_t put_index,get_index;
    uint8_t is_full;
};

struct serial_device
{
    serial_port_t port;
    struct serial_config config;
    void *serial_rx;
    void *serial_tx;
    void *priv_data;

    int (*configure)(struct serial_device *dev,struct serial_config *cfg);
    int (*ioctl)(struct serial_device *dev,int cmd,void *args);
    /* */
    int (*putc)(struct serial_device *dev,int ch);
    int (*getc)(struct serial_device *dev);
    /* 触发中断发送和接收 */
    int (*async_puts)(struct serial_device *dev,const char *fmt,...);
    int (*async_gets)(struct serial_device *dev,char *data,int len);

    int (*dma_transmit)(struct serial_device *dev,uint8_t *buf,uint32_t size,int direction);
};


#ifdef SERIAL_PORT_3_USED
extern struct serial_device serial3;
#endif // SERIAL_PORT_1_USED

#endif // __SERIAL_H

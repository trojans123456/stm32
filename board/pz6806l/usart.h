#ifndef __USART_H
#define __USART_H

#include "usart_fifo.h"

typedef enum
{
    //UART_COM1,
    //UART_COM2,
    UART_COM3,
    //UART_COM4,
    //UART_COM5,
    UART_COMNUM
}serial_port_e;

typedef struct
{
    volatile unsigned char b0:1;
    volatile unsigned char b1:1;
    volatile unsigned char b2:1;
    volatile unsigned char b3:1;
    volatile unsigned char b4:1;
    volatile unsigned char b5:1;
    volatile unsigned char b6:1;
    volatile unsigned char b7:1;
}byte_field;

typedef struct
{
    unsigned char No; //´®¿ÚºÅ
    unsigned int baudRate;
    unsigned short WordLength;
    unsigned short stopBits;
    unsigned short parity;
    unsigned short mode; //USART_Mode_Rx USART_Mode_Tx
}serial_params_t;

struct serial_;
typedef struct serial_ops
{
    void (*init)(struct serial_ *serial);
    void (*configure)(struct serial_ *serial,serial_params_t *params);
        unsigned int (*read)(struct serial_ *serial, void *pRecvBuf, unsigned int len);	//??n???
        void (*write)(struct serial_ *serial, void *pSendBuf, unsigned int len);
        void (*rt_ctrl)(struct serial_ *serial,unsigned char rtStatus);//for 485 rx_tx_switch
        void (*irq)(struct serial_ *serial);
        void (*checkover)(struct serial_ *serial);
}serial_ops_t;

typedef struct serial_
{
    serial_params_t param;
    byte_field flg; //b0 txbusy b1 rxbusy
    unsigned char RxOverTime;//
    unsigned char TxErrTime;
    queue_t     *TxList;
    queue_t     *RxList;

    void *private_user;
    serial_ops_t *ops;
}serial_t;


void usart_checkover(void);

serial_t *usart_find(unsigned int port);

#define usart_read(dev,buffer,len)  dev->ops->read(dev,buffer,len)
#define usart_write(dev,buffer,len) dev->ops->write(dev,buffer,len)

#endif
